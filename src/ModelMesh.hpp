#pragma once
#include <map>
#include <vector>
#include "ModelShader.hpp"
#include "glimac/common.hpp"

class UniqueBuffer {
public:
    UniqueBuffer()
    {
        glGenBuffers(1, &_id); // Do whatever you need to create the resource
    }
    ~UniqueBuffer()
    {
        glDeleteBuffers(1, &_id); // Do whatever you need to delete the resource
    }
    UniqueBuffer(const UniqueBuffer&)            = delete; // We disable copying
    UniqueBuffer& operator=(const UniqueBuffer&) = delete; // We disable copying
    UniqueBuffer(UniqueBuffer&& other) noexcept            // Move constructor
        : _id{other._id}
    {
        other._id = 0; // Make sure that other won't delete the _id we just copied
    }
    UniqueBuffer& operator=(UniqueBuffer&& other) noexcept // Move assignment operator
    {
        if (this != &other)
        {                             // Make sure that we don't do silly things when we try to move an object to itself
            glDeleteBuffers(1, &_id); // Delete the previous object
            _id       = other._id;    // Copy the object
            other._id = 0;            // Make sure that other won't delete the _id we just copied
        }
        return *this; // move assignment must return a reference to this, so we do it
    }

    GLuint id() const { return _id; } // The getter for the wrapped `_id`.

private:
    GLuint _id;
};

class UniqueVertexArray {
public:
    UniqueVertexArray()
    {
        glGenVertexArrays(1, &_id); // Générer le VAO
    }
    ~UniqueVertexArray()
    {
        glDeleteVertexArrays(1, &_id); // Supprimer le VAO
    }
    UniqueVertexArray(const UniqueVertexArray&)            = delete; // Désactiver la copie
    UniqueVertexArray& operator=(const UniqueVertexArray&) = delete; // Désactiver la copie
    UniqueVertexArray(UniqueVertexArray&& other) noexcept            // Constructeur de déplacement
        : _id{other._id}
    {
        other._id = 0; // Assurez-vous que 'other' ne supprime pas l'ID que nous avons copié
    }
    UniqueVertexArray& operator=(UniqueVertexArray&& other) noexcept // Opérateur d'affectation par déplacement
    {
        if (this != &other)
        {                                  // Assurez-vous que nous ne faisons pas de choses stupides lorsque nous essayons de déplacer un objet vers lui-même
            glDeleteVertexArrays(1, &_id); // Supprimer l'objet précédent
            _id       = other._id;         // Copier l'objet
            other._id = 0;                 // Assurez-vous que 'other' ne supprime pas l'ID que nous avons copié
        }
        return *this; // L'affectation par déplacement doit retourner une référence à 'this', nous le faisons donc
    }

    GLuint id() const { return _id; } // Getter pour l'ID encapsulé.

private:
    GLuint _id;
};

class Texture {
public:
    Texture()
    {
        glGenTextures(1, &_id); // Créer une nouvelle texture OpenGL
    }

    ~Texture()
    {
        glDeleteTextures(1, &_id); // Supprimer la texture OpenGL
    }

    // Désactiver la copie de l'objet Texture
    Texture(const Texture&)            = delete;
    Texture& operator=(const Texture&) = delete;

    // Activer le déplacement de l'objet Texture
    Texture(Texture&& other) noexcept
        : _id{other._id}
    {
        other._id = 0;
    }

    Texture& operator=(Texture&& other) noexcept
    {
        if (this != &other)
        {
            glDeleteTextures(1, &_id);
            _id       = other._id;
            other._id = 0;
        }
        return *this;
    }

    // Getter pour l'ID de la texture
    GLuint id() const { return _id; }

private:
    GLuint _id;
};

class OBJModel {
public:
    OBJModel();
    ~OBJModel();

    void                             LoadFromFile(const char* fileName);
    std::vector<glimac::ShapeVertex> GetVertexData();
    int                              GetVertexCount();
    Texture getTexture(); 

private:
    struct Position {
        float x, y, z;
    };
    struct textCoords {
        float x, y;
    };
    struct Normal {
        float x, y, z;
    };

    void                             LoadMaterialFile(const char* fileName);
    bool                             StartWith(std::string& line, const char* text);
    void                             AddVertexData(int vIdx, int nIdx, const char* mtl, std::vector<Position>& vertices, std::vector<Normal>& normals, std::vector<textCoords>& coord_texture);
    //std::map<std::string, Color>     MaterialMap;
    std::vector<glimac::ShapeVertex> shapeVertices;
};

class ModelMesh {
public:
    ModelMesh(const char* filePath);
    ~ModelMesh();

    void              Draw(ModelShader& Shader, glm::mat4 ProjMatrix, glm::mat4 MVMatrix);
    UniqueBuffer      vbo;
    UniqueVertexArray vao;
    Texture texture; 

private:
    void                             LoadModel(const char* filePath);
    std::vector<glimac::ShapeVertex> vertices;
    int                              vertexCount;
};