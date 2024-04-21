#pragma once

#include <p6/p6.h>
#include <cstddef>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Probability.hpp"
#include "Simulation.hpp"

class ImguiInterface {
private:
    glm::vec3 background_color;
    glm::vec3 targetColor;
    // std::vector<int> states = {0, 1, 2, 3};
    float separationPerception;
    float cohesionPerception;
    float alignPerception;
    float speedFactor;

    glm::mat4 matriceTransition{
        {3.0f / 8.0f, 3.0f / 8.0f, 1.0f / 8.0f, 1.0f / 8.0f},
        {3.0f / 12.0f, 4.0f / 12.0f, 2.0f / 12.0f, 2.0f / 12.0f},
        {1.0f / 7.0f, 1.0f / 7.0f, 3.0f / 7.0f, 2.0f / 7.0f},
        {1.0f / 8.0f, 1.0f / 8.0f, 3.0f / 8.0f, 3.0f / 8.0f}
    };

public:
    int currentState = 0;
    ImguiInterface(const glm::vec3& background_color = glm::vec3(0.0, 0.639, 1.0), const glm::vec3& targetColor = glm::vec3(0.447, 0.364, 0.0), // Default background color
                   float separationPerception = 0.0f, float cohesionPerception = 0.0f, float alignPerception = 0.0f, float speedfactor = 80.0)
        : background_color(background_color), targetColor(targetColor), separationPerception(separationPerception), cohesionPerception(cohesionPerception), alignPerception(alignPerception), speedFactor(speedfactor)
    {
    }

    void setBackground_color(const glm::vec4& new_color)
    {
        this->background_color = new_color;
    }

    void setSeparationPerception(float new_value)
    {
        this->separationPerception = new_value;
    }

    void setCohesionPerception(float new_value)
    {
        this->cohesionPerception = new_value;
    }

    void setAlignPerception(float new_value)
    {
        this->alignPerception = new_value;
    }

    glm::vec3 getBackground_color() const
    {
        return this->background_color;
    }

    float getSeparationPerception() const
    {
        return this->separationPerception;
    }

    float getCohesionPerception() const
    {
        return this->cohesionPerception;
    }

    float getAlignPerception() const
    {
        return this->alignPerception;
    }

    float getSpeedFactor() const
    {
        return this->speedFactor;
    }

    void setImguiFactorAlign(float value, std::vector<Boid*>& flock)
    {
        for (Boid* boid : flock)
        {
            boid->setImguiFactorAlign(value);
        }
    }
    void setImguiFactorCohesion(float value, std::vector<Boid*>& flock)
    {
        for (Boid* boid : flock)
        {
            boid->setImguiFactorCohesion(value);
        }
    }
    void setImguiFactorSeparation(float value, std::vector<Boid*>& flock)
    {
        for (Boid* boid : flock)
        {
            boid->setImguiFactorSeparation(value);
        }
    }

    void userInteface(std::vector<Boid*>& flock)
    {
        ImGui::Begin("Option");
        ImGui::SliderFloat("Separation", &this->separationPerception, -40.0, 40.0);
        ImGui::SliderFloat("Cohesion", &this->cohesionPerception, -40.0, 40.0);
        ImGui::SliderFloat("Align", &this->alignPerception, -40.0, 40.0);
        ImGui::SliderFloat("Speed", &this->speedFactor, 1.0, 100.0);
        // ImGui::ColorPicker4("Background Color", (float*)&this->background_color);
        ImGui::End();

        // set ImGui Options
        setImguiFactorSeparation(this->separationPerception * 0.02, flock);
        setImguiFactorCohesion(this->cohesionPerception * 0.0005, flock);
        setImguiFactorAlign(this->alignPerception * 0.35, flock);
    }

    // Fonction pour choisir une couleur en fonction des probabilités
    void choisirCouleur()
    {
        // Génération d'un nombre aléatoire entre 0 et 1
        double randNum = (double)rand() / RAND_MAX;

        // Déterminez la couleur en fonction des probabilités de transition
        if (randNum < 0.2)
            targetColor = glm::vec3(0.0, 0.639, 1.0); // Rouge
        else if (randNum < 0.4)
            targetColor = glm::vec3(0.447, 0.364, 0.0); // Bleu
        else if (randNum < 0.6)
            targetColor = glm::vec3(0.274, 0.0, 0.423); // Bleu
        else
            targetColor = glm::vec3(0.0, 0.0, 0.423); // Vert
    }

    void markov_setp2(glm::vec3 m)
    {
        float a = random1();
        if (a < m[0])
        {
            currentState = 0;

            targetColor = glm::vec3(0.0, 0.0, 0.423);
        }
        else if (a < m[0] + m[1])
        {
            currentState = 1;
            targetColor  = glm::vec3(0.274, 0.0, 0.423);
        }
        else if (a < m[0] + m[1] + m[2])
        {
            currentState = 2;
            targetColor  = glm::vec3(0.447, 0.364, 0.0);
        }
        else
        {
            currentState = 3;
            targetColor  = glm::vec3(0.0, 0.639, 1.0); // Rouge
        }
    }

    void markov_setp1(int current_state)
    {
        switch (current_state)
        {
        case 0:
            markov_setp2(matriceTransition[0]);
            break;
        case 1:
            markov_setp2(matriceTransition[1]);
            break;
        case 2:
            markov_setp2(matriceTransition[2]);
            break;

        case 3:
            markov_setp2(matriceTransition[3]);
            break;
        }
    }

    // Fonction pour réaliser une transition fluide entre la couleur actuelle et la couleur cible
    void smoothTransition(float t)
    {
        background_color = glm::mix(background_color, targetColor, t);
        // std::cout << "x:" << background_color.x << "y:" << background_color.y << "z:" << background_color.z << std::endl;
    }

    ~ImguiInterface() {}
};
