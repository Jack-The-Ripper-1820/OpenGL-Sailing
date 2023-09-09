#version 400 core

layout(triangles, equal_spacing, cw) in;

in vec3 pos[];
in vec3 col[];
in vec2 tex[];
in vec3 norm[];

out vec4 color;

float basisFunction(int i, int p, float u, int n, float* knots) {
    // Recursive Cox-de Boor formula for NURBS basis functions
    if (p == 0) {
        if (u >= knots[i] && u < knots[i + 1]) {
            return 1.0;
        }
        else {
            return 0.0;
        }
    }
    float w1 = (u - knots[i]) / (knots[i + p] - knots[i]);
    float w2 = (knots[i + p + 1] - u) / (knots[i + p + 1] - knots[i + 1]);
    return w1 * basisFunction(i, p - 1, u, n, knots) +
        w2 * basisFunction(i + 1, p - 1, u, n, knots);
}

void main() {
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    int p = 3; // Degree in u-direction
    int q = 3; // Degree in v-direction
    int n = 4; // Number of control points in u-direction
    int m = 4; // Number of control points in v-direction

    float uKnots[8] = { 0.0, 0.0, 0.0, 0.25, 0.5, 0.75, 1.0, 1.0 }; // Example u knots
    float vKnots[8] = { 0.0, 0.0, 0.0, 0.25, 0.5, 0.75, 1.0, 1.0 }; // Example v knots

    vec3 position = vec3(0.0);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            float Bu = basisFunction(i, p, u, n, uKnots);
            float Bv = basisFunction(j, q, v, m, vKnots);
            position += pos[i * m + j] * Bu * Bv;
        }
    }

    color = vec4(position, 1.0);
}
