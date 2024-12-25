#ifndef DRONE_H
#define DRONE_H

#pragma once

#include <vector>
#include <iostream>
#include <algorithm>

#include "components/simple_scene.h"
#include "../objects/objects.h"
#include "../camera.h"
#include "../environment/obstacle.h"
#include "../environment/trees/tree.h"
#include "../environment/gates/gate.h"

using namespace std;
using namespace obstacles;

#define PROPELLER_HEIGHT 0.19f
#define PROPELLER_OFFSET 0.72f
#define PROPELLER_SPEED 10.0f

#define LIMB_LENGTH 2.0f
#define END_CUBE_LENGTH 0.08f

#define DRONE_SPEED 10.0f
#define DRONE_ROTATION_SPEED 3.0f

namespace drones
{
    class Drone : public gfxc::SimpleScene
    {
        public:
            Drone();
            ~Drone() override;

            void CreateDrone(const char *name, const glm::vec3 &corner, const float scaleFactor);
            void RenderDrone(Shader *shader, camera::Camera *camera, const glm::mat4 &projectionMatrix);
            
            bool collidesWithObject(BoundingSphere *sphere, obstacles::BoundingBox *box);
            bool collidesWithTree(BoundingSphere *sphere, trees::Tree *tree);
            bool forestCollisions(BoundingSphere *sphere, vector<trees::Tree *> forest);
            bool collidesWithGate(BoundingSphere *sphere, gates::Gate *gate);
            bool gatesCollisions(BoundingSphere *sphere, vector<gates::Gate *> gates);
            bool passesThroughGate(BoundingSphere *sphere, gates::Gate *gate);

            vector<objects::Cube *> limbs{};
            vector<objects::Cube *> endCubes{};
            vector<objects::Cube *> propellers{};

            BoundingSphere *boundingSphere;

            glm::vec3 position;
            float propellerAngle = 0;
            float droneAngle = 135;

            float scaleFactor = 1;
    };
}


#endif
