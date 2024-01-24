#include "Boid.hpp"



Boid::Boid(double initPosX, double initPosY, double initVelocityX, double initVelocityY)
    : posX(initPosX), posY(initPosY), velocityX(initVelocityX), velocityY(initVelocityY) {}

void Boid::updatePosition(double deltaTime)
{
    posX += velocityX * deltaTime;
    posX += velocityY * deltaTime;
}

double Boid::getSpeed() const
{
    return std::sqrt(velocityX * velocityX + velocityY * velocityX);
}

double Boid::getDirection() const
{
    return std::atan2(velocityY, velocityX);  
}

void Boid::printPosition() const
{
    std::cout << "posX = " << posX << " | posY = " << posY << std::endl; 
}

Boid::~Boid() {}
