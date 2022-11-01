#include "mesh.h"

Mesh::Mesh(std::vector<TexturedVertex> vrtcs, std::vector<unsigned int> indcs, std::vector<Texture> txtrs) :
	texVertices(vrtcs), indices(indcs), textures(txtrs) {
	textured = true;
	setupMesh();
}
Mesh::Mesh(std::vector<ColouredVertex> vrtcs, std::vector<unsigned int> indcs, std::vector<glm::vec4> clrs) : //TODO: complete coloured mesh constructors
	colVertices(vrtcs), indices(indcs), colours(clrs) {
	
	textured = false;
	setupMesh();
}
Mesh::Mesh(std::vector<ColouredVertex> vrtcs, std::vector<unsigned int> indcs, std::vector<glm::vec3> clrs) : //remember to update colour format
	colVertices(vrtcs), indices(indcs) {
	textured = false;
	setupMesh();
}

Mesh::Mesh(std::vector<ColouredVertex> vrtcs, std::vector<unsigned int> indcs, glm::vec4 colour) :
	colVertices(vrtcs), indices(indcs) {
	textured = false;
	setupMesh();
}
Mesh::Mesh(std::vector<ColouredVertex> vrtcs, std::vector<unsigned int> indcs, glm::vec3 colour) :
	colVertices(vrtcs), indices(indcs) {
	textured = false;
	setupMesh();
}

void Mesh::setupMesh() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	if (textured) {
		//start of passing data in
		glBindVertexArray(VAO);
		//Vertex Buffer
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, texVertices.size() * sizeof(TexturedVertex), &texVertices[0], GL_STATIC_DRAW); //pass in vertices
		//Element Buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW); //pass in indices

		glEnableVertexAttribArray(0); //position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void*)0);
		glEnableVertexAttribArray(1); //normal
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void*)offsetof(TexturedVertex, normal));
		glEnableVertexAttribArray(2); //texture coords
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void*)offsetof(TexturedVertex, texCoords));

		glBindVertexArray(0);
		
	} else {
		//start of passing data in
		glBindVertexArray(VAO);
		//Vertex Buffer
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, colVertices.size() * sizeof(ColouredVertex), &colVertices[0], GL_STATIC_DRAW); //pass in vertices
		//Element Buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW); //pass in indices

		glEnableVertexAttribArray(0); //position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ColouredVertex), (void*)0);
		glEnableVertexAttribArray(1); //normal
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ColouredVertex), (void*)offsetof(ColouredVertex, normal));
		glEnableVertexAttribArray(2); //colour
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(ColouredVertex), (void*)offsetof(ColouredVertex, colour));

		glBindVertexArray(0);
	}
}

unsigned int Mesh::getVertexArray() {
	return VAO;
}

std::vector<unsigned int> Mesh::getIndices() {
	return indices;
}

std::vector<Texture> Mesh::getTextures() {
	return textures;
}

/*void Mesh::createDefaultMesh() {

	glm::vec3 mmm, pmm, mpm, mmp, ppm, pmp, mpp, ppp;
	mmm = glm::vec3(-0.5f, -0.5f, -0.5f);
	pmm = glm::vec3(0.5f, -0.5f, -0.5f);
	mpm = glm::vec3(-0.5f, 0.5f, -0.5f);
	mmp = glm::vec3(-0.5f, -0.5f, 0.5f);
	ppm = glm::vec3(0.5f, 0.5f, -0.5f);
	pmp = glm::vec3(0.5f, -0.5f, 0.5f);
	mpp = glm::vec3(-0.5f, 0.5f, 0.5f);
	ppp = glm::vec3(0.5f, 0.5f, 0.5f);

	std::vector<ColouredVertex> vrtcs;
	std::vector<unsigned int> indcs;
	std::vector<glm::vec3> clrs(24, glm::vec3(0.5f, 0.5f, 0.5f));
	
	defaultMesh.colVertices
}*/
