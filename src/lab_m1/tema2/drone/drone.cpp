#include "drone.h"

using namespace drones;

Drone::Drone() = default;
Drone::~Drone() = default;

void drones::Drone::CreateDrone(const char *name, const glm::vec3 &corner, const float scaleFactor)
{
    // Create the two limbs
    objects::Cube *limb1 = new objects::Cube();
    objects::Cube *limb2 = new objects::Cube();

    limb1->CreateCube("limb1", corner, LIMB_LENGTH, objects::droneBaseColor);
    limb2->CreateCube("limb2", corner, LIMB_LENGTH, objects::droneBaseColor);

    this->limbs.push_back(limb1);
    this->limbs.push_back(limb2);

    // Create end cubes
    objects::Cube *endCube1 = new objects::Cube();
    objects::Cube *endCube2 = new objects::Cube();
    objects::Cube *endCube3 = new objects::Cube();
    objects::Cube *endCube4 = new objects::Cube();

    endCube1->CreateCube("endCube1", corner, END_CUBE_LENGTH, objects::droneBaseColor);
    endCube2->CreateCube("endCube2", corner, END_CUBE_LENGTH, objects::droneBaseColor);
    endCube3->CreateCube("endCube3", corner, END_CUBE_LENGTH, objects::droneBaseColor);
    endCube4->CreateCube("endCube4", corner, END_CUBE_LENGTH, objects::droneBaseColor);

    this->endCubes.push_back(endCube1);
    this->endCubes.push_back(endCube2);
    this->endCubes.push_back(endCube3);
    this->endCubes.push_back(endCube4);

    // Create the four propellers
    objects::Cube *propeller1 = new objects::Cube();
    objects::Cube *propeller2 = new objects::Cube();
    objects::Cube *propeller3 = new objects::Cube();
    objects::Cube *propeller4 = new objects::Cube();

    propeller1->CreateCube("propeller1", corner, LIMB_LENGTH / 3, objects::dronePropellerColor);
    propeller2->CreateCube("propeller2", corner, LIMB_LENGTH / 3, objects::dronePropellerColor);
    propeller3->CreateCube("propeller3", corner, LIMB_LENGTH / 3, objects::dronePropellerColor);
    propeller4->CreateCube("propeller4", corner, LIMB_LENGTH / 3, objects::dronePropellerColor);

    this->propellers.push_back(propeller1);
    this->propellers.push_back(propeller2);
    this->propellers.push_back(propeller3);
    this->propellers.push_back(propeller4);

    // Set scale factor
    this->scaleFactor = scaleFactor;

    // Create bounding sphere
    this->boundingSphere = new BoundingSphere();

    this->boundingSphere->center = corner;
    this->boundingSphere->radius = BOUNDING_SPHERE_RADIUS;
}

