#include "drone.h"

using namespace drones;

Drone::Drone() = default;
Drone::~Drone() = default;

void drones::Drone::CreateDrone(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned int> &indices)
{
    // Create the two limbs
    objects::Cube *limb1 = new objects::Cube();
    objects::Cube *limb2 = new objects::Cube();

    limb1->CreateCube("limb1", glm::vec3(0, 0, 0), 2, objects::droneBaseColor);
    limb2->CreateCube("limb2", glm::vec3(0, 0, 0), 2, objects::droneBaseColor);

    this->limbs.push_back(limb1);
    this->limbs.push_back(limb2);

    // Create the four propellers
    objects::Cube *propeller1 = new objects::Cube();
    objects::Cube *propeller2 = new objects::Cube();
    objects::Cube *propeller3 = new objects::Cube();
    objects::Cube *propeller4 = new objects::Cube();

    propeller1->CreateCube("propeller1", glm::vec3(0, 0, 0), 1, objects::dronePropellerColor);
    propeller2->CreateCube("propeller2", glm::vec3(0, 0, 0), 1, objects::dronePropellerColor);
    propeller3->CreateCube("propeller3", glm::vec3(0, 0, 0), 1, objects::dronePropellerColor);
    propeller4->CreateCube("propeller4", glm::vec3(0, 0, 0), 1, objects::dronePropellerColor);

    // TODO: set initial positions

    this->propellers.push_back(propeller1);
    this->propellers.push_back(propeller2);
    this->propellers.push_back(propeller3);
    this->propellers.push_back(propeller4);
}

void drones::Drone::RenderDrone(Shader *shader, camera::Camera *camera, const glm::mat4 &projectionMatrix)
{
    // Render drone limbs
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.2f, 0.2f));
        
        this->limbs[0]->RenderObject(shader, modelMatrix, camera, projectionMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 0.2f, 2));
        
        this->limbs[1]->RenderObject(shader, modelMatrix, camera, projectionMatrix);
    }

    // Render drone propellers

}
