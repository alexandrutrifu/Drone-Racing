#ifndef DRONE_H
#define DRONE_H

#pragma once

#include <vector>
#include <iostream>

#include "components/simple_scene.h"
#include "../objects/objects.h"
#include "../camera.h"

using namespace std;

#define PROPELLER_HEIGHT 0.19f
#define PROPELLER_OFFSET 0.72f
#define PROPELLER_SPEED 10.0f
#define LIMB_LENGTH 2.0f
#define END_CUBE_LENGTH 0.08f

namespace drones
{
    class Drone : public gfxc::SimpleScene
    {
        public:
            Drone();
            ~Drone() override;

            void CreateDrone(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned int> &indices);
            void RenderDrone(Shader *shader, camera::Camera *camera, const glm::mat4 &projectionMatrix);

            vector<objects::Cube *> limbs{};
            vector<objects::Cube *> endCubes{};
            vector<objects::Cube *> propellers{};

            glm::vec3 position = glm::vec3(0, 1, 0);
            float propellerAngle = 0;
    };
}


#endif
