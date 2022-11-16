#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float scale;

void main()
{
    mat4 m = model;
    //scale transform component
    m[3][0] *= scale; 
    m[3][1] *= scale;
    m[3][2] *= scale;
    //scale scale component
    m[0][0] *= scale;
    m[1][1] *= scale;
    m[2][2] *= scale;

    TexCoords = aTexCoords;
    gl_Position = projection * view * m * vec4(aPos, 1.0);
}