#pragma once
#include "vertex_buffer.h"

class VertexArray {
private:
	unsigned int rendererID;

public:
	VertexArray();
	~VertexArray();

	void addBuffer();

};