void drones::Drone::RenderDrone(Shader *shader, camera::Camera *camera, const glm::mat4 &projectionMatrix)
{
    // Render drone limbs
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, this->position);
        modelMatrix = glm::rotate(modelMatrix, this->droneAngle, glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(this->scaleFactor));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.2f, 0.2f));
        
        this->limbs[0]->RenderObject(shader, modelMatrix, camera, projectionMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, this->position);
        modelMatrix = glm::rotate(modelMatrix, this->droneAngle, glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(this->scaleFactor));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 0.2f, 2));
        
        this->limbs[1]->RenderObject(shader, modelMatrix, camera, projectionMatrix);
    }

    // Render drone ends
    {
        glm::mat4 modelMatrix = glm::mat4(1);

        // Translate to the drone's world position
        modelMatrix = glm::translate(modelMatrix, this->position);

        // Rotate around the drone's center
        modelMatrix = glm::rotate(modelMatrix, this->droneAngle, glm::vec3(0, 1, 0));
        
        // Scale
        modelMatrix = glm::scale(modelMatrix, glm::vec3(this->scaleFactor));

        // Translate to the end cube's relative offset
        glm::vec3 relativeOffset = glm::vec3(0, END_CUBE_LENGTH, -PROPELLER_OFFSET);
        modelMatrix = glm::translate(modelMatrix, relativeOffset);

        this->endCubes[0]->RenderObject(shader, modelMatrix, camera, projectionMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);

        // Translate to the drone's world position
        modelMatrix = glm::translate(modelMatrix, this->position);

        // Rotate around the drone's center
        modelMatrix = glm::rotate(modelMatrix, this->droneAngle, glm::vec3(0, 1, 0));

        // Scale
        modelMatrix = glm::scale(modelMatrix, glm::vec3(this->scaleFactor));

        // Translate to the end cube's relative offset
        glm::vec3 relativeOffset = glm::vec3(0, END_CUBE_LENGTH, PROPELLER_OFFSET);
        modelMatrix = glm::translate(modelMatrix, relativeOffset);
        
        this->endCubes[1]->RenderObject(shader, modelMatrix, camera, projectionMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);

        // Translate to the drone's world position
        modelMatrix = glm::translate(modelMatrix, this->position);

        // Rotate around the drone's center
        modelMatrix = glm::rotate(modelMatrix, this->droneAngle, glm::vec3(0, 1, 0));

        // Scale
        modelMatrix = glm::scale(modelMatrix, glm::vec3(this->scaleFactor));

        // Translate to the end cube's relative offset
        glm::vec3 relativeOffset = glm::vec3(PROPELLER_OFFSET, END_CUBE_LENGTH, 0);
        modelMatrix = glm::translate(modelMatrix, relativeOffset);
        
        this->endCubes[2]->RenderObject(shader, modelMatrix, camera, projectionMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);

        // Translate to the drone's world position
        modelMatrix = glm::translate(modelMatrix, this->position);

        // Rotate around the drone's center
        modelMatrix = glm::rotate(modelMatrix, this->droneAngle, glm::vec3(0, 1, 0));

        // Scale
        modelMatrix = glm::scale(modelMatrix, glm::vec3(this->scaleFactor));

        // Translate to the end cube's relative offset
        glm::vec3 relativeOffset = glm::vec3(-PROPELLER_OFFSET, END_CUBE_LENGTH, 0);
        modelMatrix = glm::translate(modelMatrix, relativeOffset);
        
        this->endCubes[3]->RenderObject(shader, modelMatrix, camera, projectionMatrix);
    }

    // Render drone propellers (north, south, east, west)
    {
        glm::mat4 modelMatrix = glm::mat4(1);

        modelMatrix = glm::translate(modelMatrix, this->position);

        modelMatrix = glm::rotate(modelMatrix, this->droneAngle, glm::vec3(0, 1, 0));

        // Scale
        modelMatrix = glm::scale(modelMatrix, glm::vec3(this->scaleFactor));

        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, PROPELLER_HEIGHT, -PROPELLER_OFFSET));

        modelMatrix = glm::rotate(modelMatrix, this->propellerAngle, glm::vec3(0, 1, 0));

        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 0.2f, 2));
        
        this->propellers[0]->RenderObject(shader, modelMatrix, camera, projectionMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);

        modelMatrix = glm::translate(modelMatrix, this->position);

        modelMatrix = glm::rotate(modelMatrix, this->droneAngle, glm::vec3(0, 1, 0));

        // Scale
        modelMatrix = glm::scale(modelMatrix, glm::vec3(this->scaleFactor));

        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, PROPELLER_HEIGHT, PROPELLER_OFFSET));

        modelMatrix = glm::rotate(modelMatrix, this->propellerAngle, glm::vec3(0, 1, 0));

        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 0.2f, 2));
        
        
        this->propellers[1]->RenderObject(shader, modelMatrix, camera, projectionMatrix);
    }
    
    {
        glm::mat4 modelMatrix = glm::mat4(1);

        modelMatrix = glm::translate(modelMatrix, this->position);

        modelMatrix = glm::rotate(modelMatrix, this->droneAngle, glm::vec3(0, 1, 0));

        // Scale
        modelMatrix = glm::scale(modelMatrix, glm::vec3(this->scaleFactor));

        modelMatrix = glm::translate(modelMatrix, glm::vec3(PROPELLER_OFFSET, PROPELLER_HEIGHT, 0));

        modelMatrix = glm::rotate(modelMatrix, this->propellerAngle, glm::vec3(0, 1, 0));

        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 0.2f, 2));
        
        this->propellers[2]->RenderObject(shader, modelMatrix, camera, projectionMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);

        modelMatrix = glm::translate(modelMatrix, this->position);

        modelMatrix = glm::rotate(modelMatrix, this->droneAngle, glm::vec3(0, 1, 0));

        // Scale
        modelMatrix = glm::scale(modelMatrix, glm::vec3(this->scaleFactor));

        modelMatrix = glm::translate(modelMatrix, glm::vec3(-PROPELLER_OFFSET, PROPELLER_HEIGHT, 0));

        modelMatrix = glm::rotate(modelMatrix, this->propellerAngle, glm::vec3(0, 1, 0));

        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 0.2f, 2));
        
        this->propellers[3]->RenderObject(shader, modelMatrix, camera, projectionMatrix);
    }

}

