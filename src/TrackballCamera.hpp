#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>


class FreeflyCamera {
private:
    glm::vec3 m_Position;
    float     m_Phi;
    float     m_Theta;

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
        : m_Position(0.0f), m_Phi(glm::pi<float>()), m_Theta(0.0f), m_FrontVector(0.0f, 0.0f, -1.0f), m_LeftVector(-1.0f, 0.0f, 0.0f), m_UpVector(0.0f, 1.0f, 0.0f)
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
    }

    // Get view matrix
    glm::mat4 getViewMatrix() const
    {
        return glm::lookAt(m_Position, m_Position + m_FrontVector, m_UpVector);
    }
};

class TrackballCamera {
private:
    float m_Distance;
    float m_AngleX;
    float m_AngleY;

public:
    // Constructeur
    TrackballCamera()
        : m_Distance(2.0f), m_AngleX(0.0f), m_AngleY(0.0f) {}

    // Autres membres et méthodes de la classe peuvent être ajoutés ici selon les besoins
    // Méthodes pour déplacer et tourner la caméra
    void moveFront(float delta)
    {
        m_Distance += delta;
    }

    void rotateLeft(float degrees)
    {
        m_AngleY += degrees;
    }

    void rotateUp(float degrees)
    {
        m_AngleX += degrees;
    }

    // Méthode pour calculer la matrice de vue (ViewMatrix)
    glm::mat4 getViewMatrix() const
    {
        // Convertir les angles en radians
        float angleX_rad = glm::radians(m_AngleX);
        float angleY_rad = glm::radians(m_AngleY);

        // Construction de la matrice de translation
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -m_Distance));

        // Construction de la matrice de rotation autour de l'axe X
        glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), angleX_rad, glm::vec3(1.0f, 0.0f, 0.0f));

        // Construction de la matrice de rotation autour de l'axe Y
        glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), angleY_rad, glm::vec3(0.0f, 1.0f, 0.0f));

        // La matrice de vue est obtenue par la multiplication des trois matrices
        glm::mat4 viewMatrix = translation * rotationX * rotationY;

        return viewMatrix;
    }
};