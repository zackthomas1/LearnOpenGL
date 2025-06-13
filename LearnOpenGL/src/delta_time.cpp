#include "pch.h"
#include "delta_time.h"
#include <GLFW/glfw3.h>

namespace LearnOpenGL {
	DeltaTime::DeltaTime()
	{
		current_frame_time_ = glfwGetTime();
		delta_time_ = 0.0f; 
		last_frame_time_ = current_frame_time_;
	}

	void DeltaTime::Step()
	{
		current_frame_time_ = glfwGetTime();
		delta_time_ = current_frame_time_ - last_frame_time_;
		last_frame_time_ = current_frame_time_;
		//std::cout << delta_time_ << std::endl;
	}
}
