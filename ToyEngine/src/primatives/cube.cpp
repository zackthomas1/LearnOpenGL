#include "pch.h"
#include "cube.h"
#include <glad/glad.h>
namespace ToyEngine
{
	Cube::Cube() {
		// define cube vertex data  
		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		// create Vertex Buffer Object (VBO)
		glGenBuffers(1, &vbo_);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
	}

	Cube::~Cube()
	{
		//de-allocate all resources
		glDeleteVertexArrays(1, &vao_);
		glDeleteBuffers(1, &vbo_);
	}

	void Cube::Draw() const
	{
		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}