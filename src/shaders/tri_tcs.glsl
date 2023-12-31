#version 440 

struct OutputPatch
{
    vec3 B030;
    vec3 B021;
    vec3 B012;
    vec3 B003;
    vec3 B102;
    vec3 B201;
    vec3 B300;
    vec3 B210;
    vec3 B120;
    vec3 B111;

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

    vec3 N200;
    vec3 N020;
    vec3 N002;
    vec3 N110;
    vec3 N011;
    vec3 N101;
};

layout(vertices = 3) out;

uniform float TessellationLevel;

in vec4 vCol[];
in vec2 TexCoord[];
in vec3 Normal[];
in vec3 FragPos[];


out patch OutputPatch oPatch;
out patch vec4 pCol;

void main() {
    pCol = vCol[gl_InvocationID];

    oPatch.B300 = FragPos[0];
    oPatch.B030 = FragPos[1];
    oPatch.B003 = FragPos[2];

    vec3 b300 = oPatch.B300, b003 = oPatch.B003, b030 = oPatch.B030;
   
    vec2 t300 = TexCoord[0], t030 = TexCoord[1], t003 = TexCoord[2];
    vec2 t1 = t300, t2 = t030, t3 = t003;
    oPatch.T300 = t300, oPatch.T030 = t030, oPatch.T003 = t003;

    vec3 p1 = oPatch.B300, p2 = oPatch.B030, p3 = oPatch.B003;
    vec3 n1 = normalize(Normal[0]), n2 = normalize(Normal[1]), n3 = normalize(Normal[2]);

    float w12 = dot(vec3(p2 - p1), n1);
    float w21 = dot(vec3(p1 - p2), n2);
    float w23 = dot(vec3(p3 - p2), n2);
    float w32 = dot(vec3(p2 - p3), n3);
    float w31 = dot(vec3(p1 - p3), n3);
    float w13 = dot(vec3(p3 - p1), n1);

    float wt12 = dot(t2 - t1, vec2(n1));
    float wt21 = dot(t1 - t2, vec2(n2));
    float wt23 = dot(t3 - t2, vec2(n2));
    float wt32 = dot(t2 - t3, vec2(n3));
    float wt31 = dot(t1 - t3, vec2(n3));
    float wt13 = dot(t3 - t1, vec2(n1));

    vec3 b210 = (2. * p1 + p2 - w12 * n1) / 3.;
    vec3 b120 = (2. * p2 + p1 - w21 * n2) / 3.;
    vec3 b021 = (2. * p2 + p3 - w23 * n2) / 3.;
    vec3 b012 = (2. * p3 + p2 - w32 * n3) / 3.;
    vec3 b102 = (2. * p3 + p1 - w31 * n3) / 3.;
    vec3 b201 = (2. * p1 + p3 - w13 * n1) / 3.;

    vec2 t210 = (2. * t1 + t2 - wt12 * vec2(n1)) / 3.;
    vec2 t120 = (2. * t2 + t1 - wt21 * vec2(n2)) / 3.;
    vec2 t021 = (2. * t2 + t3 - wt23 * vec2(n2)) / 3.;
    vec2 t012 = (2. * t3 + t2 - wt32 * vec2(n3)) / 3.;
    vec2 t102 = (2. * t3 + t1 - wt31 * vec2(n3)) / 3.;
    vec2 t201 = (2. * t1 + t3 - wt13 * vec2(n1)) / 3.;

    vec3 e = (b120 + b120 + b021 + b012 + b102 + b210) / 6.;
    vec3 v = (p1 + p2 + p3) / 3.;
    vec3 b111 = e + (e - v) / 2.;

    vec2 et = (t120 + t120 + t021 + t012 + t102 + t210) / 6.;
    vec2 vt = (t1 + t2 + t3) / 3.;
    vec2 t111 = et + (et - vt) / 2.;

    oPatch.B021 = b021;
    oPatch.B012 = b012;
    oPatch.B102 = b102;
    oPatch.B201 = b201;
    oPatch.B210 = b210;
    oPatch.B120 = b120;
    oPatch.B111 = b111;

    oPatch.T021 = t021;
    oPatch.T012 = t012;
    oPatch.T102 = t102;
    oPatch.T201 = t201;
    oPatch.T210 = t210;
    oPatch.T120 = t120;
    oPatch.T111 = t111;

    oPatch.N200 = n1;
    oPatch.N020 = n2;
    oPatch.N002 = n3;

    float v12 = 2.0 * dot(p2 - p1, n1 + n2) / dot(p2 - p1, p2 - p1);
    float v23 = 2.0 * dot(p3 - p2, n2 + n3) / dot(p3 - p2, p3 - p2);
    float v31 = 2.0 * dot(p1 - p3, n3 + n1) / dot(p1 - p3, p1 - p3);

    vec3 h110 = n1 + n2 - v12 * vec3(p2 - p1);
    vec3 h011 = n2 + n3 - v23 * vec3(p3 - p2);
    vec3 h101 = n3 + n1 - v31 * vec3(p1 - p3);

    oPatch.N110 = normalize(h110);
    oPatch.N011 = normalize(h011);
    oPatch.N101 = normalize(h101);

    gl_TessLevelOuter[0] = TessellationLevel;
    gl_TessLevelOuter[1] = TessellationLevel;
    gl_TessLevelOuter[2] = TessellationLevel;
    gl_TessLevelInner[0] = TessellationLevel;
}