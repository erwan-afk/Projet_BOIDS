#include "Simulation.hpp"


Simulation::Simulation()
{
    this->ctx = p6::Context{{this->window_width,this->windows_height,this->name}};
    this->ctx.maximize_window();
}