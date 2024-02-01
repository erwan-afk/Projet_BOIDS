#include "Simulation.hpp"

// Définition du constructeur
Simulation::Simulation()
    : name("Projet Boids"), window_width(this->window_width), window_height(this->window_height), ctx()
{
    // Initialisations supplémentaires si nécessaires
}

void Simulation::Run()
{
    // initalisation des boids

    // lancer la boucle infini
    Render();
    this->ctx.start();
}

void Simulation::Render()
{
    // Declare your infinite update loop.
    ctx.update = [&]() {
        // Clear the background with a fading effect
        ctx.use_stroke = false;
        ctx.fill       = {0.2f, 0.1f, 0.3f, 1.0f};
        ctx.rectangle(p6::Center{0.0, 0.0}, p6::Radii{0.3, 0.5}, p6::Rotation{0.0_turn});
        // Draw something

        ctx.fill = {1.f, 0.7f, 0.2f};
        ctx.circle(p6::Center{ctx.mouse()}, p6::Radius{0.1f});
        // ctx.circle(p6::Center{firstBoid.getPosX(), firstBoid.getPosY()}, p6::Radius{0.05f});
        // firstBoid.updatePosition(deltaTime);
        // std::cout << firstBoid.getPosX() << std::endl;
    };
}

// Simulation::~Simulation(){

// }
