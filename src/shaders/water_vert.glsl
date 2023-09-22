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
//uniform float time;
//
//void main()
//{
//    vCol = vec4(clamp(col, 0.0f, 1.0f), 1.0f);
//
//    TexCoord = tex;
//
//    Normal = norm;
//
//    // Calculate the sine wave displacement along the Y-axis
//    float displacement = sin(time) * 0.1; // Adjust the 0.1 to control the amplitude of the wave
//
//    // Modify the position
//    vec3 modifiedPos = pos;
//    
//    if (gl_VertexID % 2 == 0) {
//        modifiedPos.y += displacement;
//    }
//
//    FragPos = modifiedPos;
//}

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
//uniform float time;
//
//// Define the number of sine components you want to sum
//const int numSines = 5; // You can adjust this as needed
//
//void main()
//{
//    vCol = vec4(clamp(col, 0.0f, 1.0f), 1.0f);
//
//    TexCoord = tex;
//
//    Normal = norm;
//
//    // Calculate the total sine wave displacement along the Y-axis
//    float displacement = 0.0;
//
//    for (int i = 0; i < numSines; ++i) {
//        // Calculate the frequency and amplitude for each sine component
//        float frequency = float(i + 1); // Adjust the frequency as needed
//        float amplitude = 0.1 / frequency; // Adjust the amplitude as needed
//
//        // Add the contribution of the current sine component
//        displacement += amplitude * sin(frequency * time + pos.x);
//    }
//
//    // Modify the position
//    vec3 modifiedPos = pos + vec3(0.0, displacement, 0.0);
//
//    FragPos = modifiedPos;
//
//    // Calculate the final position
//    gl_Position = projection * view * model * vec4(modifiedPos, 1.0);
//}

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
//uniform float time;
//
//const int numWaves = 5; // Number of wave components
//const float waveSpeed = 0.05; // Adjust the wave speed as needed
//
//void main()
//{
//    vCol = vec4(clamp(col, 0.0f, 1.0f), 1.0f);
//    TexCoord = tex;
//    Normal = norm;
//
//    // Calculate the total wave displacement along the Y-axis
//    float displacement = 0.0;
//
//    for (int i = 0; i < numWaves; ++i) {
//        // Adjust the frequency, amplitude, and direction for each wave component
//        float frequency = float(i + 1) * 2.0;
//        float amplitude = 10 * 0.1 / frequency;
//        vec2 direction = normalize(vec2(1.0, 1.0)); // Adjust the direction
//
//        // Calculate the contribution of the current wave component
//        float wave = amplitude * sin(frequency * dot(direction, gl_VertexID * vec2(1.0, 0.0) - time * waveSpeed));
//        displacement += wave;
//    }
//
//    // Modify the position
//    vec3 modifiedPos = pos + vec3(0.0, displacement, 0.0);
//    FragPos = modifiedPos;
//
//    // Calculate the final position
//    gl_Position = projection * view * model * vec4(modifiedPos, 1.0);
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

// Include Perlin noise functions (you can define these functions or use a library)
float random(vec2 st)
{
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

float interpolate(float a, float b, float t)
{
    return (1.0 - t) * a + t * b;
}

float noise(vec2 st)
{
    vec2 i = floor(st);
    vec2 f = fract(st);

    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    vec2 u = f * f * (3.0 - 2.0 * f);

    return interpolate(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}

void main()
{
    vCol = vec4(clamp(col, 0.0f, 1.0f), 1.0f);
    TexCoord = tex;
    Normal = norm;

    // Calculate the sine wave displacement along the Y-axis
    float sineDisplacement = sin(time) * 0.1; // Adjust the 0.1 to control the amplitude of the sine wave

    // Calculate Perlin noise displacement
    // Calculate Perlin noise displacement
    vec2 noiseOffset = vec2(100.0, 200.0); // Adjust the offsets as needed
    float perlinDisplacement = noise((vec2(gl_VertexID) + noiseOffset) * 0.1);

    // Combine sine wave and Perlin noise displacements
    float displacement = sineDisplacement + perlinDisplacement * 0.1; // Adjust the scale as needed

    // Modify the position
    vec3 modifiedPos = pos;
    if (gl_VertexID % 2 == 0) {
        modifiedPos.y += displacement;
    }

    FragPos = modifiedPos;

    // Calculate the final position
    gl_Position = projection * view * model * vec4(modifiedPos, 1.0);
}



