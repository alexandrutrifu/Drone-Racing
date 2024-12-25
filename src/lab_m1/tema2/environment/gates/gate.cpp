#include "gate.h"

using namespace gates;

Gate::Gate() = default;
Gate::~Gate() = default;

void gates::Gate::CreateGate(const char *name, const glm::vec3 &corner, float bigRadius)
{
    vector<VertexFormat> vertices{};
    vector<unsigned int> indices{}; // Use unsigned int for indices

    float smallRadius = bigRadius - 0.1f;

    int step = 10;
    int numSegments = 360 / step;

    glm::vec3 defaultColor = glm::vec3(1, 0, 0); // Example: red color for the vertices

    // Generate first outer circle
    for (int i = 0; i < 360; i += step) {
        float angle = i * M_PI / 180;
        glm::vec3 position = glm::vec3(corner.x + cos(angle) * bigRadius, corner.y + sin(angle) * bigRadius, corner.z);
        vertices.emplace_back(position, defaultColor); // Add position and color
    }

    // Generate first inner circle
    for (int i = 0; i < 360; i += step) {
        float angle = i * M_PI / 180;
        glm::vec3 position = glm::vec3(corner.x + cos(angle) * smallRadius, corner.y + sin(angle) * smallRadius, corner.z);
        vertices.emplace_back(position, defaultColor); // Add position and color
    }

    // Generate second outer circle
    for (int i = 0; i < 360; i += step) {
        float angle = i * M_PI / 180;
        glm::vec3 position = glm::vec3(corner.x + cos(angle) * bigRadius, corner.y + sin(angle) * bigRadius, corner.z - 0.3);
        vertices.emplace_back(position, defaultColor); // Add position and color
    }

    // Generate second inner circle
    for (int i = 0; i < 360; i += step) {
        float angle = i * M_PI / 180;
        glm::vec3 position = glm::vec3(corner.x + cos(angle) * smallRadius, corner.y + sin(angle) * smallRadius, corner.z - 0.3);
        vertices.emplace_back(position, defaultColor); // Add position and color
    }

    // Generate indices

    // 1. Front face (outer and inner circles)
    for (int i = 0; i < numSegments; i++) {
        int next = (i + 1) % numSegments;

        // Outer circle front face
        indices.push_back(i);                 // Current outer vertex
        indices.push_back(next);             // Next outer vertex
        indices.push_back(numSegments + i);  // Current inner vertex

        indices.push_back(next);             // Next outer vertex
        indices.push_back(numSegments + next); // Next inner vertex
        indices.push_back(numSegments + i);  // Current inner vertex
    }

    // 2. Back face (outer and inner circles)
    for (int i = 0; i < numSegments; i++) {
        int offset = numSegments * 2;
        int next = (i + 1) % numSegments;

        // Outer circle back face
        indices.push_back(offset + i);                 // Current outer vertex
        indices.push_back(offset + numSegments + i);   // Current inner vertex
        indices.push_back(offset + next);             // Next outer vertex

        indices.push_back(offset + next);             // Next outer vertex
        indices.push_back(offset + numSegments + i);   // Current inner vertex
        indices.push_back(offset + numSegments + next); // Next inner vertex
    }

    // 3. Sides connecting front and back (outer circles)
    for (int i = 0; i < numSegments; i++) {
        int next = (i + 1) % numSegments;
        int offset = numSegments * 2;

        indices.push_back(i);                 // Front outer current
        indices.push_back(offset + i);        // Back outer current
        indices.push_back(next);              // Front outer next

        indices.push_back(next);              // Front outer next
        indices.push_back(offset + i);        // Back outer current
        indices.push_back(offset + next);     // Back outer next
    }

    // 4. Sides connecting front and back (inner circles)
    for (int i = 0; i < numSegments; i++) {
        int next = (i + 1) % numSegments;
        int offset = numSegments * 2;

        indices.push_back(numSegments + i);            // Front inner current
        indices.push_back(numSegments + next);         // Front inner next
        indices.push_back(offset + numSegments + i);   // Back inner current

        indices.push_back(numSegments + next);         // Front inner next
        indices.push_back(offset + numSegments + next); // Back inner next
        indices.push_back(offset + numSegments + i);   // Back inner current
    }

    // Use your mesh creation function here with vertices and indices
    // For example:
    this->mesh = Gate::CreateMesh(name, vertices, indices);
}

void Gate::RenderGate(Shader *shader, camera::Camera *camera, const glm::mat4 &projectionMatrix)
{
    // Render gate
    {
        glm::mat4 modelMatrix = glm::mat4(1);

        // Rotate the gate to the desired orientation
        modelMatrix = glm::rotate(modelMatrix, glm::radians(this->gateAngle), glm::vec3(0, 1, 0));
        
        // Translate the gate to the desired position
        modelMatrix = glm::translate(modelMatrix, this->position);

        // Scale the gate
        // modelMatrix = glm::scale(modelMatrix, );

        this->RenderObject(shader, modelMatrix, camera, projectionMatrix);
    }
}

