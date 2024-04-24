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
    const int          window_width     = 1920;
    const int          window_height    = 1080;
    bool               inWindow         = true;
    const unsigned int nb_flock         = 300;
    const unsigned int nb_big_plant     = 32;
    const unsigned int nb_bush          = 64;
    const unsigned int nb_coral         = 40;
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
