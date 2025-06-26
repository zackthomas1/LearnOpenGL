#include "pch.h"
#include "Model.h"
#include "stb_image.h"
#include <glad/glad.h>

namespace LearnOpenGL
{
    Model::Model(char* path, bool flip_vertically, bool gamma):
        flip_vertically_(flip_vertically), gamma_(gamma)
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
        //Declare Importer object from Assimp namespace
        Assimp::Importer importer; 
        
        //aiProcess_Triangulate: tells Assimp that model does not (entirely) consist of triangles. Transform all model primatives into triangles
        //aiProcess_FlipUVs: flips the texture coordinates on the y-axis where necessary during processing
        //aiProcess_GenNormals: creates normal vectors for each vertex if the model doesn't contain normal vectors.
        //aiProcess_SplitLargeMeshes: splits large meshes into smaller sub-meshes which is useful if your rendering has a maximum number of vertices allowed and can only process smaller meshes.
        //aiProcess_OptimizeMeshes: does the reverse by trying to join several meshes into one larger mesh, reducing drawing calls for optimization.
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            TY_CORE_ERROR(importer.GetErrorString());
            return;
        }
        directory = path.substr(0, path.find_last_of('/'));
        processNode(scene->mRootNode, scene);
    }
    
    void Model::processNode(aiNode* node, const aiScene* scene)
    {
        //Each node (possibly) contains a set of children.
        //First process the node in question, then process all the node's children. 
        //Recursively calls the same function on node childern until all nodes have been processed. 
        
        //proccess all the node's meshes
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            //check each of the node's mesh indices and retrieve 
            //corresponding mesh by indexing the scene's mMeshes array
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]]; 
            meshes.push_back(processMesh(mesh, scene)); 
        }

        //do the same for each of its children
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            processNode(node->mChildren[i], scene);
        }
    }
    
    Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<Vertex> vertices; 
        std::vector<unsigned int> indices; 
        std::vector<Texture> textures; 

        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex; 
            //process vertex positions, normals, and texture coordinates
            vertex.Position	= glm::vec3(mesh->mVertices[i].x,	mesh->mVertices[i].y,	mesh->mVertices[i].z);
            vertex.Normal	= glm::vec3(mesh->mNormals[i].x,	mesh->mNormals[i].y,	mesh->mNormals[i].z);
            
            if (mesh->mTextureCoords[0]) {
                vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y); 
            }
            else {
                vertex.TexCoords = glm::vec2(0.0f); 
            }

            vertices.push_back(vertex); 
        }

        //process indices 
        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i]; 
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                indices.push_back(face.mIndices[j]); 
            }
        }

        //process material 
        if (mesh->mMaterialIndex >= 0) {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            
            std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse"); 
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end()); 
            
            std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular"); 
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end()); 
        }

        return Mesh(vertices, indices, textures);
    }

    std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
    {
        //iterates over all the texture locations of the given texture type, retrieves the texture's file location, 
        //then loads and generates the texture and stores the information in a Vertex struct.
        std::vector<Texture> textures; 
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
            aiString str; 
            mat->GetTexture(type, i, &str); 
            bool skip = false; 
            
            //check if texture has already been loaded.
            for (unsigned int j = 0; j < textures_loaded.size(); j++) {
                if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
                    textures.push_back(textures_loaded[j]);
                    skip = true; 
                    break;
                }
            }
            
            // Load texture only if it has not already been loaded 
            if(!skip) {
                Texture texture;
                texture.id = TextureFromFile(str.C_Str(), directory);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                textures_loaded.push_back(texture);
            }
        }

        return textures;
    }
    unsigned int Model::TextureFromFile(const char* path, const std::string& directory)
    {
        std::string filename = std::string(path);
        filename = directory + "/" + filename; 

        unsigned int textureID; 
        glGenTextures(1, &textureID);

        int width, height, nr_channels; 
        stbi_set_flip_vertically_on_load(flip_vertically_);
        unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nr_channels, 0);

        GLenum internal_format = 0, data_format = 0;
        if (nr_channels == 1) {
            internal_format = GL_RED; 
            data_format		= GL_RED; 
        }
        else if (nr_channels == 3) {
            internal_format = GL_RGB8; 
            data_format		= GL_RGB; 
        }
        else if (nr_channels == 4) {
            internal_format = GL_RGBA8; 
            data_format		= GL_RGBA;
        }

        if (data)
        {
            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, data_format, width, height, 0, data_format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            //
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        else {
            TY_CORE_ERROR("Texture failed to load at path:", path);
        }
        stbi_image_free(data);

        return textureID;
    }
}
