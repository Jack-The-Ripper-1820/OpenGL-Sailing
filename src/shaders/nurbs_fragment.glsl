#version 400 core

in vec3 tessControlPoints[];

out vec4 color;

void main()
{
    // The fragment shader can use data from the tessellation evaluation shader
    // or any other relevant data to produce the final color.
    color = vec4(tessControlPoints[0], 1.0);
}
