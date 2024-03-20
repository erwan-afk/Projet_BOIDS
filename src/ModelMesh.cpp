#include "ModelMesh.hpp"
#include <cstddef>
#include <fstream>
#include <iostream>
#include <vector>
#include "ModelShader.hpp"
#include "glm/fwd.hpp"
#include <sstream>

ModelMesh::ModelMesh(const char* filePath)
{
    vertexCount = 0;

    LoadModel(filePath);

    // Generate VAO
    GLuint vaoId;
    glGenVertexArrays(1, &vaoId); // Utilisez la méthode id() pour obtenir l'identifiant GLuint
    glBindVertexArray(vao.id());

    // Bind VBO and upload data
    glBindBuffer(GL_ARRAY_BUFFER, vbo.id());
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glimac::ShapeVertex), vertices.data(), GL_STATIC_DRAW);

    // Specify vertex attribute pointers
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

void ModelMesh::Draw(ModelShader& Shader, glm::mat4 ProjMatrix, glm::mat4 MVMatrix)
{
    glBindVertexArray(vao.id());

    // Envoyer les matrices aux uniformes
    Shader.setMVPMatrix(ProjMatrix, MVMatrix);
    Shader.setMVMatrix(MVMatrix);
    // Calculer la matrice NormalMatrix
    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
    Shader.setNormalMatrix(NormalMatrix);


    glBindTexture(GL_TEXTURE_2D, texture.id());
    // Draw the triangle


    // Dessiner les triangles
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);


    // Draw the triangle
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindVertexArray(0);
}

ModelMesh::~ModelMesh()
{
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
    std::vector<textCoords>   textures;

    std::ifstream file(fileName);
    if (file)
    {
        char        currentMtlName[100];
        std::string line;
        while (std::getline(file, line))
        {
            if (StartWith(line, "mtllib"))
            {
                std::istringstream iss(line.substr(7)); // Ignorer "mtllib " dans la ligne
                std::string        mtlFileName;
                iss >> mtlFileName;

                // Ajoutez le préfixe au nom de fichier
                const char* prefix           = "../meshs/";
                std::string prefixedFileName = std::string(prefix) + mtlFileName;
                std::cout << prefixedFileName << std::endl;

                // Charger le fichier de matériaux
                LoadMaterialFile(prefixedFileName.c_str());
            }

            if (StartWith(line, "v "))
            {
                Position           pos;
                std::istringstream iss(line.substr(2)); // Ignorer "v " dans la ligne
                iss >> pos.x >> pos.y >> pos.z;
                vertices.push_back(pos);
            }
            if (StartWith(line, "vn "))
            {
                Normal             n;
                std::istringstream iss(line.substr(3)); // Ignorer "vn " dans la ligne
                iss >> n.x >> n.y >> n.z;
                normals.push_back(n);
            }
            if (StartWith(line, "vt "))
            {
                textCoords            t ;
                std::istringstream iss(line.substr(3)); // Ignorer "vt " dans la ligne
                iss >> t.x >> t.y;
                textures.push_back(t);
            }
            if (StartWith(line, "usemtl"))
            {
                std::istringstream iss(line.substr(7)); // Ignorer "usemtl " dans la ligne
                iss >> currentMtlName;
            }
          if (StartWith(line, "f "))
            {
                std::istringstream iss(line.substr(2)); // Ignorer "f " dans la ligne
                char slash;
                int v1, v2, v3;
                int n1, n2, n3;
                int t1, t2, t3; // Ajouter les indices de texture

                iss >> v1 >> slash >> t1 >> slash >> n1 >>
                    v2 >> slash >> t2 >> slash >> n2 >>
                    v3 >> slash >> t3 >> slash >> n3;

                // Créer les vertices de type glimac::ShapeVertex à partir des indices v1, v2, v3 et n1, n2, n3
                glimac::ShapeVertex shapeVertex1, shapeVertex2, shapeVertex3;
                shapeVertex1.position = glm::vec3(vertices[v1 - 1].x, vertices[v1 - 1].y, vertices[v1 - 1].z);
                shapeVertex1.normal = glm::vec3(normals[n1 - 1].x, normals[n1 - 1].y, normals[n1 - 1].z);
                shapeVertex1.texCoords = glm::vec2(textures[t1 - 1].x, textures[t1 - 1].y); // Remplir texCoords pour le premier vertex

                shapeVertex2.position = glm::vec3(vertices[v2 - 1].x, vertices[v2 - 1].y, vertices[v2 - 1].z);
                shapeVertex2.normal = glm::vec3(normals[n2 - 1].x, normals[n2 - 1].y, normals[n2 - 1].z);
                shapeVertex2.texCoords = glm::vec2(textures[t2 - 1].x, textures[t2 - 1].y); // Remplir texCoords pour le deuxième vertex

                shapeVertex3.position = glm::vec3(vertices[v3 - 1].x, vertices[v3 - 1].y, vertices[v3 - 1].z);
                shapeVertex3.normal = glm::vec3(normals[n3 - 1].x, normals[n3 - 1].y, normals[n3 - 1].z);
                shapeVertex3.texCoords = glm::vec2(textures[t3 - 1].x, textures[t3 - 1].y); // Remplir texCoords pour le troisième vertex

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

Texture OBJModel::getTexture(){
    return texture;
}
void OBJModel::LoadMaterialFile(const char* filename)
{
    std::ifstream file(filename);
    if (file)
    {
        char        texture_path[100];
        std::string line;
        while (std::getline(file, line))
        {
            // if (StartWith(line, "newmtl"))
            // {
            //     std::istringstream iss(line.substr(7)); // Ignorer "newmtl " dans la ligne
            //     iss >> mtlName;
            //     MaterialMap[mtlName] = Color();
            // }

            // if (StartWith(line, "Kd"))
            // {
            //     Color&             color = MaterialMap[mtlName];
            //     std::istringstream iss(line.substr(3)); // Ignorer "Kd " dans la ligne
            //     iss >> color.r >> color.g >> color.b;
            // }
            if (StartWith(line, "map_Kd")){
                std::istringstream iss(line.substr(7)); 
                iss >> texture_path;

                    const img::Image texture_img = p6::load_image_buffer(texture_path);
                    
                    GLuint Texture_id; 
                    glGenTextures(1, &Texture_id);
                    glBindTexture(GL_TEXTURE_2D, texture.id());
                    glTexImage2D(
                        GL_TEXTURE_2D,
                        0,
                        GL_RGBA,
                        texture_img.width(),
                        texture_img.height(),
                        0,
                        GL_RGBA,
                        GL_UNSIGNED_BYTE,
                        texture_img.data()
                    );
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glBindTexture(GL_TEXTURE_2D, 0);
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

void OBJModel::AddVertexData(int vIdx, int nIdx, const char* mtl, std::vector<Position>& vertices, std::vector<Normal>& normals, std::vector<textCoords>& coord_texture)
{
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