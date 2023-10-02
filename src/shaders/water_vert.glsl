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
//
//float random(vec2 st) {
//    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453);
//}
//
//float perlin(vec2 p) {
//    vec2 i = floor(p);
//    vec2 f = fract(p);
//
//    // Smoothstep function for interpolation
//    f = f * f * (3.0 - 2.0 * f);
//
//    float a = random(i);
//    float b = random(i + vec2(1.0, 0.0));
//    float c = random(i + vec2(0.0, 1.0));
//    float d = random(i + vec2(1.0, 1.0));
//
//    float u = f.x * f.x * (3.0 - 2.0 * f.x);
//    float v = f.y * f.y * (3.0 - 2.0 * f.y);
//
//    return mix(a, b, u) + (c - a) * v * (1.0 - u) + (d - b) * u * v;
//}
//
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
//        float frequency = float(i + 1) * 5.0;
//        float amplitude = 0.1 / frequency;
//        vec2 direction = normalize(vec2(1.0, 1.0)); // Adjust the direction
//
//        // Calculate the contribution of the current wave component
//        float wave = amplitude * sin(frequency * dot(direction, gl_VertexID * vec2(1.0, 0.0) - time * waveSpeed));
//
//        // Add Perlin noise here (displace by Perlin noise)
//        vec2 perlinOffset = vec2(0.0, displacement);
//        wave += 0.5 * perlin((gl_VertexID * vec2(1.0, 0.0) - time * waveSpeed) * 0.1 + perlinOffset);
//
//        
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
//// Include Perlin noise functions (you can define these functions or use a library)
//float random(vec2 st)
//{
//    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
//}
//
//float interpolate(float a, float b, float t)
//{
//    return (1.0 - t) * a + t * b;
//}
//
//float noise(vec2 st)
//{
//    vec2 i = floor(st);
//    vec2 f = fract(st);
//
//    float a = random(i);
//    float b = random(i + vec2(1.0, 0.0));
//    float c = random(i + vec2(0.0, 1.0));
//    float d = random(i + vec2(1.0, 1.0));
//
//    vec2 u = f * f * (3.0 - 2.0 * f);
//
//    return interpolate(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
//}
//
//void main()
//{
//    vCol = vec4(clamp(col, 0.0f, 1.0f), 1.0f);
//    TexCoord = tex;
//    Normal = norm;
//
//    // Calculate the sine wave displacement along the Y-axis
//    float sineDisplacement = sin(time) * 1.0; // Adjust the 0.1 to control the amplitude of the sine wave
//
//    // Calculate Perlin noise displacement
//    // Calculate Perlin noise displacement
//    vec2 noiseOffset = vec2(100.0, 200.0); // Adjust the offsets as needed
//    float perlinDisplacement = noise((vec2(gl_VertexID) + noiseOffset) * 0.1);
//
//    // Combine sine wave and Perlin noise displacements
//    float displacement = sineDisplacement + perlinDisplacement * 0.1; // Adjust the scale as needed
//
//    // Modify the position
//    vec3 modifiedPos = pos;
//    if (gl_VertexID % 2 == 0) {
//        modifiedPos.y += displacement;
//    }
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
//float random(vec2 st) {
//    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453);
//}
//
//float perlin(vec2 p) {
//    vec2 i = floor(p);
//    vec2 f = fract(p);
//
//    // Smoothstep function for interpolation
//    f = f * f * (3.0 - 2.0 * f);
//
//    float a = random(i);
//    float b = random(i + vec2(1.0, 0.0));
//    float c = random(i + vec2(0.0, 1.0));
//    float d = random(i + vec2(1.0, 1.0));
//
//    float u = f.x * f.x * (3.0 - 2.0 * f.x);
//    float v = f.y * f.y * (3.0 - 2.0 * f.y);
//
//    return mix(a, b, u) + (c - a) * v * (1.0 - u) + (d - b) * u * v;
//}
//
//void main() {
//    vCol = vec4(clamp(col, 0.0f, 1.0f), 1.0f);
//    TexCoord = tex;
//
//    // Calculate the total wave displacement along the Y-axis
//    float displacement = 0.0;
//
//    for (int i = 0; i < numWaves; ++i) {
//        // Generate random frequencies and amplitudes
//        float frequency = 0.1 + random(vec2(i, time));
//        float amplitude = 0.05 + random(vec2(i * 2.0, time * 0.5));
//
//        // Adjust the direction for each wave component
//        vec2 direction = normalize(vec2(1.0, 1.0)); // Adjust the direction
//
//        // Calculate the contribution of the current wave component
//        float wave = amplitude * sin(frequency * dot(direction, gl_VertexID * vec2(1.0, 0.0) - time * waveSpeed));
//
//        // Add Perlin noise here (displace by Perlin noise)
//        vec2 perlinOffset = vec2(0.0, displacement);
//        wave += 0.5 * perlin((gl_VertexID * vec2(1.0, 0.0) - time * waveSpeed) * 0.1 + perlinOffset);
//
//        displacement += wave;
//
//        // Calculate the normal for the current wave
//        vec3 ddx = vec3(1.0, 0.0, waveSpeed * frequency * amplitude * cos(frequency * dot(direction, gl_VertexID * vec2(1.0, 0.0) - time * waveSpeed)));
//        vec3 ddy = vec3(0.0, 1.0, waveSpeed * frequency * amplitude * cos(frequency * dot(direction, gl_VertexID * vec2(1.0, 0.0) - time * waveSpeed)));
//        vec3 waveNormal = normalize(cross(ddx, ddy));
//
//        // Accumulate normals from each wave
//        Normal += waveNormal;
//    }
//
//    // Modify the position
//    vec3 modifiedPos = pos + vec3(0.0, displacement, 0.0);
//    FragPos = modifiedPos;
//
//    // Normalize the accumulated normal
//    Normal = normalize(Normal);
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
//uniform float time; //glfwGetTime() directly given
//
//const int numWaves = 100; // Number of wave components
//const float waveSpeed = 0.05; // Adjust the wave speed as needed
//const float waveLength = 1.0;
//
//float random(vec2 st) {
//    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453);
//}
//    
//float perlin(vec2 p) {
//    vec2 i = floor(p);
//    vec2 f = fract(p);
//    
//    // Smoothstep function for interpolation
//    f = f * f * (3.0 - 2.0 * f);
//    
//    float a = random(i);
//    float b = random(i + vec2(1.0, 0.0));
//    float c = random(i + vec2(0.0, 1.0));
//    float d = random(i + vec2(1.0, 1.0));
//    
//    float u = f.x * f.x * (3.0 - 2.0 * f.x);
//    float v = f.y * f.y * (3.0 - 2.0 * f.y);
//    
//    return mix(a, b, u) + (c - a) * v * (1.0 - u) + (d - b) * u * v;
//}
//
//void main() {
//    vCol = vec4(clamp(col, 0.0f, 1.0f), 1.0f);
//    TexCoord = tex;
//
//    // Calculate the total wave displacement along the Y-axis
//    float displacement = 0.0;
//
//    for (int i = 0; i < numWaves; ++i) {
//        // Generate random frequencies and amplitudes
//        float frequency = 1.0 + perlin(vec2(i, time));
//        float amplitude = 0.01 + perlin(vec2(i * 2.0, time * 0.5));
//
//        // Adjust the direction for each wave component
//        vec2 direction = normalize(vec2(1.0, 1.0)); // Adjust the direction
//
//        // Calculate the contribution of the current wave component
//        float wave = amplitude * sin(frequency * dot(direction, gl_VertexID * vec2(1.0, 0.0) - time * waveSpeed));
//
//        // Add Perlin noise here (displace by Perlin noise)
//        vec2 perlinOffset = vec2(0.0, displacement);
//        wave += 0.5 * perlin((gl_VertexID * vec2(1.0, 0.0) - time * waveSpeed) * 0.1 + perlinOffset);
//
//        displacement += wave;
//
//        // Calculate the normal for the current wave
//        vec3 ddx = vec3(1.0, 0.0, waveSpeed * frequency * amplitude * cos(frequency * dot(direction, gl_VertexID * vec2(1.0, 0.0) - time * waveSpeed)));
//        vec3 ddy = vec3(0.0, 1.0, waveSpeed * frequency * amplitude * cos(frequency * dot(direction, gl_VertexID * vec2(1.0, 0.0) - time * waveSpeed)));
//        vec3 waveNormal = normalize(cross(ddx, ddy));
//
//        // Accumulate normals from each wave
//        Normal += waveNormal;
//    }
//
//    // Modify the position
//    vec3 modifiedPos = pos + vec3(0.0, displacement, 0.0);
//    FragPos = modifiedPos;
//
//    // Normalize the accumulated normal
//    Normal = norm;
//
//    // Calculate the final position
//    //gl_Position = projection * view * model * vec4(modifiedPos, 1.0);
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
//
//// Define arrays for wave parameters
//float amplitudes[numWaves];
//float frequencies[numWaves];
//float phases[numWaves];
//
//float random(vec2 seed) {
//    return fract(sin(dot(seed, vec2(12.9898, 78.233))) * 43758.5453);
//}
//
//// Function to initialize random parameters
//void initializeRandomWaves() {
//    for (int i = 0; i < numWaves; ++i) {
//        // Set random amplitudes, frequencies, and phases
//        amplitudes[i] = random(vec2(float(i), clamp(time, 0, 1)));
//        frequencies[i] = random(vec2(float(i) * 2.0, clamp(time, 0, 1)));
//        phases[i] = random(vec2(float(i) * 3.0, clamp(time, 0, 1))) * 6.28; // Random phase between 0 and 2*pi
//    }
//}
//
//void main() {
//    vCol = vec4(clamp(col, 0.0f, 1.0f), 1.0f);
//    TexCoord = tex;
//    Normal = norm;
//
//    // Calculate the total wave displacement along the Y-axis
//    float displacement = 0.0;
//
//    // Initialize random wave parameters
//    initializeRandomWaves();
//
//    for (int i = 0; i < numWaves; ++i) {
//        // Calculate the contribution of the current sine component
//        float wave = amplitudes[i] * sin(frequencies[i] * pos.x + clamp(time, 0, 1) + phases[i]);
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

