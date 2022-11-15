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

    glBindTexture(GL_TEXTURE_2D, textureBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 800, 600, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureBuffer, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Error: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
