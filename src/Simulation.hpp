#pragma once

#include <string>
#include <vector>
#include "Big_plant.hpp"
#include "Boid.hpp"
#include "glm/fwd.hpp"
#include "p6/p6.h"

class Simulation {
private:
    std::string        name             = "Projet_BOIDS";
    const int          window_width     = 1280;
    const int          window_height    = 720;
    const unsigned int nb_flock         = 200;
    const unsigned int nb_big_plant     = 8;
    const unsigned int nb_bush          = 32;
    const unsigned int nb_coral         = 32;
    const float        speed_factor     = 0.005;
    float              totalTimeElapsed = 0.0;
    float              t;
    float              animation_daytime_duration = 5.0f;

    std::vector<Boid*>  flock;
    std::vector<Plant*> wildlife_big_plant;
    std::vector<Plant*> wildlife_bush;
    std::vector<Plant*> wildlife_coral;

    void Render();

public:
    p6::Context ctx;
    Simulation();

    void setImguiFactorAlign(float value);
    void setImguiFactorCohesion(float value);
    void setImguiFactorSeparation(float value);

    void Run();

    //~Simulation();
};
