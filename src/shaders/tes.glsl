#version 440

layout(triangles, equal_spacing, cw) in;

in vec4 tc_DirectionalLightSpacePos[];
in vec2 tc_TexCoord[];
in vec3 tc_Normal[];
in vec3 tc_FragPos[];
in vec4 tc_vCol[];

out vec4 vCol;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out vec4 DirectionalLightSpacePos;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 directionalLightTransform;

void main()
{
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;
    float w = gl_TessCoord.z;

    vec4 position = vec4(0.0);

    for (int i = 0; i < 3; i++) {
        position += gl_in[i].gl_Position * gl_TessLevelOuter[i] * u * v * w;
    }

    position += gl_in[0].gl_Position * u * gl_TessLevelOuter[0];
    position += gl_in[1].gl_Position * v * gl_TessLevelOuter[1];
    position += gl_in[2].gl_Position * w * gl_TessLevelOuter[2];

    DirectionalLightSpacePos = tc_DirectionalLightSpacePos[0] * u +
        tc_DirectionalLightSpacePos[1] * v +
        tc_DirectionalLightSpacePos[2] * w;

    TexCoord = tc_TexCoord[0] * u + tc_TexCoord[1] * v + tc_TexCoord[2] * w;
    Normal = normalize(tc_Normal[0] * u + tc_Normal[1] * v + tc_Normal[2] * w);
    FragPos = tc_FragPos[0] * u + tc_FragPos[1] * v + tc_FragPos[2] * w;
    vCol = tc_vCol[0] * u + tc_vCol[1] * v + tc_vCol[2] * w;
    gl_Position = projection * view * model * position;
}
