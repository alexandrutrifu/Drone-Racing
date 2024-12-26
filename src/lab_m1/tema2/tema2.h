#ifndef TEMA2_H
#define TEMA2_H

#pragma once

#include "components/simple_scene.h"
#include "components/text_renderer.h"
#include "camera.h"
#include "drone/drone.h"
#include "environment/trees/tree.h"
#include "environment/gates/gate.h"

#include <vector>
#include <iostream>

using namespace std;

namespace m1
{
    const glm::vec3 skyBlue = glm::vec3(0.612, 0.800, 0.925);

    struct ViewportSpace
    {
        ViewportSpace() : x(0), y(0), width(1), height(1) {}
        ViewportSpace(int x, int y, int width, int height)
            : x(x), y(y), width(width), height(height) {}
        int x;
        int y;
        int width;
        int height;
    };

    class Tema2 : public gfxc::SimpleScene
    {
    public:
        Tema2();
        ~Tema2() override;

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void DrawScene(camera::Camera *camera, float deltaTimeSeconds);
        void SetViewportArea(const ViewportSpace & viewSpace, glm::vec3 colorColor, bool clear);

        // Draw the heads-up display
        void DrawHUD();

    protected:
        ViewportSpace viewSpace;

        float cx{}, cy{};
        glm::mat3 modelMatrix{};
        float translateX{}, translateY{};
        float scaleX{}, scaleY{};
        float scaleCar1{}, scaleCar2{};
        float angularStep{};

        float left = -5;
        float right = 5;
        float bottom = -5;
        float top = 5;
        float fov = RADIANS(60);
        float zNear = 0.01, zFar = 200;

        float sensivityOX = 0.015f;
        float sensivityOY = 0.015f;

        // Camera attributes
        camera::Camera *camera;
        camera::Camera *miniMapCamera;
        glm::mat4 projectionMatrix = glm::perspective(fov, window->props.aspectRatio, zNear, zFar);
        bool renderCameraTarget;

        // Objects
        terrain::Terrain *terrain{};
        drones::Drone *drone{};
        vector<trees::Tree *> forest{};
        vector<gates::Gate *> gates{};
        vector<int> checkpointOrder{};

        // Gates
        gates::Gate *currentGate;
        int currentCheckpoint = 0;

        // Text
        // The actual renderer object
        gfxc::TextRenderer *textRenderer;

        // Draw primitives mode
        GLenum polygonMode;

        // Timer
        float nanoSecondsPassed = 0;
        int timerMinutes = 2;
        int timerSeconds1 = 0;
        int timerSeconds2 = 0;
        
        // Game over flag
        bool gameOver = false;

        // Starting countdown
        bool startCountdown = true;
        int countdown = 4;
    };
}   // namespace m1


#endif
