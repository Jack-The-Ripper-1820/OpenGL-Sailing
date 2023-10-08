#version 440

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 col;
layout(location = 2) in vec2 tex;
layout(location = 3) in vec3 norm;

out vec4 vCol;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 directionalLightTransform;
uniform float time; //glfwGetTime() directly given

const int NUM_WAVES = 10;
const float A[NUM_WAVES] = { 0.5, 0.35, 0.25, 0.15, 0.78, 0.34,0.25, 0.9, 0.45, 0.67 }; // Amplitudes
const vec2 D[NUM_WAVES] = { vec2(1.0, 0.0), vec2(0.7, 0.7), vec2(0.0, 1.0), vec2(-0.7, 0.7),
vec2(0.8, 0.1), vec2(0.4, -0.3), vec2(0.2, 1.0), vec2(-0.3, 0.4) , vec2(7.0, -4.0), vec2(-0.8, -1.0) };  // Directions
const float S[NUM_WAVES] = { 1.0, 1.2, 1.4, 1.6, 1.75, 1.74, 1.18, 1.3, 0.4, 1.8 }; // Speeds
const float Q = 0.1;  // Steepness

const float tiling = 6.0f;

vec3 GerstnerWave(vec3 position, float time) {
    float displaceX = 0.0;
    float displaceY = 0.0;
    float displaceZ = 0.0;

    for (int i = 0; i < NUM_WAVES; i++) {
        float waveFactor = D[i].x * position.x + D[i].y * position.z - S[i] * time;

        displaceX += ((gl_VertexID % 4 == 0) ? Q * A[i] * sin(waveFactor) : Q * A[i] * cos(waveFactor)) * 0.1;
        displaceY += (gl_VertexID % 2 == 0) ? A[i] * cos(waveFactor) : 0;
        displaceZ += 0;// (gl_VertexID % 2 == 1) ? Q * A[i] * sin(waveFactor) : 0;
    }

    return vec3(position.x + displaceX, position.y + displaceY, position.z + displaceZ);
}


void main() {
    vec3 displacedPos =  GerstnerWave(pos, time);
    FragPos = vec3(model * vec4(displacedPos, 1.0));
    vCol = vec4(clamp(col, 0.0, 1.0), 1.0);
    TexCoord = tex;
    Normal = norm; 
}




