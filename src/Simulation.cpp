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

void Simulation::setImguiFactor(float value)
{
    for (Boid* boid : flock)
    {
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

void Simulation::Render()
{
    const p6::Shader shader = p6::load_shader(
        "shaders/3D.vs.glsl",
        "shaders/normals.fs.glsl"
    );

    UniqueBuffer vbo; // Use UniqueBuffer to manage VBO

    const std::vector<glimac::ShapeVertex> vertices_sphere = glimac::cone_vertices(2.f, 1.f, 32, 16);
    const std::vector<glimac::ShapeVertex> vertices_cube   = {
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

    glBindBuffer(GL_ARRAY_BUFFER, vbo.id());
    glBufferData(GL_ARRAY_BUFFER, vertices_sphere.size() * sizeof(ShapeVertex), vertices_sphere.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Use UniqueBuffer to automatically handle deletion
    UniqueBuffer vaoBuffer;
    GLuint       vao = vaoBuffer.id();
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo.id());

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

    double deltaTime = 0.001;
    // ImGui default values
    glm::vec4 background_color     = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f); // Default background color
    float     separationPerception = 1.0;

    TrackballCamera camera;

    ctx.mouse_moved = [&](p6::MouseMove) {
        glm::vec2 deltamouse = ctx.mouse_delta();

        camera.rotateLeft(deltamouse.x * 50);
        camera.rotateUp(deltamouse.y * 50);
    };

    ctx.mouse_scrolled = [&](p6::MouseScroll) {
        glm::vec2 deltamouse = ctx.mouse_delta();
        camera.moveFront(deltamouse.y * 50);
    };

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ImGui::Begin("Option");
        ImGui::SliderFloat("Separation", &separationPerception, 1.0, 10.0);
        ImGui::ColorPicker4("Background Color", (float*)&background_color);
        ImGui::End();

        // set ImGui Options
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

        for (const auto& boid : flock)
        {
            // boid->edges(this->ctx);
            boid->updatePosition(deltaTime);
            boid->flock(flock, this->ctx);
            // boid->show(ctx);
            boid->showOpenGL(this->ctx, uMVPMatrixLocation, uMVMatrixLocation, uNormalMatrixLocation, ProjMatrix, viewMatrix, vertices_sphere);
        }

        glDrawArrays(GL_TRIANGLES, 0, vertices_cube.size());

        glBindVertexArray(0);
    };

    ctx.start();
}
