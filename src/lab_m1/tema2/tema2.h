#ifndef TEMA2_H
#define TEMA2_H

#pragma once

#include "components/simple_scene.h"
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

    protected:
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
    };
}   // namespace m1


#endif
