#ifndef GATE_H
#define GATE_H

#pragma once

#include <vector>
#include <iostream>

#include "components/simple_scene.h"
#include "../../objects/objects.h"
#include "../../camera.h"
#include "../obstacle.h"
#include "../terrain/terrain.h"

#define NUM_GATES (terrain::terrainSize / 5)
#define GATE_RADIUS 2
#define GATE_PASSIVE_COLOR glm::vec3(0.5, 0.5, 0.5)
#define GATE_ACTIVE_COLOR glm::vec3(0, 1, 0)

using namespace std;

namespace gates
{
    class Gate : public obstacles::Obstacle, public objects::Object
    {
        public:
            Gate();
            ~Gate() override;

            void CreateGate(const char *name, const glm::vec3 &corner, float bigRadius);
            void RenderGate(Shader *shader, camera::Camera *camera, const glm::mat4 &projectionMatrix);

            void RenderObject(Shader *shader, const glm::mat4 &modelMatrix, camera::Camera *camera, const glm::mat4 &projectionMatrix) override;

            void setPosition(const glm::vec3 &position, float radius);

            static vector<Gate *> generateGates();

            glm::vec3 position{};
            float radius{};
            float gateAngle{};
            bool isActive = false;
            glm::vec3 gateColor = GATE_PASSIVE_COLOR;
    };
}


#endif
