#include "Boid.hpp"



Boid::Boid(float initPosX, float initPosY, float initVelocityX, float initVelocityY)
    : posX(initPosX), posY(initPosY), velocityX(initVelocityX), velocityY(initVelocityY) {}

void Boid::updatePosition(double deltaTime)
{
    posX += velocityX * deltaTime;
    posX += velocityY * deltaTime;
}

float Boid::getPosX() const {
    return posX; 
}
float Boid::getPosY() const {
    return posY; 
}


float Boid::getSpeed() const
{
    return std::sqrt(velocityX * velocityX + velocityY * velocityY);
}


float Boid::getDirection() const
{
    return std::atan2(velocityY, velocityX);  
}

void Boid::printPosition() const
{
    std::cout << "posX = " << posX << " | posY = " << posY << std::endl; 
}

Boid::~Boid() {}
