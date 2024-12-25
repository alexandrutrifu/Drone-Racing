#ifndef TERRAIN_H
#define TERRAIN_H

#pragma once

#include <vector>
#include <iostream>

#include "components/simple_scene.h"
#include "../../objects/objects.h"
#include "../../camera.h"
#include "../obstacle.h"

using namespace std;

namespace terrain
{
    const glm::vec3 groundColor = glm::vec3(0.188, 0.42, 0.251);
    const float terrainSize = 30;

    class Terrain : public obstacles::Obstacle, public objects::Object
    {
        public:
            Terrain();
            ~Terrain() override;

            void CreateTerrain(const char *name, const glm::vec3 &corner);
            void RenderTerrain(Shader *shader, camera::Camera *camera, const glm::mat4 &projectionMatrix);
    };
}


#endif
