#pragma once

#include <cmath>
#include <iostream>


class Boid {
private:
    double posX;
    double posY;

    double velocityX;
    double velocityY;

public:
    // Constructeur
    Boid(double initPosX, double initPosY, double initVelocityX, double initVelocityY);

    void   updatePosition(double deltaTime);
    double getSpeed() const;
    double getDirection() const;
    void   printPosition() const;

    // Destructeur
    ~Boid();
};
