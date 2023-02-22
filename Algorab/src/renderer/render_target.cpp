#include "render_target.h"

RenderTarget::RenderTarget(unsigned int w, unsigned int h) :
    width(w), height(h) {
    glGenFramebuffers(1, &frameBuffer);
    glGenTextures(1, &textureBuffer);
    glGenRenderbuffers(1, &renderBuffer);
    setUpRenderTarget();
}

void RenderTarget::setUpRenderTarget() {
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureBuffer);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, width, height, GL_TRUE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureBuffer, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT32F, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Error: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    //basically from learn opengl
    float screenVertices[] = {
        1.0f, -1.0f,  1.0f, 0.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
        -1.0f, 1.0f,  0.0f, 1.0f,

        1.0f,  1.0f,  1.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f,  0.0f, 1.0f
    };

    glGenVertexArrays(1, &screenVAO);
    glGenBuffers(1, &screenVBO);

    glBindVertexArray(screenVAO);
    glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), &screenVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void RenderTarget::setTargetDimensions(unsigned int w, unsigned int h) {
    width = w;
    height = h;
    setUpRenderTarget();
}

unsigned int RenderTarget::getFrameBuffer() {
    return frameBuffer;
}

unsigned int RenderTarget::getTextureBuffer() {
    return textureBuffer;
}

unsigned int RenderTarget::getWidth() {
    return width;
}

unsigned int RenderTarget::getHeight() {
    return height;
}

unsigned int RenderTarget::getScreenVAO() {
    return screenVAO;
}

unsigned int RenderTarget::getScreenVBO() {
    return screenVBO;
}
