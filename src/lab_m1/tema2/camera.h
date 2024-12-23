#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"


namespace camera
{
    class Camera
    {
     public:
        Camera()
        {
            position    = glm::vec3(0, 2, 5);
            forward     = glm::vec3(0, 0, -1);
            up          = glm::vec3(0, 1, 0);
            right       = glm::vec3(1, 0, 0);
            distanceToTarget = 2;
        }

        Camera(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
        {
            Set(position, center, up);
        }

        ~Camera()
        { }

        // Update camera
        void Set(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
        {
            this->position = position;
            forward     = glm::normalize(center - position);
            right       = glm::cross(forward, up);
            this->up    = glm::cross(right, forward);
        }

        void MoveForward(float distance)
        {
            glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
            position += dir * distance;
        }

        void TranslateForward(float distance)
        {
            position += distance * forward;

        }

        void TranslateUpward(float distance)
        {
            position += distance * up;

        }

        void TranslateRight(float distance)
        {
            glm::vec3 dir = glm::normalize(glm::vec3(right.x, 0, right.z));
            position += dir * distance;
        }

        void RotateFirstPerson_OX(float angle)
        {
            glm::mat3 r = glm::mat3(glm::rotate(glm::mat4(1), angle, right));
            
            forward = glm::normalize(r * forward);

            up = glm::cross(right, forward);
            up = glm::normalize(r * up);
        }

        void RotateFirstPerson_OY(float angle)
        {
            glm::mat3 r = glm::mat3(glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0)));
            
            forward = glm::normalize(r * forward);
            right = glm::normalize(r * right);

            up = glm::cross(right, forward);
            up = glm::normalize(r * up);
        }

        void RotateFirstPerson_OZ(float angle)
        {
            glm::mat3 r = glm::mat3(glm::rotate(glm::mat4(1), angle, forward));
            right = glm::normalize(r * right);

            up = glm::cross(right, forward);
            up = glm::normalize(r * up);

        }

        void RotateThirdPerson_OX(float angle)
        {
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OX(angle);
            TranslateForward(-distanceToTarget);

        }

        void RotateThirdPerson_OY(float angle)
        {
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OY(angle);
            TranslateForward(-distanceToTarget);

        }

        void RotateThirdPerson_OZ(float angle)
        {
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OZ(angle);
            TranslateForward(-distanceToTarget);

        }

        glm::mat4 GetViewMatrix()
        {
            // Returns the view matrix
            return glm::lookAt(position, position + forward, up);
        }

        glm::vec3 GetTargetPosition()
        {
            return position + forward * distanceToTarget;
        }

     public:
        float distanceToTarget;
        glm::vec3 position;
        glm::vec3 forward;
        glm::vec3 right;
        glm::vec3 up;
    };
}   // namespace implemented
