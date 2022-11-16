#version 430 core
out vec4 FragColour;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1; //check if models have more than one diffuse texture
//no material struct for now but will orobably use one in the future

void main()
{    
    FragColour = texture(texture_diffuse1, TexCoords);
    FragColour = vec4(1.0, 0.5, 0.5, 1.0);
}