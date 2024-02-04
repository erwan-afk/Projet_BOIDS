#pragma once

#include <cmath>
#include <iostream>
#include "Boid.hpp"
#include "p6/p6.h"

class Boid {
private:
    float posX;
    float posY;

    float velocityX;
    float velocityY;

    float accelerationX = 0.0;
    float accelerationY = 0.0;

    float maxSpeed = 0.0;

public:
    // Constructeur
    Boid(float initPosX, float initPosY, float initVelocityX, float initVelocityY);
    /*Getter*/
    float getPosX() const;
    float getPosY() const;

    float getVelocityX() const;
    float getVelocityY() const;

    /*Update et affichage*/
    void updatePosition(double deltaTime);
    void edges();
    void show(p6::Context& ctx) const;

    /*Physique*/
    std::pair<float, float> align(std::vector<Boid*> Boids);
    void                    flock(std::vector<Boid*> Boids);

    /*Fonction qui ne servent pas pour l'instant*/
    float getSpeed() const;
    float getDirection() const;
    void  printPosition() const;

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