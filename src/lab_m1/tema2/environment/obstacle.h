#ifndef OBSTACLE_H
#define OBSTACLE_H

#pragma once

#include <vector>
#include <iostream>

#include "components/simple_scene.h"
#include "../objects/objects.h"
#include "../camera.h"

using namespace std;

#define BOUNDING_SPHERE_RADIUS 0.8f

namespace obstacles
{
    class BoundingBox : public gfxc::SimpleScene
    {
        public:
            BoundingBox() = default;
            ~BoundingBox() = default;

            glm::vec2 xLimits;
            glm::vec2 yLimits;
            glm::vec2 zLimits;
    };

    class BoundingSphere : public gfxc::SimpleScene
    {
        public:
            BoundingSphere() = default;
            ~BoundingSphere() = default;

            glm::vec3 center;
            float radius;
    };

    class Obstacle : public gfxc::SimpleScene
    {
        public:
            Obstacle() = default;
            ~Obstacle() override = default;

            bool overlapsWith(BoundingBox *boundingBox);

            BoundingBox *boundingBox{};
    };
}


#endif
