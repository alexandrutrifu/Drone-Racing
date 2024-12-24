#include "objects.h"
#include "../camera.h"
#include <vector>

using namespace objects;
using namespace std;

Cone::Cone() = default;
Cone::~Cone() = default;

void Cone::CreateCone(const char *name, const glm::vec3 &corner, float height, float radius, const glm::vec3 &color)
{
    // Create a simple cone
    vector<VertexFormat> vertices;

    // Top vertex
    vertices.push_back(VertexFormat(corner + glm::vec3(0, height, 0), glm::vec3(0, 1, 0), color));

    // Base vertices
    for (int i = 0; i < 360; i += 10) {
        float angle = i * M_PI / 180;
        vertices.push_back(VertexFormat(corner + glm::vec3(radius * cos(angle), 0, radius * sin(angle)), glm::vec3(0, -1, 0), color));
    }

    vector<unsigned int> indices;

    // Base indices
    for (int i = 1; i < vertices.size() - 1; i++) {
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back(i + 1);
    }

    // Last triangle
    indices.push_back(0);
    indices.push_back(vertices.size() - 1);
    indices.push_back(1);

    this->mesh = Cone::CreateMesh(name, vertices, indices);
}