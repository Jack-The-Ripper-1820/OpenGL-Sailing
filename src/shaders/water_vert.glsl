#version 440

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 col;
layout(location = 2) in vec2 tex;
layout(location = 3) in vec3 norm;

out vec4 vCol;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
//out vec4 DirectionalLightSpacePos;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 directionalLightTransform;
uniform float time;

void main()
{
    vCol = vec4(clamp(col, 0.0f, 1.0f), 1.0f);

    TexCoord = tex;

    Normal = norm;

    // Calculate the sine wave displacement along the Y-axis
    float displacement = sin(time) * 0.1; // Adjust the 0.1 to control the amplitude of the wave

    // Modify the position
    vec3 modifiedPos = pos;
    
    if (gl_VertexID % 2 == 0) {
        modifiedPos.y += displacement;
    }
    //pos = modifiedPos;

    // Transform the modified position
    //vec4 finalPos = projection * view * model * vec4(modifiedPos, 1.0f);

    /*FragPos = pos;*/

    FragPos = modifiedPos;

    //gl_Position = finalPos;
}
