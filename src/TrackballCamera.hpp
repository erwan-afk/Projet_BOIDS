#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"

class FreeflyCamera {
public:
    glm::vec3 m_Position;

private:
    float m_Phi;
    float m_Theta;

    glm::vec3 m_FrontVector;
    glm::vec3 m_LeftVector;
    glm::vec3 m_UpVector;

    // Private method to compute direction vectors
    void computeDirectionVectors()
    {
        m_FrontVector.x = std::sin(m_Theta) * std::cos(m_Phi);
        m_FrontVector.y = std::sin(m_Phi);
        m_FrontVector.z = std::cos(m_Theta) * std::cos(m_Phi);
        m_LeftVector    = glm::normalize(glm::cross(m_FrontVector, glm::vec3(0.0f, 1.0f, 0.0f)));
        m_UpVector      = glm::normalize(glm::cross(m_LeftVector, m_FrontVector));
    }

public:
    // Constructor
    FreeflyCamera()
        : m_Position(0.0f), m_Phi(glm::pi<float>()), m_Theta(glm::pi<float>()), m_FrontVector(0.0f, 0.0f, -1.0f), m_LeftVector(-1.0f, 0.0f, 0.0f), m_UpVector(0.0f, 1.0f, 0.0f)
    {
        computeDirectionVectors();
    }

    // Move along left vector
    void moveLeft(float t) { m_Position += t * m_LeftVector; }

    // Move along front vector
    void moveFront(float t) { m_Position += t * m_FrontVector; }

    // Rotate left
    void rotateLeft(float degrees)
    {
        float radians = glm::radians(degrees);
        m_Theta += radians;
        computeDirectionVectors();
    }

    // Rotate up
    void rotateUp(float degrees)
    {
        float radians = glm::radians(degrees);
        m_Phi += radians;
        computeDirectionVectors();
        // std::cout << m_Phi << std::endl;
    }

    // Get view matrix
    glm::mat4 getViewMatrix() const
    {
        return glm::lookAt(m_Position, m_Position + m_FrontVector, m_UpVector);
    }

    // Get position
    glm::vec3 getPosition() const { return glm::vec3(m_Position.x, m_Position.y - 0.2f, m_Position.z + 0.5f); }

    float getPhi()
    {
        return m_Phi;
    }

    float getTheta()
    {
        return m_Theta;
    }
    // Get front vector
    glm::vec3 getFrontVector() const { return m_FrontVector; }

    glm::vec3 getLeftVector() const
    {
        return m_LeftVector;
    }

    glm::vec3 getUpVector() const
    {
        return m_UpVector;
    }
};

class TrackballCamera {
private:
public:
    float m_RepereX;
    float m_RepereY;
    float m_RepereZ;
    float deltamousX;
    float deltamousY;
    float m_AngleX;
    float m_AngleY;
    float m_Distance;
    // Constructeur
    TrackballCamera()
        : m_Distance(0.4f), m_AngleX(0.0f), m_AngleY(0.0f), m_RepereX(0.0f), m_RepereY(0.0f), m_RepereZ(0.0f), deltamousX(0.0f), deltamousY(0.0f) {}

    // Autres membres et méthodes de la classe peuvent être ajoutés ici selon les besoins
    // Méthodes pour déplacer et tourner la caméra
    void moveFront(float delta)
    {
        m_Distance += delta;
    }

    void moveUp(float delta)
    {
        m_RepereY += delta;
    }

    void moveLeft(float delta)
    {
        m_RepereX += delta;
    }

    void moveForward(float delta)
    {
        m_RepereZ += delta;
    }

    void rotateLeft(float degrees)
    {
        m_AngleY += degrees;
    }

    void rotateUp(float degrees)
    {
        m_AngleX += degrees;
    }

    glm::vec3 getPositionRepere()
    {
        return -glm::vec3(m_RepereX, m_RepereY, m_RepereZ);
    }

    void setDeltaMouse(glm::vec2 delta)
    {
        deltamousX = delta.x;
        deltamousY = delta.y;
    }

