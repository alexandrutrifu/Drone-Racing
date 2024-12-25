#include "lab_m1/tema2/tema2.h"

using namespace m1;


Tema2::Tema2()
= default;

Tema2::~Tema2()
= default;

void Tema2::Init()
{
    renderCameraTarget = false;

    camera = new camera::Camera();
    camera->Set(glm::vec3(0, 2, 0.2), glm::vec3(0, 2, 0), glm::vec3(0, 1, 0));
    camera->distanceToTarget = 2;

    glm::vec3 corner = glm::vec3(0, 0, 0);

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create terrain
    {
        terrain = new terrain::Terrain();
        terrain->CreateTerrain("terrain", corner);
    }

    // Create the drone meshes
    {
        drone = new drones::Drone();
        drone->CreateDrone("drone", corner, 1);

        this->drone->position = glm::vec3(0, 1, 0);
    }

    // Create the tree meshes
    {
        forest = trees::Tree::generateForest();
    }

    // Create gates
    {
        gates = gates::Gate::generateGates();

        // Checkpoint order
        for (int i = 0; i < gates.size(); i++) {
            checkpointOrder.push_back(i);
        }

        std::random_shuffle(checkpointOrder.begin(), checkpointOrder.end());

        // Activate first gate
        gates[checkpointOrder[0]]->isActive = true;
        gates[checkpointOrder[0]]->gateColor = GATE_ACTIVE_COLOR;
        currentGate = gates[checkpointOrder[0]];
    }

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader *shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Terrain shader
    {
        Shader *shader = new Shader("TerrainShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "terrainShaders", "TerrainVertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "terrainShaders", "TerrainFragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Gate shader
    {
        Shader *shader = new Shader("GateShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "gateShaders", "GateVertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "gateShaders", "GateFragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
}

void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glEnable(GL_DEPTH_TEST);
    // Enable line smoothing and blending
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LESS);
    glClearColor(skyBlue.x, skyBlue.y, skyBlue.z, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::Update(float deltaTimeSeconds)
{
    // Render the camera target. This is useful for understanding where
    // the rotation point is, when moving in third-person camera mode.
    // if (renderCameraTarget)
    // {
    //     glm::mat4 modelMatrix = glm::mat4(1);
    //     modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    //     modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
    //     RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
    // }

    // Render terrain
    terrain->RenderTerrain(shaders["TerrainShader"], camera, projectionMatrix);

    // Render drone
    drone->RenderDrone(shaders["LabShader"], camera, projectionMatrix);

    // Update the propeller angle
    drone->propellerAngle += PROPELLER_SPEED * deltaTimeSeconds;

    // Render trees
    for (auto &tree : forest) {
        tree->RenderTree(shaders["LabShader"], camera, projectionMatrix);
    }

    // Render gates
    for (auto &gate : gates) {
        gate->RenderGate(shaders["GateShader"], camera, projectionMatrix);
    }

    // Check if drone passes through a gate
    if (drone->passesThroughGate(drone->boundingSphere, currentGate)) {
        // Deactivate the current gate
        currentGate->isActive = false;
        currentGate->gateColor = GATE_PASSIVE_COLOR;

        // Check if game is over
        if (currentCheckpoint == NUM_GATES - 1) {
            // Game over
            cout << "Game over!" << endl;
            exit(0);
        }

        // Activate the next gate
        currentGate = gates[checkpointOrder[++currentCheckpoint]];
        currentGate->isActive = true;
        currentGate->gateColor = GATE_ACTIVE_COLOR;
    }
}


void Tema2::FrameEnd()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
   // move the camera only if MOUSE_RIGHT button is pressed
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        // float cameraSpeed = 6.0f;

        // if (window->KeyHold(GLFW_KEY_W)) {
        //     // TODO(student): Translate the camera forward
        //     camera->MoveForward(cameraSpeed * deltaTime);
        // }

        // if (window->KeyHold(GLFW_KEY_A)) {
        //     // TODO(student): Translate the camera to the left
        //     camera->TranslateRight(-cameraSpeed * deltaTime);

        // }

        // if (window->KeyHold(GLFW_KEY_S)) {
        //     // TODO(student): Translate the camera backward
        //     camera->MoveForward(-cameraSpeed * deltaTime);

        // }

        // if (window->KeyHold(GLFW_KEY_D)) {
        //     // TODO(student): Translate the camera to the right
        //     camera->TranslateRight(cameraSpeed * deltaTime);

        // }

        // if (window->KeyHold(GLFW_KEY_Q)) {
        //     // TODO(student): Translate the camera downward
        //     camera->TranslateUpward(-cameraSpeed * deltaTime);

        // }

        // if (window->KeyHold(GLFW_KEY_E)) {
        //     // TODO(student): Translate the camera upward
        //     camera->TranslateUpward(cameraSpeed * deltaTime);

        // }

        // if (window->KeyHold(GLFW_KEY_LEFT)) {
        //     fov -= cameraSpeed * deltaTime;
        //     projectionMatrix = glm::perspective(fov, window->props.aspectRatio, zNear, zFar);

        // }

        // if (window->KeyHold(GLFW_KEY_RIGHT)) {
        //     fov += cameraSpeed * deltaTime;
        //     projectionMatrix = glm::perspective(fov, window->props.aspectRatio, zNear, zFar);
        // }

        // if (window->KeyHold(GLFW_KEY_K)) {
        //     bottom -= cameraSpeed * deltaTime;
        //     top += cameraSpeed * deltaTime;
        //     left -= cameraSpeed * deltaTime;
        //     right += cameraSpeed * deltaTime;
        //     projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
        // }
        
        // if (window->KeyHold(GLFW_KEY_L)) {
        //     left += cameraSpeed * deltaTime;
        //     right -= cameraSpeed * deltaTime;
        //     bottom += cameraSpeed * deltaTime;
        //     top -= cameraSpeed * deltaTime;
        //     projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
        // }
    } else {
        glm::vec3 nextPosition = drone->position; // Start with the current position
        glm::vec3 cameraNextPosition = camera->position;

        // Predict the next position
        // Calculate drone's forward direction based on rotation
        glm::vec3 forwardDir = glm::vec3(
            sin(drone->droneAngle),
            0,
            cos(drone->droneAngle)
        );

        // Forward and backward movement (W/S)
        if (window->KeyHold(GLFW_KEY_W)) {
            nextPosition += forwardDir * deltaTime * DRONE_SPEED;
            cameraNextPosition += forwardDir * deltaTime * DRONE_SPEED;
        }
        if (window->KeyHold(GLFW_KEY_S)) {
            nextPosition -= forwardDir * deltaTime * DRONE_SPEED;
            cameraNextPosition -= forwardDir * deltaTime * DRONE_SPEED;
        }

        // Left and right strafe movement (A/D)
        if (window->KeyHold(GLFW_KEY_A)) {
            nextPosition -= glm::cross(forwardDir, glm::vec3(0, 1, 0)) * deltaTime * DRONE_SPEED;
            cameraNextPosition -= glm::cross(forwardDir, glm::vec3(0, 1, 0)) * deltaTime * DRONE_SPEED;
        }
        if (window->KeyHold(GLFW_KEY_D)) {
            nextPosition += glm::cross(forwardDir, glm::vec3(0, 1, 0)) * deltaTime * DRONE_SPEED;
            cameraNextPosition += glm::cross(forwardDir, glm::vec3(0, 1, 0)) * deltaTime * DRONE_SPEED;
        }
        if (window->KeyHold(GLFW_KEY_Q)) {
            cameraNextPosition -= glm::normalize(camera->up) * deltaTime * DRONE_SPEED;
            nextPosition.y -= deltaTime * DRONE_SPEED;
        }
        if (window->KeyHold(GLFW_KEY_E)) {
            cameraNextPosition += glm::normalize(camera->up) * deltaTime * DRONE_SPEED;
            nextPosition.y += deltaTime * DRONE_SPEED;
        }
        if (window->KeyHold(GLFW_KEY_LEFT)) {
            drone->droneAngle += deltaTime * DRONE_ROTATION_SPEED;
            camera->RotateThirdPerson_OY(deltaTime * DRONE_ROTATION_SPEED);
        }

        if (window->KeyHold(GLFW_KEY_RIGHT)) {
            drone->droneAngle -= deltaTime * DRONE_ROTATION_SPEED;
            camera->RotateThirdPerson_OY(-deltaTime * DRONE_ROTATION_SPEED);
        }

        // Ensure the angle is within a valid range (optional, for better numerical stability)
        if (drone->droneAngle > 360.0f) drone->droneAngle -= 360.0f;
        if (drone->droneAngle < 0.0f) drone->droneAngle += 360.0f;

        // Form new bounding sphere for next position
        obstacles::BoundingSphere *nextBoundingSphere = new obstacles::BoundingSphere();

        nextBoundingSphere->center = nextPosition;
        nextBoundingSphere->radius = BOUNDING_SPHERE_RADIUS;

        // Update position only if no collision occurs
        if (nextPosition.y > 0.2f
            && !drone->forestCollisions(nextBoundingSphere, forest)
            && !drone->gatesCollisions(nextBoundingSphere, gates)) {
            // Move camera to the new position
            camera->position = cameraNextPosition;

            camera->forward = glm::normalize(forwardDir);
            camera->right = glm::cross(camera->forward, camera->up);

            drone->position = nextPosition;
            drone->boundingSphere->center = nextPosition; // Update the bounding sphere position
        } else {
            // Print a message if a collision occurs
            cout << "Collision detected!" << endl;
        }

        if (drone->gatesCollisions(nextBoundingSphere, gates)) {
            cout << "Collision with gate detected!" << endl;
        }
    }
}


void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_T)
    {
        renderCameraTarget = !renderCameraTarget;
    }

    if (key == GLFW_KEY_O) {
        projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
    }

    if (key == GLFW_KEY_P) {
        projectionMatrix = glm::perspective(fov, window->props.aspectRatio, zNear, zFar);
    }
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    // {
    //     float mouseSensitivityX = 0.005f;
    //     float mouseSensitivityY = 0.005f;

    //     if (window->GetSpecialKeyState() == 0) {
    //         renderCameraTarget = false;
    //         // TODO(student): Rotate the camera in first-person mode around
    //         // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
    //         // variables for setting up the rotation speed.
    //         camera->RotateFirstPerson_OX(-deltaY * mouseSensitivityY);
    //         camera->RotateFirstPerson_OY(-deltaX * mouseSensitivityX);

    //     }

    //     if (window->GetSpecialKeyState() & GLFW_MOD_CONTROL) {
    //         renderCameraTarget = true;
    //         // TODO(student): Rotate the camera in third-person mode around
    //         // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
    //         // variables for setting up the rotation speed.
    //         camera->RotateThirdPerson_OX(-deltaY * mouseSensitivityY);
    //         camera->RotateThirdPerson_OY(-deltaX * mouseSensitivityX);

    //     }
    // }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // if (button == GLFW_MOUSE_BUTTON_RIGHT) {
    //     // Snap the camera back to the drone
    //     glm::vec3 dronePosition = drone->position;
    //     float droneAngle = drone->droneAngle;

    //     // Offset the camera relative to the drone
    //     glm::vec3 offset = glm::vec3(0, 2, 0.2f); // Position the camera slightly behind and above the drone
    //     glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), droneAngle, glm::vec3(0, 1, 0));
    //     glm::vec3 rotatedOffset = glm::vec3(rotationMatrix * glm::vec4(offset, 1.0f));

    //     // Calculate the new camera position
    //     glm::vec3 cameraPosition = dronePosition + rotatedOffset;

    //     // The camera should look at the drone's position
    //     glm::vec3 cameraForward = glm::normalize(dronePosition - cameraPosition);

    //     // Calculate the new right and up vectors
    //     glm::vec3 cameraRight = glm::normalize(glm::cross(glm::vec3(0, 1, 0), cameraForward));
    //     glm::vec3 cameraUp = glm::cross(cameraForward, cameraRight);

    //     // Update the camera's position and orientation
    //     camera->Set(cameraPosition, glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

    //     // Explicitly update the camera's orientation vectors
    //     camera->forward = cameraForward;
    //     camera->right = cameraRight;
    //     camera->up = cameraUp;
    // }
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
