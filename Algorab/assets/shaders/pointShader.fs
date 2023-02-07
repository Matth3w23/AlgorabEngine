#version 430 core
out vec4 FragColour;

in vec3 PointColour;
//in float PointSize;

void main()
{    
    //FragColour = vec4(PointColour, 1.0);
    FragColour = vec4(0.0f, 1.0f, 0.0f, 1.0f);


    //from stack overflow to make points circular
    vec2 circCoord = 2.0 * gl_PointCoord - 1.0;
    if (dot(circCoord, circCoord) > 1.0) {
        discard;
    }
}