    // Méthode pour calculer la matrice de vue (ViewMatrix)
    glm::mat4 getViewMatrix() const
    {
        // Convertir les angles en radians
        float angleX_rad = glm::radians(m_AngleX);
        float angleY_rad = glm::radians(m_AngleY);

        // Construction de la matrice de rotation autour de l'axe X
        glm::mat4 Scale = glm::rotate(glm::mat4(1.0f), angleX_rad, glm::vec3(1.0f, 0.0f, 0.0f));

        glm::mat4 translationOriginel = glm::translate(glm::mat4(1.0f), glm::vec3(-m_RepereX, -m_RepereY, -m_RepereZ));
        // Construction de la matrice de translation
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -m_Distance));

        // Construction de la matrice de rotation autour de l'axe X
        glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), angleX_rad, glm::vec3(1.0f, 0.0f, 0.0f));

        // Construction de la matrice de rotation autour de l'axe Y
        glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), angleY_rad, glm::vec3(0.0f, 1.0f, 0.0f));

        // La matrice de vue est obtenue par la multiplication des trois matrices
        glm::mat4 viewMatrix = translation * rotationX * rotationY * translationOriginel;

        return viewMatrix;
    }
};

class Arpenteur {
private:
    glm::vec3 m_Position;
    float     m_Yaw; // Angle de rotation autour de l'axe vertical

public:
    Arpenteur(const glm::vec3& position, float yaw = glm::pi<float>())
        : m_Position(position), m_Yaw(yaw) {}

    void moveLeft(float distance)
    {
        m_Position.x += distance * cos(m_Yaw);
        m_Position.z += distance * sin(m_Yaw);
    }

    void moveFront(float distance)
    {
        // Calcul de la direction perpendiculaire à l'angle de rotation actuel (yaw)
        // Cette direction est calculée en tournant l'angle de 90 degrés dans le sens inverse des aiguilles d'une montre
        float perpendicularYaw = m_Yaw + glm::pi<float>() / 2.0f;

        // Déplace le mesh dans la direction perpendiculaire spécifiée
        m_Position.x += distance * cos(perpendicularYaw);
        m_Position.z += distance * sin(perpendicularYaw);
    }

    void rotateLeft(float angle)
    {
        m_Yaw -= angle;
    }

    void setPosition(const glm::vec3& position)
    {
        m_Position = position;
    }

    void setOrientation(float yaw)
    {
        m_Yaw = yaw;
    }

    glm::vec3 getPosition() const
    {
        return m_Position;
    }

    float getYaw() const
    {
        return m_Yaw;
    }

    // Méthode pour déplacer le mesh dans la direction regardée par la caméra
    void moveInDirection(const glm::vec3& direction, float distance)
    {
        // Déplace le mesh dans la direction spécifiée
        m_Position += direction * distance;
    }

    void moveLeftOfDirection(const glm::vec3& direction, float distance)
    {
        // Calculer un vecteur perpendiculaire à la direction spécifiée
        glm::vec3 leftDirection = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), direction));

        // Déplacer le mesh dans la direction perpendiculaire calculée
        m_Position += leftDirection * distance;
    }

    // Méthode pour obtenir la direction de vue de la caméra en fonction de son angle de rotation
    glm::vec3 getCameraDirection(const FreeflyCamera& camera) const
    {
        // Utilisez le vecteur de direction frontale de la caméra
        return camera.getFrontVector();
    }
};

class ThirdPersonCamera {
private:
    const Arpenteur& m_Arpenteur;
    float            m_Distance;
    float            m_Phi;

public:
    ThirdPersonCamera(const Arpenteur& arpenteur, float distance = 0.4f)
        : m_Arpenteur(arpenteur), m_Distance(distance), m_Phi(glm::pi<float>()) {}

    glm::mat4 getViewMatrix() const
    {
        glm::vec3 cameraPosition;
        cameraPosition.x = m_Arpenteur.getPosition().x + m_Distance * sin(m_Phi) * cos(m_Arpenteur.getYaw());
        cameraPosition.y = m_Arpenteur.getPosition().y + m_Distance * sin(m_Phi);
        cameraPosition.z = m_Arpenteur.getPosition().z + m_Distance * cos(m_Phi) * cos(m_Arpenteur.getYaw());

        return glm::lookAt(cameraPosition, m_Arpenteur.getPosition(), glm::vec3(0.0f, 1.0f, 0.0f));
    }
};