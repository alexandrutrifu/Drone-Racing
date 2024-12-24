#include "terrain.h"

using namespace terrain;

Terrain::Terrain() = default;
Terrain::~Terrain() = default;

void Terrain::CreateTerrain(const char *name, const glm::vec3 &corner)
{
    // Create a simple 4-vertex mesh
    vector<VertexFormat> vertices
    {
        VertexFormat(corner + glm::vec3(-size, 0,  size), glm::vec3(0, 1, 1), groundColor),
        VertexFormat(corner + glm::vec3(size, 0,  size), glm::vec3(1, 0, 1), groundColor),
        VertexFormat(corner + glm::vec3(-size, 0, -size), glm::vec3(1, 0, 0), groundColor),
        VertexFormat(corner + glm::vec3(size, 0, -size), glm::vec3(0, 1, 0), groundColor),
    };

    vector<unsigned int> indices =
    {
        0, 1, 2,        1, 3, 2
    };

    this->mesh = Terrain::CreateMesh(name, vertices, indices);
}

void Terrain::RenderTerrain(Shader *shader, camera::Camera *camera, const glm::mat4 &projectionMatrix)
{
    // Render terrain
    {
        glm::mat4 modelMatrix = glm::mat4(1);

        this->RenderObject(shader, modelMatrix, camera, projectionMatrix);
    }
}
