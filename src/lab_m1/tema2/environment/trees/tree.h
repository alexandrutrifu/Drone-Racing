#ifndef TREE_H
#define TREE_H

#pragma once

#include <vector>
#include <iostream>

#include "components/simple_scene.h"
#include "../../objects/objects.h"
#include "../../camera.h"

#define TRUNK_HEIGHT 0.8f
#define TRUNK_RADIUS 0.2
#define CONE_HEIGHT (TRUNK_HEIGHT * 2)
#define CONE_RADIUS 0.7f

using namespace std;

namespace trees
{
    class Tree : public gfxc::SimpleScene
    {
        public:
            Tree();
            ~Tree() override;

            void CreateTree(const char *name, const glm::vec3 &corner, const float scaleFactor);
            void RenderTree(Shader *shader, camera::Camera *camera, const glm::mat4 &projectionMatrix);

            objects::Cylinder *trunk{};
            vector<objects::Cone *> cones{};

            glm::vec3 position = glm::vec3(2, 1, 0);
            float scaleFactor = 1;
    };
}


#endif
