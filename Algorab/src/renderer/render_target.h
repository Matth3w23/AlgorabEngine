#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <iostream>

class RenderTarget {
private:
	unsigned int frameBuffer;
	unsigned int renderBuffer;
	unsigned int textureBuffer;
	unsigned int width;
	unsigned int height;
public:
	RenderTarget(unsigned int w = 800, unsigned int h = 600, bool simple = false);
	void setUpRenderTarget(bool simple = false);

	void setTargetDimensions(unsigned int w = 800, unsigned int h = 600);
	
	unsigned int getFrameBuffer();
	unsigned int getTextureBuffer();
	unsigned int getWidth();
	unsigned int getHeight();

};
