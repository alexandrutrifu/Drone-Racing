#include "objects.h"
#include "../camera.h"
#include <vector>

using namespace objects;
using namespace std;

Cylinder::Cylinder() = default;
Cylinder::~Cylinder() = default;

void Cylinder::CreateCylinder(const char *name, const glm::vec3 &corner, float height, float radius, const glm::vec3 &color)
{
    // Create a simple cylinder
    vector<VertexFormat> vertices;

    int circleVertices = numVertices / 2;
    int step = 360 / circleVertices;

    // Base vertices
    for (int i = 0; i < 360; i += step) {
        float angle = i * M_PI / 180;
        vertices.push_back(VertexFormat(corner + glm::vec3(radius * cos(angle), 0, radius * sin(angle)), glm::vec3(0, -1, 0), color));
    }

    // Top vertices
    for (int i = 0; i < 360; i += step) {
        float angle = i * M_PI / 180;
        vertices.push_back(VertexFormat(corner + glm::vec3(radius * cos(angle), height, radius * sin(angle)), glm::vec3(0, 1, 0), color));
    }

    vector<unsigned int> indices;

    // Base indices
    for (int i = 1; i < circleVertices - 1; i++) {
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back(i + 1);
    }

    // Last triangle
    indices.push_back(0);
    indices.push_back(circleVertices - 1);
    indices.push_back(1);

    // Top indices
    for (int i = circleVertices + 1; i < numVertices - 1; i++) {
        indices.push_back(circleVertices);
        indices.push_back(i);
        indices.push_back(i + 1);
    }

    // Last triangle for the top
    indices.push_back(circleVertices);
    indices.push_back(numVertices - 1);
    indices.push_back(circleVertices + 1);

    // Side indices
    for (int i = 0; i < circleVertices; i++) {
        int next = (i + 1) % circleVertices;
        indices.push_back(i);   // Bottom vertex
        indices.push_back(circleVertices + i);  // Top vertex
        indices.push_back(circleVertices + next);   // Next top vertex

        indices.push_back(i);   // Bottom vertex
        indices.push_back(circleVertices + next);   // Next top vertex
        indices.push_back(next);    // Next bottom vertex
    }

    this->mesh = Cylinder::CreateMesh(name, vertices, indices);
}