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

    accelerationX *= 0;
    accelerationY *= 0;
}

void Boid::edges(p6::Context& ctx)
{
    if (getPosX() > ctx.aspect_ratio())
    {
        posX = -ctx.aspect_ratio();
    }
    else if (getPosX() < -ctx.aspect_ratio())
    {
        posX = ctx.aspect_ratio();
    }

    if (getPosY() > 1.0)
    {
        posY = -1.0;
    }
    else if (getPosY() < -1.0)
    {
        posY = 1.0;
    }
}

p6::Radians Boid::getDirection() const
{
    return p6::Radians(std::atan2(velocityY, velocityX));
}

void Boid::printPosition() const
{
    const float alignmentFactor = 0.005; // Ajustez cette valeur selon vos besoins
    const float minSpeed        = 0.002; // Ajustez cette valeur selon vos besoins

void Boid::show(p6::Context& ctx) const
{
    ctx.stroke        = {1.f, 0.7f, 0.2f};
    ctx.stroke_weight = .005f;
    ctx.use_stroke    = true;
    ctx.use_fill      = false;
    ctx.equilateral_triangle(p6::Center{getPosX(), getPosY()}, p6::Radius{0.03f}, p6::Rotation{getDirection()});
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

std::pair<float, float> Boid::cohesion(std::vector<Boid*> Boids)
{
    float perception = 0.2;
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
            avgVelocityX += other->getPosX();
            avgVelocityY += other->getPosY();
            total++;
        }
    }

    if (total > 0)
    {
        // Calculer la moyenne des composantes de vélocité
        avgVelocityX /= total;
        avgVelocityY /= total;

        // Soustraire la position actuelle du boid
        avgVelocityX -= posX;
        avgVelocityY -= posY;

        // Soustraire la vélocité actuelle du boid
        avgVelocityX -= velocityX;
        avgVelocityY -= velocityY;
    }

    return std::make_pair(avgVelocityX, avgVelocityY);
}

std::pair<float, float> Boid::separation(std::vector<Boid*> Boids)
{
    float perception = 0.1;
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
            float diffX = getPosX() - other->getPosX();
            float diffY = getPosY() - other->getPosY();

            diffX /= distance;
            diffY /= distance;

            avgVelocityX += diffX;
            avgVelocityY += diffY;
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
