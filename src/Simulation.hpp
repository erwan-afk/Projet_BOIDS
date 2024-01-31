#pragma once

#include <string>
#include "p6/p6.h"

class Simulation {
private:
    std::string name = "Projet_BOIDS";
    const int   window_width = 1280;
    const int window_height = 720;
    p6::Context ctx; 
    
    void Render(); 
    
   
    
public:
    Simulation();
    void Run(); 
    //~Simulation();
};


