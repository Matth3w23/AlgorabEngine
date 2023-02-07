#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in float aSize;
layout (location = 2) in vec3 aCol;

uniform mat4 view;
uniform mat4 projection;

out vec3 PointColour;
//out float PointSize;

void main()
{
    PointColour = aCol;
    //PointSize = aSize; //don't think is needed
    gl_Position = projection * view * vec4(aPos, 1.0);
    gl_PointSize = aSize;
    //gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}