#include "lab_m1/tema2/tema2.h"
#include "tema2.h"

using namespace m1;


Tema2::Tema2()
= default;

Tema2::~Tema2()
= default;

void Tema2::Init()
{
    renderCameraTarget = false;

    camera = new camera::Camera();
    camera->Set(glm::vec3(0, 2, 3), glm::vec3(0, 2, 0), glm::vec3(0, 1, 0));
    camera->distanceToTarget = 3;

    // Mini-map camera
    miniMapCamera = new camera::Camera();
    miniMapCamera->Set(glm::vec3(0, 2, 0.2), glm::vec3(0, 2, 0), glm::vec3(0, 1, 0));
    miniMapCamera->distanceToTarget = 0.2;

    // Initial rotation
    // miniMapCamera->RotateThirdPerson_OY(glm::radians(45.0f));

    glm::vec3 corner = glm::vec3(0, 0, 0);

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Text
    {
        // Default mode for filling polygons
        polygonMode = GL_FILL;

        // Create the text renderer object
        glm::ivec2 resolution = window->GetResolution();
        textRenderer = new gfxc::TextRenderer(window->props.selfDir, resolution.x, resolution.y);

        textRenderer->Load(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FONTS, "Hack-Bold.ttf"), 80);
    }

    // Create terrain
    {
        terrain = new terrain::Terrain();
        terrain->CreateTerrain("terrain", corner);
    }

    // Create the drone meshes
    {
        drone = new drones::Drone();
        drone->CreateDrone("drone", corner, 0.8f);

        this->drone->position = glm::vec3(0, 1, 0);
        this->drone->droneAngle = 135;

        // Update camera vectors
        camera->forward = glm::normalize(glm::vec3(
            sin(drone->droneAngle),
            0,
            cos(drone->droneAngle))
        );

        camera->right = glm::normalize(glm::cross(camera->forward, camera->up));

        // Update mini-map camera vectors
        miniMapCamera->forward = glm::normalize(glm::vec3(
            sin(drone->droneAngle),
            0,
            cos(drone->droneAngle))
        );

        miniMapCamera->right = glm::normalize(glm::cross(miniMapCamera->forward, miniMapCamera->up));
    }

    // Create the tree meshes
    {
        forest = trees::Tree::generateForest();
    }

    // Create gates
    {
        gates = gates::Gate::generateGates(forest);

        // Checkpoint order
        for (int i = 0; i < gates.size(); i++) {
            checkpointOrder.push_back(i);
            checkpointOrder.push_back(i);
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
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    // Clears the color buffer (using the previously set color) and depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable line smoothing and blending
    glClearColor(skyBlue.x, skyBlue.y, skyBlue.z, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const glm::ivec2 resolution = window->GetResolution();
}


void Tema2::Update(float deltaTimeSeconds)
{
    // Time update
    nanoSecondsPassed += deltaTimeSeconds;

    // Update countdown timer
    if (startCountdown) {
        if (nanoSecondsPassed >= 1.0f) {
            nanoSecondsPassed = 0.0f;
            countdown--;
        }

        if (countdown < 0) {
            startCountdown = false;
        }
    } else {
        // Substract a second from the timer
        if (nanoSecondsPassed >= 1.0f) {
            nanoSecondsPassed = 0.0f;
            timerSeconds2--;
        }

        // Check if a minute has passed
        if (timerSeconds2 < 0) {
            timerSeconds2 = 9;
            timerSeconds1--;
        }

        // Check if the timer has reached 0
        if (timerSeconds1 < 0) {
            timerSeconds1 = 5;
            timerMinutes--;
        }

        // If time is up, game over
        if (timerMinutes < 0) {
            gameOver = true;
        }
    }

    glm::ivec2 resolution = window->GetResolution();

    // Main viewport
    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0), true);

    projectionMatrix = glm::perspective(fov, window->props.aspectRatio, zNear, zFar);

    DrawHUD();
    DrawScene(camera, deltaTimeSeconds);

    // Mini-map viewport
    viewSpace = ViewportSpace(resolution.x - resolution.x / 4, resolution.y - resolution.y / 4, resolution.x / 4, resolution.y / 4);
    SetViewportArea(viewSpace, glm::vec3(0.5f), true);
    
    // Calculate the camera's position around the drone
    glm::vec3 minimapPosition = drone->position + glm::vec3(
        drone->boundingSphere->radius * cos(drone->droneAngle), // X offset
        4,                     // Y (height) offset
        drone->boundingSphere->radius * sin(drone->droneAngle)  // Z offset
    );

    // Set the minimap camera's position and orientation
    miniMapCamera->Set(
        minimapPosition,            // Camera position
        drone->position,            // Look at the drone
        glm::vec3(0, 1, 0)          // Up vector
    );

    // Draw the scene
    DrawScene(miniMapCamera, deltaTimeSeconds);
}


