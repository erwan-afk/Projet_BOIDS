#pragma once

#include <p6/p6.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "glm/fwd.hpp"

class Light {
public:
    Light(const glm::mat4& position, const glm::vec3& color, float intensity, float radius)
        : position(position), color(color), intensity(intensity), radius(radius) {}

    glm::mat4 position;
    glm::vec3 color;
    float     intensity;
    float     radius;
};

class ModelShader {
public:
    ModelShader(const std::string& vertexPath, const std::string& fragmentPath)
        : shader(p6::load_shader(vertexPath, fragmentPath))
    {
        // Obtenir les emplacements des uniformes après avoir chargé le shader
        uMVPMatrixLocation     = glGetUniformLocation(shader.id(), "uMVPMatrix");
        uMVMatrixLocation      = glGetUniformLocation(shader.id(), "uMVMatrix");
        uNormalMatrixLocation  = glGetUniformLocation(shader.id(), "uNormalMatrix");
        textureSamplerLocation = glGetUniformLocation(shader.id(), "textureSampler");
        colorFog               = glGetUniformLocation(shader.id(), "colorFog");
        uNumLightsLocation     = glGetUniformLocation(shader.id(), "uNumLights");

        color = glm::vec3(0.0, 0.0, 1.0);
    }

    ~ModelShader() {}

    GLuint uMVPMatrixLocation;
    GLuint uMVMatrixLocation;
    GLuint uNormalMatrixLocation;
    GLuint textureSamplerLocation;
    GLuint colorFog;
    GLuint uNumLightsLocation;

    glm::vec3 color;

    // Méthode pour utiliser le shader
    void use() const
    {
        shader.use();
    }
    // Méthodes pour ajouter et configurer des lumières
    void addLight(const Light& light)
    {
        lights.push_back(light);
    }

    void setLightPosition(unsigned int index, const glm::mat4& position)
    {
        if (index < lights.size())
        {
            lights[index].position = position;
        }
    }

    void setLightColor(unsigned int index, const glm::vec3& color)
    {
        if (index < lights.size())
        {
            lights[index].color = color;
        }
    }

    void setLightIntensity(unsigned int index, float intensity)
    {
        if (index < lights.size())
        {
            lights[index].intensity = intensity;
        }
    }

    // Méthode pour activer les lumières
    void enableLights() const
    {
        // Vérifier s'il y a des lumières à activer
        if (lights.empty())
        {
            glUniform1i(uNumLightsLocation, 0); // Aucune lumière à activer
            return;
        }

        // Envoyer le nombre de lumières au shader
        glUniform1i(uNumLightsLocation, static_cast<int>(lights.size()));

        // Envoyer les propriétés de chaque lumière aux uniforms
        for (unsigned int i = 0; i < lights.size(); ++i)
        {
            std::string lightUniformPrefix = "uLights[" + std::to_string(i) + "].";

            // Envoyer la position de la lumière
            glUniform3fv(glGetUniformLocation(shader.id(), (lightUniformPrefix + "position").c_str()), 1, glm::value_ptr(glm::vec3(lights[i].position[3])));

            // Envoyer la couleur de la lumière
            glUniform3fv(glGetUniformLocation(shader.id(), (lightUniformPrefix + "color").c_str()), 1, glm::value_ptr(lights[i].color));

            // Envoyer l'intensité de la lumière
            glUniform1f(glGetUniformLocation(shader.id(), (lightUniformPrefix + "intensity").c_str()), lights[i].intensity);

            glUniform1f(glGetUniformLocation(shader.id(), (lightUniformPrefix + "radius").c_str()), lights[i].radius);
        }
    }

    // Méthodes pour envoyer les matrices aux uniformes
    void setMVPMatrix(const glm::mat4& ProjMatrix, const glm::mat4& MVMatrix) const
    {
        glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
    }

    void setMVMatrix(const glm::mat4& MVMatrix) const
    {
        glUniformMatrix4fv(uMVMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVMatrix));
    }

    void setNormalMatrix(const glm::mat4& NormalMatrix) const
    {
        glUniformMatrix4fv(uNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
    }

    void setColorFog(const glm::vec3& color) const
    {
        glUniform3fv(colorFog, 1, glm::value_ptr(color));
    }

    void setColorLight(const glm::vec3& colorChanging)
    {
        glm::vec3 white = glm::vec3(1.0f, 1.0f, 1.0f);
        color           = colorChanging * 1.2f;
        // glUniform3fv(uLightColorLocation, 1, glm::value_ptr(color));
    }

    const p6::Shader   shader;
    std::vector<Light> lights;
};
