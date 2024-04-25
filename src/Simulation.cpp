/*Inclusion pour la 3D OpenGL*/

#include <p6/p6.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
/**/
#include <chrono>
#include "Boid.hpp"
#include "GLFW/glfw3.h"
#include "ImguiInterface.hpp"
#include "ModelMesh.hpp"
#include "ModelShader.hpp"
#include "Probability.hpp"
#include "Simulation.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/trigonometric.hpp"

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
            loi_normale(0.0f, 0.1f),         // x
            loi_normale(0.0f, 0.1f),         // y
            loi_normale(0.0f, 0.1f),         // z
            p6::random::number(-0.2f, 0.2f), // Angle de queue
            p6::random::number(-this->speed_factor, this->speed_factor),
            p6::random::number(-this->speed_factor, this->speed_factor),
            p6::random::number(-this->speed_factor, this->speed_factor)
        });
    }

    for (unsigned int b = 0; b < this->nb_big_plant; b++)
    {
        wildlife_big_plant.push_back(new Plant{
            loi_beta(0.5f, 0.5f),
            loi_beta(0.5f, 0.5f),
            p6::random::number(0.5f, 1.6f),
            0.0f,
        });
    }

    for (unsigned int c = 0; c < this->nb_bush; c++)
    {
        wildlife_bush.push_back(new Plant{
            exponentielle(1.f),
            loi_depareto(4.f, 5.f, 0.f),
            p6::random::number(0.1f, 1.f),
            p6::random::number(-0.1f, 0.1f),
        });
    }

    for (unsigned int d = 0; d < this->nb_coral; d++)
    {
        if (bernoulli(0.8))
        {
            wildlife_coral.push_back(new Plant{
                loi_laplace(0.0f, 2.0f),
                loi_laplace(0.0f, 2.0f),
                loi_geometric(0.8f),
                0.0f,
            });
        }

        // std::cout << wildlife_coral[d]->scale << std::endl;
    }

    Render();
}

glm::vec3 randomRotationAxis()
{
    // Initialiser le générateur de nombres aléatoires
    std::random_device              rd;
    std::mt19937                    gen(rd());
    std::uniform_int_distribution<> dis(0, 1); // Distribution uniforme pour choisir entre 0 et 1

    // Choisir aléatoirement entre les axes
    if (dis(gen) == 0)
    {
        return glm::vec3(1.0f, 0.0f, 0.0f); // Axe X
    }
    else
    {
        return glm::vec3(0.0f, 0.0f, 1.0f); // Axe Z
    }
}

