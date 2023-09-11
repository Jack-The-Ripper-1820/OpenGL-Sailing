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

    /*vec2 T0;
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
    vec2 T2;*/

    vec3 N0;
    vec3 N1;
    vec3 N2;
    vec3 N3;

    /*vec4 C0;
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
    vec4 C2;*/
};

layout(vertices = 4) out;

uniform float TessellationLevel;

in vec4 vCol[];
in vec2 TexCoord[];
in vec3 Normal[];
in vec3 FragPos[];
//in vec4 DirectionalLightSpacePos[];

//in vec4 DirectionalLightSpacePos[];

out patch OutputPatch oPatch;
//out patch vec4 pDirectionalLightSpacePos;
out patch vec4 pCol;
out vec2 tc_TexCoord[];

void main() {
    //pDirectionalLightSpacePos = DirectionalLightSpacePos[gl_InvocationID];
    
    pCol = vCol[gl_InvocationID];
    tc_TexCoord[gl_InvocationID] = TexCoord[gl_InvocationID];

    oPatch.B0 = FragPos[0];
    oPatch.B1 = FragPos[1];
    oPatch.B2 = FragPos[2];
    oPatch.B3 = FragPos[3];

   /* oPatch.T0 = TexCoord[0];
    oPatch.T1 = TexCoord[1];
    oPatch.T2 = TexCoord[2];
    oPatch.T3 = TexCoord[3];*/

    /*oPatch.C0 = vCol[0];
    oPatch.C1 = vCol[1];
    oPatch.C2 = vCol[2];
    oPatch.C3 = vCol[3];*/

    vec3 p0 = oPatch.B0, p1 = oPatch.B1, p2 = oPatch.B2, p3 = oPatch.B3;
    //vec2 t0 = oPatch.T0, t1 = oPatch.T1, t2 = oPatch.T2, t3 = oPatch.T3;
    //vec4 c0 = oPatch.C0, c1 = oPatch.C1, c2 = oPatch.C2, c3 = oPatch.C3;

   /* for (int i = 0; i < 4; i++) {
        oPatch.Normal[i] = Normal[i];
        oPatch.TexCoord[i] = TexCoord[i];
        oPatch.Color[i] = vCol[i];
    }*/

    vec3 n0 = Normal[0];
    vec3 n1 = Normal[1];
    vec3 n2 = Normal[2];
    vec3 n3 = Normal[3];

    float v01 = 2 * dot(p1 - p0, n0 + n1) / dot(p1 - p0, p1 - p0);
    vec3 h01 = n0 + n1 - v01 * (p1 - p0);

    float v12 = 2 * dot(p2 - p1, n1 + n2) / dot(p2 - p1, p2 - p1);
    vec3 h12 = n1 + n2 - v12 * (p2 - p1);

    float v23 = 2 * dot(p3 - p2, n2 + n3) / dot(p3 - p2, p3 - p2);
    vec3 h23 = n2 + n3 - v23 * (p3 - p2);

    float v03 = 2 * dot(p3 - p0, n0 + n3) / dot(p3 - p0, p3 - p0);
    vec3 h03 = n0 + n3 - v03 * (p3 - p0);

    vec3 n01 = normalize(h01);
    vec3 n10 = n01;

    vec3 n12 = normalize(h12);
    vec3 n21 = n12;

    vec3 n23 = normalize(h23);
    vec3 n32 = n23;

    vec3 n03 = normalize(h03);
    vec3 n30 = n03;

    vec3 n0123 = (2 * (n01 + n12 + n23 + n30) + (n0 + n1 + n2 + n3)) / 12.f;

    oPatch.N0 = n0;
    oPatch.N1 = n1;
    oPatch.N2 = n2;
    oPatch.N3 = n3;
    /*oPatch.N01 = n01;
    oPatch.N12 = n12;
    oPatch.N23 = n23;
    oPatch.N30 = n30;
    oPatch.N10 = n10;
    oPatch.N0123 = n0123;*/

    float w01 = dot(p1 - p0, n0);
    float w10 = dot(p0 - p1, n1);
    float w12 = dot(p2 - p1, n1);
    float w21 = dot(p1 - p2, n2);
    float w23 = dot(p3 - p2, n2);
    float w32 = dot(p2 - p3, n3);
    float w30 = dot(p0 - p3, n3);
    float w03 = dot(p3 - p0, n0);

    /*float wt01 = dot(t1 - t0, vec2(n0));
    float wt10 = dot(t0 - t1, vec2(n1));
    float wt12 = dot(t2 - t1, vec2(n1));
    float wt21 = dot(t1 - t2, vec2(n2));
    float wt23 = dot(t3 - t2, vec2(n2));
    float wt32 = dot(t2 - t3, vec2(n3));
    float wt30 = dot(t0 - t3, vec2(n3));
    float wt03 = dot(t3 - t0, vec2(n0));*/

    /*float wc01 = dot(c1 - c0, vec4(n0, 1.0));
    float wc10 = dot(c0 - c1, vec4(n1, 1.0));
    float wc12 = dot(c2 - c1, vec4(n1, 1.0));
    float wc21 = dot(c1 - c2, vec4(n2, 1.0));
    float wc23 = dot(c3 - c2, vec4(n2, 1.0));
    float wc32 = dot(c2 - c3, vec4(n3, 1.0));
    float wc30 = dot(c0 - c3, vec4(n3, 1.0));
    float wc03 = dot(c3 - c0, vec4(n0, 1.0));*/

    vec3 b01 = (2. * p0 + p1 - w01 * n0) / 3.;
    vec3 b10 = (2. * p1 + p0 - w10 * n1) / 3.;
    vec3 b12 = (2. * p1 + p2 - w12 * n1) / 3.;
    vec3 b21 = (2. * p2 + p1 - w21 * n2) / 3.;
    vec3 b23 = (2. * p2 + p3 - w23 * n2) / 3.;
    vec3 b32 = (2. * p3 + p2 - w32 * n3) / 3.;
    vec3 b30 = (2. * p3 + p0 - w30 * n3) / 3.;
    vec3 b03 = (2. * p0 + p3 - w03 * n0) / 3.;

    /*vec2 t01 = (2. * t0 + t1 - wt01 * vec2(n0)) / 3.;
    vec2 t10 = (2. * t1 + t0 - wt10 * vec2(n1)) / 3.;
    vec2 t12 = (2. * t1 + t2 - wt12 * vec2(n1)) / 3.;
    vec2 t21 = (2. * t2 + t1 - wt21 * vec2(n2)) / 3.;
    vec2 t23 = (2. * t2 + t3 - wt23 * vec2(n2)) / 3.;
    vec2 t32 = (2. * t3 + t2 - wt32 * vec2(n3)) / 3.;
    vec2 t30 = (2. * t3 + t0 - wt30 * vec2(n3)) / 3.;
    vec2 t03 = (2. * t0 + t3 - wt03 * vec2(n0)) / 3.;*/

    /*vec4 c01 = (2. * c0 + c1 - wc01 * vec4(n0, 1)) / 3.;
    vec4 c10 = (2. * c1 + c0 - wc10 * vec4(n1, 1)) / 3.;
    vec4 c12 = (2. * c1 + c2 - wc12 * vec4(n1, 1)) / 3.;
    vec4 c21 = (2. * c2 + c1 - wc21 * vec4(n2, 1)) / 3.;
    vec4 c23 = (2. * c2 + c3 - wc23 * vec4(n2, 1)) / 3.;
    vec4 c32 = (2. * c3 + c2 - wc32 * vec4(n3, 1)) / 3.;
    vec4 c30 = (2. * c3 + c0 - wc30 * vec4(n3, 1)) / 3.;
    vec4 c03 = (2. * c0 + c3 - wc03 * vec4(n0, 1)) / 3.;*/

    oPatch.B01 = b01;
    oPatch.B10 = b10;
    oPatch.B12 = b12;
    oPatch.B21 = b21;
    oPatch.B23 = b23;
    oPatch.B32 = b32;
    oPatch.B30 = b30;
    oPatch.B03 = b03;

    /*oPatch.T01 = t01;
    oPatch.T10 = t10;
    oPatch.T12 = t12;
    oPatch.T21 = t21;
    oPatch.T23 = t23;
    oPatch.T32 = t32;
    oPatch.T30 = t30;
    oPatch.T03 = t03;*/

    /*oPatch.C01 = c01;
    oPatch.C10 = c10;
    oPatch.C12 = c12;
    oPatch.C21 = c21;
    oPatch.C23 = c23;
    oPatch.C32 = c32;
    oPatch.C30 = c30;
    oPatch.C03 = c03;*/

    vec3 q = b03 + b01 + b10 + b12 + b21 + b23 + b32 + b30;
    //vec2 qt = t03 + t01 + t10 + t12 + t21 + t23 + t32 + t30;
    //vec4 qc = c03 + c01 + c10 + c12 + c21 + c23 + c32 + c30;


    vec3 e0 = (2. * (b01 + b03 + q) - (b21 + b23)) / 18.;
    vec3 v0 = (4. * p0 + 2. * (p3 + p1) + p2) / 9.;
    vec3 b02 = e0 + (e0 - v0) / 2.;
    oPatch.B02 = b02;

    vec3 e1 = (2. * (b12 + b10 + q) - (b32 + b30)) / 18.;
    vec3 v1 = (4. * p1 + 2. * (p0 + p2) + p3) / 9.;
    vec3 b13 = e1 + (e1 - v1) / 2.;
    oPatch.B13 = b13;

    vec3 e2 = (2. * (b23 + b21 + q) - (b03 + b01)) / 18.;
    vec3 v2 = (4. * p2 + 2. * (p1 + p3) + p0) / 9.;
    vec3 b20 = e2 + (e2 - v2) / 2.;
    oPatch.B20 = b20;

    vec3 e3 = (2. * (b30 + b32 + q) - (b10 + b12)) / 18.;
    vec3 v3 = (4. * p3 + 2. * (p2 + p0) + p1) / 9.;
    vec3 b31 = e3 + (e3 - v3) / 2.;
    oPatch.B31 = b31;

    /*vec2 et0 = (2. * (t01 + t03 + qt) - (t21 + t23)) / 18.;
    vec2 vt0 = (4. * t0 + 2. * (t3 + t1) + t2) / 9.;
    vec2 t02 = et0 + (et0 - vt0) / 2.;
    oPatch.T02 = t02;

    vec2 et1 = (2. * (t12 + t10 + qt) - (t32 + t30)) / 18.;
    vec2 vt1 = (4. * t1 + 2. * (t0 + t2) + t3) / 9.;
    vec2 t13 = et1 + (et1 - vt1) / 2.;
    oPatch.T13 = t13;

    vec2 et2 = (2. * (t23 + t21 + qt) - (t03 + t01)) / 18.;
    vec2 vt2 = (4. * t2 + 2. * (t1 + t3) + t0) / 9.;
    vec2 t20 = et2 + (et2 - vt2) / 2.;
    oPatch.T20 = t20;

    vec2 et3 = (2. * (t30 + t32 + qt) - (t10 + t12)) / 18.;
    vec2 vt3 = (4. * t3 + 2. * (t2 + t0) + t1) / 9.;
    vec2 t31 = et3 + (et3 - vt3) / 2.;
    oPatch.T31 = t31;*/

   /* vec4 ec0 = (2. * (c01 + c03 + qc) - (c21 + c23)) / 18.;
    vec4 vc0 = (4. * c0 + 2. * (c3 + c1) + c2) / 9.;
    vec4 c02 = ec0 + (ec0 - vc0) / 2.;
    oPatch.C02 = c02;

    vec4 ec1 = (2. * (c12 + c10 + qc) - (c32 + c30)) / 18.;
    vec4 vc1 = (4. * c1 + 2. * (c0 + c2) + c3) / 9.;
    vec4 c13 = ec1 + (ec1 - vc1) / 2.;
    oPatch.C13 = c13;

    vec4 ec2 = (2. * (c23 + c21 + qc) - (c03 + c01)) / 18.;
    vec4 vc2 = (4. * c2 + 2. * (c1 + c3) + c0) / 9.;
    vec4 c20 = ec2 + (ec2 - vc2) / 2.;
    oPatch.C20 = c20;

    vec4 ec3 = (2. * (c30 + c32 + qc) - (c10 + c12)) / 18.;
    vec4 vc3 = (4. * c3 + 2. * (c2 + c0) + c1) / 9.;
    vec4 c31 = ec3 + (ec3 - vc3) / 2.;
    oPatch.C31 = c31;*/

    gl_TessLevelOuter[0] = TessellationLevel;
    gl_TessLevelOuter[1] = TessellationLevel;
    gl_TessLevelOuter[2] = TessellationLevel;
    gl_TessLevelOuter[3] = TessellationLevel;
    gl_TessLevelInner[0] = TessellationLevel;
    gl_TessLevelInner[1] = TessellationLevel;
}