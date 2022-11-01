#pragma once
#include <glad/glad.h>

class VertexBuffer {
private:
	unsigned int rendererID;
	unsigned int numberVertexes;

public:
	VertexBuffer(void* data, unsigned int size, unsigned int numVerts);
	~VertexBuffer();

	unsigned int getNumberVertexes();

	void Bind();
	void Unbind();
};