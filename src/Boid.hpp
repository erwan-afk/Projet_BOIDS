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

    float opt_maxSpeed = 4.0;
    float opt_minSpeed;

    float alignmentfact;
    float biasfact;
    float cohesionfact;
    float separationfact;
    float maxForce;
    float vision;

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

    void setImguiFactor(float boidVisionFact, float separationForceFact, float cohesionForceFact, float alignForceFact, float alignBiasFact, float minSpeedFact);

    /*Update et affichage*/
    void updatePosition(p6::Context& ctx);
    void show(p6::Context& ctx) const;

    void showOpenGL(p6::Context& ctx, ModelShader& modelShader, glm::mat4 ProjMatrix, glm::mat4 viewMatrix, ModelMesh& fish2, ModelMesh& fish2_tail) const;

    /*Physique*/
    void      flock(std::vector<Boid*> const& Boids, p6::Context& ctx);
    glm::vec3 align(const std::vector<Boid*>& Boids, const float opt_bias, const float opt_maxSpeed, const float opt_maxForce);
    glm::vec3 cohesion(const std::vector<Boid*>& Boids, const float opt_maxSpeed, const float opt_maxForce);
    glm::vec3 separation(const std::vector<Boid*>& Boids, const float opt_maxSpeed, const float opt_maxForce);
    glm::vec3 separationEdges(std::vector<Boid*> const& Boids, p6::Context& ctx);

    /*Fonction qui ne servent pas pour l'instant*/
    float     getSpeed() const;
    glm::vec3 getDirection() const;
    void      printPosition() const;

    // Destructeur
    ~Boid();
};

/*

class Boid {
public:
    bool predator;
    Pvector location;
    Pvector velocity;
    Pvector acceleration;
    float maxSpeed;
    float maxForce;
    Boid() {}
    Boid(float x, float y);
    Boid(float x, float y, bool predCheck);
    void applyForce(const Pvector& force);
    // Three Laws that boids follow
    Pvector Separation(const vector<Boid>& Boids);
    Pvector Alignment(const vector<Boid>& Boids);
    Pvector Cohesion(const vector<Boid>& Boids);
    //Functions involving SFML and visualisation linking
    Pvector seek(const Pvector& v);
    void run(const vector<Boid>& v);
    void update();
    void flock(const vector<Boid>& v);
    void borders();
    float angle(const Pvector& v);
};*/