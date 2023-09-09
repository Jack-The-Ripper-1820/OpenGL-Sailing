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

    //vec4 DirectionalLightSpacePos[3];

    //vec4 C030;
    //vec4 C021;
    //vec4 C012;
    //vec4 C003;
    //vec4 C102;
    //vec4 C201;
    //vec4 C300;
    //vec4 C210;
    //vec4 C120;
    //vec4 C111;

    //vec3 Normal[3];
    //vec2 TexCoord[3];
    //vec4 Color[3];
};

//layout(vertices = 3) in;

layout(vertices = 3) out;

uniform float TessellationLevel;

in vec4 vCol[];
in vec2 TexCoord[];
in vec3 Normal[];
in vec3 FragPos[];
in vec4 DirectionalLightSpacePos[];

//in vec4 DirectionalLightSpacePos[];

out patch OutputPatch oPatch;
out patch vec4 pDirectionalLightSpacePos;
out patch vec4 pCol;

void main() {
    pDirectionalLightSpacePos = DirectionalLightSpacePos[gl_InvocationID];
    pCol = vCol[gl_InvocationID];

    /*for (int i = 0; i < 3; i++) {
        oPatch.Normal[i] = Normal[i];
        oPatch.TexCoord[i] = TexCoord[i];
        oPatch.Color[i] = vCol[i];
    }*/
    /*for (int i = 0; i < 3; i++) {
        oPatch.DirectionalLightSpacePos[0] = DirectionalLightSpacePos[i];
    }*/

    oPatch.B300 = FragPos[0];
    oPatch.B030 = FragPos[1];
    oPatch.B003 = FragPos[2];

    vec3 b300 = oPatch.B300, b003 = oPatch.B003, b030 = oPatch.B030;
   
    vec2 t300 = TexCoord[0], t030 = TexCoord[1], t003 = TexCoord[2];
    vec2 t1 = t300, t2 = t030, t3 = t003;
    oPatch.T300 = t300, oPatch.T030 = t030, oPatch.T003 = t003;

    /*vec4 c300 = vCol[0], c030 = vCol[1], c003 = vCol[2];
    vec4 c1 = c300, c2 = c030, c3 = c003;
    oPatch.C300 = c300, oPatch.C030 = c030, oPatch.C003 = c003;*/

    vec3 p1 = oPatch.B300, p2 = oPatch.B030, p3 = oPatch.B003;
    vec3 n1 = Normal[0], n2 = Normal[1], n3 = Normal[2];

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

    /*float wc12 = dot(c2 - c1, vec4(n1, 1));
    float wc21 = dot(c1 - c2, vec4(n2, 1));
    float wc23 = dot(c3 - c2, vec4(n2, 1));
    float wc32 = dot(c2 - c3, vec4(n3, 1));
    float wc31 = dot(c1 - c3, vec4(n3, 1));
    float wc13 = dot(c3 - c1, vec4(n1, 1));*/

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

    /*vec4 c210 = (2. * c1 + c2 - wc12 * vec4(n1, 1)) / 3.;
    vec4 c120 = (2. * c2 + c1 - wc21 * vec4(n2, 1)) / 3.;
    vec4 c021 = (2. * c2 + c3 - wc23 * vec4(n2, 1)) / 3.;
    vec4 c012 = (2. * c3 + c2 - wc32 * vec4(n3, 1)) / 3.;
    vec4 c102 = (2. * c3 + c1 - wc31 * vec4(n3, 1)) / 3.;
    vec4 c201 = (2. * c1 + c3 - wc13 * vec4(n1, 1)) / 3.;*/

    vec3 e = (b120 + b120 + b021 + b012 + b102 + b210) / 6.;
    vec3 v = (p1 + p2 + p3) / 3.;
    vec3 b111 = e + (e - v) / 2.;

    vec2 et = (t120 + t120 + t021 + t012 + t102 + t210) / 6.;
    vec2 vt = (t1 + t2 + t3) / 3.;
    vec2 t111 = et + (et - vt) / 2.;

    /*vec4 ec = (c120 + c120 + c021 + c012 + c102 + c210) / 6.;
    vec4 vc = (c1 + c2 + c3) / 3.;
    vec4 c111 = ec + (ec - vc) / 2.;*/

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

    /*oPatch.C021 = c021;
    oPatch.C012 = c012;
    oPatch.C102 = c102;
    oPatch.C201 = c201;
    oPatch.C210 = c210;
    oPatch.C120 = c120;
    oPatch.C111 = c111;*/

    oPatch.N200 = n1;
    oPatch.N020 = n2;
    oPatch.N002 = n3;

    float v12 = 2.0 * dot(p2 - p1, n1 + n2) / dot(p2 - p1, p2 - p1);
    float v23 = 2.0 * dot(p3 - p2, n2 + n3) / dot(p3 - p2, p3 - p2);
    float v31 = 2.0 * dot(p1 - p3, n3 + n1) / dot(p1 - p3, p1 - p3);

    vec3 h110 = n1 + n2 - v12 * vec3(p2 - p1);
    vec3 h011 = n1 + n3 - v23 * vec3(p3 - p1);
    vec3 h101 = n2 + n3 - v31 * vec3(p3 - p2);

    oPatch.N110 = normalize(h110);
    oPatch.N011 = normalize(h011);
    oPatch.N101 = normalize(h101);

    gl_TessLevelOuter[0] = TessellationLevel;
    gl_TessLevelOuter[1] = TessellationLevel;
    gl_TessLevelOuter[2] = TessellationLevel;
    gl_TessLevelInner[0] = TessellationLevel;
}