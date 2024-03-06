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

float Boid::getSeparationPerception() const
{
    return this->separationPerception;
}
void Boid::setSeparationPerception(float value)
{
    this->separationPerception = value;
}

void Boid::updatePosition(double deltaTime)
{
    posX += velocityX;
    posY += velocityY;

    velocityX += accelerationX;
    velocityY += accelerationY;

    accelerationX = 0;
    accelerationY = 0;
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

void Boid::flock(std::vector<Boid*> const& Boids, p6::Context& ctx)
{
    const float alignmentFactor = 0.005; // Ajustez cette valeur selon vos besoins
    const float minSpeed        = 0.002; // Ajustez cette valeur selon vos besoins

    /* cohesion factor */
    const float cohesionFactor = 0.0002; // Ajustez cette valeur selon vos besoins

    const float separationsFactor = 0.00005; // Ajustez cette valeur selon vos besoins

    const float separationsEdgesFactor = 0.00001; // Ajustez cette valeur selon vos besoins

    std::pair<float, float> alignment      = align(Boids);
    std::pair<float, float> cohesions      = cohesion(Boids);
    std::pair<float, float> separations    = separation(Boids);
    std::pair<float, float> separationEdge = separationEdges(Boids, ctx);

    // Appliquer l'alignement comme une fraction de la moyenne des vélocités des voisins

    accelerationX = (alignment.first * alignmentFactor + cohesions.first * cohesionFactor + separations.first * separationsFactor + separationEdge.first * separationsEdgesFactor);
    accelerationY = (alignment.second * alignmentFactor + cohesions.second * cohesionFactor + separations.second * separationsFactor + separationEdge.second * separationsEdgesFactor);

    /*
    accelerationX = separations.first * separationsFactor;
    accelerationY = separations.second * separationsFactor;
    */
    // Limiter la vitesse à minSpeed
    float speed = std::sqrt(velocityX * velocityX + velocityY * velocityY);
    if (speed < minSpeed)
    {
        float speedScale = minSpeed / speed;
        velocityX *= speedScale;
        velocityY *= speedScale;
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

void Boid::showOpenGL(p6::Context& ctx, GLuint uMVPMatrixLocation, GLuint uMVMatrixLocation, GLuint uNormalMatrixLocation, glm::mat4 ProjMatrix, std::vector<glimac::ShapeVertex> vertices_sphere) const
{
    // Position de la sphère dans l'espace view
    glm::vec3 spherePosition = glm::vec3(getPosX(), getPosY(), -2.0f);

    // Calculez la matrice de modèle-vue en utilisant la translation
    glm::mat4 MVMatrix = glm::translate(glm::mat4(1.0f), spherePosition);
    // MVMatrix = glm::rotate(MVMatrix, ctx.time(), {0.f, 1.f, 0.f}); // Translation * Rotation
    MVMatrix = glm::scale(MVMatrix, glm::vec3{0.02f});

    // Calculer la matrice NormalMatrix
    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

    glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
    glUniformMatrix4fv(uMVMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVMatrix));
    glUniformMatrix4fv(uNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

    glDrawArrays(GL_TRIANGLES, 0, vertices_sphere.size());
}

std::pair<float, float> Boid::align(std::vector<Boid*> const& Boids)
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

std::pair<float, float> Boid::cohesion(std::vector<Boid*> const& Boids)
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

std::pair<float, float> Boid::separation(std::vector<Boid*> const& Boids)
{
    float total = 0;

    float perception = 0.1;

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

std::pair<float, float> Boid::separationEdges(std::vector<Boid*> const& Boids, p6::Context& ctx)
{
    float perception = 0.1;
    float total      = 0;

    float avgVelocityX = 0;
    float avgVelocityY = 0;

    /*

    for (const auto& other : Boids)
    {
        if (other != this)
        {
            float dx = other->getPosX() - getPosX();
            float dy = other->getPosY() - getPosY();

            // Calculer la distance euclidienne entre les deux boids
            float distance = std::sqrt(dx * dx + dy * dy);

            if (distance < perception)
            {
                // Ajouter les composantes de la vélocité à avgVelocityX et avgVelocityY
                avgVelocityX += dx / distance;
                avgVelocityY += dy / distance;
                total++;
            }
        }
    }

    */

    // Éviter les bords de l'écran
    float offsetScreen = 0.3;
    float sizeScreenX  = ctx.aspect_ratio() - offsetScreen;
    float sizeScreenY  = 1.0 - offsetScreen;

    if (getPosX() < -sizeScreenX - perception)
    {
        avgVelocityX += (perception - getPosX()) / perception;
        total++;
    }
    else if (getPosX() > sizeScreenX - perception)
    {
        avgVelocityX += (sizeScreenX - perception - getPosX()) / perception;
        total++;
    }
    else if (getPosX() < -sizeScreenX - perception)
    {
        avgVelocityX -= (sizeScreenX - perception - getPosX()) / perception;
        total++;
    }

    if (getPosY() < -sizeScreenY - perception)
    {
        avgVelocityY += (perception - getPosY()) / perception;
        total++;
    }
    else if (getPosY() > sizeScreenY - perception)
    {
        avgVelocityY += (sizeScreenY - perception - getPosY()) / perception;
        total++;
    }

    // Moyenner la vélocité si des voisins sont trouvés
    if (total > 0)
    {
        avgVelocityX /= total;
        avgVelocityY /= total;
    }

    // Retourner la moyenne des composantes de vélocité
    return std::make_pair(avgVelocityX, avgVelocityY);
}

float Boid::getSpeed() const
{
    return std::sqrt(velocityX * velocityX + velocityY * velocityY);
}

void Boid::printPosition() const
{
    std::cout << "posX = " << posX << " | posY = " << posY << std::endl;
}

Boid::~Boid() {}
