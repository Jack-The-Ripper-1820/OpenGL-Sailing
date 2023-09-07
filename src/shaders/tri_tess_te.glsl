#version 440

struct OutputPatch
{
    vec4 B030;
    vec4 B021;
    vec4 B012;
    vec4 B003;
    vec4 B102;
    vec4 B201;
    vec4 B300;
    vec4 B210;
    vec4 B120;
    vec4 B111;

    vec2 T030;
    vec2 T021;
    vec2 T012;
    vec2 T003;
    vec2 T102;
    vec2 T201;
    vec2 T300;
    vec2 T210;
    vec2 T120;
    vec2 T111;

    vec4 N200;
    vec4 N020;
    vec4 N002;
    vec4 N110;
    vec4 N011;
    vec4 N101;

    vec4 C030;
    vec4 C021;
    vec4 C012;
    vec4 C003;
    vec4 C102;
    vec4 C201;
    vec4 C300;
    vec4 C210;
    vec4 C120;
    vec4 C111;

    vec4 Normal[3];
    vec2 TexCoord[3];
    vec4 Color[3];
};

layout(triangles, equal_spacing, ccw) in;

in patch OutputPatch oPatch;
in vec4 DirectionalLightSpacePos;

out vec4 vCol;
out vec2 TexCoord;
out vec4 Normal;
out vec4 FragPos;
out vec4 DirectionalLightSpacePos;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 directionalLightTransform;

vec4 interpolate3D(vec4 v0, vec4 v1, vec4 v2)
{
    return vec4(gl_TessCoord.x) * v0 + vec4(gl_TessCoord.y) * v1 + vec4(gl_TessCoord.z) * v2;
}


void main() {
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;
    float w = gl_TessCoord.z;

    float u3 = pow(u, 3);
    float v3 = pow(v, 3);
    float w3 = pow(w, 3);
    float u2 = pow(u, 2);
    float v2 = pow(v, 2);
    float w2 = pow(w, 2);

    vec4 pos = oPatch.B300 * w3 + oPatch.B030 * u3 + oPatch.B003 * v3 +
        oPatch.B210 * 3.0 * w2 * u + oPatch.B120 * 3.0 * w * u2 + oPatch.B201 * 3.0 * w2 * v +
        oPatch.B021 * 3.0 * u2 * v + oPatch.B102 * 3.0 * w * v2 + oPatch.B012 * 3.0 * u * v2 +
        oPatch.B111 * 6.0 * w * u * v;

    vec2 tex = oPatch.T300 * w3 + oPatch.T030 * u3 + oPatch.T003 * v3 +
        oPatch.T210 * 3.0 * w2 * u + oPatch.T120 * 3.0 * w * u2 + oPatch.T201 * 3.0 * w2 * v +
        oPatch.T021 * 3.0 * u2 * v + oPatch.T102 * 3.0 * w * v2 + oPatch.T012 * 3.0 * u * v2 +
        oPatch.T111 * 6.0 * w * u * v;

    vec4 norm = oPatch.N200 * w2 + oPatch.N020 * u2 + oPatch.N002 * v2 + oPatch.N110 * w * u + oPatch.N011 * u * v + oPatch.N101 * w * v;

    vec4 col = oPatch.C300 * w3 + oPatch.C030 * u3 + oPatch.C003 * v3 +
        oPatch.C210 * 3.0 * w2 * u + oPatch.C120 * 3.0 * w * u2 + oPatch.C201 * 3.0 * w2 * v +
        oPatch.C021 * 3.0 * u2 * v + oPatch.C102 * 3.0 * w * v2 + oPatch.C012 * 3.0 * u * v2 +
        oPatch.C111 * 6.0 * w * u * v;

    FragPos = (model * vec4(pos, 1.0)).xyz;
    TexCoord = tex;
    Normal = mat3(transpose(inverse(model))) * norm;
    vCol = col;

    gl_Position = projection * view * model * vec4(pos, 1.0);
    DirectionalLightSpacePos = directionalLightTransform * model * vec4(pos, 1.0);
}