#pragma once
#include <glad/glad.h>

class ElementBuffer {
private:
	unsigned int rendererID;
	unsigned int numberIndices;
public:
	ElementBuffer(unsigned int* data, unsigned int numIndcs);
	~ElementBuffer();

	void Bind() const;
	void Unbind() const;

	unsigned int getNumIndices();
};