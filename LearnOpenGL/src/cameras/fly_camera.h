#pragma once
#include "camera.h"

namespace LearnOpenGL
{
    // default camera values
    const float kFov = 45.0f, kAspect = 800.0f/600.0f;

    class FlyCamera : public Camera
    {
    public:
        FlyCamera(glm::vec3 position = kCameraPosition, glm::vec3 up = kCameraUp, 
            float pitch = kPitch, float yaw = kYaw, float znear = kZnear, float zfar = kZfar,
            float aspect = kAspect, float fov = kFov);
        FlyCamera(float pos_x, float pos_y, float pos_z, 
            float up_x, float up_y, float up_z, 
            float pitch = kPitch, float yaw = kYaw, float znear = kZnear, float zfar = kZfar,
            float aspect = kAspect, float fov = kFov);
        ~FlyCamera() {}

        // getter/setters
        glm::mat4 GetProjectionMatrix() const;

        // call-back
        void ProcessMouseMovement(float x_offset, float y_offset, GLboolean constrainPitch = TRUE);
        void ProcessMouseScroll(float y_offset);

    private:
        // camera attributes
        float fov_, aspect_;
    };
}
