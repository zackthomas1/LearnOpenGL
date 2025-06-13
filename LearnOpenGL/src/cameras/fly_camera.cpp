#include "pch.h"
#include "fly_camera.h"

namespace LearnOpenGL{
	FlyCamera::FlyCamera(glm::vec3 position, glm::vec3 up, 
		float pitch, float yaw, float znear, float zfar,
		float aspect, float fov) :
			Camera(position, up, pitch, yaw, znear, zfar),
			aspect_(aspect), fov_(fov)
	{

	}

	FlyCamera::FlyCamera(float pos_x, float pos_y, float pos_z, 
		float up_x, float up_y, float up_z, 
		float pitch, float yaw, float znear, float zfar,
		float aspect, float fov) :
			Camera(glm::vec3(pos_x, pos_y, pos_z), glm::vec3(up_x, up_y, up_z), pitch, yaw, znear, zfar),
			aspect_(aspect), fov_(fov)
	{

	}

	// getter/setter
	glm::mat4 FlyCamera::GetProjectionMatrix() const
	{
		return glm::perspective(glm::radians(fov_), aspect_, znear_, zfar_);
	}

	// call-back
	void FlyCamera::ProcessMouseMovement(float x_offset, float y_offset, GLboolean constrainPitch)
	{
		//std::cout << "x_offset: " << x_offset << "\ty_offset: " << x_offset << std::endl;

		x_offset *= mouse_sensitivity_;
		y_offset *= mouse_sensitivity_;

		yaw_ += x_offset;
		pitch_ -= y_offset;

		if (pitch_ > 89.0f)
			pitch_ = 89.0f;
		else if (pitch_ < -89.0f)
			pitch_ = -89.0f;

		UpdateCameraVectors();
	}
	void FlyCamera::ProcessMouseScroll(float y_offset)
	{
		fov_ -= y_offset;
		if (fov_ < 1.0f)
			fov_ = 1.0f;
		else if (fov_ > 90.0f)
			fov_ = 90.0f;
	}
}