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

    cone1->CreateCone("cone1", corner, CONE_HEIGHT, CONE_RADIUS * 1.3f, objects::treeLeavesColor);
    cone2->CreateCone("cone2", corner + glm::vec3(0, 0.7f, 0), CONE_HEIGHT, CONE_RADIUS, objects::treeLeavesColor);

    this->cones.push_back(cone1);
    this->cones.push_back(cone2);

    // Set scale factor
    this->scaleFactor = scaleFactor;
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
}
