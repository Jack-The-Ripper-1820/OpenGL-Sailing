#version 440

layout(vertices = 3) out;

in vec4 vCol[];
in vec2 TexCoord[];
in vec3 Normal[];
in vec3 FragPos[];
in vec4 DirectionalLightSpacePos[];

out vec4 tc_DirectionalLightSpacePos[];
out vec2 tc_TexCoord[];
out vec3 tc_Normal[];
out vec3 tc_FragPos[];
out vec4 tc_vCol[];

void main()
{
    tc_DirectionalLightSpacePos[gl_InvocationID] = DirectionalLightSpacePos[gl_InvocationID];
    tc_TexCoord[gl_InvocationID] = TexCoord[gl_InvocationID];
    tc_Normal[gl_InvocationID] = Normal[gl_InvocationID];
    tc_FragPos[gl_InvocationID] = FragPos[gl_InvocationID];
    tc_vCol[gl_InvocationID] = vCol[gl_InvocationID];

    // Specify tessellation levels
    gl_TessLevelOuter[0] = 5.0;  // Outer tessellation level for edge 0
    gl_TessLevelOuter[1] = 5.0;  // Outer tessellation level for edge 1
    gl_TessLevelOuter[2] = 5.0;  // Outer tessellation level for edge 2
    gl_TessLevelInner[0] = 5.0;  // Inner tessellation level

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
