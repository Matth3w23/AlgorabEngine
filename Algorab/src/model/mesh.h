#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <glad/glad.h>

#include <iostream>

struct TexturedVertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct ColouredVertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec4 colour;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh {
private:
	unsigned int VAO, VBO, EBO;
	std::vector<TexturedVertex> texVertices;
	std::vector<ColouredVertex> colVertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures = {};
	std::vector<glm::vec4> colours = {}; //vec4 as may support alpha later?
	bool textured;
public:
	Mesh(std::vector<TexturedVertex> vrtcs, std::vector<unsigned int> indcs, std::vector<Texture> txtrs);
	Mesh(std::vector<ColouredVertex> vrtcs, std::vector<unsigned int> indcs, std::vector<glm::vec4> clrs);
	Mesh(std::vector<ColouredVertex> vrtcs, std::vector<unsigned int> indcs, std::vector<glm::vec3> clrs);
	Mesh(std::vector<ColouredVertex> vrtcs, std::vector<unsigned int> indcs, glm::vec4 colour);
	Mesh(std::vector<ColouredVertex> vrtcs, std::vector<unsigned int> indcs, glm::vec3 colour);

	void setupMesh();

	unsigned int getVertexArray();
	std::vector<unsigned int>& getIndices();
	std::vector<Texture>& getTextures();

};