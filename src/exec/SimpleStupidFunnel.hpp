#pragma once

#include <sim-driver/SimData.hpp>
#include <sim-driver/OpenGLTypes.hpp>

struct GLFWwindow;

class SimpleStupidFunnel
{
public:
    SimpleStupidFunnel(int width, int height, sim::SimData *pSimData);

    void onRender(int width, int height, double alpha);
    void onGuiRender(int width, int height);

    void keyCallback(GLFWwindow *pWindow, int key, int scancode, int action, int mods);

private:
    sim::SimData &simData_;
    int current_test_{5};

    sim::StandardPipeline glpl_;
    std::pair<int, int> strip_indices_;
    std::pair<int, int> left_indices_;
    std::pair<int, int> right_indices_;
    std::pair<int, int> path_indices_;

    void reset_test();
};
