#pragma once
#include "primative.h"
namespace ToyEngine
{
	class Plane : public Primative
	{
	public:
		Plane();
		~Plane();

		void Draw() const override;
	private:
		float vertices_[32] = {
			// positions			// colors			// uv
			 0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,	// top right 
			 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f, // bottom right
			-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f, // bottom left
			-0.5f,  0.5f, 0.0f,		1.0f, 1.0f, 0.0f,	0.0f, 1.0f	// top left
		};

		//  Element Buffer Object (EBO)
		unsigned int indices_[6] = {
			0,1,3,	// first triangle
			1,2,3,	// second triangle
		};
	};
}