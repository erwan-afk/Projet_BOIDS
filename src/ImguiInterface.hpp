#pragma once

#include <p6/p6.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Simulation.hpp"

class ImguiInterface {
private:
    glm::vec3 background_color;
    float     boidVision;
    float     separationForce;
    float     cohesionForce;
    float     alignForce;
    float     alignBias;
    float     minSpeed;

public:
    ImguiInterface(const glm::vec3& background_color = glm::vec3(0.0, 0.639, 1.0), // Default background color
                   float boidVision = 0.01f, float separationForce = 1.1f, float cohesionForce = 1.0f, float alignForce = 1.1f, float alignBias = 1.5f, float minSpeed = 1.0f)
        : background_color(background_color), boidVision(boidVision), separationForce(separationForce), cohesionForce(cohesionForce), alignForce(alignForce), alignBias(alignBias), minSpeed(minSpeed)
    {
    }

    glm::vec3 getBackground_color() const
    {
        return background_color;
    }

    void setValue(std::vector<Boid*>& flock, float boidVisionFact, float separationForceFact, float cohesionForceFact, float alignForceFact, float alignBiasFact, float minSpeedFact)
    {
        for (Boid* boid : flock)
        {
            boid->setImguiFactor(boidVisionFact, separationForceFact, cohesionForceFact, alignForceFact, alignBiasFact, minSpeedFact);
        }
    }

    void userInteface(std::vector<Boid*>& flock)
    {
        ImGui::Begin("Option");
        ImGui::SliderFloat("Separation", &this->separationPerception, -40.0, 40.0);
        ImGui::SliderFloat("Cohesion", &this->cohesionPerception, -40.0, 40.0);
        ImGui::SliderFloat("Align", &this->alignPerception, -40.0, 40.0);
        ImGui::SliderFloat("Speed", &this->speedFactor, 1.0, 100.0);
        ImGui::ColorPicker4("Background Color", (float*)&this->background_color);

        ImGui::End();

        setValue(flock, this->boidVision / (2.0f * 150.0f), this->separationForce / (2.0f * 4.0f), this->cohesionForce / (2.0f * 4.0f), this->alignForce / (2.0f * 4.0f), this->alignBias / (2.0f * 4.0f), this->minSpeed / (2.0f * 4.0f));
    }

    // Fonction pour choisir une couleur en fonction des probabilités
    void choisirCouleur()
    {
        // Génération d'un nombre aléatoire entre 0 et 1
        double randNum = (double)rand() / RAND_MAX;

        // Déterminez la couleur en fonction des probabilités de transition
        if (randNum < 0.4)
            background_color = glm::vec3(1.0, 0.0, 0.0); // Rouge
        else if (randNum < 0.7)
            background_color = glm::vec3(0.0, 0.0, 1.0); // Bleu
        else
            background_color = glm::vec3(0.0, 1.0, 0.0); // Vert
    }

    ~ImguiInterface() {}
};
