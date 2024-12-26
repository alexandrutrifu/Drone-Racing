#ifndef TREE_H
#define TREE_H

#pragma once

#include <vector>
#include <iostream>
#include <random>
#include <time.h>

#include "components/simple_scene.h"
#include "../../objects/objects.h"
#include "../../camera.h"
#include "../obstacle.h"
#include "../terrain/terrain.h"

#define TRUNK_HEIGHT 0.8f
#define TRUNK_RADIUS 0.2
#define CONE_HEIGHT (TRUNK_HEIGHT * 2)
#define BIG_CONE_RADIUS (CONE_RADIUS * 1.3f)
#define CONE_RADIUS 0.7f
#define CONE_OFFSET glm::vec3(0, 0.7f, 0)

using namespace std;
using namespace obstacles;

namespace trees
{
    const int treeMaxHeight = 5;
    const int forestSize = terrain::terrainSize * 1.5f;
    
    const glm::vec3 sapGreen = glm::vec3(0.349, 0.486, 0.169);
    const glm::vec3 palmGreen = glm::vec3(0.435, 0.6, 0.251);
    const glm::vec3 ebonyGreen = glm::vec3(0.341, 0.424, 0.306);
    const glm::vec3 laSalleGreen = glm::vec3(0.031, 0.471, 0.188);

    const vector<glm::vec3> treeColors = {sapGreen, palmGreen, ebonyGreen, laSalleGreen};
    const glm::vec3 treeTrunkColor = glm::vec3(0.388, 0.259, 0);

    class Tree : public obstacles::Obstacle
    {
        public:
            Tree();
            ~Tree() override;

            void CreateTree(const char *name, const glm::vec3 &corner, const float scaleFactor,
                            const glm::vec3 &treeTrunkColor = trees::treeTrunkColor, const glm::vec3 &treeLeavesColor = sapGreen);
            void RenderTree(Shader *shader, camera::Camera *camera, const glm::mat4 &projectionMatrix);
            void setPosition(const glm::vec3 &position);
            bool collidesWithSphere(BoundingSphere *sphere, obstacles::BoundingBox *box);

            static vector<Tree *> generateForest();

            objects::Cylinder *trunk{};
            vector<objects::Cone *> cones{};

            objects::Cube *boundingBoxCube{};

            glm::vec3 position = glm::vec3(2, 1, 0);
            float scaleFactor = 1;
    };
}


#endif
