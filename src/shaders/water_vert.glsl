//#version 440
//
//layout(location = 0) in vec3 pos;
//layout(location = 1) in vec3 col;
//layout(location = 2) in vec2 tex;
//layout(location = 3) in vec3 norm;
//
//out vec4 vCol;
//out vec2 TexCoord;
//out vec3 Normal;
//out vec3 FragPos;
//
//uniform mat4 model;
//uniform mat4 projection;
//uniform mat4 view;
//uniform mat4 directionalLightTransform;
//uniform float time; //glfwGetTime() directly given
//
//const int NUM_WAVES = 10;
//const float A[NUM_WAVES] = { 0.5, 0.35, 0.25, 0.15, 0.78, 0.34,0.25, 0.9, 0.45, 0.67 }; // Amplitudes
//const vec2 D[NUM_WAVES] = { vec2(1.0, 0.0), vec2(0.7, 0.7), vec2(0.0, 1.0), vec2(-0.7, 0.7),
//vec2(0.8, 0.1), vec2(0.4, -0.3), vec2(0.2, 1.0), vec2(-0.3, 0.4) , vec2(7.0, -4.0), vec2(-0.8, -1.0) };  // Directions
//const float S[NUM_WAVES] = { 1.0, 1.2, 1.4, 1.6, 1.75, 1.74, 1.18, 1.3, 0.4, 1.8 }; // Speeds
//const float Q = 0.1;  // Steepness
//
//const float tiling = 6.0f;
//
//vec3 GerstnerWave(vec3 position, float time) {
//    float displaceX = 0.0;
//    float displaceY = 0.0;
//    float displaceZ = 0.0;
//
//    for (int i = 0; i < NUM_WAVES; i++) {
//        float waveFactor = D[i].x * position.x + D[i].y * position.z - S[i] * time;
//
//        displaceX += ((gl_VertexID % 4 == 0) ? Q * A[i] * sin(waveFactor) : Q * A[i] * cos(waveFactor)) * 0.1;
//        displaceY += (gl_VertexID % 2 == 0) ? A[i] * cos(waveFactor) : 0;
//        displaceZ += 0;// (gl_VertexID % 2 == 1) ? Q * A[i] * sin(waveFactor) : 0;
//    }
//
//    return vec3(position.x + displaceX, position.y + displaceY, position.z + displaceZ);
//}
//
//
//void main() {
//    vec3 displacedPos =  GerstnerWave(pos, time);
//    FragPos = vec3(model * vec4(displacedPos, 1.0));
//    vCol = vec4(clamp(col, 0.0, 1.0), 1.0);
//    TexCoord = tex;
//    Normal = norm; 
//}

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
uniform float time;

const uint gerstner_waves_length = 9;
struct GerstnerWave {
    vec2 direction;
    float amplitude;
    float steepness;
    float frequency;
    float speed;
} gerstner_waves[9];

float rand(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void initWaves() {
    for (uint i = 0; i < gerstner_waves_length; ++i) {
        float randomAngle = 2.0 * 3.14159 * rand(vec2(float(i), float(i + 1)));
        gerstner_waves[i].direction = normalize(vec2(cos(randomAngle), sin(randomAngle)));

        // Random amplitude between 0.1 and 1.0
        gerstner_waves[i].amplitude = 0.1 + 0.9 * rand(vec2(float(i + 2), float(i + 3)));

        // Steepness varies inversely with amplitude, let's choose a value between 0.1 and 0.3
        gerstner_waves[i].steepness = 0.1 + (0.3 - 0.1) * (1.0 - gerstner_waves[i].amplitude);

        // Random frequency between 0.2 and 2.0
        gerstner_waves[i].frequency = 0.2 + 1.8 * rand(vec2(float(i + 4), float(i + 5)));

        // Speed, generally set between 0.5 and 1.5 for varied wave speed
        gerstner_waves[i].speed = 0.5 + 1.0 * rand(vec2(float(i + 6), float(i + 7)));
    }
}


vec3 gerstner_wave_normal(vec3 position, float time) {
    vec3 wave_normal = vec3(0.0, 1.0, 0.0);
    for (uint i = 0; i < gerstner_waves_length; ++i) {
        float proj = dot(position.xz, gerstner_waves[i].direction),
            phase = time * gerstner_waves[i].speed,
            psi = proj * gerstner_waves[i].frequency + phase,
            Af = gerstner_waves[i].amplitude *
            gerstner_waves[i].frequency,
            alpha = Af * sin(psi);

        wave_normal.y -= gerstner_waves[i].steepness * alpha;

        float x = gerstner_waves[i].direction.x,
            y = gerstner_waves[i].direction.y,
            omega = Af * cos(psi);

        wave_normal.x -= x * omega;
        wave_normal.z -= y * omega;
    } 
    
    return wave_normal;
}

vec3 gerstner_wave_position(vec3 position, float time) {
    vec3 wave_position = vec3(position.x, 0, position.y);
    for (uint i = 0; i < gerstner_waves_length; ++i) {
        float proj = dot(position.xz, gerstner_waves[i].direction),
            phase = time * gerstner_waves[i].speed,
            theta = proj * gerstner_waves[i].frequency + phase,
            height = gerstner_waves[i].amplitude * sin(theta);

        wave_position.y += height * 0.5;

        float maximum_width = gerstner_waves[i].steepness *
            gerstner_waves[i].amplitude,
            width = maximum_width * cos(theta),
            x = gerstner_waves[i].direction.x,
            y = gerstner_waves[i].direction.y;

        wave_position.x += x * width * 0.5;
        wave_position.z += y * width * 0.5;
    } 
    
    //float x = (gl_VertexID % 4 == 0) ? wave_position.x * 1 : wave_position.x * 1;
    //float y = (gl_VertexID % 2 == 0) ? wave_position.y * 0.7 : position.y;
    //float y = wave_position.y * 0.7;
    //float z = wave_position.z * 0.5;
    return vec3(wave_position.x, wave_position.y, position.z);
}

void main() {
    initWaves();
    vec3 wave_position = gerstner_wave_position(pos, time);
    vec3 normal = gerstner_wave_normal(wave_position, time);
    FragPos = vec3(model * vec4(wave_position, 1.0));
    vCol = vec4(clamp(col, 0.0, 1.0), 1.0);
    TexCoord = tex;
    Normal = norm;
}

