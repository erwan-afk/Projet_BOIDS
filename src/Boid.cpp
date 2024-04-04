#include "Boid.hpp"
#include <algorithm>
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"

Boid::Boid(float initPosX, float initPosY, float initPosZ, float initVelocityX, float initVelocityY, float initVelocityZ)
    : posX(initPosX), posY(initPosY), posZ(initPosZ), velocityX(initVelocityX), velocityY(initVelocityY), velocityZ(initVelocityZ) {}

float Boid::getPosX() const
{
    return this->posX;
}
float Boid::getPosY() const
{
    return this->posY;
}
float Boid::getPosZ() const
{
    return this->posZ;
}

float Boid::getVelocityX() const
{
    return this->velocityX;
}
float Boid::getVelocityY() const
{
    return this->velocityY;
}
float Boid::getVelocityZ() const
{
    return this->velocityZ;
}

void Boid::setImguiFactor(float boidVisionFact, float separationForceFact, float cohesionForceFact, float alignForceFact, float alignBiasFact, float minSpeedFact)
{
    this->opt_minSpeed = minSpeedFact;

    this->alignmentfact  = alignForceFact;
    this->biasfact       = alignBiasFact;
    this->cohesionfact   = cohesionForceFact;
    this->separationfact = separationForceFact;

    this->vision = boidVisionFact;
}

void Boid::updatePosition(p6::Context& ctx)
{
    // Update velocity
    velocityX += accelerationX * ctx.delta_time();
    velocityY += accelerationY * ctx.delta_time();
    velocityZ += accelerationZ * ctx.delta_time();

    // Apply minSpeed if enabled
    if (opt_minSpeed)
    {
        // std::cout << opt_minSpeed << std::endl;
        float speedSquared = velocityX * velocityX + velocityY * velocityY + velocityZ * velocityZ;
        if (speedSquared == 0)
        {
            // Randomize velocity if magnitude is zero
            velocityX = p6::random::number(-opt_minSpeed, opt_minSpeed);
            velocityY = p6::random::number(-opt_minSpeed, opt_minSpeed);
            velocityZ = p6::random::number(-opt_minSpeed, opt_minSpeed);
        }
        else if (speedSquared < opt_minSpeed * opt_minSpeed)
        {
            // Scale velocity to reach minSpeed if magnitude is less than minSpeed
            float speedScale = opt_minSpeed / std::sqrt(speedSquared);
            velocityX *= speedScale;
            velocityY *= speedScale;
            velocityZ *= speedScale;
        }
    }

    // Limit velocity to maxSpeed
    float speed = std::sqrt(velocityX * velocityX + velocityY * velocityY + velocityZ * velocityZ);
    if (speed > opt_maxSpeed)
    {
        float speedScale = opt_maxSpeed / speed;
        velocityX *= speedScale;
        velocityY *= speedScale;
        velocityZ *= speedScale;
    }

    // Update position
    posX += velocityX * ctx.delta_time();
    posY += velocityY * ctx.delta_time();
    posZ += velocityZ * ctx.delta_time();

    if (posX < -1.0 || posX > 1.0)
    {
        velocityX *= -1;
        posX = std::clamp(posX, -1.0f, 1.0f);
    }
    if (posY < -1.0 || posY > 1.0)
    {
        velocityY *= -1;
        posY = std::clamp(posY, -1.0f, 1.0f);
    }
    if (posZ < -1.0 || posZ > 1.0)
    {
        velocityZ *= -1;
        posZ = std::clamp(posZ, -1.0f, 1.0f);
    }
}

glm::vec3 Boid::getDirection() const
{
    glm::vec3 velocity(getVelocityX(), getVelocityY(), getVelocityZ());
    float     magnitude = glm::length(velocity);

    if (magnitude != 0.0f)
    {
        return velocity / magnitude;
    }
    else
    {
        // Gérer le cas où la magnitude est nulle
        return glm::vec3(0.0f, 0.0f, 0.0f);
    }
}

