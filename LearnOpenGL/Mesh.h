#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "shader_s.h"
namespace LearnOpenGL
{
	// Memory layout of Structs in C++ is sequential.
	// Therefore a struct can be represented as an array of data. 
	// The array would contains the struct's variables in sequential order.
	// This directly translates to a float (actually byte) array required for an array buffer.
	struct Vertex {
		//Size: 32 bytes (8 floats * 4 bytes each)
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	struct Texture {
		unsigned int id;
		std::string type;
	};

	class Mesh
	{
	public:
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

		void Draw(Shader shader);

		// mesh data 
		std::vector<Vertex>			vertices; 
		std::vector<unsigned int>	indices; 
		std::vector<Texture>		textures;
	private:
		unsigned int  VOA_, VBO_, EBO_; 

		void setupMesh();
	};
}

