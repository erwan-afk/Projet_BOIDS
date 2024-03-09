#include "Simulation.hpp"
#include "Boid.hpp"
#include "TrackballCamera.hpp"

/*Inclusion pour la 3D OpenGL*/
#include <p6/p6.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glimac/cone_vertices.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"

struct ShapeVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

// Définition du constructeur
Simulation::Simulation()
    : name("Projet Boids"), window_width(this->window_width), window_height(this->window_height), ctx()
{
    // Initialisations supplémentaires si nécessaires
}

void Simulation::setImguiFactor(float value) {
      for (Boid* boid : flock) {
        boid->setImguiFactor(value);
    }
}


void Simulation::Run()
{
    // initalisation des boids
    for (unsigned int a = 0; a < this->nb_flock; a++)
    {
        // unique_ptr plutôt que new (ou alors pas de ptr du tout)
        flock.push_back(new Boid{p6::random::number(-1, 1), p6::random::number(-1, 1), p6::random::number(-1, 1), p6::random::number(-this->speed_factor, this->speed_factor), p6::random::number(-this->speed_factor, this->speed_factor), p6::random::number(-this->speed_factor, this->speed_factor)});
    }

    // lancer la boucle infini
    Render();
}


void Simulation::Render()
{
    const p6::Shader shader = p6::load_shader(
        "shaders/3D.vs.glsl",
        "shaders/normals.fs.glsl"
    );

    // Création d'un seul VBO :
    GLuint vbo;
    glGenBuffers(1, &vbo);
    // À partir de ce point, la variable vbo contient l'identifiant d'un VBO

    // Binding d'un VBO sur la cible GL_ARRAY_BUFFER:
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // On peut à présent modifier le VBO en passant par la cible GL_ARRAY_BUFFER

    const std::vector<glimac::ShapeVertex> vertices_sphere = glimac::cone_vertices(2.f, 1.f, 32, 16);

    glBufferData(GL_ARRAY_BUFFER, vertices_sphere.size() * sizeof(ShapeVertex), vertices_sphere.data(), GL_STATIC_DRAW);

    // DeBinding d'un VBO sur la cible GL_ARRAY_BUFFER:
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint vao;
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    static constexpr GLuint vertex_attr_position = 0;
    glEnableVertexAttribArray(vertex_attr_position);

    glVertexAttribPointer(vertex_attr_position, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)(offsetof(ShapeVertex, position)));

    static constexpr GLuint vertex_attr_normal = 1;
    glEnableVertexAttribArray(vertex_attr_normal);

    glVertexAttribPointer(vertex_attr_normal, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)(offsetof(ShapeVertex, normal)));

    static constexpr GLuint vertex_attr_texCoords = 2;
    glEnableVertexAttribArray(vertex_attr_texCoords);

    glVertexAttribPointer(vertex_attr_texCoords, 2, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)(offsetof(ShapeVertex, texCoords)));

    // Unbind the VAO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint uMVPMatrixLocation    = glGetUniformLocation(shader.id(), "uMVPMatrix");
    GLuint uMVMatrixLocation     = glGetUniformLocation(shader.id(), "uMVMatrix");
    GLuint uNormalMatrixLocation = glGetUniformLocation(shader.id(), "uNormalMatrix");

    glEnable(GL_DEPTH_TEST);

    double    deltaTime            = 0.001;
    //ImGui default values
    glm::vec4 background_color     = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f); // Default background color
    float     separationPerception = 1.0;

    TrackballCamera camera;

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ImGui::Begin("Option");
        ImGui::SliderFloat("Separation", &separationPerception, 1.0, 10.0);
        ImGui::ColorPicker4("Background Color", (float*)&background_color);
        ImGui::End();

        //set ImGui Options
        Simulation::setImguiFactor(separationPerception);

        // Clear the screen
        glClearColor(background_color.r, background_color.g, background_color.b, background_color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Bind the VAO before rendering
        glBindVertexArray(vao);

        shader.use();

        // Calculez la matrice de projection
        glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
        // Calcul de la matrice de vue
        glm::mat4 viewMatrix = camera.getViewMatrix();

        /* -------------------  */

        // Clear the background with a fading effect
        // ctx.background({background_color.x, background_color.y, background_color.z});

        // draw

        for (const auto& boid : flock)
        {
            // boid->edges(this->ctx);
            boid->updatePosition(deltaTime);
            boid->flock(flock, this->ctx);
            // boid->show(ctx);
            boid->showOpenGL(this->ctx, uMVPMatrixLocation, uMVMatrixLocation, uNormalMatrixLocation, ProjMatrix, vertices_sphere);
        }
        glBindVertexArray(0);
    };

    ctx.start();

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}
