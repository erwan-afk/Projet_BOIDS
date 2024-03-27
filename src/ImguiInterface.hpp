#pragma once

#include <p6/p6.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Simulation.hpp"

class ImguiInterface {
private:
    glm::vec3 background_color;
    float     separationPerception;
    float     cohesionPerception;
    float     alignPerception;
    float     speedFactor;

public:
    ImguiInterface(const glm::vec3& background_color = glm::vec3(0.0, 0.639, 1.0), // Default background color
                   float separationPerception = 0.0f, float cohesionPerception = 0.0f, float alignPerception = 0.0f, float speedfactor = 1.0)
        : background_color(background_color), separationPerception(separationPerception), cohesionPerception(cohesionPerception), alignPerception(alignPerception), speedFactor(speedfactor)
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
        ImGui::SliderFloat("Separation", &this->separationPerception, 0.0, 1.0);
        ImGui::SliderFloat("Cohesion", &this->cohesionPerception, 0.0, 1.0);
        ImGui::SliderFloat("Align", &this->alignPerception, 0.0, 1.0);
        ImGui::SliderFloat("Speed", &this->speedFactor, 1.0, 5.0);
        ImGui::ColorPicker4("Background Color", (float*)&this->background_color);
        ImGui::End();

        // set ImGui Options
        setImguiFactorSeparation(this->separationPerception * 0.02, flock);
        setImguiFactorCohesion(this->cohesionPerception * 0.0005, flock);
        setImguiFactorAlign(this->alignPerception * 0.35, flock);
    }

    ~ImguiInterface() {}
};
