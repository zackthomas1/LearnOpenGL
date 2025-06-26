#include "pch.h"
#include "Mesh.h"
#include <glad/glad.h>

namespace LearnOpenGL {

	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) : 
		m_vertices(vertices), m_indices(indices), m_textures(textures)
	{
		setupMesh(); 
	}

	void Mesh::Draw(Shader shader)
	{
		unsigned int diffuseNR = 1; 
		unsigned int specularNR = 1; 
		for (unsigned int i = 0; i < m_textures.size(); i++) {
			//activate proper texture unit before binding
			glActiveTexture(GL_TEXTURE0 + i);

			//retrieve texture number (the N in diffuse_textureN)
			std::string number; 
			std::string name = m_textures[i].type; 

			if (name == "texture_diffuse")
				number = std::to_string(diffuseNR++);
			else if (name == "texture_specular")
				number = std::to_string(specularNR++);

			shader.SetInt(("material." + name + number).c_str(), i);
			glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
		}

		// draw mesh 
		glBindVertexArray(VOA_); 
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	// render primitives as wireframes
		glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(m_indices.size()), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//set back to defaults once configured
		glActiveTexture(GL_TEXTURE0);
	}
	
	void Mesh::setupMesh()
	{
		// Gen buffers
		glGenVertexArrays(1, &VOA_);
		glGenBuffers(1, &VBO_);
		glGenBuffers(1, &EBO_);

		// Bind buffers
		glBindVertexArray(VOA_);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_);

		// Set vertice buffer
		glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

		// Set element array buffer 
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_); 
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);
		
		// Set vertex attribute pointers
		// --------------
		// aPos
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		// aNormals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

		// aTexCoords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

		// Release vertex array objecct 
		glBindVertexArray(0);
	}
}