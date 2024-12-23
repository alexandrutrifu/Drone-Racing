#ifndef DRONE_H
#define DRONE_H

#pragma once

#include <vector>
#include <iostream>

#include "components/simple_scene.h"
#include "../objects/objects.h"
#include "../camera.h"

using namespace std;

namespace drones
{
    class Drone : public gfxc::SimpleScene
    {
        public:
            Drone();
            ~Drone() override;

            void CreateDrone(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned int> &indices);
            void RenderDrone(Shader *shader, camera::Camera *camera, const glm::mat4 &projectionMatrix);

            vector<Mesh *> meshes{};
            vector<objects::Cube *> limbs{};
            vector<objects::Cube *> propellers{};
    };
}


#endif