void Tema2::FrameEnd()
{
    // DrawHUD();
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    if (gameOver || startCountdown) {
        return;
    }

    // Create a temporary camera
    camera::Camera* tempCamera = new camera::Camera();
    tempCamera->forward = camera->forward;
    tempCamera->right = camera->right;
    tempCamera->up = camera->up;
    tempCamera->position = camera->position;

    // Temporary minimap camera
    camera::Camera* tempMiniMapCamera = new camera::Camera();
    tempMiniMapCamera->forward = miniMapCamera->forward;
    tempMiniMapCamera->right = miniMapCamera->right;
    tempMiniMapCamera->up = miniMapCamera->up;
    tempMiniMapCamera->position = miniMapCamera->position;

    glm::vec3 nextPosition = drone->position;

    // Forward and backward movement (W/S)
    if (window->KeyHold(GLFW_KEY_W)) {
        tempCamera->TranslateForward(deltaTime * DRONE_SPEED);
        tempMiniMapCamera->TranslateForward(deltaTime * DRONE_SPEED);
        nextPosition += tempCamera->forward * deltaTime * DRONE_SPEED;
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        tempCamera->TranslateForward(-deltaTime * DRONE_SPEED);
        tempMiniMapCamera->TranslateForward(-deltaTime * DRONE_SPEED);
        nextPosition -= tempCamera->forward * deltaTime * DRONE_SPEED;
    }

    // Left and right strafing (A/D)
    if (window->KeyHold(GLFW_KEY_A)) {
        tempCamera->TranslateRight(-deltaTime * DRONE_SPEED);
        tempMiniMapCamera->TranslateRight(-deltaTime * DRONE_SPEED);
        nextPosition -= tempCamera->right * deltaTime * DRONE_SPEED;
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        tempCamera->TranslateRight(deltaTime * DRONE_SPEED);
        tempMiniMapCamera->TranslateRight(deltaTime * DRONE_SPEED);
        nextPosition += tempCamera->right * deltaTime * DRONE_SPEED;
    }

    // Upward and downward movement (Q/E)
    if (window->KeyHold(GLFW_KEY_Q)) {
        tempCamera->TranslateUpward(-deltaTime * DRONE_SPEED);
        tempMiniMapCamera->TranslateUpward(-deltaTime * DRONE_SPEED);
        nextPosition -= tempCamera->up * deltaTime * DRONE_SPEED;
    }
    if (window->KeyHold(GLFW_KEY_E)) {
        tempCamera->TranslateUpward(deltaTime * DRONE_SPEED);
        tempMiniMapCamera->TranslateUpward(deltaTime * DRONE_SPEED);
        nextPosition += tempCamera->up * deltaTime * DRONE_SPEED;
    }

    // Handle drone rotation (LEFT/RIGHT arrow keys)
    if (window->KeyHold(GLFW_KEY_LEFT)) {
        drone->droneAngle += deltaTime * DRONE_ROTATION_SPEED;
        tempCamera->RotateThirdPerson_OY(deltaTime * DRONE_ROTATION_SPEED);
        tempMiniMapCamera->RotateThirdPerson_OY(deltaTime * DRONE_ROTATION_SPEED);
    }
    if (window->KeyHold(GLFW_KEY_RIGHT)) {
        drone->droneAngle -= deltaTime * DRONE_ROTATION_SPEED;
        tempCamera->RotateThirdPerson_OY(-deltaTime * DRONE_ROTATION_SPEED);
        tempMiniMapCamera->RotateThirdPerson_OY(-deltaTime * DRONE_ROTATION_SPEED);
    }

    // Check collisions before applying changes
    obstacles::BoundingSphere* nextBoundingSphere = new obstacles::BoundingSphere();

    nextBoundingSphere->center = nextPosition;
    nextBoundingSphere->radius = BOUNDING_SPHERE_RADIUS;

    if (nextPosition.y > 0.2f
        && !drone->forestCollisions(nextBoundingSphere, forest)
        && !drone->gatesCollisions(nextBoundingSphere, gates)) {
        // Apply the temporary camera's values to the main camera
        camera->position = tempCamera->position;
        camera->forward = tempCamera->forward;
        camera->right = tempCamera->right;
        camera->up = tempCamera->up;

        // Apply the temporary minimap camera's values to the minimap camera
        miniMapCamera->position = tempMiniMapCamera->position;
        miniMapCamera->forward = tempMiniMapCamera->forward;
        miniMapCamera->right = tempMiniMapCamera->right;
        miniMapCamera->up = tempMiniMapCamera->up;

        // Update the drone's position
        drone->position = nextPosition;
        drone->boundingSphere->center = nextPosition;
    } else {
        std::cout << "Collision detected!" << std::endl;
    }

    // Ensure drone angle is within 0-360 degrees
    if (drone->droneAngle > 360.0f) drone->droneAngle -= 360.0f;
    if (drone->droneAngle < 0.0f) drone->droneAngle += 360.0f;    
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
    // Add mouse move event
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Add mouse scroll event
}


