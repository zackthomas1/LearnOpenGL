#pragma once
#include "camera.h"

namespace ToyEngine
{
	// default camera values
	const float kLeft = -4.0f, kRight = 4.0f,
		kBottom = -3.0f, kTop = 3.0;

	class OrthographicCamera : public Camera {
		
	public:
		OrthographicCamera(glm::vec3 position = kCameraPosition, glm::vec3 up = kCameraUp,
			float pitch = kPitch, float yaw = kYaw, float znear = kZnear, float zfar = kZfar,
			float left = kLeft, float right = kRight, float bottom = kBottom, float top = kTop);
		OrthographicCamera(float pos_x, float pos_y, float pos_z,
			float up_x, float up_y, float up_z,
			float pitch = kPitch, float yaw = kYaw, float znear = kZnear, float zfar = kZfar,
			float left = kLeft, float right = kRight, float bottom = kBottom, float top = kTop);
		~OrthographicCamera() {}

		// getter/setters
		glm::mat4 GetProjectionMatrix() const;

	private:
		// camera attributes
		float left_, right_, bottom_, top_;
	};
}