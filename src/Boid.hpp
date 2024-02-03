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

    float accelerationX;
    float accelerationY;

public:
    // Constructeur
    Boid(float initPosX, float initPosY, float initVelocityX, float initVelocityY);

    float getPosX() const;
    float getPosY() const;

    void  updatePosition(double deltaTime);
    float getSpeed() const;
    float getDirection() const;
    void  printPosition() const;

    void show(p6::Context& ctx) const;

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