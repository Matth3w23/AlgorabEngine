#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <iostream>



class RenderTarget {
private:
	unsigned int frameBuffer;
	unsigned int renderBuffer;
	unsigned int textureBuffer;
	unsigned int screenVAO, screenVBO;
	unsigned int width;
	unsigned int height;
public:
	RenderTarget(unsigned int w = 1200, unsigned int h = 800);
	void setUpRenderTarget();

	void setTargetDimensions(unsigned int w = 1200, unsigned int h = 800);
	
	unsigned int getFrameBuffer();
	unsigned int getTextureBuffer();
	unsigned int getWidth();
	unsigned int getHeight();
	unsigned int getScreenVAO();
	unsigned int getScreenVBO();

};