bool drones::Drone::collidesWithObject(BoundingSphere *sphere, obstacles::BoundingBox *box)
{
    const float x = std::max(box->xLimits.x, std::min(sphere->center.x, box->xLimits.y));
    const float y = std::max(box->yLimits.x, std::min(sphere->center.y, box->yLimits.y));
    const float z = std::max(box->zLimits.x, std::min(sphere->center.z, box->zLimits.y));

    const float distance = std::sqrt(
        (x - sphere->center.x) * (x - sphere->center.x) +
        (y - sphere->center.y) * (y - sphere->center.y) +
        (z - sphere->center.z) * (z - sphere->center.z)
    );

    return distance < sphere->radius;
}

bool drones::Drone::collidesWithTree(BoundingSphere *sphere, trees::Tree *tree)
{
    // Handle collision with each tree part
    return this->collidesWithObject(sphere, tree->boundingBox);
}

bool drones::Drone::forestCollisions(BoundingSphere *sphere, vector<trees::Tree *> forest)
{
    return std::any_of(forest.begin(), forest.end(), [this, sphere](trees::Tree *tree) {
        return this->collidesWithTree(sphere, tree);
    });
}

bool drones::Drone::collidesWithGate(BoundingSphere *sphere, gates::Gate *gate)
{
    // Gate dimensions
    float gateHalfRadius = gate->radius / 2.0f; // Half of the gate's radius
    float gateThickness = 0.4f;                // Depth along the Z-axis
    float gateHeight = gate->radius;           // Total height of the gate

    // Calculate the relative bounds for each side of the gate

    // Left side of the gate
    bool collidesLeft = sphere->center.x >= gate->position.x - gate->radius
                     && sphere->center.x <= gate->position.x - gateHalfRadius
                     && sphere->center.y >= gate->position.y - gateHeight
                     && sphere->center.y <= gate->position.y + gateHeight
                     && sphere->center.z >= gate->position.z - gateThickness
                     && sphere->center.z <= gate->position.z + gateThickness;

    // Right side of the gate
    bool collidesRight = sphere->center.x >= gate->position.x + gateHalfRadius
                      && sphere->center.x <= gate->position.x + gate->radius
                      && sphere->center.y >= gate->position.y - gateHeight
                      && sphere->center.y <= gate->position.y + gateHeight
                      && sphere->center.z >= gate->position.z - gateThickness
                      && sphere->center.z <= gate->position.z + gateThickness;

    // Top part of the gate
    bool collidesTop = sphere->center.x >= gate->position.x - gate->radius
                    && sphere->center.x <= gate->position.x + gate->radius
                    && sphere->center.y >= gate->position.y + gateHalfRadius
                    && sphere->center.y <= gate->position.y + gateHeight
                    && sphere->center.z >= gate->position.z - gateThickness
                    && sphere->center.z <= gate->position.z + gateThickness;

    // Bottom part of the gate
    bool collidesBottom = sphere->center.x >= gate->position.x - gate->radius
                       && sphere->center.x <= gate->position.x + gate->radius
                       && sphere->center.y >= gate->position.y - gateHeight
                       && sphere->center.y <= gate->position.y - gateHalfRadius
                       && sphere->center.z >= gate->position.z - gateThickness
                       && sphere->center.z <= gate->position.z + gateThickness;

    // Return true if the sphere collides with any part of the gate
    return collidesLeft || collidesRight || collidesTop || collidesBottom;
}

bool drones::Drone::gatesCollisions(BoundingSphere *sphere, vector<gates::Gate *> gates)
{
    return std::any_of(gates.begin(), gates.end(), [this, sphere](gates::Gate *gate) {
        return this->collidesWithGate(sphere, gate) && !this->passesThroughGate(sphere, gate);
    });
}

bool drones::Drone::passesThroughGate(BoundingSphere *sphere, gates::Gate *gate)
{
    return sphere->center.x < gate->position.x + gate->radius * 0.8f
        && sphere->center.x > gate->position.x - gate->radius * 0.8f
        && sphere->center.y < gate->position.y + gate->radius * 0.8f
        && sphere->center.y > gate->position.y - gate->radius * 0.8f
        && sphere->center.z < gate->position.z + 0.3f
        && sphere->center.z > gate->position.z - 0.3f;
}
