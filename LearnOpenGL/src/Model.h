#pragma once
#include <string>
#include <vector>
#include "shader_s.h"
#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
namespace LearnOpenGL
{
	class Model
	{
	public:
		Model( char *path, bool flip_vertically=true, bool gamma=false);
		void Draw(Shader &shader);

	private:
		// Model data 
		std::vector<Mesh> meshes;
		std::vector<Texture> textures_loaded;
		std::string directory; 
		bool flip_vertically_, gamma_;

		//
		void loadModel(std::string path); 
		void processNode(aiNode *node, const aiScene *scene); 
		Mesh processMesh(aiMesh *mesh, const aiScene *scene);
		std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
		unsigned int TextureFromFile(const char* path, const std::string& directory);
	};

}
