#include "element_buffer.h"

ElementBuffer::ElementBuffer(unsigned int* data, unsigned int numIndcs) :
    numberIndices(numIndcs) {
    glGenBuffers(1, &rendererID);
    Bind();
}

ElementBuffer::~ElementBuffer() {
    glDeleteBuffers(1, &rendererID);
}

void ElementBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
}

void ElementBuffer::Unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

unsigned int ElementBuffer::getNumIndices() {
    return numberIndices;
}
