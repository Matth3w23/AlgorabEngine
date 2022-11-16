#version 430 core
out vec4 FragColour;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main() {
	FragColour = texture(screenTexture, TexCoords);
}