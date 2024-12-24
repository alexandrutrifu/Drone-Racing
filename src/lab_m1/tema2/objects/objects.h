#ifndef OBJECTS_H
#define OBJECTS_H

#pragma once

#include <vector>
#include <iostream>

#include "components/simple_scene.h"
#include "../camera.h"

namespace objects
{
    const glm::vec3 skyBlue = glm::vec3(0.612, 0.800, 0.925);
    const glm::vec3 droneBaseColor = glm::vec3(0.776, 0.776, 0.776);
    const glm::vec3 dronePropellerColor = glm::vec3(0, 0, 0);

    class Object : public gfxc::SimpleScene
    {
        public:
            Object();
            ~Object() override;

            Mesh *CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned int> &indices);
            void RenderObject(Shader *shader, const glm::mat4 &modelMatrix, camera::Camera *camera, const glm::mat4 &projectionMatrix);

            Mesh *mesh{};
    };

    class Cube : public Object {
        public:
            Cube();
            ~Cube() override;

            void CreateCube(const char *name, const glm::vec3 &corner, float length, const glm::vec3 &color);
    };

    class Cylinder : public Object {
        public:
            Cylinder();
            ~Cylinder() override;

            void CreateCylinder(const char *name, const glm::vec3 &corner, float height, float radius, const glm::vec3 &color);

            static const int numVertices = 72;
    };

    class Cone : public Object {
        public:
            Cone();
            ~Cone() override;

            void CreateCone(const char *name, const glm::vec3 &corner, float height, float radius, const glm::vec3 &color);
    };
}


#endif
