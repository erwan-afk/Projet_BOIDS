#include "Boid.hpp"
#include <algorithm>
#include <iostream>
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"

Boid::Boid(float initPosX, float initPosY, float initPosZ, float initTailAngle, float initVelocityX, float initVelocityY, float initVelocityZ)
    : posX(initPosX), posY(initPosY), posZ(initPosZ), tailAngle(initTailAngle), velocityX(initVelocityX), velocityY(initVelocityY), velocityZ(initVelocityZ) {}

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

float Boid::getTailAngle() const
{
    return this->tailAngle;
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

void Boid::setImguiFactorAlign(float value)
{
    this->imguiFactorAlign = value;
}
void Boid::setImguiFactorCohesion(float value)
{
    this->imguiFactorCohesion = value;
}
void Boid::setImguiFactorSeparation(float value)
{
    this->imguiFactorSeparation = value;
}

void Boid::setImguiQualityBoids(bool value)
{
    this->imguiQualityBoids = value;
}

void Boid::updatePosition(p6::Context& ctx, float speedFactor)
{
    posX += (velocityX * ctx.delta_time()) * speedFactor;
    posY += (velocityY * ctx.delta_time()) * speedFactor;
    posZ += (velocityZ * ctx.delta_time()) * speedFactor;

    // Déclarer une variable pour la direction de l'oscillation

    // Dans votre fonction updatePosition
    // Calculer l'oscillation de la queue
    float oscillationStep = 0.001f * speedFactor; // Pas d'oscillation
    if (oscillationDirection)
    {
        tailAngle += oscillationStep; // Augmenter l'angle
        if (tailAngle >= 0.2f)
        {
            oscillationDirection = false; // Changer de direction
        }
    }
    else
    {
        tailAngle -= oscillationStep; // Diminuer l'angle
        if (tailAngle <= -0.2f)
        {
            oscillationDirection = true; // Changer de direction
        }
    }

    // std::cout << tailAngle << std::endl;

    velocityX += accelerationX;
    velocityY += accelerationY;
    velocityZ += accelerationZ;

    accelerationX = 0;
    accelerationY = 0;
    accelerationZ = 0;
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
    const float minSpeed = 0.002; // Ajustez cette valeur selon vos besoins

    const float alignmentFactor = 0.1; // Ajustez cette valeur selon vos besoins

    const float cohesionFactor = 0.001; // Ajustez cette valeur selon vos besoins

    const float separationsFactor = 0.0002; // Ajustez cette valeur selon vos besoins

    const float separationsEdgesFactor = 0.000001; // Ajustez cette valeur selon vos besoins

    glm::vec3 alignment      = align(Boids);
    glm::vec3 cohesions      = cohesion(Boids);
    glm::vec3 separations    = separation(Boids);
    glm::vec3 separationEdge = separationEdges(Boids, ctx);

    // Appliquer l'alignement comme une fraction de la moyenne des vélocités des voisins

    // accelerationX = (alignment.x * alignmentFactor + cohesions.x * cohesionFactor + separations.x * separationsFactor * this->imguiFactor + separationEdge.x * separationsEdgesFactor);
    // accelerationY = (alignment.y * alignmentFactor + cohesions.y * cohesionFactor + separations.y * separationsFactor * this->imguiFactor + separationEdge.y * separationsEdgesFactor);
    // accelerationZ = (alignment.z * alignmentFactor + cohesions.z * cohesionFactor + separations.z * separationsFactor * this->imguiFactor + separationEdge.z * separationsEdgesFactor);
    accelerationX = (alignment.x * alignmentFactor + cohesions.x * cohesionFactor + separations.x * separationsFactor + separationEdge.x * separationsEdgesFactor);
    accelerationY = (alignment.y * alignmentFactor + cohesions.y * cohesionFactor + separations.y * separationsFactor + separationEdge.y * separationsEdgesFactor);
    accelerationZ = (alignment.z * alignmentFactor + cohesions.z * cohesionFactor + separations.z * separationsFactor + separationEdge.z * separationsEdgesFactor);

    /*
    accelerationX = separations.first * separationsFactor;
    accelerationY = separations.second * separationsFactor;
    */
    // Limiter la vitesse à minSpeed
    float speed = std::sqrt(velocityX * velocityX + velocityY * velocityY + velocityZ * velocityZ);
    if (speed < minSpeed)
    {
        float speedScale = minSpeed / speed;
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
// Fonction pour calculer la distance entre deux vecteurs
float Boid::distanceCamera(const glm::vec3& v1, const glm::vec3& v2) const
{
    return glm::distance(v1, v2);
}

bool Boid::CloseEnough(const glm::vec3& v1, const glm::vec3& v2, float threshold) const
{
    return distanceCamera(v1, v2) <= threshold * threshold;
}

void Boid::showOpenGL(p6::Context& ctx, ModelShader& Shader, glm::mat4 ProjMatrix, glm::mat4 viewMatrix, ModelMesh& fish2, ModelMesh& fish2_low, ModelMesh& fish2_tail, FreeflyCamera& camera2) const
{
    // Position de la sphère dans l'espace view
    glm::vec3 spherePosition = glm::vec3(getPosX(), getPosY(), getPosZ());
    glm::vec3 CameraPosition = camera2.getPosition();

    if (CloseEnough(spherePosition, CameraPosition, 1.5f))
    {
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

        glm::mat4 MVMatrix = viewMatrix * glm::translate(glm::mat4{1.f}, spherePosition) * glm::scale(glm::mat4{1.f}, glm::vec3(0.08f)) * rotationMatrix;

        if (CloseEnough(spherePosition, CameraPosition, 1.0f) && imguiQualityBoids)
        {
            fish2.Draw(Shader, ProjMatrix, MVMatrix, viewMatrix);
            fish2_tail.Draw(Shader, ProjMatrix, MVMatrix * glm::rotate(glm::mat4(1.0f), getTailAngle(), glm::vec3(1.0f, 0.0f, 0.0f)), viewMatrix);
        }
        else
        {
            fish2_low.Draw(Shader, ProjMatrix, MVMatrix, viewMatrix);
            fish2_tail.Draw(Shader, ProjMatrix, MVMatrix * glm::rotate(glm::mat4(1.0f), getTailAngle(), glm::vec3(1.0f, 0.0f, 0.0f)), viewMatrix);
        }
    }
}

glm::vec3 Boid::align(const std::vector<Boid*>& Boids)
{
    float perception = this->imguiFactorAlign;
    float total      = 0;

    glm::vec3 avgVelocity(0.0f);

    for (const auto& other : Boids)
    {
        glm::vec3 delta = glm::vec3(other->getPosX() - getPosX(), other->getPosY() - getPosY(), other->getPosZ() - getPosZ());

        // Calculer la distance euclidienne entre les deux boids
        float distance = glm::length(delta);

        if (other != this && distance < perception)
        {
            // Ajouter les composantes de la vélocité à avgVelocity
            avgVelocity += glm::vec3(other->getVelocityX(), other->getVelocityY(), other->getVelocityZ());
            total++;
        }
    }

    if (total > 0)
    {
        // Calculer la moyenne des composantes de vélocité
        avgVelocity /= static_cast<float>(total);

        // Soustraire la vélocité actuelle du boid
        avgVelocity -= glm::vec3(velocityX, velocityY, velocityZ);
    }

    return avgVelocity;
}

glm::vec3 Boid::cohesion(const std::vector<Boid*>& Boids)
{
    float perception = this->imguiFactorCohesion;
    float total      = 0;

    glm::vec3 avgPosition(0.0f);
    glm::vec3 avgVelocity(0.0f);

    for (const auto& other : Boids)
    {
        glm::vec3 delta = glm::vec3(other->getPosX() - getPosX(), other->getPosY() - getPosY(), other->getPosZ() - getPosZ());

        // Calculer la distance euclidienne entre les deux boids
        float distance = glm::length(delta);

        if (other != this && distance < perception)
        {
            // Ajouter les composantes de la position et de la vélocité à avgPosition et avgVelocity
            avgPosition += glm::vec3(other->getPosX(), other->getPosY(), other->getPosZ());
            avgVelocity += glm::vec3(other->getVelocityX(), other->getVelocityY(), other->getVelocityZ());
            total++;
        }
    }

    if (total > 0)
    {
        // Calculer la moyenne des composantes de position et de vélocité
        avgPosition /= static_cast<float>(total);
        avgVelocity /= static_cast<float>(total);

        // Soustraire la position et la vélocité actuelles du boid
        avgPosition -= glm::vec3(posX, posY, posZ);
        avgVelocity -= glm::vec3(velocityX, velocityY, velocityZ);
    }

    return avgPosition + avgVelocity;
}

glm::vec3 Boid::separation(const std::vector<Boid*>& Boids)
{
    float perception = this->imguiFactorSeparation;
    float total      = 0;

    glm::vec3 avgVelocity(0.0f);

    for (const auto& other : Boids)
    {
        glm::vec3 delta = glm::vec3(other->getPosX() - getPosX(), other->getPosY() - getPosY(), other->getPosZ() - getPosZ());

        // Calculer la distance euclidienne entre les deux boids
        float distance = glm::length(delta);

        if (other != this && distance < perception)
        {
            // Ajouter les composantes normalisées de la vélocité à avgVelocity
            glm::vec3 normalizedDelta = glm::normalize(delta);
            avgVelocity -= normalizedDelta; // Inverser le vecteur de direction
            total++;
        }
    }

    if (total > 0)
    {
        // Calculer la moyenne des composantes de vélocité
        avgVelocity /= static_cast<float>(total);

        // Ajouter la vélocité actuelle du boid
        avgVelocity += glm::vec3(velocityX, velocityY, velocityZ);
    }

    return avgVelocity;
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
