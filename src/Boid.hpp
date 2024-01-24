#pragma once

#include <iostream>
#include <cmath>

class Boid {
private:
    float posX;
    float posY;

    float velocityX;
    float velocityY;

public:
    //Constructeur
    Boid(float initPosX, float initPosY, float initVelocityX, float initVelocityY);


    float getPosX() const; 
    float getPosY() const; 

    void updatePosition(double deltaTime);
    float getSpeed() const;
    float getDirection() const;
    void printPosition() const;
    


    //Destructeur
    ~Boid(); 

    

    
 }; 