void Boid::flock(std::vector<Boid*> const& Boids, p6::Context& ctx)
{
    glm::vec3 alignment   = align(Boids, biasfact, opt_maxSpeed, maxForce);
    glm::vec3 cohesions   = cohesion(Boids, opt_maxSpeed, maxForce);
    glm::vec3 separations = separation(Boids, opt_maxSpeed, maxForce);

    accelerationX = (alignment.x * alignmentfact + cohesions.x * cohesionfact + separations.x * separationfact);
    accelerationY = (alignment.y * alignmentfact + cohesions.y * cohesionfact + separations.y * separationfact);
    accelerationZ = (alignment.z * alignmentfact + cohesions.z * cohesionfact + separations.z * separationfact);

    // Limiter la vitesse à minSpeed
    float speed = std::sqrt(velocityX * velocityX + velocityY * velocityY + velocityZ * velocityZ);
    if (speed < opt_minSpeed)
    {
        float speedScale = opt_minSpeed / speed;
        velocityX *= speedScale;
        velocityY *= speedScale;
        velocityZ *= speedScale;
    }
}

void Boid::show(p6::Context& ctx) const
{
    ctx.stroke        = {1.f, 0.7f, 0.2f};
    ctx.stroke_weight = .005f;
    ctx.use_stroke    = true;
    ctx.use_fill      = false;
    ctx.equilateral_triangle(p6::Center{getPosX(), getPosY()}, p6::Radius{0.03f}, p6::Rotation{getDirection()});
}

void Boid::showOpenGL(p6::Context& ctx, ModelShader& Shader, glm::mat4 ProjMatrix, glm::mat4 viewMatrix, ModelMesh& fish2) const
{
    // Position de la sphère dans l'espace view
    glm::vec3 spherePosition = glm::vec3(getPosX(), getPosY(), getPosZ());

    // Appliquer la matrice de vue à la matrice modèle-vue (MVMatrix)
    glm::vec3 direction(getVelocityX(), getVelocityY(), getVelocityZ());
    direction = glm::normalize(direction);
    glm::vec3 directionOrthogonale(-direction.y, direction.x, 0.0f);
    directionOrthogonale      = glm::normalize(directionOrthogonale);
    glm::vec3 directionFinale = glm::cross(direction, directionOrthogonale);
    directionFinale           = glm::normalize(directionFinale);

    glm::mat4 rotationMatrix(
        glm::vec4(directionFinale, 0.0f),
        glm::vec4(direction, 0.0f),
        glm::vec4(directionOrthogonale, 0.0f),
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
    );

    glm::mat4 MVMatrix = viewMatrix * glm::translate(glm::mat4{1.f}, spherePosition) * glm::scale(glm::mat4{1.f}, glm::vec3(0.015f)) * rotationMatrix;

    fish2.Draw(Shader, ProjMatrix, MVMatrix);
}

glm::vec3 Boid::align(const std::vector<Boid*>& Boids, const float opt_bias, const float opt_maxSpeed, const float opt_maxForce)
{
    glm::vec3 avgVelocity(0.0f);
    glm::vec3 thisVelocity(velocityX, velocityY, velocityZ);

    for (const auto& other : Boids)
    {
        glm::vec3 delta    = glm::vec3(other->getPosX() - getPosX(), other->getPosY() - getPosY(), other->getPosZ() - getPosZ());
        float     distance = glm::length(delta);

        if (other != this && distance < vision)
        {
            float b = opt_bias * glm::dot(glm::vec3(other->getVelocityX(), other->getVelocityY(), other->getVelocityZ()), thisVelocity);
            avgVelocity += glm::vec3(other->getVelocityX(), other->getVelocityY(), other->getVelocityZ()) * b;
        }
    }

    if (glm::length(avgVelocity) > 0)
    {
        avgVelocity = glm::normalize(avgVelocity) * opt_maxSpeed - thisVelocity;
        avgVelocity = glm::clamp(avgVelocity, -opt_maxForce, opt_maxForce);
    }

    return avgVelocity;
}

