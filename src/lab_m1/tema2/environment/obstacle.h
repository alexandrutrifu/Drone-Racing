#ifndef OBSTACLE_H
#define OBSTACLE_H

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
#define DRONE_SPEED 3.0f
#define DRONE_ROTATION_SPEED 5.0f

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
