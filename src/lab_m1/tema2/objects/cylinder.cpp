#include "objects.h"
#include "../camera.h"
#include <vector>

using namespace objects;
using namespace std;

Cylinder::Cylinder() = default;
Cylinder::~Cylinder() = default;

void Cylinder::CreateCylinder(const char *name, const glm::vec3 &corner, float length, const glm::vec3 &color)
{
    // Create a simple cube
    vector<VertexFormat> vertices
    {
        
    };

    vector<unsigned int> indices =
    {
        0, 1, 2,        1, 3, 2,
        2, 3, 7,        2, 7, 6,
        1, 7, 3,        1, 5, 7,
        6, 7, 4,        7, 5, 4,
        0, 4, 1,        1, 4, 5,
        2, 6, 4,        0, 2, 4,
    };

    this->mesh = Cylinder::CreateMesh(name, vertices, indices);
}