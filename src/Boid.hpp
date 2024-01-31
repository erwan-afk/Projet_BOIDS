#pragma once

#include <cmath>
#include <iostream>


class Boid {
private:
    float posX;
    float posY;

<<<<<<< HEAD
    double velocityX;
    double velocityY;
    
=======
    float velocityX;
    float velocityY;

>>>>>>> 2f595985e811bdf25b89cf2d055feb9c24a43662
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