glm::vec3 Boid::cohesion(const std::vector<Boid*>& Boids, const float opt_maxSpeed, const float opt_maxForce)
{
    glm::vec3 avgPosition(0.0f);
    glm::vec3 thisPosition(posX, posY, posZ);

    for (const auto& other : Boids)
    {
        glm::vec3 delta    = glm::vec3(other->getPosX() - getPosX(), other->getPosY() - getPosY(), other->getPosZ() - getPosZ());
        float     distance = glm::length(delta);

        if (other != this && distance < vision)
        {
            avgPosition += glm::vec3(other->getPosX(), other->getPosY(), other->getPosZ());
        }
    }

    if (glm::length(avgPosition) > 0)
    {
        avgPosition /= static_cast<float>(Boids.size());
        glm::vec3 avgAcceleration = avgPosition - thisPosition;
        avgAcceleration           = glm::normalize(avgAcceleration) * opt_maxSpeed - glm::vec3(velocityX, velocityY, velocityZ);
        avgAcceleration           = glm::clamp(avgAcceleration, -opt_maxForce, opt_maxForce);
        return avgAcceleration;
    }

    return glm::vec3(0.0f);
}

glm::vec3 Boid::separation(const std::vector<Boid*>& Boids, const float opt_maxSpeed, const float opt_maxForce)
{
    glm::vec3 separationVector(0.0f);
    glm::vec3 thisPosition(posX, posY, posZ);

    for (const auto& other : Boids)
    {
        glm::vec3 delta    = glm::vec3(other->getPosX() - getPosX(), other->getPosY() - getPosY(), other->getPosZ() - getPosZ());
        float     distance = glm::length(delta);

        if (other != this && distance < vision)
        {
            glm::vec3 separation = glm::normalize(delta) / distance;
            separationVector += separation;
        }
    }

    if (glm::length(separationVector) > 0)
    {
        separationVector /= static_cast<float>(Boids.size());
        separationVector = glm::normalize(separationVector) * opt_maxSpeed - glm::vec3(velocityX, velocityY, velocityZ);
        separationVector = glm::clamp(separationVector, -opt_maxForce, opt_maxForce);
    }

    return separationVector;
}

glm::vec3 Boid::separationEdges(const std::vector<Boid*>& Boids, p6::Context& ctx)
{
    float perception = 0.005f;
    float total      = 0;

    glm::vec3 avgVelocity(0.0f);

    // Éviter les bords de l'écran
    float offsetScreen = 0.3f;
    float sizeScreenX  = 1.0f - offsetScreen;
    float sizeScreenY  = 1.0f - offsetScreen;
    float sizeScreenZ  = 1.0f - offsetScreen; // Assumption: Z dimension is within [0, 1]

    if (getPosX() < -sizeScreenX - perception)
    {
        avgVelocity.x += (perception - getPosX()) / perception;
        total++;
    }
    else if (getPosX() > sizeScreenX - perception)
    {
        avgVelocity.x += (sizeScreenX - perception - getPosX()) / perception;
        total++;
    }

    if (getPosY() < -sizeScreenY - perception)
    {
        avgVelocity.y += (perception - getPosY()) / perception;
        total++;
    }
    else if (getPosY() > sizeScreenY - perception)
    {
        avgVelocity.y += (sizeScreenY - perception - getPosY()) / perception;
        total++;
    }

    if (getPosZ() < -sizeScreenZ - perception)
    {
        avgVelocity.z += (perception - getPosZ()) / perception;
        total++;
    }
    else if (getPosZ() > sizeScreenZ - perception)
    {
        avgVelocity.z += (sizeScreenZ - perception - getPosZ()) / perception;
        total++;
    }

    // Moyenner la vélocité si des voisins sont trouvés
    if (total > 0)
    {
        avgVelocity /= static_cast<float>(total);
    }

    // Retourner la moyenne des composantes de vélocité
    return avgVelocity;
}

float Boid::getSpeed() const
{
    return std::sqrt(velocityX * velocityX + velocityY * velocityY + velocityZ * velocityZ);
}

void Boid::printPosition() const
{
    std::cout << "posX = " << posX << " | posY = " << posY << std::endl;
}

Boid::~Boid() {}
