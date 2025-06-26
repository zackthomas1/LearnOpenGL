#pragma once
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace LearnOpenGL
{
    // defines serveral possible options for camera movement.
    // Use as an abstraction to stay away from window_system specific input methods
    enum Camera_Movement
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    // default camera values
    const glm::vec3 kWorldUp = glm::vec3(0.0, 1.0, 0.0);
    const glm::vec3 kCameraPosition = glm::vec3(0.0, 0.0, 3.0),
        kCameraFront = glm::vec3(0.0, 0.0, -1.0),
        kCameraUp = glm::vec3(0.0, 1.0, 0.0);
    const float kPitch = 0.0f, kYaw = -90.0f,
        kZnear = 0.1f, kZfar = 100.0f,
        kMovementSpeed = 10.0f, kMouseSensitivity = 0.1f;

    class Camera
    {
    public: 
        Camera(glm::vec3 position, glm::vec3 up, 
            float pitch, float yaw, 
            float znear, float zfar);
        virtual ~Camera() {}

        // getter/setters
        glm::vec3 GetPosition() const { return camera_position_; }
        glm::vec3 GetForwardVec() const { return camera_front_; }
        virtual glm::mat4 GetViewMatrix() const;
        virtual glm::mat4 GetProjectionMatrix() const = 0;

        void SetMovementSpeed(float speed); 
        void SetMouseSensitivity(float sensitivity); 
        void SetNear(float znear);
        void SetFar(float zfar);

        // call-back
        virtual void ProcessKeyboard(Camera_Movement direction, float time_step);
        virtual void ProcessMouseMovement(float x_offset, float y_offset, GLboolean constrainPitch = TRUE) {}
        virtual void ProcessMouseScroll(float y_offset) {}
    protected: 
        // camera attributes
        float pitch_, yaw_,	// Euler angles
            znear_, zfar_,
            movement_speed_, mouse_sensitivity_;

        glm::vec3 camera_position_,
            camera_front_,
            camera_up_,
            camera_right_;

        // protected methods
        virtual void UpdateCameraVectors();

    };
}
