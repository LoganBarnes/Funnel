#include <sim-driver/OpenGLSimulation.hpp>
#include "SimpleStupidFunnel.hpp"

int main()
{
    sim::OpenGLSimulation<SimpleStupidFunnel>{}.runNoFasterThanRealTimeLoop();
    return 0;
}
