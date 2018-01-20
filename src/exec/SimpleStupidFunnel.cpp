#include "SimpleStupidFunnel.hpp"
#include <imgui.h>
#include <sim-driver/OpenGLHelper.hpp>
#include <ssf/Config.hpp>
#include <ssf/test_cases.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ssf/ssf.hpp>
#include <iostream>
#include <sim-driver/WindowManager.hpp>

SimpleStupidFunnel::SimpleStupidFunnel(int, int, sim::SimData *pSimData) : simData_{*pSimData}
{
    sim::OpenGLHelper::setDefaults();
    glpl_.program = sim::OpenGLHelper::createProgram(ssf::shader_path() + "ssf.vert", ssf::shader_path() + "ssf.frag");
    reset_test();

    simData_.cameraMover.setUsingOrbitMode(true);
    simData_.cameraMover.setOrbitOffsetDistance(15);
}

void SimpleStupidFunnel::onRender(int, int, double)
{
    // render based on drawing order
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    // setup used for all shapes
    glUseProgram(*(glpl_.program));

    sim::OpenGLHelper::setMatrixUniform(glpl_.program,
                                        "screen_from_world",
                                        glm::value_ptr(simData_.camera().getPerspectiveScreenFromWorldMatrix()));

    auto draw = [&](const glm::vec3 &color, const std::pair<int, int> &indices, GLenum mode) {
        if (indices.second > 0) {
            sim::OpenGLHelper::setFloatUniform(glpl_.program, "color", glm::value_ptr(color), 3);
            sim::OpenGLHelper::renderBuffer(glpl_.vao, indices.first, indices.second, mode);
        }
    };

    // gray triangles
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    draw(glm::vec3(0.5f), strip_indices_, GL_TRIANGLE_STRIP);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // orange gates
    draw(glm::vec3(1.f, 0.5f, 0.1f), strip_indices_, GL_LINE_STRIP);

    // red left
    draw(glm::vec3(1.f, 0.f, 0.f), left_indices_, GL_LINE_STRIP);

    // green right
    draw(glm::vec3(0.f, 1.f, 0.f), right_indices_, GL_LINE_STRIP);

    // yellow path
    draw(glm::vec3(1.f, 1.f, 0.f), path_indices_, GL_LINE_STRIP);

    // reset defaults
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

void SimpleStupidFunnel::onGuiRender(int, int)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    if (ImGui::Begin("Window", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        if (ImGui::Combo("Test Case",
                         &current_test_,
                         [](void *data, int index, const char **description) {
                             ssf::Test<float> &test = static_cast<ssf::Test<float> *>(data)[index];
                             *description = test.name.c_str();
                             return true;
                         },
                         ssf::test_cases<float>().data(),
                         static_cast<int>(ssf::test_cases<float>().size()))) {
            reset_test();
        }
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

void SimpleStupidFunnel::keyCallback(GLFWwindow *, int key, int, int action, int)
{
    auto num_tests = static_cast<int>(ssf::test_cases<float>().size());

    if (action == GLFW_RELEASE) {
        switch (key) {
        case GLFW_KEY_LEFT:
            current_test_ = (current_test_ + num_tests - 1) % num_tests;
            reset_test();
            break;
        case GLFW_KEY_RIGHT:
            current_test_ = (current_test_ + 1) % num_tests;
            reset_test();
            break;
        default:
            break;
        }
    }
}

void SimpleStupidFunnel::reset_test()
{
    std::vector<glm::vec2> vbo_data = ssf::test_cases<float>()[current_test_].func().input;
    strip_indices_ = std::make_pair(0, vbo_data.size());

    left_indices_ = std::make_pair(vbo_data.size(), 0);
    vbo_data.emplace_back(0, 0);
    vbo_data.emplace_back(0, 0);

    right_indices_ = std::make_pair(vbo_data.size(), 0);
    vbo_data.emplace_back(0, 0);
    vbo_data.emplace_back(0, 0);

    path_indices_ = std::make_pair(vbo_data.size(), 0);
    vbo_data.insert(vbo_data.end(), vbo_data.size() - 4, {0, 0});

    glpl_.vbo = sim::OpenGLHelper::createBuffer(vbo_data.data(), vbo_data.size(), GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
    glpl_.vao = sim::OpenGLHelper::createVao(glpl_.program, glpl_.vbo, 0, {{"world_position", 2, GL_FLOAT, nullptr}});
}
