#include "terrain.h"

using namespace terrain;

Terrain::Terrain() = default;
Terrain::~Terrain() = default;

void Terrain::CreateTerrain(const char *name, const glm::vec3 &corner)
{
    float size = terrainSize;

    vector<VertexFormat> vertices{};
    vector<unsigned int> indices{};

    int step = 1;
    int vertexCount = (2 * size / step) + 1;  // Total vertices along one axis

    // Generate vertices from -size to size
    for (int i = 0; i < vertexCount; i++) {
        for (int j = 0; j < vertexCount; j++) {
            float xPos = corner.x - size + i * step;
            float zPos = corner.z - size + j * step;

            vertices.push_back(VertexFormat(glm::vec3(xPos, 0, zPos), glm::vec3(0, 1, 0), groundColor));
        }
    }

    // Generate indices for triangles
    for (int i = 0; i < vertexCount - 1; i++) {
        for (int j = 0; j < vertexCount - 1; j++) {
            int topLeft = i * vertexCount + j;
            int topRight = topLeft + 1;
            int bottomLeft = (i + 1) * vertexCount + j;
            int bottomRight = bottomLeft + 1;

            // First triangle
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            // Second triangle
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

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
