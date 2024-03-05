#pragma once

#include <string>
#include <vector>
#include "Boid.hpp"
#include "p6/p6.h"

class Simulation {
private:
    std::string name          = "Projet_BOIDS";
    const int   window_width  = 1280;
    const int   window_height = 720;
    p6::Context ctx;

    std::vector<Boid*> flock;

    void Render();

public:
    Simulation();
    void Run();
    void setSeparationPerception(float value); 
    //~Simulation();
};
