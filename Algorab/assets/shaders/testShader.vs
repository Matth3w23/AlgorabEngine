#version 430 core
layout (location = 0) in vec3 aPos;

void main()
{
    gl_PointSize = 5;
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}