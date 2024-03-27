#pragma once

#include <p6/p6.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

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
    }

    ~ModelShader() {}

    GLuint uMVPMatrixLocation;
    GLuint uMVMatrixLocation;
    GLuint uNormalMatrixLocation;
    GLuint textureSamplerLocation;
    GLuint colorFog;

    // Méthode pour utiliser le shader
    void use() const
    {
        shader.use();
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

private:
    const p6::Shader shader;
};
