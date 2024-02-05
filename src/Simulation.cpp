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
    for (unsigned int a = 0; a < this->nb_flock; a++)
    {
        flock.push_back(new Boid{p6::random::number(-ctx.aspect_ratio(), ctx.aspect_ratio()), p6::random::number(-1, 1), p6::random::number(-this->speed_factor, this->speed_factor), p6::random::number(-this->speed_factor, this->speed_factor)});
    }

    // lancer la boucle infini
    Render();
    ctx.start();
}

void Simulation::Render()
{
    double deltaTime = 0.001;
    // Declare your infinite update loop.
    ctx.update = [&]() {
        // Clear the background with a fading effect
        ctx.background({0.0f, 0.0f, 0.0f});
        // Draw something

        for (const auto& boid : flock)
        {
            boid->edges(this->ctx);
            boid->updatePosition(deltaTime);
            boid->flock(flock);
            boid->show(this->ctx);
        }
    };
}
