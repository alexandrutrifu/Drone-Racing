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

using namespace std;

#define PROPELLER_HEIGHT 0.19f
#define PROPELLER_OFFSET 0.72f
#define PROPELLER_SPEED 10.0f

#define LIMB_LENGTH 2.0f
#define END_CUBE_LENGTH 0.08f

#define DRONE_SPEED 3.0f
#define DRONE_ROTATION_SPEED 5.0f

#define BOUNDING_SPHERE_RADIUS 0.8f

namespace drones
{
    class BoundingSphere : public gfxc::SimpleScene
    {
        public:
            BoundingSphere() = default;
            ~BoundingSphere() = default;

            glm::vec3 center;
            float radius;
    };

    class Drone : public gfxc::SimpleScene
    {
        public:
            Drone();
            ~Drone() override;

            void CreateDrone(const char *name, const glm::vec3 &corner, const float scaleFactor);
            void RenderDrone(Shader *shader, camera::Camera *camera, const glm::mat4 &projectionMatrix);
            bool collidesWithObject(obstacles::BoundingBox *box);

            vector<objects::Cube *> limbs{};
            vector<objects::Cube *> endCubes{};
            vector<objects::Cube *> propellers{};

            BoundingSphere *boundingSphere;

            glm::vec3 position;
            float propellerAngle = 0;
            float droneAngle = 0;

            float scaleFactor = 1;
    };
}


#endif
