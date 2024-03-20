

/*Inclusion pour la 3D OpenGL*/

#include <p6/p6.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
/**/
#include "Boid.hpp"
#include "ModelMesh.hpp"
#include "Rules.hpp"
#include "Simulation.hpp"
#include "TrackballCamera.hpp"
#include "glimac/cone_vertices.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"

// Définition du constructeur
Simulation::Simulation()
    : name("Projet Boids"), window_width(this->window_width), window_height(this->window_height), ctx() {}

void Simulation::setImguiFactorAlign(float value)
{
    for (Boid* boid : flock)
    {
        boid->setImguiFactorAlign(value);
    }
}
void Simulation::setImguiFactorCohesion(float value)
{
    for (Boid* boid : flock)
    {
        boid->setImguiFactorCohesion(value);
    }
}
void Simulation::setImguiFactorSeparation(float value)
{
    for (Boid* boid : flock)
    {
        boid->setImguiFactorSeparation(value);
    }
}

void Simulation::Run()
{
    // initalisation des boids
    for (unsigned int a = 0; a < this->nb_flock; a++)
    {
        // unique_ptr plutôt que new (ou alors pas de ptr du tout)
        flock.push_back(new Boid{
            p6::random::number(-1, 1),
            p6::random::number(-1, 1),
            p6::random::number(-1, 1),
            p6::random::number(-this->speed_factor, this->speed_factor),
            p6::random::number(-this->speed_factor, this->speed_factor),
            p6::random::number(-this->speed_factor, this->speed_factor)
        });
    }

    // lancer la boucle infini
    for (int a = 0; a < 100; a++)
    {
        srand(static_cast<unsigned int>(time(nullptr)));
        std::cout << uniforme(0.0, 1.0) << std::endl;
    }

    Render();
}

void Simulation::Render()
{
    /*
    const p6::Shader shader = p6::load_shader(
        "shaders/3D.vs.glsl",
        "shaders/normals.fs.glsl"
    );
    */

    // Création d'une instance de ModelShader avec les shaders chargés
    ModelShader Shader("shaders/3D.vs.glsl", "shaders/normals.fs.glsl");

    /*

    OBJModel fish;
    fish.LoadFromFile("../meshs/fish.obj");

    std::vector<glimac::ShapeVertex> vertices_sphere = fish.GetVertexData();
    int                              vertexCount     = fish.GetVertexCount();
*/
    ModelMesh fish2("../meshs/fish.obj");
    ModelMesh cube("../meshs/cube2.obj");
    ModelMesh fond_marin("../meshs/fond_marin.obj");

    // const std::vector<glimac::ShapeVertex> vertices_sphere = glimac::cone_vertices(2.f, 1.f, 32, 16);

    /*

    const std::vector<glimac::ShapeVertex> vertices_cube = {
        // Define cube vertices (positions only)
        {glm::vec3(-0.5f, -0.5f, -0.5f)}, // Vertex 0
        {glm::vec3(0.5f, -0.5f, -0.5f)},  // Vertex 1
        {glm::vec3(0.5f, 0.5f, -0.5f)},   // Vertex 2
        {glm::vec3(-0.5f, 0.5f, -0.5f)},  // Vertex 3
        {glm::vec3(-0.5f, -0.5f, 0.5f)},  // Vertex 4
        {glm::vec3(0.5f, -0.5f, 0.5f)},   // Vertex 5
        {glm::vec3(0.5f, 0.5f, 0.5f)},    // Vertex 6
        {glm::vec3(-0.5f, 0.5f, 0.5f)}    // Vertex 7
    };

    UniqueBuffer vbo; // Use UniqueBuffer to manage VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo.id());
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(glimac::ShapeVertex), vertices_sphere.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Use UniqueBuffer to automatically handle deletion
    UniqueBuffer vaoBuffer;
    GLuint       vao = vaoBuffer.id();
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



    GLuint uMVPMatrixLocation    = glGetUniformLocation(shader.id(), "uMVPMatrix");
    GLuint uMVMatrixLocation     = glGetUniformLocation(shader.id(), "uMVMatrix");
    GLuint uNormalMatrixLocation = glGetUniformLocation(shader.id(), "uNormalMatrix");

    */

    glEnable(GL_DEPTH_TEST);

    double deltaTime = 0.001;
    // ImGui default values
    glm::vec4 background_color     = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f); // Default background color
    float     separationPerception = 0.0;
    float     scohesionPerception  = 0.0;
    float     alignPerception      = 0.0;

    TrackballCamera camera;

    ctx.mouse_dragged = [&](p6::MouseDrag) {
        glm::vec2 deltamouse = ctx.mouse_delta();

        camera.rotateLeft(deltamouse.x * 50);
        camera.rotateUp(deltamouse.y * 50);
    };

    ctx.mouse_scrolled = [&](p6::MouseScroll e) {
        camera.moveFront(-e.dy);
    };

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ImGui::Begin("Option");
        ImGui::SliderFloat("Separation", &separationPerception, 0.0, 1.0);
        ImGui::SliderFloat("Cohesion", &scohesionPerception, 0.0, 1.0);
        ImGui::SliderFloat("Align", &alignPerception, 0.0, 1.0);
        ImGui::ColorPicker4("Background Color", (float*)&background_color);
        ImGui::End();

        // set ImGui Options
        Simulation::setImguiFactorSeparation(separationPerception * 0.02);
        Simulation::setImguiFactorCohesion(scohesionPerception * 0.0005);
        Simulation::setImguiFactorAlign(alignPerception * 0.35);

        // Clear the screen
        glClearColor(background_color.r, background_color.g, background_color.b, background_color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Bind the VAO before rendering

        ///////////////////////////////////////////////////////////////////////////////////////////

        // shader.use();
        // Utiliser le shaderrr
        Shader.use();

        // glBindVertexArray(vao);

        // Calculez la matrice de projection
        glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
        // Calcul de la matrice de vue
        glm::mat4 viewMatrix = camera.getViewMatrix();

        for (const auto& boid : flock)
        {
            // boid->edges(this->ctx);
            boid->updatePosition(deltaTime);
            boid->flock(flock, this->ctx);
            // boid->show(ctx);
            boid->showOpenGL(this->ctx, Shader, ProjMatrix, viewMatrix, fish2);
        }

        cube.Draw(Shader, ProjMatrix, viewMatrix);
        fond_marin.Draw(Shader, ProjMatrix, viewMatrix * glm::translate(glm::mat4{1.f}, glm::vec3(0.0f, -1.0f, 0.0f)));

        // glDrawArrays(GL_TRIANGLES, 0, vertices_cube.size());

        // glBindVertexArray(0);
    };

    ctx.start();
}
