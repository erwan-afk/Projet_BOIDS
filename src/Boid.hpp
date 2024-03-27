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

public:
    // Constructeur
    Boid(float initPosX, float initPosY, float initPosZ, float initVelocityX, float initVelocityY, float initVelocityZ);
    /*Getter*/
    float getPosX() const;
    float getPosY() const;
    float getPosZ() const;

    float getVelocityX() const;
    float getVelocityY() const;
    float getVelocityZ() const;

    void setImguiFactorAlign(float value);
    void setImguiFactorCohesion(float value);
    void setImguiFactorSeparation(float value);

    /*Update et affichage*/
    void updatePosition(p6::Context& ctx, float speedFactor);
    void edges(p6::Context& ctx);
    void show(p6::Context& ctx) const;

    void showOpenGL(p6::Context& ctx, ModelShader& modelShader, glm::mat4 ProjMatrix, glm::mat4 viewMatrix, ModelMesh& fish2) const;

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