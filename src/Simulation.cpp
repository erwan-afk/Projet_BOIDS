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
    // flock.push_back(new Boid(0.0, 0.0, 2.0, 2.0));

    // lancer la boucle infini
    Render();
    ctx.start();
}

void Simulation::Render()
{
    Boid   ok(0.0, 0.0, 0.01, 0.01);
    double deltaTime = 0.001;
    // Declare your infinite update loop.
    ctx.update = [&]() {
        // Clear the background with a fading effect

        // Draw something
        /*
        for (const auto& boid : flock)
        {
            boid->updatePosition(deltaTime);
            std::cout << boid->getPosX() << std::endl;
            boid->show(this->ctx);
        }
        */
        // ok.updatePosition(deltaTime);
        // ok.show(this->ctx);
        // std::cout << ok.getPosX() << std::endl;

        ctx.fill = {1.f, 0.7f, 0.2f};

        // Update the position first
        ok.updatePosition(deltaTime);

        // Draw the circle at the updated position
        ctx.circle(p6::Center{ok.getPosX(), ok.getPosY()}, p6::Radius{0.05f});

        // Print the updated position
    };
}

// Simulation::~Simulation(){

// }
