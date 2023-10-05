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
};

layout(quads, equal_spacing, ccw) in;

in patch OutputPatch oPatch;
in patch vec4 pCol;
in vec2 tc_TexCoord[];
in vec3 tc_Normal[];


out vec4 vCol;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 directionalLightTransform;

void main() {
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vCol = pCol;

    vec2 t0 = tc_TexCoord[0];
    vec2 t1 = tc_TexCoord[1];
    vec2 t2 = tc_TexCoord[2];
    vec2 t3 = tc_TexCoord[3];

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

    vec3 n0 = normalize(tc_Normal[0]);
    vec3 n1 = normalize(tc_Normal[1]);
    vec3 n2 = normalize(tc_Normal[2]);
    vec3 n3 = normalize(tc_Normal[3]);

    float wt01 = dot(t1 - t0, vec2(n0));
    float wt10 = dot(t0 - t1, vec2(n1));
    float wt12 = dot(t2 - t1, vec2(n1));
    float wt21 = dot(t1 - t2, vec2(n2));
    float wt23 = dot(t3 - t2, vec2(n2));
    float wt32 = dot(t2 - t3, vec2(n3));
    float wt30 = dot(t0 - t3, vec2(n3));
    float wt03 = dot(t3 - t0, vec2(n0));

    vec2 t01 = (2. * t0 + t1 - wt01 * vec2(n0)) / 3.;
    vec2 t10 = (2. * t1 + t0 - wt10 * vec2(n1)) / 3.;
    vec2 t12 = (2. * t1 + t2 - wt12 * vec2(n1)) / 3.;
    vec2 t21 = (2. * t2 + t1 - wt21 * vec2(n2)) / 3.;
    vec2 t23 = (2. * t2 + t3 - wt23 * vec2(n2)) / 3.;
    vec2 t32 = (2. * t3 + t2 - wt32 * vec2(n3)) / 3.;
    vec2 t30 = (2. * t3 + t0 - wt30 * vec2(n3)) / 3.;
    vec2 t03 = (2. * t0 + t3 - wt03 * vec2(n0)) / 3.;

    vec2 qt = t03 + t01 + t10 + t12 + t21 + t23 + t32 + t30;

    vec2 et0 = (2. * (t01 + t03 + qt) - (t21 + t23)) / 18.;
    vec2 vt0 = (4. * t0 + 2. * (t3 + t1) + t2) / 9.;
    vec2 t02 = et0 + (et0 - vt0) / 2.;

    vec2 et1 = (2. * (t12 + t10 + qt) - (t32 + t30)) / 18.;
    vec2 vt1 = (4. * t1 + 2. * (t0 + t2) + t3) / 9.;
    vec2 t13 = et1 + (et1 - vt1) / 2.;

    vec2 et2 = (2. * (t23 + t21 + qt) - (t03 + t01)) / 18.;
    vec2 vt2 = (4. * t2 + 2. * (t1 + t3) + t0) / 9.;
    vec2 t20 = et2 + (et2 - vt2) / 2.;

    vec2 et3 = (2. * (t30 + t32 + qt) - (t10 + t12)) / 18.;
    vec2 vt3 = (4. * t3 + 2. * (t2 + t0) + t1) / 9.;
    vec2 t31 = et3 + (et3 - vt3) / 2.;

    /*vec2 tex = bu0 * (bv0 * t0 + bv1 * t01 + bv2 * t10 + bv3 * t1)
        + bu1 * (bv0 * t03 + bv1 * t02 + bv2 * t13 + bv3 * t12)
        + bu2 * (bv0 * t30 + bv1 * t31 + bv2 * t20 + bv3 * t21)
        + bu3 * (bv0 * t3 + bv1 * t32 + bv2 * t23 + bv3 * t2);*/

    float tu0 = (1. - u);
    float tu1 = u;

    float tv0 = (1. - v);
    float tv1 = v;

    vec2 tex = tu0 * (tv0 * t0 + tv1 * t1)
        + tu1 * (tv0 * t3 + tv1 * t2);

    float v01 = (2. * (dot(p1 - p0, n0 + n0) / dot(p1 - p0, p1 - p0)));
    float v12 = (2. * (dot(p2 - p1, n1 + n1) / dot(p2 - p1, p2 - p1)));
    float v23 = (2. * (dot(p3 - p2, n2 + n2) / dot(p3 - p2, p3 - p2)));
    float v30 = (2. * (dot(p0 - p3, n3 + n3) / dot(p0 - p3, p0 - p3)));

    vec3 n01 = normalize(n0 + n1 - v01 * (p1 - p0));
    vec3 n12 = normalize(n1 + n2 - v12 * (p2 - p1));
    vec3 n23 = normalize(n2 + n3 - v23 * (p3 - p2));
    vec3 n30 = normalize(n3 + n0 - v30 * (p0 - p3));

    vec3 n0123 = normalize(((2. * (n01 + n12 + n23 + n30)) + (n0 + n1 + n2 + n3)) / 12.);

    float nu0 = pow(1. - u, 2);
    float nu1 = 2. * u * (1. - u);
    float nu2 = u * u;

    float nv0 = pow(1. - v, 2);
    float nv1 = 2. * v * (1. - v);
    float nv2 = v * v;

    vec3 norm = nu0 * (nv0 * n0 + nv1 * n01 + nv2 * n1) + 
        nu1 * (nv0 * n30 + nv1 * n0123 + nv2 * n12) + 
        nu2 * (nv0 * n3 + nv1 * n23 + nv2 * n2);

    FragPos = (model * vec4(pos, 1.0)).xyz;

    TexCoord = tex;

    Normal = mat3(transpose(inverse(model))) * norm;

    gl_Position = projection * view * model * vec4(pos, 1.0);
}