vec3 ComputeGerstnerNormal(vec3 position, float time) {
    vec3 normal = vec3(0.0);

    for (int i = 0; i < NUM_WAVES; i++) {
        float waveFactor = D[i].x * position.x + D[i].y * position.z - S[i] * time;

        // Compute the gradient of the Gerstner wave
        float gradientX = Q * A[i] * cos(waveFactor);
        float gradientY = -A[i] * sin(waveFactor);
        float gradientZ = Q * A[i] * cos(waveFactor);

        //// Add the contribution of each wave to the normal
        //normal.x += gradientX;
        //normal.y += gradientY;
        //normal.z += gradientZ;

        // Find a vector perpendicular to the gradient (cross product with an arbitrary vector)
        vec3 tangent = normalize(vec3(gradientX, gradientY, gradientZ));
        vec3 arbitraryVec = vec3(1.0, 0.0, 0.0);  // An arbitrary vector for the cross product
        normal += normalize(cross(tangent, arbitraryVec));
    }

    // Normalize the resulting normal vector
    return normalize(normal);
}


void main() {
    vec3 displacedPos =  GerstnerWave(pos, time);
    FragPos = vec3(model * vec4(displacedPos, 1.0));
    //gl_Position = projection * view * vec4(FragPos, 1.0);
    vCol = vec4(clamp(col, 0.0, 1.0), 1.0);
    TexCoord = tex;
    Normal = norm;
}




