#pragma once

#include <cmath>
#include <iostream>


class Boid {
private:
    float posX;
    float posY;

    double velocityX;
    double velocityY;

public:
    //Constructeur
    Boid(float initPosX, float initPosY, float initVelocityX, float initVelocityY);


    float getPosX() const; 
    float getPosY() const; 

    void updatePosition(double deltaTime);
    float getSpeed() const;
    float getDirection() const;
    void printPosition() const;
    

    // Destructeur
    ~Boid();
};
