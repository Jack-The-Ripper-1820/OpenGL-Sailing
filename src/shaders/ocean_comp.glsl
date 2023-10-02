#version 440

const int NUM_WAVES = 10;
const float A[NUM_WAVES] = { 0.5, 0.35, 0.25, 0.15, 0.78, 0.34,0.25, 0.9, 0.45, 0.67 }; // Amplitudes
const vec2 D[NUM_WAVES] = { vec2(1.0, 0.0), vec2(0.7, 0.7), vec2(0.0, 1.0), vec2(-0.7, 0.7),
vec2(0.8, 0.1), vec2(0.4, -0.3), vec2(0.2, 1.0), vec2(-0.3, 0.4) , vec2(7.0, -4.0), vec2(-0.8, -1.0) };  // Directions
const float S[NUM_WAVES] = { 1.0, 1.2, 1.4, 1.6, 1.75, 1.74, 1.18, 1.3, 0.4, 1.8 }; // Speeds
const float Q = 0.1;  // Steepness

const float tiling = 6.0f;

layout(local_size_x = 256) in;  // This can be adjusted based on your hardware capabilities

struct VertexData {
    vec3 pos;
    vec3 col;
    vec2 tex;
    vec3 norm;
};

layout(std430, binding = 0) buffer vertexInputOutputBuffer {
    float oceanVertices[];
};

uniform float time;

vec3 GerstnerWave(vec3 position, float time, uint invocationID) {
    float displaceX = 0.0;
    float displaceY = 0.0;
    float displaceZ = 0.0;

    for (int i = 0; i < NUM_WAVES; i++) {
        float waveFactor = D[i].x * position.x + D[i].y * position.z - S[i] * time;

        displaceX += ((invocationID % 4 == 0) ? Q * A[i] * sin(waveFactor) : Q * A[i] * cos(waveFactor)) * 0.1;
        displaceY += (invocationID % 2 == 0) ? A[i] * cos(waveFactor) : 0;
        displaceZ += 0; // (invocationID % 2 == 1) ? Q * A[i] * sin(waveFactor) : 0;
    }

    return vec3(position.x + displaceX, position.y + displaceY, position.z + displaceZ);
}



void main() {
    uint id = gl_GlobalInvocationID.x;
    uint idx = id * 11; // Since each vertex has 11 components

    vec3 currentPos = vec3(oceanVertices[idx], oceanVertices[idx + 1], oceanVertices[idx + 2]);
    vec3 displacedPos = GerstnerWave(currentPos, time, id);

    oceanVertices[idx] = displacedPos.x;
    oceanVertices[idx + 1] = displacedPos.y;
    oceanVertices[idx + 2] = displacedPos.z;
}
