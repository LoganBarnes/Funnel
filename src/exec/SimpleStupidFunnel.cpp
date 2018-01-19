#include "SimpleStupidFunnel.hpp"
#include <imgui.h>
#include <sim-driver/OpenGLHelper.hpp>
#include <ssf/Config.hpp>
#include <ssf/test_cases.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

SimpleStupidFunnel::SimpleStupidFunnel(int, int, sim::SimData *pSimData) : simData_{*pSimData}
{
    sim::OpenGLHelper::setDefaults();
    glpl_.program = sim::OpenGLHelper::createProgram(ssf::shader_path() + "ssf.vert", ssf::shader_path() + "ssf.frag");
    reset_test();

    simData_.cameraMover.setOrbitOffsetDistance(15);
}

void SimpleStupidFunnel::onRender(int, int, double)
{
    // render based on drawing order
//    glDisable(GL_DEPTH_TEST);

    // setup used for all shapes
    glUseProgram(*(glpl_.program));

    sim::OpenGLHelper::setMatrixUniform(glpl_.program,
                                        "screen_from_world",
                                        glm::value_ptr(simData_.camera().getPerspectiveScreenFromWorldMatrix()));

    //    auto draw = [&](const glm::vec3 &color, const std::pair<int, int> &indices, GLenum mode) {
    //        if (indices.second > 0) {
    //            sim::OpenGLHelper::setFloatUniform(glpl_.program, "color", glm::value_ptr(color));
    //            sim::OpenGLHelper::renderBuffer(glpl_.vao, indices.first, indices.second, mode);
    //        }
    //    };
    //
    //    // gray triangles
    //    draw(glm::vec3(0.5f), strip_indices_, GL_TRIANGLE_STRIP);
    //
    //    // orange gates
    //    draw(glm::vec3(1.f, 0.5f, 0.1f), strip_indices_, GL_LINE_STRIP);
    //
    //    // red left
    //    draw(glm::vec3(1.f, 0.f, 0.f), left_indices_, GL_LINE_STRIP);
    //
    //    // green right
    //    draw(glm::vec3(0.f, 1.f, 0.f), right_indices_, GL_LINE_STRIP);
    //
    //    // yellow path
    //    draw(glm::vec3(1.f, 1.f, 0.f), path_indices_, GL_LINE_STRIP);

    // reset defaults
//    glEnable(GL_DEPTH_TEST);

    glDisable(GL_CULL_FACE);

    glm::vec3 color(1.f);
    sim::OpenGLHelper::setFloatUniform(glpl_.program, "color", glm::value_ptr(color));
    sim::OpenGLHelper::renderBuffer(glpl_.vao, 0, 3, GL_TRIANGLES);
}

void SimpleStupidFunnel::onGuiRender(int, int)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    if (ImGui::Begin("Window", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

void SimpleStupidFunnel::reset_test()
{
    //    std::vector<glm::vec2> vbo_data = ssf::test_cases<float>()[current_test_].func().input;
    //    strip_indices_ = {0, vbo_data.size()};
    //
    //    left_indices_ = {vbo_data.size(), 0};
    //    vbo_data.emplace_back(0, 0);
    //    vbo_data.emplace_back(0, 0);
    //
    //    right_indices_ = {vbo_data.size(), 0};
    //    vbo_data.emplace_back(0, 0);
    //    vbo_data.emplace_back(0, 0);
    //
    //    path_indices_ = {vbo_data.size(), 0};
    //    vbo_data.insert(vbo_data.end(), vbo_data.size() - 4, {0, 0});

    std::vector<glm::vec2> vbo_data = {{0, 0}, {1, 1}, {-1, 2}};

    glpl_.vbo = sim::OpenGLHelper::createBuffer(vbo_data.data(), vbo_data.size(), GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
    glpl_.vao = sim::OpenGLHelper::createVao(glpl_.program, glpl_.vbo, 0, {{"world_position", 2, GL_FLOAT, nullptr}});
}
