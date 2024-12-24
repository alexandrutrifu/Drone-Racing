#include "tree.h"

using namespace trees;

Tree::Tree() = default;
Tree::~Tree() = default;

void Tree::CreateTree(const char *name, const glm::vec3 &corner, const float scaleFactor)
{
    // Create cylinder for trunk
    objects::Cylinder *trunk = new objects::Cylinder();

    trunk->CreateCylinder("trunk", corner, TRUNK_HEIGHT, TRUNK_RADIUS, objects::treeTrunkColor);

    this->trunk = trunk;

    // Create two cones for leaves
    objects::Cone *cone1 = new objects::Cone();
    objects::Cone *cone2 = new objects::Cone();

    cone1->CreateCone("cone1", corner, CONE_HEIGHT, BIG_CONE_RADIUS, objects::treeLeavesColor);
    cone2->CreateCone("cone2", corner + CONE_OFFSET, CONE_HEIGHT, CONE_RADIUS, objects::treeLeavesColor);

    this->cones.push_back(cone1);
    this->cones.push_back(cone2);

    // Set scale factor
    this->scaleFactor = scaleFactor;

    // Create bounding box
    glm::vec2 xBound = glm::vec2(corner.x - BIG_CONE_RADIUS, corner.x + BIG_CONE_RADIUS);
    glm::vec2 yBound = glm::vec2(corner.y, corner.y + TRUNK_HEIGHT + CONE_OFFSET.y + CONE_HEIGHT);
    glm::vec2 zBound = glm::vec2(corner.z - BIG_CONE_RADIUS, corner.z + BIG_CONE_RADIUS);

    this->boundingBox = new obstacles::BoundingBox();

    this->boundingBox->xLimits = xBound;
    this->boundingBox->yLimits = yBound;
    this->boundingBox->zLimits = zBound;
}

void Tree::RenderTree(Shader *shader, camera::Camera *camera, const glm::mat4 &projectionMatrix)
{
    // Render trunk
    {
        glm::mat4 modelMatrix = glm::mat4(1);

        modelMatrix = glm::translate(modelMatrix, this->position);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(this->scaleFactor));

        this->trunk->RenderObject(shader, modelMatrix, camera, projectionMatrix);
    }

    // Render leaves
    {
        glm::mat4 modelMatrix = glm::mat4(1);

        modelMatrix = glm::translate(modelMatrix, this->position);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(this->scaleFactor));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, TRUNK_HEIGHT, 0));

        this->cones[0]->RenderObject(shader, modelMatrix, camera, projectionMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);

        modelMatrix = glm::translate(modelMatrix, this->position);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(this->scaleFactor));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, TRUNK_HEIGHT, 0));

        this->cones[1]->RenderObject(shader, modelMatrix, camera, projectionMatrix);
    }

    // Change bounding box position
    this->boundingBox->xLimits.x = this->position.x - BIG_CONE_RADIUS * this->scaleFactor;
    this->boundingBox->xLimits.y = this->position.x + BIG_CONE_RADIUS * this->scaleFactor;

    this->boundingBox->yLimits.x = this->position.y;
    this->boundingBox->yLimits.y = this->position.y + (TRUNK_HEIGHT + CONE_OFFSET.y + CONE_HEIGHT) * this->scaleFactor;

    this->boundingBox->zLimits.x = this->position.z - BIG_CONE_RADIUS * this->scaleFactor;
    this->boundingBox->zLimits.y = this->position.z + BIG_CONE_RADIUS * this->scaleFactor;
}
