#version 440

layout(vertices = 16) out;

in vec4 vCol[];
in vec2 TexCoord[];
in vec3 Normal[];
in vec3 FragPos[];

out vec4 tc_vCol[];
out vec2 tc_TexCoord[];
out vec3 tc_Normal[];
out vec3 tc_FragPos[];

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform float TessellationLevel;

void main()
{
	tc_vCol[gl_InvocationID] = vCol[gl_InvocationID];
	tc_TexCoord[gl_InvocationID] = TexCoord[gl_InvocationID];
	tc_Normal[gl_InvocationID] = Normal[gl_InvocationID];
	tc_FragPos[gl_InvocationID] = FragPos[gl_InvocationID];

	/*for (int i = 0; i < 16; i++) {
		tc_FragPos[i] = FragPos[i];
		tc_vCol[i] = vCol[i];
		tc_TexCoord[i] = TexCoord[i];
		tc_Normal[i] = Normal[i];
	}*/

	gl_TessLevelInner[0] = TessellationLevel; // Adjust these tessellation levels as needed
	gl_TessLevelInner[1] = TessellationLevel;
	gl_TessLevelOuter[0] = TessellationLevel;
	gl_TessLevelOuter[1] = TessellationLevel;
	gl_TessLevelOuter[2] = TessellationLevel;
	gl_TessLevelOuter[3] = TessellationLevel;
}

