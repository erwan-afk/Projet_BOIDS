#include "Boid.hpp"

Boid::Boid(float initPosX, float initPosY, float initVelocityX, float initVelocityY)
    : posX(initPosX), posY(initPosY), velocityX(initVelocityX), velocityY(initVelocityY) {}

float Boid::getPosX() const
{
    return this->posX;
}
float Boid::getPosY() const
{
    return this->posX;
}

void Boid::updatePosition(double deltaTime)
{
    posX += velocityX;
    posY += velocityY;

    std::cout << velocityX << std::endl;

    velocityX += accelerationX * deltaTime;
    velocityY += accelerationY * deltaTime;
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

void Boid::show(p6::Context& ctx) const
{
    ctx.fill = {1.f, 0.7f, 0.2f};
    ctx.circle(p6::Center{getPosX(), getPosY()}, p6::Radius{0.05f});
}

Boid::~Boid() {}
