#include "tree.h"

using namespace trees;

Tree::Tree() = default;
Tree::~Tree() = default;

void Tree::CreateTree(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned int> &indices)
{
}

void Tree::RenderTree(Shader *shader, camera::Camera *camera, const glm::mat4 &projectionMatrix)
{
}
