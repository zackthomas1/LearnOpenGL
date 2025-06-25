#pragma once
#include <string>
#include <vector>
#include "shader_s.h"
#include "Mesh.h"
namespace LearnOpenGL
{
	class Model
	{
	public:
		Model( char *path);
		void Draw(Shader &shader);

	private:
		// Model data 
		std::vector<Mesh> meshes;
		std::string directory; 

		void loadModel(std::string path); 
		void processNode(aiNode *node, const aiScene *scene); 
		Mesh processMesh(aiMesh *mesh, const aiScene *scene);
		std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	};

}
