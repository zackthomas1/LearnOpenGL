#include "pch.h"
#include "orthographic_camera.h"

namespace ToyEngine 
{
	OrthographicCamera::OrthographicCamera(glm::vec3 position, glm::vec3 up,
		float pitch, float yaw, float znear, float zfar,
		float left, float right, float bottom, float top) :
			Camera(position, up, pitch, yaw, znear, zfar), 
			left_(left), right_(right), bottom_(bottom), top_(top)
	{

	}
	OrthographicCamera::OrthographicCamera(float pos_x, float pos_y, float pos_z,
		float up_x, float up_y, float up_z,
		float pitch, float yaw, float znear, float zfar,
		float left, float right, float bottom, float top) :
			Camera(glm::vec3(pos_x, pos_y, pos_z), glm::vec3(up_x, up_y, up_z), pitch, yaw, znear, zfar),
				left_(left), right_(right), bottom_(bottom), top_(top)
	{

	}

	// getters/setters
	glm::mat4 OrthographicCamera::GetProjectionMatrix() const
	{
		return glm::ortho(left_, right_, bottom_, top_, znear_, zfar_);
	}

	// call-back
	void OrthographicCamera::ProcessMouseScroll(float y_offset)
	{

	}
}