void Simulation::Render()
{
    // Définition de la position de la souris à (0, 0) relativement à la fenêtre
    glfwSetCursorPos(ctx.underlying_glfw_window(), ctx.current_canvas_width(), ctx.current_canvas_height());
    glfwSetInputMode(ctx.underlying_glfw_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    ModelShader    Shader("shaders/3D.vs.glsl", "shaders/normals.fs.glsl");
    ImguiInterface Interface;

    ModelMesh fish_high("../meshs/fish.obj");
    ModelMesh fish_low("../meshs/fish_low.obj");
    ModelMesh fish_tail("../meshs/fish_tail.obj");
    ModelMesh cube("../meshs/cube.obj");
    ModelMesh ocean("../meshs/ocean.obj");

    ModelMesh ground("../meshs/ground.obj");
    ModelMesh big_plant("../meshs/big_plant.obj");
    ModelMesh rocks("../meshs/rocks.obj");
    ModelMesh coral("../meshs/coral.obj");
    ModelMesh bush("../meshs/bush.obj");
    ModelMesh boat("../meshs/boat.obj");

    ModelMesh arpenteur("../meshs/arpenteur.obj");
    ModelMesh arpenteur_elice("../meshs/arpenteur_elice.obj");

    Light soleil(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.4f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f), 5.0f, 2.0f);
    Light lampe(glm::translate(glm::mat4{1.f}, glm::vec3{0.8f, -0.8f, 0.8f}), glm::vec3(1.0f, 1.0f, 1.0f), 5.0f, 0.4f);
    Shader.addLight(soleil);
    Shader.addLight(lampe);

    FreeflyCamera camera;

    ctx.mouse_moved = [&](p6::MouseMove) {
        if (inWindow)
        {
            glm::vec2 deltamouse = ctx.mouse_delta();
            camera.rotateLeft(-deltamouse.x * 50);
            camera.rotateUp(-deltamouse.y * 50);
        }
    };

    ctx.key_pressed = [&](p6::Key e) {
        if (e.physical == GLFW_KEY_ESCAPE)
        {
            glfwSetInputMode(ctx.underlying_glfw_window(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetCursorPos(ctx.underlying_glfw_window(), ctx.current_canvas_width(), ctx.current_canvas_height());
            inWindow = false;
        }
    };

    ctx.mouse_pressed = [&](p6::MouseButton button) {
        if (button.button == p6::Button::Right)
        {
            inWindow = true;
            glfwSetInputMode(ctx.underlying_glfw_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    };

    glEnable(GL_DEPTH_TEST);

    auto lastPrintTime = std::chrono::steady_clock::now();

    // infinite update loop
    ctx.update = [&]() {
        // Mesurer le temps écoulé depuis la dernière frame
        std::chrono::steady_clock::time_point frameStart = std::chrono::steady_clock::now();

        auto                         currentTime           = std::chrono::steady_clock::now();
        std::chrono::duration<float> elapsedSinceLastPrint = currentTime - lastPrintTime;
        t                                                  = std::min(1.0f, elapsedSinceLastPrint.count() / animation_daytime_duration);
        Interface.smoothTransition(t);

        if (elapsedSinceLastPrint.count() >= animation_daytime_duration)
        {
            // Vérifier si plus de 5 secondes se sont écoulées depuis la dernière fois que totalTimeElapsed a été affiché
            Interface.markov_setp1(Interface.currentState);
            lastPrintTime = currentTime;
        }
        // Charger les entrées sur l'interface de l'utilisateur
        Interface.userInteface(flock);

        // Appliquer les mouvements de la caméra
        if (ctx.key_is_pressed(GLFW_KEY_W))
        {
            if (camera.m_Position.z < 1.0f)
            {
                camera.moveFront(0.01f);
                arpenteur_elice.speed_rotation = 4.0f;
            }
        }
        if (ctx.key_is_pressed(GLFW_KEY_S))
        {
            camera.moveFront(-0.01f);
            arpenteur_elice.speed_rotation = -4.0f;
        }
        if (ctx.key_is_pressed(GLFW_KEY_A))
        {
            camera.moveLeft(-0.01f);
            arpenteur_elice.speed_rotation = 4.0f;
        }
        if (ctx.key_is_pressed(GLFW_KEY_D))
        {
            camera.moveLeft(0.01f);
            arpenteur_elice.speed_rotation = -4.0f;
        }
        if (ctx.key_is_pressed(GLFW_KEY_E))
        {
            camera.moveUp(0.01f);
            arpenteur_elice.speed_rotation = -4.0f;
        }
        if (ctx.key_is_pressed(GLFW_KEY_Q))
        {
            camera.moveUp(-0.01f);
            arpenteur_elice.speed_rotation = -4.0f;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Utiliation du Shader et définition de position de la camera fixé a l'arpenteur :
        // ainsi que la couleur du soleil (top light) relié a celle de l'environnement (fog)
        Shader.use();
        Shader.setFog(Interface.getImguiFog());

        Shader.setColorFog(Interface.getBackground_color());

        Shader.setLightColor(0, Interface.getBackground_color() * 1.2f);

        // Définition de la matrice de projection et de la matrice de vue
        glm::mat4 ProjMatrix = glm::perspective(glm::radians(90.f), ctx.aspect_ratio(), 0.1f, 100.f);
        glm::mat4 viewMatrix = camera.getViewMatrix();

        // Positionner la lumière avec la ViewMatrix et la translation
        Shader.setLightPosition(0, viewMatrix * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.4f, 0.0f)));
        Shader.setLightPosition(1, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.2f, -Interface.getLightPositionZ())));

        // Affichage des meshs
        for (const auto& boid : flock)
        {
            boid->updatePosition(ctx, Interface.getSpeedFactor());
            boid->flock(flock, this->ctx);
            boid->showOpenGL(ctx, Shader, ProjMatrix, viewMatrix, fish_high, fish_low, fish_tail, camera);
        }

        cube.Draw(Shader, ProjMatrix, viewMatrix, viewMatrix);
        ocean.Draw(Shader, ProjMatrix, viewMatrix * glm::translate(glm::mat4{1.f}, glm::vec3{0.f, -0.1f, 0.f}) * glm::scale(glm::mat4{1.f}, glm::vec3(2.7f)), viewMatrix);
        ground.Draw(Shader, ProjMatrix, viewMatrix * glm::translate(glm::mat4{1.f}, glm::vec3{0.f, -0.9f, 0.f}) * glm::scale(glm::mat4{1.f}, glm::vec3(1.3f)), viewMatrix);
        rocks.Draw(Shader, ProjMatrix, viewMatrix * glm::translate(glm::mat4{1.f}, glm::vec3{0.8f, -0.8f, 0.8f}) * glm::scale(glm::mat4{1.f}, glm::vec3(1.3f)) * glm::rotate(glm::mat4(1.0f), 80.f, glm::vec3(0.0f, 1.0f, 0.0f)), viewMatrix);
        boat.Draw(Shader, ProjMatrix, viewMatrix * glm::translate(glm::mat4{1.f}, glm::vec3{-0.8f, -0.85f, -0.8f}) * glm::scale(glm::mat4{1.f}, glm::vec3(0.3f)) * glm::rotate(glm::mat4(1.0f), -45.f, glm::vec3(0.0f, 1.0f, 0.0f)), viewMatrix);

        for (const auto& Plant : wildlife_big_plant)
        {
            big_plant.Draw(Shader, ProjMatrix, viewMatrix * glm::translate(glm::mat4{1.f}, glm::vec3{Plant->posX, -0.9f, Plant->posY}) * glm::scale(glm::mat4{1.f}, glm::vec3(Plant->scale)), viewMatrix);
        }
        for (const auto& Plant : wildlife_coral)
        {
            coral.Draw(Shader, ProjMatrix, viewMatrix * glm::translate(glm::mat4{1.f}, glm::vec3{Plant->posX, -0.9f, Plant->posY}) * glm::scale(glm::mat4{1.f}, glm::vec3(Plant->scale)), viewMatrix);
        }

        for (const auto& Plant : wildlife_bush)
        {
            bush.Draw(Shader, ProjMatrix, viewMatrix * glm::translate(glm::mat4{1.f}, glm::vec3{Plant->posX, -0.9f, Plant->posY}) * glm::scale(glm::mat4{1.f}, glm::vec3(Plant->scale)) * glm::rotate(glm::mat4(1.0f), Plant->getPlantAngleX(), glm::vec3(1.0f, 0.0f, 0.0f)), viewMatrix);
            Plant->updatePlantAngle(1.0);
        }

        arpenteur.Draw(Shader, ProjMatrix, glm::translate(glm::mat4{1.f}, glm::vec3{0.0f, -0.03f, -0.2f}) * glm::scale(glm::mat4{1.f}, glm::vec3{0.02}) * glm::rotate(glm::mat4(1.0f), glm::radians(180.f), glm::vec3(0.0f, 1.0f, 0.0f)), viewMatrix);
        arpenteur_elice.Draw(Shader, ProjMatrix, glm::translate(glm::mat4{1.f}, glm::vec3{0.0f, -0.03f, -0.2f}) * glm::scale(glm::mat4{1.f}, glm::vec3{0.02}) * glm::rotate(glm::mat4(1.0f), glm::radians(180.f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(arpenteur_elice.GetRotationMeshAngle()), glm::vec3(0.0f, 0.0f, 1.0f)), viewMatrix);

        // Calculer le temps écoulé depuis le début de la frame
        std::chrono::steady_clock::time_point frameEnd      = std::chrono::steady_clock::now();
        std::chrono::duration<float>          frameDuration = std::chrono::duration_cast<std::chrono::duration<float>>(frameEnd - frameStart);

        // Mettre à jour le temps total écoulé dans le jeu
        totalTimeElapsed += frameDuration.count();
    };

    ctx.start();
}
