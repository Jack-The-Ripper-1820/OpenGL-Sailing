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
uniform float time;

const uint waves_count = 9;
struct GerstnerWave {
    vec2 direction;
    float amplitude;
    float steepness;
    float frequency;
    float speed;
} gerstner_waves[waves_count];

float rand(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void initWaves() {
    for (uint i = 0; i < waves_count; ++i) {
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


vec3 calc_wave(vec3 position, float time) {
    vec3 wave_position = vec3(position.x, 0, position.y);
    for (uint i = 0; i < waves_count; ++i) {
        float proj = dot(position.xz, gerstner_waves[i].direction);
        float phase = time * gerstner_waves[i].speed;
        float theta = proj * gerstner_waves[i].frequency + phase;
        float height = gerstner_waves[i].amplitude * sin(theta);

        wave_position.y += height * 0.1;

        float maximum_width = gerstner_waves[i].steepness * gerstner_waves[i].amplitude;
        float width = maximum_width * cos(theta);
        float x = gerstner_waves[i].direction.x;
        float y = gerstner_waves[i].direction.y;

        wave_position.x += x * width * 0.2;
        wave_position.z += y * width * 0.2;
    } 

    return vec3(wave_position.x, wave_position.y, position.z);
}

void main() {
    initWaves();
    vec3 wavePos = calc_wave(pos, time);
    FragPos = vec3(model * vec4(wavePos, 1.0));
    vCol = vec4(clamp(col, 0.0, 1.0), 1.0);
    TexCoord = tex;
    Normal = norm;
}

