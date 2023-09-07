#version 440

struct OutputPatch
{
    vec3 B0;
    vec3 B03;
    vec3 B30;
    vec3 B3;
    vec3 B01;
    vec3 B02;
    vec3 B31;
    vec3 B32;
    vec3 B10;
    vec3 B13;
    vec3 B20;
    vec3 B23;
    vec3 B1;
    vec3 B12;
    vec3 B21;
    vec3 B2;

    vec2 T0;
    vec2 T03;
    vec2 T30;
    vec2 T3;
    vec2 T01;
    vec2 T02;
    vec2 T31;
    vec2 T32;
    vec2 T10;
    vec2 T13;
    vec2 T20;
    vec2 T23;
    vec2 T1;
    vec2 T12;
    vec2 T21;
    vec2 T2;

    vec4 C0;
    vec4 C03;
    vec4 C30;
    vec4 C3;
    vec4 C01;
    vec4 C02;
    vec4 C31;
    vec4 C32;
    vec4 C10;
    vec4 C13;
    vec4 C20;
    vec4 C23;
    vec4 C1;
    vec4 C12;
    vec4 C21;
    vec4 C2;

    vec3 Normal[4];
    vec2 TexCoord[4];
    vec4 Color[4];
};

layout(quads, equal_spacing, ccw) in;

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

void main() {
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;
    vec3 p0 = oPatch.B0, p1 = oPatch.B1, p2 = oPatch.B2, p3 = oPatch.B3;


    float bu0 = pow(1. - u, 3);
    float bu1 = 3. * u * pow(1. - u, 2);
    float bu2 = 3. * u * u * (1. - u);
    float bu3 = u * u * u;

    float bv0 = pow(1. - v, 3);
    float bv1 = 3. * v * pow(1. - v, 2);
    float bv2 = 3. * v * v * (1. - v);
    float bv3 = v * v * v;

    vec3 pos = bu0 * (bv0 * oPatch.B0 + bv1 * oPatch.B01 + bv2 * oPatch.B10 + bv3 * oPatch.B1)
        + bu1 * (bv0 * oPatch.B03 + bv1 * oPatch.B02 + bv2 * oPatch.B13 + bv3 * oPatch.B12)
        + bu2 * (bv0 * oPatch.B30 + bv1 * oPatch.B31 + bv2 * oPatch.B20 + bv3 * oPatch.B21)
        + bu3 * (bv0 * oPatch.B3 + bv1 * oPatch.B32 + bv2 * oPatch.B23 + bv3 * oPatch.B2);

    vec2 tex = bu0 * (bv0 * oPatch.T0 + bv1 * oPatch.T01 + bv2 * oPatch.T10 + bv3 * oPatch.T1)
        + bu1 * (bv0 * oPatch.T03 + bv1 * oPatch.T02 + bv2 * oPatch.T13 + bv3 * oPatch.T12)
        + bu2 * (bv0 * oPatch.T30 + bv1 * oPatch.T31 + bv2 * oPatch.T20 + bv3 * oPatch.T21)
        + bu3 * (bv0 * oPatch.T3 + bv1 * oPatch.T32 + bv2 * oPatch.T23 + bv3 * oPatch.T2);

    vec4 col = bu0 * (bv0 * oPatch.C0 + bv1 * oPatch.C01 + bv2 * oPatch.C10 + bv3 * oPatch.C1)
        + bu1 * (bv0 * oPatch.C03 + bv1 * oPatch.C02 + bv2 * oPatch.C13 + bv3 * oPatch.C12)
        + bu2 * (bv0 * oPatch.C30 + bv1 * oPatch.C31 + bv2 * oPatch.C20 + bv3 * oPatch.C21)
        + bu3 * (bv0 * oPatch.C3 + bv1 * oPatch.C32 + bv2 * oPatch.C23 + bv3 * oPatch.C2);

    TexCoord = tex;
    vCol = col;

    vec2 t0 = oPatch.TexCoord[0];
    vec2 t1 = oPatch.TexCoord[1];
    vec2 t2 = oPatch.TexCoord[2];
    vec2 t3 = oPatch.TexCoord[3];

    vec3 n0 = oPatch.Normal[0];
    vec3 n1 = oPatch.Normal[1];
    vec3 n2 = oPatch.Normal[2];
    vec3 n3 = oPatch.Normal[3];

    float v01 = (2. * (dot(p1 - p0, n0 + n1) / dot(p1 - p0, p1 - p0)));
    float v12 = (2. * (dot(p2 - p1, n1 + n2) / dot(p2 - p1, p2 - p1)));
    float v23 = (2. * (dot(p3 - p2, n2 + n3) / dot(p3 - p2, p3 - p2)));
    float v30 = (2. * (dot(p0 - p3, n3 + n0) / dot(p0 - p3, p0 - p3)));

    vec3 n01 = normalize(n0 + n1 - v01 * (p1 - p0));
    vec3 n12 = normalize(n1 + n2 - v12 * (p2 - p1));
    vec3 n23 = normalize(n2 + n3 - v23 * (p3 - p2));
    vec3 n30 = normalize(n3 + n0 - v30 * (p0 - p3));

    vec3 n0123 = ((2. * (n01 + n12 + n23 + n30)) + (n0 + n1 + n2 + n3)) / 12.;

    float nu0 = pow(1. - u, 2);
    float nu1 = 2. * u * (1. - u);
    float nu2 = u * u;

    float nv0 = pow(1. - v, 2);
    float nv1 = 2. * v * (1. - v);
    float nv2 = v * v;

    vec3 norm = nu0 * (nv0 * n0 + nv1 * n01 + nv2 * n1) + nu1 * (nv0 * n30 + nv1 * n0123 + nv2 * n12) + nu2 * (nv0 * n3 + nv1 * n23 + nv2 * n2);

    gl_Position = projection * view * model * vec4(pos, 1.0);

    FragPos = (model * vec4(pos, 1.0)).xyz;

    Normal = mat3(transpose(inverse(model))) * norm;

    DirectionalLightSpacePos = directionalLightTransform * model * vec4(pos, 1.0);

}