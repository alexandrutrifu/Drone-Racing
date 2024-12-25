#ifndef GATE_H
#define GATE_H

#pragma once

#include <vector>
#include <iostream>

#include "components/simple_scene.h"
#include "../../objects/objects.h"
#include "../../camera.h"
#include "../obstacle.h"

#define GATE_RADIUS 2

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

            glm::vec3 position{};
            float gateAngle{};
    };
}


#endif
