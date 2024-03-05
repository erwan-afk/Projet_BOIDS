#include "Simulation.hpp"
#include "Boid.hpp"

// Définition du constructeur
Simulation::Simulation()
    : name("Projet Boids"), window_width(this->window_width), window_height(this->window_height), ctx()
{
    // Initialisations supplémentaires si nécessaires
}


void Simulation::Run()
{
    // initalisation des boids
    for (int a = 0; a < 100; a++)
    {
        flock.push_back(new Boid{p6::random::number(-ctx.aspect_ratio(), ctx.aspect_ratio()), p6::random::number(-1, 1), p6::random::number(-0.005, 0.005), p6::random::number(-0.005, 0.005)});
    }

    // lancer la boucle infini
    Render();
    ctx.start();
}


void Simulation::setSeparationPerception(float value) {
    for (const auto& boid : flock) {
        boid->setSeparationPerception(value);
    }
}




void Simulation::Render()
{
    double deltaTime = 0.001;
    glm::vec4 background_color = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f); // Default background color
    float separationPerception = 0.1; 
    

    
    // Declare your infinite update loop.
    ctx.update = [&]() {

        ImGui::Begin("Option");
        ImGui::SliderFloat("Separation", &separationPerception, 0.05, 0.4);
        ImGui::ColorPicker4("Background Color", (float*)&background_color);
     

        ImGui::End();

        // Clear the background with a fading effect
        ctx.background({background_color.x, background_color.y, background_color.z});
        setSeparationPerception(separationPerception);

   
   
        //draw
        for (const auto& boid : flock)
        {
            boid->edges(this->ctx);
            boid->updatePosition(deltaTime);
            boid->flock(flock);
            boid->show(this->ctx);
        }
    };
}
