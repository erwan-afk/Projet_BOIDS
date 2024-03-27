/*Inclusion pour la 3D OpenGL*/

#include <p6/p6.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
/**/
#include "Boid.hpp"
#include "GLFW/glfw3.h"
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
    for (int a = 0; a < 200; a++)
    {
        srand(static_cast<unsigned int>(time(nullptr)));
        std::cout << uniforme(0.0, 1.0) << std::endl;
    }

    Render();
}

void Simulation::Render()
{
    ModelShader Shader("shaders/3D.vs.glsl", "shaders/normals.fs.glsl");

    ModelMesh fish2("../meshs/fish.obj");
    ModelMesh cube("../meshs/cube2.obj");
    ModelMesh ocean("../meshs/ocean.obj");

    glEnable(GL_DEPTH_TEST);

    double deltaTime = 0.001;

    glm::vec4 background_color     = glm::vec4(0.0f, 0.3725f, 1.0f, 1.0f); // Default background color
    float     separationPerception = 0.0;
    float     scohesionPerception  = 0.0;
    float     alignPerception      = 0.0;

    TrackballCamera camera;
    FreeflyCamera   camera2;

    ctx.mouse_dragged = [&](p6::MouseDrag) {
        glm::vec2 deltamouse = ctx.mouse_delta();

        camera.rotateLeft(deltamouse.x * 50);
        camera.rotateUp(deltamouse.y * 50);

        camera2.rotateLeft(-deltamouse.x * 50);
        camera2.rotateUp(-deltamouse.y * 50);
    };

    ctx.mouse_scrolled = [&](p6::MouseScroll e) {
        camera.moveFront(-e.dy);
        camera2.moveFront(-e.dy);
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

        if (ctx.key_is_pressed(GLFW_KEY_W))
        {
            camera2.moveFront(0.01f);
        }

        if (ctx.key_is_pressed(GLFW_KEY_A))
        {
            camera2.moveLeft(-0.01f);
        }
        if (ctx.key_is_pressed(GLFW_KEY_D))
        {
            camera2.moveLeft(0.01f);
        }

        if (ctx.key_is_pressed(GLFW_KEY_S))
        {
            camera2.moveFront(-0.01f);
        }

        glClearColor(background_color.r, background_color.g, background_color.b, background_color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Shader.use();

        Shader.setColorFog(glm::vec3(0.0, 0.639, 1.0));

        glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);

        glm::mat4 viewMatrix = camera2.getViewMatrix();

        for (const auto& boid : flock)
        {
            boid->updatePosition(deltaTime);
            boid->flock(flock, this->ctx);
            boid->showOpenGL(this->ctx, Shader, ProjMatrix, viewMatrix, fish2);
        }

        cube.Draw(Shader, ProjMatrix, viewMatrix);
        ocean.Draw(Shader, ProjMatrix, viewMatrix * glm::scale(glm::mat4{1.f}, glm::vec3(2.0f)));
    };

    ctx.start();
}
