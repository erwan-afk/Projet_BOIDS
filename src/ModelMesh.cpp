#include "ModelMesh.hpp"
#include <cstddef>
#include <fstream>
#include <iostream>
#include <vector>
#include "ModelShader.hpp"
#include "glm/fwd.hpp"

ModelMesh::ModelMesh(const char* filePath)
{
    vertexCount = 0;
    MVMatrix    = glm::mat4(1.0f);

    LoadModel(filePath);

    glBindBuffer(GL_ARRAY_BUFFER, vbo.id());
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glimac::ShapeVertex), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Use UniqueBuffer to automatically handle deletion

    GLuint vao = vaoBuffer.id();
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo.id());

    static constexpr GLuint vertex_attr_position = 0;
    glEnableVertexAttribArray(vertex_attr_position);
    glVertexAttribPointer(vertex_attr_position, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, position)));

    static constexpr GLuint vertex_attr_normal = 1;
    glEnableVertexAttribArray(vertex_attr_normal);
    glVertexAttribPointer(vertex_attr_normal, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, normal)));

    static constexpr GLuint vertex_attr_texCoords = 2;
    glEnableVertexAttribArray(vertex_attr_texCoords);
    glVertexAttribPointer(vertex_attr_texCoords, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, texCoords)));

    // Unbind the VAO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

ModelMesh::~ModelMesh()
{
}

void ModelMesh::Draw(ModelShader& Shader)
{
    Shader.setMVMatrix(MVMatrix);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void ModelMesh::LoadModel(const char* filePath)
{
    OBJModel objModel;
    objModel.LoadFromFile(filePath);

    vertices    = objModel.GetVertexData();
    vertexCount = objModel.GetVertexCount();
}

void OBJModel::LoadFromFile(const char* fileName)
{
    std::vector<Position> vertices;
    std::vector<Normal>   normals;

    std::ifstream file(fileName);
    if (file)
    {
        char        currentMtlName[100];
        std::string line;
        while (std::getline(file, line))
        {
            if (StartWith(line, "mtllib"))
            {
                char mtlFileName[100];
                (void)sscanf_s(line.c_str(), "mtllib %s", mtlFileName, sizeof(mtlFileName));
                // Ajoutez le préfixe au nom de fichier
                // Définir le préfixe
                const char* prefix = "../meshs/"; // Remplacez "prefix_" par le préfixe souhaité

                // Concaténer le préfixe avec le nom de fichier
                char prefixedFileName[100]; // Assurez-vous que la taille est suffisamment grande pour contenir le préfixe et le nom de fichier
                snprintf(prefixedFileName, sizeof(prefixedFileName), "%s%s", prefix, mtlFileName);
                std::cout << prefixedFileName << std::endl;

                // Charger le fichier de matériaux
                LoadMaterialFile(prefixedFileName);
            }
            if (StartWith(line, "v "))
            {
                Position pos;
                sscanf_s(line.c_str(), "v %f %f %f", &pos.x, &pos.y, &pos.z);
                vertices.push_back(pos);
            }
            if (StartWith(line, "vn "))
            {
                Normal n;
                sscanf_s(line.c_str(), "vn %f %f %f", &n.x, &n.y, &n.z);
                normals.push_back(n);
            }
            if (StartWith(line, "usemtl"))
            {
                (void)sscanf_s(line.c_str(), "usemtl %s", currentMtlName, sizeof(currentMtlName));
            }
            if (StartWith(line, "f "))
            {
                int v1, v2, v3;
                int n1, n2, n3;
                (void)sscanf_s(line.c_str(), "f %d/%*d/%d %d/%*d/%d %d/%*d/%d", &v1, &n1, &v2, &n2, &v3, &n3);

                // Créer les vertices de type glimac::ShapeVertex à partir des indices v1, v2, v3 et n1, n2, n3
                glimac::ShapeVertex shapeVertex1, shapeVertex2, shapeVertex3;
                shapeVertex1.position = glm::vec3(vertices[v1 - 1].x, vertices[v1 - 1].y, vertices[v1 - 1].z);
                shapeVertex1.normal   = glm::vec3(normals[n1 - 1].x, normals[n1 - 1].y, normals[n1 - 1].z);
                // Les coordonnées de texture sont ignorées dans ce format

                shapeVertex2.position = glm::vec3(vertices[v2 - 1].x, vertices[v2 - 1].y, vertices[v2 - 1].z);
                shapeVertex2.normal   = glm::vec3(normals[n2 - 1].x, normals[n2 - 1].y, normals[n2 - 1].z);

                shapeVertex3.position = glm::vec3(vertices[v3 - 1].x, vertices[v3 - 1].y, vertices[v3 - 1].z);
                shapeVertex3.normal   = glm::vec3(normals[n3 - 1].x, normals[n3 - 1].y, normals[n3 - 1].z);

                // Ajouter les vertices à shapeVertices
                shapeVertices.push_back(shapeVertex1);
                shapeVertices.push_back(shapeVertex2);
                shapeVertices.push_back(shapeVertex3);
            }
        }
    }
    else
    {
        std::cout << "OBJ file loading failed" << std::endl;
    }
}

std::vector<glimac::ShapeVertex> OBJModel::GetVertexData()
{
    return shapeVertices;
}

int OBJModel::GetVertexCount()
{
    return shapeVertices.size();
}
void OBJModel::LoadMaterialFile(const char* filename)
{
    std::ifstream file(filename);
    if (file)
    {
        char        mtlName[100];
        std::string line;
        while (std::getline(file, line))
        {
            if (StartWith(line, "newmtl"))
            {
                (void)sscanf_s(line.c_str(), "newmtl %s", mtlName, sizeof(mtlName));
                MaterialMap[mtlName] = Color();
            }
            if (StartWith(line, "Kd"))
            {
                Color& color = MaterialMap[mtlName];
                sscanf_s(line.c_str(), "Kd %f %f %f", &color.r, &color.g, &color.b);
            }
        }
    }
    else
    {
        std::cout << "Materal file loading failed" << std::endl;
    }
}

bool OBJModel::StartWith(std::string& line, const char* text)
{
    size_t textLen = strlen(text);
    if (line.size() < textLen)
    {
        return false;
    }
    for (size_t i = 0; i < textLen; i++)
    {
        if (line[i] == text[i])
            continue;
        else
            return false;
    }
    return true;
}

void OBJModel::AddVertexData(int vIdx, int nIdx, const char* mtl, std::vector<Position>& vertices, std::vector<Normal>& normals)
{
    Color    c = MaterialMap[mtl];
    Position p = vertices[vIdx - 1];
    Normal   n = normals[nIdx - 1];

    glimac::ShapeVertex shapeVertex;
    shapeVertex.position = glm::vec3(p.x, p.y, p.z);
    shapeVertex.normal   = glm::vec3(n.x, n.y, n.z);

    shapeVertices.push_back(shapeVertex);
}

OBJModel::OBJModel()
{
    // Constructeur par défaut, peut être vide
}

OBJModel::~OBJModel()
{
}