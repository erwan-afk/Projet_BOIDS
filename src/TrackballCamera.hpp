#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class TrackballCamera {
private:
    float m_Distance;
    float m_AngleX;
    float m_AngleY;

public:
    // Constructeur
    TrackballCamera()
        : m_Distance(5.0f), m_AngleX(0.0f), m_AngleY(0.0f) {}

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
        glm::mat4 viewMatrix = rotationY * rotationX * translation;

        return viewMatrix;
    }
};