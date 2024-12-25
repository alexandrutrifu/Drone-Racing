#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 3) in vec3 v_normal;
layout(location = 2) in vec2 texture_coordinate;
layout(location = 1) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Output
out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_texture;
out vec3 frag_color;
out float frag_noise;

// Noise function
float noise(vec2 pos) {
    return fract(sin(dot(pos.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
    // Pass attributes to the fragment shader
    frag_normal = v_normal;
    frag_texture = texture_coordinate;
    frag_color = v_color;

    // Apply smoother noise to the y-coordinate for height variation
    vec3 pos = v_position;
    float frequency = 0.01; // Lower frequency for wider curves
    float amplitude = 0.3; // Smaller amplitude for smoother terrain

    float noiseValue = noise(v_position.xz * frequency);

    pos.y += noiseValue * amplitude;

    // Send updated position to fragment shader
    frag_position = pos;
    frag_noise = noiseValue;

    // Compute final position
    gl_Position = Projection * View * Model * vec4(pos, 1.0);
}