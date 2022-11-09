#pragma once
#include "mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <string>
#include <iostream>
#include <glad/glad.h>
#include "../stb_image/stb_image.h"

class Model {
private:
	std::vector<Mesh> meshes;

	float furthestVertexDist = 0.0f;

	bool currentlyDefault;
	std::string defaultModelPath = "assets/models/cube/source/cube/cube.obj";

	std::string currentModelDirectory;

	std::vector<Texture> textures_loaded;

	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

public:
	Model(std::string path);
	Model(std::vector<Mesh> mshs);

	void addModel(std::string path);
	void resetMeshes(); //set to default mesh

	std::vector<Mesh>* getMeshes();
	float getfurVertDist();


};