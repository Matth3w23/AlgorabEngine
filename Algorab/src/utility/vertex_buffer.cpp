#include "vertex_buffer.h"

VertexBuffer::VertexBuffer(void* data, unsigned int size, unsigned int numVerts) :
    numberVertexes(numVerts) {

    glGenBuffers(1, &rendererID);
    Bind();
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW); //update to support more than static draw in the future?
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &rendererID);
}

unsigned int VertexBuffer::getNumberVertexes() {
    return numberVertexes;
}

void VertexBuffer::Bind() {
    glBindBuffer(GL_ARRAY_BUFFER, rendererID);
}

void VertexBuffer::Unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
