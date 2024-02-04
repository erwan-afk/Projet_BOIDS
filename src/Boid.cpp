#include "Boid.hpp"

Boid::Boid(float initPosX, float initPosY, float initVelocityX, float initVelocityY)
    : posX(initPosX), posY(initPosY), velocityX(initVelocityX), velocityY(initVelocityY) {}

float Boid::getPosX() const
{
    return this->posX;
}
float Boid::getPosY() const
{
    return this->posY;
}

float Boid::getVelocityX() const
{
    return this->velocityX;
}
float Boid::getVelocityY() const
{
    return this->velocityY;
}

void Boid::updatePosition(double deltaTime)
{
    posX += velocityX;
    posY += velocityY;

    velocityX += accelerationX;
    velocityY += accelerationY;
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
    ctx.fill       = {1.f, 0.7f, 0.2f};
    ctx.use_stroke = false;
    ctx.circle(p6::Center{getPosX(), getPosY()}, p6::Radius{0.05f});
}

std::pair<float, float> Boid::align(std::vector<Boid*> Boids)
{
    float perception = 0.4;
    float total      = 0;

    float avgVelocityX = 0;
    float avgVelocityY = 0;

    for (const auto& other : Boids)
    {
        float dx = other->getPosX() - getPosX();
        float dy = other->getPosY() - getPosY();

        // Calculer la distance euclidienne entre les deux boids
        float distance = std::sqrt(dx * dx + dy * dy);

        if (other != this && distance < perception)
        {
            // Ajouter les composantes de la vélocité à avgVelocityX et avgVelocityY
            avgVelocityX += other->getVelocityX();
            avgVelocityY += other->getVelocityY();
            total++;
        }
    }

    if (total > 0)
    {
        // Calculer la moyenne des composantes de vélocité
        avgVelocityX /= total;
        avgVelocityY /= total;

        // Soustraire la vélocité actuelle du boid
        avgVelocityX -= velocityX;
        avgVelocityY -= velocityY;
    }

    return std::make_pair(avgVelocityX, avgVelocityY);
}

void Boid::flock(std::vector<Boid*> Boids)
{
    const float alignmentFactor = 0.01; // Ajustez cette valeur selon vos besoins

    std::pair<float, float> alignment = align(Boids);

    // Appliquer l'alignement comme une fraction de la moyenne des vélocités des voisins
    accelerationX = alignment.first * alignmentFactor;
    accelerationY = alignment.second * alignmentFactor;
}

Boid::~Boid() {}
