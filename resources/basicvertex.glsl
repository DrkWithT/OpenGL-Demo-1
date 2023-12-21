#version 330 core

layout (location = 0) in vec3 aPos;

uniform float yDelta; // update this variable to bob vertex by y-axis

void main()
{
    // linearly translate vertices by y-delta...
    gl_Position = vec4(aPos.x, aPos.y + yDelta, aPos.z, 1.0);
}
