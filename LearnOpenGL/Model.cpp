#include "pch.h"
#include "Model.h"

namespace LearnOpenGL
{
	Model::Model(char* path)
	{
		loadModel(path);
	}

	void Model::Draw(Shader &shader)
	{
		for (unsigned int i = 0; i < meshes.size(); i++) {
			meshes[i].Draw(shader);
		}
	}
	
	void Model::loadModel(std::string path)
	{
	}
	
	void Model::processNode(aiNode* node, const aiScene* scene)
	{
	}
	
	Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		return Mesh();
	}

	std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
	{
		return std::vector<Texture>();
	}
}
