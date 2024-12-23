#ifndef TREE_H
#define TREE_H

#pragma once

#include <vector>
#include <iostream>

#include "components/simple_scene.h"
#include "../objects/objects.h"
#include "../camera.h"

using namespace std;

namespace trees
{
    class Tree : public gfxc::SimpleScene
    {
        public:
            Tree();
            ~Tree() override;

            void CreateTree(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned int> &indices);
            void RenderTree(Shader *shader, camera::Camera *camera, const glm::mat4 &projectionMatrix);

            vector<objects::Cube *> limbs{};
            vector<objects::Cube *> endCubes{};
            vector<objects::Cube *> propellers{};

            glm::vec3 position = glm::vec3(0, 1, 0);
            float propellerAngle = 0;
            float droneAngle = 0;
    };
}


#endif