void Tema2::OnWindowResize(int width, int height)
{
}

void m1::Tema2::DrawScene(camera::Camera *camera, float deltaTimeSeconds)
{
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
    if (!gameOver && !startCountdown) {
        if (drone->passesThroughGate(drone->boundingSphere, currentGate)) {
            // Deactivate the current gate
            currentGate->isActive = false;
            currentGate->gateColor = GATE_PASSIVE_COLOR;

            // Check if game is over
            if (currentCheckpoint == checkpointOrder.size() - 1) {
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
}

void m1::Tema2::SetViewportArea(const ViewportSpace &viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(skyBlue.x, skyBlue.y, skyBlue.z, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    // GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    // GetSceneCamera()->Update();
}

void m1::Tema2::DrawHUD()
{
    const float kTopY = 10.f;
    const float kRowHeight = window->GetResolution().y * 0.04f;

    int rowIndex = 0;
    std::string polygonModeText = "";

    if (polygonMode == GL_LINE)
    {
        polygonModeText = "wireframe";
    }
    if (polygonMode == GL_FILL)
    {
        polygonModeText = "solid";
    }
    if (polygonMode == GL_POINT)
    {
        polygonModeText = "points";
    }

    // Get timer string
    std::string timerString = std::to_string(timerMinutes) + ":" + std::to_string(timerSeconds1) + std::to_string(timerSeconds2);

    if (gameOver) {
        textRenderer->RenderText("Game over! Score: " + std::to_string(currentCheckpoint - 1) + "/" + std::to_string(checkpointOrder.size()),
                                window->GetResolution().x * 0.04f, kTopY + kRowHeight * 2, 1.0f, glm::vec3(1, 1, 1));
        return;
    }

    if (startCountdown) {
        textRenderer->RenderText("Starting in " + std::to_string(countdown) + " seconds", window->GetResolution().x * 0.01f, kTopY + kRowHeight, 1.0f, glm::vec3(1, 1, 1));
        return;
    }

    textRenderer->RenderText(timerString, window->GetResolution().x * 0.04f, kTopY + kRowHeight, 1.0f, glm::vec3(1, 1, 1));
}
