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
    camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

    // Left bottom corner
    glm::vec3 corner = glm::vec3(0, 0, 0);

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create the drone meshes
    {
        drone = new drones::Drone();
        drone->CreateDrone("drone", corner, 1);

        this->drone->position = corner;
    }

    // Create the tree meshes
    {
        for (int i = 1; i < 5; i++) {
            trees::Tree *tree = new trees::Tree();

            std::string treeName = "tree" + std::to_string(i);
            tree->CreateTree(treeName.c_str(), corner, i * 0.5f);

            tree->position = glm::vec3(i * 3, 0, 0);

            trees.push_back(tree);
        }
    }   

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader *shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
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

    // Render drone
    drone->RenderDrone(shaders["LabShader"], camera, projectionMatrix);

    // PRINT DRONE POSITION
    // Print sphere ends
                cout << "Sphere center " << drone->boundingSphere->center.x << " "
                    << drone->boundingSphere->center.y << " "
                    << drone->boundingSphere->center.z << endl;
                cout << "Sphere ends: " << drone->boundingSphere->center.x - drone->boundingSphere->radius << " "
                    << drone->boundingSphere->center.x + drone->boundingSphere->radius << " "
                    << drone->boundingSphere->center.y - drone->boundingSphere->radius << " "
                    << drone->boundingSphere->center.y + drone->boundingSphere->radius << " "
                    << drone->boundingSphere->center.z - drone->boundingSphere->radius << " "
                    << drone->boundingSphere->center.z + drone->boundingSphere->radius << endl;

    // Render trees
    for (auto &tree : trees) {
        tree->RenderTree(shaders["LabShader"], camera, projectionMatrix);
    }
    
    drone->propellerAngle += PROPELLER_SPEED * deltaTimeSeconds;
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
        float cameraSpeed = 2.0f;

        if (window->KeyHold(GLFW_KEY_W)) {
            // TODO(student): Translate the camera forward
            camera->MoveForward(cameraSpeed * deltaTime);

        }

        if (window->KeyHold(GLFW_KEY_A)) {
            // TODO(student): Translate the camera to the left
            camera->TranslateRight(-cameraSpeed * deltaTime);

        }

        if (window->KeyHold(GLFW_KEY_S)) {
            // TODO(student): Translate the camera backward
            camera->MoveForward(-cameraSpeed * deltaTime);

        }

        if (window->KeyHold(GLFW_KEY_D)) {
            // TODO(student): Translate the camera to the right
            camera->TranslateRight(cameraSpeed * deltaTime);

        }

        if (window->KeyHold(GLFW_KEY_Q)) {
            // TODO(student): Translate the camera downward
            camera->TranslateUpward(-cameraSpeed * deltaTime);

        }

        if (window->KeyHold(GLFW_KEY_E)) {
            // TODO(student): Translate the camera upward
            camera->TranslateUpward(cameraSpeed * deltaTime);

        }

        if (window->KeyHold(GLFW_KEY_LEFT)) {
            fov -= cameraSpeed * deltaTime;
            projectionMatrix = glm::perspective(fov, window->props.aspectRatio, zNear, zFar);

        }

        if (window->KeyHold(GLFW_KEY_RIGHT)) {
            fov += cameraSpeed * deltaTime;
            projectionMatrix = glm::perspective(fov, window->props.aspectRatio, zNear, zFar);
        }

        if (window->KeyHold(GLFW_KEY_K)) {
            bottom -= cameraSpeed * deltaTime;
            top += cameraSpeed * deltaTime;
            left -= cameraSpeed * deltaTime;
            right += cameraSpeed * deltaTime;
            projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
        }
        
        if (window->KeyHold(GLFW_KEY_L)) {
            left += cameraSpeed * deltaTime;
            right -= cameraSpeed * deltaTime;
            bottom += cameraSpeed * deltaTime;
            top -= cameraSpeed * deltaTime;
            projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
        }
    } else {
        // Check for collisions
        int index = 1;
        for (auto &tree : trees) {
            if (drone->collidesWithObject(tree->boundingBox)) {
                // Print sphere ends
                cout << "Sphere ends: " << drone->boundingSphere->center.x - drone->boundingSphere->radius << " "
                    << drone->boundingSphere->center.x + drone->boundingSphere->radius << " "
                    << drone->boundingSphere->center.y - drone->boundingSphere->radius << " "
                    << drone->boundingSphere->center.y + drone->boundingSphere->radius << " "
                    << drone->boundingSphere->center.z - drone->boundingSphere->radius << " "
                    << drone->boundingSphere->center.z + drone->boundingSphere->radius << endl;

                cout << "We hit tree " << index << endl;
                cout << tree->boundingBox->xLimits.x << " " << tree->boundingBox->xLimits.y << endl;
                cout << tree->boundingBox->yLimits.x << " " << tree->boundingBox->yLimits.y << endl;
                cout << tree->boundingBox->zLimits.x << " " << tree->boundingBox->zLimits.y << endl;

                cout << "Drone collided with tree" << endl;
                // return;
            }
            index++;
        }

        // Move the drone
        if (window->KeyHold(GLFW_KEY_A)) {
            drone->position.x -= deltaTime * DRONE_SPEED;
            drone->boundingSphere->center.x -= deltaTime * DRONE_SPEED;
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            drone->position.x += deltaTime * DRONE_SPEED;
            drone->boundingSphere->center.x += deltaTime * DRONE_SPEED;
        }

        if (window->KeyHold(GLFW_KEY_W)) {
            drone->position.z -= deltaTime * DRONE_SPEED;
            drone->boundingSphere->center.z -= deltaTime * DRONE_SPEED;
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            drone->position.z += deltaTime * DRONE_SPEED;
            drone->boundingSphere->center.z += deltaTime * DRONE_SPEED;
        }

        if (window->KeyHold(GLFW_KEY_Q)) {
            drone->position.y -= deltaTime * DRONE_SPEED;
            drone->boundingSphere->center.y -= deltaTime * DRONE_SPEED;
        }

        if (window->KeyHold(GLFW_KEY_E)) {
            drone->position.y += deltaTime * DRONE_SPEED;
            drone->boundingSphere->center.y += deltaTime * DRONE_SPEED;
        }

        if (window->KeyHold(GLFW_KEY_LEFT)) {
            drone->droneAngle -= deltaTime * DRONE_ROTATION_SPEED;
        }

        if (window->KeyHold(GLFW_KEY_RIGHT)) {
            drone->droneAngle += deltaTime * DRONE_ROTATION_SPEED;
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
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.002f;
        float sensivityOY = 0.002f;

        if (window->GetSpecialKeyState() == 0) {
            renderCameraTarget = false;
            // TODO(student): Rotate the camera in first-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            camera->RotateFirstPerson_OX(-deltaY * sensivityOY);
            camera->RotateFirstPerson_OY(-deltaX * sensivityOX);

        }

        if (window->GetSpecialKeyState() & GLFW_MOD_CONTROL) {
            renderCameraTarget = true;
            // TODO(student): Rotate the camera in third-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            camera->RotateThirdPerson_OX(-deltaY * sensivityOY);
            camera->RotateThirdPerson_OY(-deltaX * sensivityOX);

        }
    }
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
}


void Tema2::OnWindowResize(int width, int height)
{
}
