#include "pch.h"
#include "camera.h"

namespace LearnOpenGL
{
	Camera::Camera(glm::vec3 position, glm::vec3 up,
		float pitch, float yaw, float znear, float zfar) :
			camera_position_(position), camera_up_(up),
			pitch_(pitch), yaw_(yaw), znear_(znear), zfar_(zfar)
	{
		movement_speed_ = kMovementSpeed;
		mouse_sensitivity_ = kMouseSensitivity;

		UpdateCameraVectors();
	}

	// getter/setter
	glm::mat4 Camera::GetViewMatrix() const
	{
		return glm::lookAt(camera_position_, camera_position_ + camera_front_, camera_up_);
	}

	void Camera::SetMovementSpeed(float speed)
	{
		movement_speed_ = speed;
	}
	void Camera::SetMouseSensitivity(float sensitivity)
	{
		mouse_sensitivity_ = sensitivity;
	}
	void Camera::SetNear(float znear)
	{
		znear_ = znear;
	}
	void Camera::SetFar(float zfar)
	{
		zfar_ = zfar;
	}

	// call-back
	void Camera::ProcessKeyboard(Camera_Movement direction, float time_step) 
	{
		float velocity = movement_speed_ * time_step;
		switch (direction)
		{
		case FORWARD:
			camera_position_ += velocity * camera_front_;
			break;
		case BACKWARD:
			camera_position_ -= velocity * camera_front_;
			break;
		case LEFT:
			camera_position_ -= velocity * camera_right_;
			break;
		case RIGHT:
			camera_position_ += velocity * camera_right_;
			break;
		case UP:
			camera_position_ += velocity * camera_up_;
			break;
		case DOWN:
			camera_position_ -= velocity * camera_up_;
			break;
		}

		UpdateCameraVectors();
	}

	// protected functions
	void Camera::UpdateCameraVectors()
	{
		// Calculate camera front vector from Euler rotations
		glm::vec3 camera_direction;
		camera_direction.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
		camera_direction.y = sin(glm::radians(pitch_));
		camera_direction.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
		camera_front_ = glm::normalize(camera_direction);

		camera_right_ = glm::normalize(glm::cross(camera_front_, kWorldUp));
		camera_up_ = glm::normalize(glm::cross(camera_right_, camera_front_));
	}

}