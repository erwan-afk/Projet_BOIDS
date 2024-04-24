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
#include "Hitbox.hpp"
#include "ImguiInterface.hpp"
#include "ModelMesh.hpp"
#include "ModelShader.hpp"
#include "Probability.hpp"
#include "Simulation.hpp"
#include "glimac/cone_vertices.hpp"
#include "glimac/sphere_vertices.hpp"
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
        });
        // std::cout << wildlife_big_plant[b]->posX << std::endl;
    }

    for (unsigned int c = 0; c < this->nb_bush; c++)
    {
        wildlife_bush.push_back(new Plant{
            exponentielle(1.f),
            exponentielle(1.f),
            p6::random::number(0.1f, 1.f),
        });
    }

    for (unsigned int d = 0; d < this->nb_coral; d++)
    {
        if (bernoulli(0.8))
        {
            wildlife_coral.push_back(new Plant{
                loi_normale(0.0f, 0.4f),
                loi_normale(0.0f, 0.4f),
                p6::random::number(0.5f, 1.3f),
            });
        }
    }

    Render();
}

void Simulation::Render()
{
    // Définition de la position de la souris à (0, 0) relativement à la fenêtre
    glfwSetCursorPos(ctx.underlying_glfw_window(), ctx.current_canvas_width(), ctx.current_canvas_height());
    glfwSetInputMode(ctx.underlying_glfw_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    ModelShader    Shader("shaders/3D.vs.glsl", "shaders/normals.fs.glsl");
    ImguiInterface Interface;

    ModelMesh fish2("../meshs/fish3.obj");
    ModelMesh fish2_low("../meshs/fish3-lod-low.obj");
    ModelMesh fish2_tail("../meshs/fish3_tail.obj");
    ModelMesh cube("../meshs/cube2.obj");
    ModelMesh ocean("../meshs/ocean.obj");

    /*Décors*/
    ModelMesh ground("../meshs/ground.obj");
    ModelMesh big_plant("../meshs/big_plant.obj");

    ModelMesh rocks("../meshs/rocks.obj");

    ModelMesh coral("../meshs/coral.obj");
    ModelMesh bush("../meshs/bush.obj");
    ModelMesh boat("../meshs/boat.obj");

    ModelMesh arpenteur("../meshs/arpenteur.obj");
    ModelMesh arpenteur_elice("../meshs/arpenteur_elice.obj");

    glm::mat4 positionMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.4f, 0.0f));
    glm::vec3 color          = glm::vec3(1.0f, 1.0f, 1.0f); // Par exemple, couleur blanche
    float     intensity      = 5.0f;
    float     radius         = 2.0f; // Par exemple, intensité à 1.0

    Light soleil(positionMatrix, color, intensity, radius);
    Shader.addLight(soleil);

    glm::mat4 positionMatrix2 = glm::translate(glm::mat4{1.f}, glm::vec3{0.8f, -0.8f, 0.8f});
    glm::vec3 color2          = glm::vec3(1.0f, 1.0f, 1.0f); // Par exemple, couleur blanche
    float     intensity2      = 5.0f;
    float     radius2         = 0.4f;
    Light     lampe(positionMatrix2, color2, intensity2, radius2);
    Shader.addLight(lampe);

    glEnable(GL_DEPTH_TEST);

    double deltaTime = 0.001;

    // TrackballCamera camera;
    FreeflyCamera camera2;
    // Arpenteur       sousmarin(camera2.getPosition(), camera2.getTheta());
    //  ThirdPersonCamera camera3(sousmarin);

    ctx.mouse_moved = [&](p6::MouseMove) {
        if (inWindow)
        {
            glm::vec2 deltamouse = ctx.mouse_delta();

            // camera.rotateLeft(deltamouse.x * 50);
            // camera.rotateUp(deltamouse.y * 50);

            // camera.setDeltaMouse(glm::vec2(-deltamouse.x * 50, -deltamouse.y * 50));

            camera2.rotateLeft(-deltamouse.x * 50);
            camera2.rotateUp(-deltamouse.y * 50);
        }
    };

    // ctx.mouse_scrolled = [&](p6::MouseScroll e) {
    //     camera.moveFront(-e.dy * 0.1);
    //     // camera2.moveFront(-e.dy);
    // };

    ctx.key_pressed = [&](p6::Key e) {
        if (e.physical == GLFW_KEY_ESCAPE)
        {
            glfwSetInputMode(ctx.underlying_glfw_window(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            // Définition de la position de la souris à (0, 0) relativement à la fenêtre
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

    // Initialiser le temps de la dernière impression au début du jeu
    auto lastPrintTime = std::chrono::steady_clock::now();

    srand(time(NULL));

    // Declare your infinite update loop.
    ctx.update = [&]() {
        // Mesurer le temps écoulé depuis la dernière trame
        std::chrono::steady_clock::time_point frameStart = std::chrono::steady_clock::now();

        // Vérifier si plus de 5 secondes se sont écoulées depuis la dernière fois que totalTimeElapsed a été affiché
        auto                         currentTime           = std::chrono::steady_clock::now();
        std::chrono::duration<float> elapsedSinceLastPrint = currentTime - lastPrintTime;

        // Calculez t en normalisant le temps écoulé par rapport à la durée de transition
        t = std::min(1.0f, elapsedSinceLastPrint.count() / animation_daytime_duration);

        // Appelez la fonction smoothTransition avec la valeur de t pour mettre à jour la couleur actuelle
        Interface.smoothTransition(t);
        if (elapsedSinceLastPrint.count() >= animation_daytime_duration)
        {
            // Interface.choisirCouleur();
            Interface.markov_setp1(Interface.currentState);
            std::cout << Interface.currentState << std::endl;
            lastPrintTime = currentTime; // Mettre à jour le temps de la dernière impression
        }

        Interface.userInteface(flock);
        // Interface.choisirCouleur();

        // Appliquer les mouvements de la caméra
        if (ctx.key_is_pressed(GLFW_KEY_W))
        {
            if (camera2.m_Position.z < 1.0f)
            {
                camera2.moveFront(0.01f);
                arpenteur_elice.speed_rotation = 4.0f;
            }
        }
        if (ctx.key_is_pressed(GLFW_KEY_S))
        {
            camera2.moveFront(-0.01f);
            arpenteur_elice.speed_rotation = -4.0f;
        }
        if (ctx.key_is_pressed(GLFW_KEY_A))
        {
            camera2.moveLeft(-0.01f);
            arpenteur_elice.speed_rotation = 4.0f;
        }
        if (ctx.key_is_pressed(GLFW_KEY_D))
        {
            camera2.moveLeft(0.01f);
            arpenteur_elice.speed_rotation = -4.0f;
        }
        if (ctx.key_is_pressed(GLFW_KEY_E))
        {
            camera2.moveUp(0.01f);
            arpenteur_elice.speed_rotation = -4.0f;
        }
        if (ctx.key_is_pressed(GLFW_KEY_Q))
        {
            camera2.moveUp(-0.01f);
            arpenteur_elice.speed_rotation = -4.0f;
        }
        // std::cout << camera2.m_Phi << camera2.m_Theta << std::endl;
        //   glClearColor(background_color.r, background_color.g, background_color.b, background_color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Shader.use();

        Shader.setColorFog(Interface.getBackground_color());
        Shader.lights[0].color = Interface.getBackground_color() * 1.2f;
        Shader.setColorLight(Interface.getBackground_color());
        Shader.lights[1].position = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.2f, -Interface.getLightPositionZ()));

        // std::cout << Shader.lights[1].position[3].y << std::endl;

        glm::mat4 ProjMatrix = glm::perspective(glm::radians(90.f), ctx.aspect_ratio(), 0.1f, 100.f);

        glm::mat4 viewMatrix = camera2.getViewMatrix();

        for (const auto& boid : flock)
        {
            boid->updatePosition(ctx, Interface.getSpeedFactor());
            boid->flock(flock, this->ctx);
            boid->showOpenGL(ctx, Shader, ProjMatrix, viewMatrix, fish2, fish2_low, fish2_tail, camera2);
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
            bush.Draw(Shader, ProjMatrix, viewMatrix * glm::translate(glm::mat4{1.f}, glm::vec3{Plant->posX, -0.9f, Plant->posY}) * glm::scale(glm::mat4{1.f}, glm::vec3(Plant->scale)), viewMatrix);
        }

        // Dessiner l'arpenteur
        arpenteur.Draw(Shader, ProjMatrix, glm::translate(glm::mat4{1.f}, glm::vec3{0.0f, -0.03f, -0.2f}) * glm::scale(glm::mat4{1.f}, glm::vec3{0.02}) * glm::rotate(glm::mat4(1.0f), glm::radians(180.f), glm::vec3(0.0f, 1.0f, 0.0f)), viewMatrix);
        arpenteur_elice.Draw(Shader, ProjMatrix, glm::translate(glm::mat4{1.f}, glm::vec3{0.0f, -0.03f, -0.2f}) * glm::scale(glm::mat4{1.f}, glm::vec3{0.02}) * glm::rotate(glm::mat4(1.0f), glm::radians(180.f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(arpenteur_elice.GetRotationMeshAngle()), glm::vec3(0.0f, 0.0f, 1.0f)), viewMatrix);

        // Calculer le temps écoulé depuis le début de la trame
        std::chrono::steady_clock::time_point frameEnd      = std::chrono::steady_clock::now();
        std::chrono::duration<float>          frameDuration = std::chrono::duration_cast<std::chrono::duration<float>>(frameEnd - frameStart);

        // Mettre à jour le temps total écoulé dans le jeu
        totalTimeElapsed += frameDuration.count();
    };

    ctx.start();
}
