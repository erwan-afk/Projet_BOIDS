#pragma once

#include <p6/p6.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <utility>
#include <vector>
#include "Boid.hpp"
#include "ModelMesh.hpp"
#include "ModelShader.hpp"
#include "TrackballCamera.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "p6/p6.h"

class Boid {
private:
    float posX;
    float posY;
    float posZ;

    float tailAngle;
    bool  oscillationDirection = true; // true pour une augmentation, false pour une diminution

    float velocityX;
    float velocityY;
    float velocityZ;

    float accelerationX = 0.0;
    float accelerationY = 0.0;
    float accelerationZ = 0.0;

    float maxSpeed = 0.0;

    float imguiFactorAlign;
    float imguiFactorCohesion;
    float imguiFactorSeparation;

    float imguiQualityBoids;

public:
    // Constructeur
    Boid(float initPosX, float initPosY, float initPosZ, float initTailAngle, float initVelocityX, float initVelocityY, float initVelocityZ);
    /*Getter*/
    float getPosX() const;
    float getPosY() const;
    float getPosZ() const;

    float getTailAngle() const;

    float getVelocityX() const;
    float getVelocityY() const;
    float getVelocityZ() const;

    void setImguiFactorAlign(float value);
    void setImguiFactorCohesion(float value);
    void setImguiFactorSeparation(float value);
    void setImguiQualityBoids(bool value);

    /*Update et affichage*/
    void updatePosition(p6::Context& ctx, float speedFactor);
    void edges(p6::Context& ctx);
    void show(p6::Context& ctx) const;

    float distanceCamera(const glm::vec3& v1, const glm::vec3& v2) const;
    bool  CloseEnough(const glm::vec3& v1, const glm::vec3& v2, float threshold) const;

    void showOpenGL(p6::Context& ctx, ModelShader& modelShader, glm::mat4 ProjMatrix, glm::mat4 viewMatrix, ModelMesh& fish2, ModelMesh& fish2_low, ModelMesh& fish2_tail, FreeflyCamera& camera2) const;

    /*Physique*/
    void      flock(std::vector<Boid*> const& Boids, p6::Context& ctx);
    glm::vec3 align(std::vector<Boid*> const& Boids);
    glm::vec3 cohesion(std::vector<Boid*> const& Boids);
    glm::vec3 separation(std::vector<Boid*> const& Boids);
    glm::vec3 separationEdges(std::vector<Boid*> const& Boids, p6::Context& ctx);

    /*Fonction qui ne servent pas pour l'instant*/
    float     getSpeed() const;
    glm::vec3 getDirection() const;
    void      printPosition() const;

    // Destructeur
    ~Boid();
};
