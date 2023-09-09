#version 400 core

layout(vertices = 16) out;

in vec3 pos[];
in vec3 col[];
in vec2 tex[];
in vec3 norm[];

out vec4 vCol[];

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_Position = projection * view * model * vec4(pos[gl_InvocationID], 1.0);

    vCol[gl_InvocationID] = vec4(clamp(col[gl_InvocationID], 0.0f, 1.0f), 1.0f);
}

