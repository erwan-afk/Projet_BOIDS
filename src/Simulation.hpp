#pragma once

#include <string>
#include <vector>
#include "Boid.hpp"
#include "p6/p6.h"

class Simulation {
private:
    std::string        name          = "Projet_BOIDS";
    const int          window_width  = 1280;
    const int          window_height = 720;
    const unsigned int nb_flock      = 100;
    const float        speed_factor  = 0.005;
    p6::Context        ctx;

    std::vector<Boid*> flock;

    void Render();

public:
    Simulation();
    void Run();
    //~Simulation();
};
