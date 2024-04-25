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
    float     m_Phi;
    float     m_Theta;

private:
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
        : m_Position(glm::vec3(0.93, -0.76, 0.94)), m_Phi(glm::pi<float>()), m_Theta(glm::pi<float>() - 1.4f), m_FrontVector(0.0f, 0.0f, -1.0f), m_LeftVector(-1.0f, 0.0f, 0.0f), m_UpVector(0.0f, 1.0f, 0.0f)
    {
        computeDirectionVectors();
    }

    // Move along left vector
    void moveLeft(float t)
    {
        glm::vec3 newPosition = m_Position + t * m_LeftVector;

        // Vérifier chaque composante de la nouvelle position
        if (newPosition.x >= -1.0f && newPosition.x <= 1.0f && newPosition.y >= -1.0f && newPosition.y <= 1.0f && newPosition.z >= -1.0f && newPosition.z <= 1.0f)
        {
            // Si toutes les composantes sont dans la plage [-1.0, 1.0], mettre à jour la position
            m_Position = newPosition;
        }
    }

    // Move along front vector
    void moveFront(float t)
    {
        glm::vec3 newPosition = m_Position + t * m_FrontVector;

        // Vérifier chaque composante de la nouvelle position
        if (newPosition.x >= -1.0f && newPosition.x <= 1.0f && newPosition.y >= -1.0f && newPosition.y <= 1.0f && newPosition.z >= -1.0f && newPosition.z <= 1.0f)
        {
            // Si aucune composante ne dépasse 1.0, mettre à jour la position
            m_Position = newPosition;
        }
    }
    void moveUp(float t)
    {
        glm::vec3 newPosition = m_Position + t * m_UpVector;

        // Vérifier chaque composante de la nouvelle position
        if (newPosition.x >= -1.0f && newPosition.x <= 1.0f && newPosition.y >= -1.0f && newPosition.y <= 1.0f && newPosition.z >= -1.0f && newPosition.z <= 1.0f)
        {
            // Si toutes les composantes sont dans la plage [-1.0, 1.0], mettre à jour la position
            m_Position = newPosition;
        }
    }

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
    glm::vec3 getPosition() const { return glm::vec3(m_Position.x, m_Position.y, m_Position.z); }

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
