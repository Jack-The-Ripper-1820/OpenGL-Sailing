#version 440
#extension GL_NV_shader_buffer_load : enable

//layout(quads, equal_spacing, cw) in;
layout(quads, fractional_odd_spacing, ccw) in;


in vec4 tc_vCol[];
in vec2 tc_TexCoord[];
in vec3 tc_Normal[];
in vec3 tc_FragPos[];

out vec4 vCol;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;


//float basisFunction(int i, int p, float u, int n, float knots[8]) {
//    // Recursive Cox-de Boor formula for NURBS basis functions
//    if (p == 0) {
//        if (u >= knots[i] && u < knots[i + 1]) {
//            return 1.0;
//        }
//        else {
//            return 0.0;
//        }
//    }
//    float w1 = (u - knots[i]) / (knots[i + p] - knots[i]);
//    float w2 = (knots[i + p + 1] - u) / (knots[i + p + 1] - knots[i + 1]);
//    return w1 * basisFunction(i, p - 1, u, n, knots) +
//        w2 * basisFunction(i + 1, p - 1, u, n, knots);
//}

//float basisFunction(int i, int p, float u, int n, float knots[8]) {
//    // Iterative Cox-de Boor formula for NURBS basis functions
//    float N[8]; // Array to store basis functions
//
//    for (int j = 0; j <= p; ++j) {
//        for (int k = 0; k < n; ++k) {
//            if (j == 0 && knots[k] <= u && u < knots[k + 1]) {
//                N[k] = 1.0;
//            }
//            else {
//                N[k] = 0.0;
//            }
//        }
//
//        for (int d = 1; d <= p; ++d) {
//            for (int k = 0; k < n - d; ++k) {
//                float w1 = (u - knots[k]) / (knots[k + d] - knots[k]);
//                float w2 = (knots[k + d + 1] - u) / (knots[k + d + 1] - knots[k + 1]);
//                N[k] = w1 * N[k] + w2 * N[k + 1];
//            }
//        }
//    }
//
//    return N[i];
//}

//float basisFunction(float t, int i, int n)
//{
//    // B-spline basis function
//    if (n == 1) {
//        if (t >= float(i) && t < float(i + 1)) {
//            return 1.0;
//        }
//        else {
//            return 0.0;
//        }
//    }
//    else {
//        float term1 = (t - float(i)) / float(n - 1) * basisFunction(t, i, n - 1);
//        float term2 = (float(i + n) - t) / float(n - 1) * basisFunction(t, i + 1, n - 1);
//        return term1 + term2;
//    }
//}

float basisFunction(float t, int i, int n)
{
    // B-spline basis function (iterative version)
    float result = 0.0;

    for (int k = 0; k <= n - 1; ++k) {
        float kin1 = float(i + k + 1);
        float ki1 = float(i + 1 + k);
        float kin = float(i + k);
        float ki = float(i + k);

        float a = 0.0;
        float b = 0.0;

        if (n == 1) {
            if (t >= ki && t < ki1) {
                return 1.0;
            }
            else {
                return 0.0;
            }
        }
        else {
            if (t >= ki && t < ki1) {
                a = 1.0;
            }
            if (t >= kin && t < kin1) {
                b = 1.0;
            }
            result = a + (t - ki) / (ki1 - ki) * (b - a);
        }
    }

    return result;
}

vec3 calculateSplineSurfacePoint(vec3 controlPoints[4][4], vec2 uv)
{
    // B-spline surface interpolation
    vec3 surfacePoint = vec3(0.0);
    float u = uv.x;
    float v = uv.y;

    for (int i = 0; i < 4; i++) {
        float bi = basisFunction(u, i, 4);
        for (int j = 0; j < 4; j++) {
            float bj = basisFunction(v, j, 4);
            surfacePoint += bi * bj * controlPoints[i][j];
        }
    }

    return surfacePoint;
}



void main() {

   // float u = gl_TessCoord.x;
   // float v = gl_TessCoord.y;

   // int p = 3; // Degree in u-direction
   // int q = 3; // Degree in v-direction
   // int n = 4; // Number of control points in u-direction
   // int m = 4; // Number of control points in v-direction
   // //[0.0, 0.0, 0.0, 0.0, 0.1, 0.1, 0.5, 1.0, 1.0, 1.0, 1.0]
   // float uKnots[8] = { 0.0, 0.0, 0.0, 0.25, 0.5, 0.75, 1.0, 1.0 };
   // float vKnots[8] = { 0.0, 0.0, 0.0, 0.25, 0.5, 0.75, 1.0, 1.0 };

   // vec3 pos = vec3(0.0);
   // vec2 tex = vec2(0.0);
   // vec3 norm = vec3(0.0);
   // vec3 col = vec3(0.0);

   // for (int i = 0; i < n; ++i) {
   //     for (int j = 0; j < m; ++j) {
   //         float Bu = basisFunction(i, p, u, n, uKnots);
   //         float Bv = basisFunction(j, q, v, m, vKnots);
   //         pos += tc_FragPos[i * m + j] * Bu * Bv;
   //         norm += tc_Normal[i * m + j] * Bu * Bv;
   //         col += vec3(tc_vCol[i * m + j]) * Bu * Bv;
   //         tex += tc_TexCoord[i * m + j] * Bu * Bv;
   //     }
   // }

   // FragPos = (model * vec4(pos, 1.0)).xyz;

   // TexCoord = tex;

   // Normal = mat3(transpose(inverse(model))) * norm;

   // vCol = vec4(col, 1.0);

   ///* TexCoord = tc_TexCoord[gl_InvocationID];

   // Normal = mat3(transpose(inverse(model))) * tc_Normal[gl_InvocationID];;

   // vCol = vec4(tc_vCol[gl_InvocationID], 1.0);*/

   // gl_Position = projection * view * model * vec4(pos, 1.0);

    vec2 uv = gl_TessCoord.xy;

    // Use the texture coordinates (tex) as the parameter for B-spline surface interpolation
    vec3 controlPoints[4][4];

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            controlPoints[i][j] = tc_FragPos[i * 4 + j];
        }
    }
    vec3 surfacePoint = calculateSplineSurfacePoint(controlPoints, uv);

    // Set the output values
    FragPos = surfacePoint;
    vCol = vec4(0, 0, 1, 1);
    //vCol = vec4(clamp(tc_vCol[0], 0.0f, 1.0f), 1.0f);
    TexCoord = tc_TexCoord[0];
    Normal = vec3(0, 1, 0);
    gl_Position = projection * view * model * vec4(surfacePoint, 1.0);
}




