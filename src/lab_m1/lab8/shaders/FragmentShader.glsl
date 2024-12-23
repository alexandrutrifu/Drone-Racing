#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;

uniform vec3 second_light_direction;
uniform vec3 second_light_position;

uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;

// Other uniforms
uniform int spotlight;
uniform float cutoff_angle;

vec3 world_normal2;

// Output
layout(location = 0) out vec4 out_color;

vec3 point_light_contribution(vec3 light_pos, vec3 light_dir) {
    float ambient_light = 0.25;

    vec3 L = normalize(light_pos - world_position);
    vec3 V = normalize(eye_position - world_position);
    vec3 R = reflect(-L, world_normal2);

    float ambient = material_kd * ambient_light;

    float diffuse_light = material_kd * max(dot(world_normal2, L), 0);

    float specular_light = 0.f;
    if (diffuse_light > 0) {
        specular_light = material_ks * pow(max(dot(V, R), 0), material_shininess);
    }

    float light = 0.f;
    if (spotlight == 1) {
        float cutoff_rad = radians(cutoff_angle);
        float spot_light = dot(-L, light_dir);

        if (spot_light > cos(cutoff_rad)) {
            float d = distance(light_pos, world_position);
            float attenuation = 1.0 / (d * d + 1);

            float linear_att = (spot_light - cos(cutoff_rad)) / (1.0f - cos(cutoff_rad));
            float light_att_factor = pow(linear_att, 2);

            light = ambient + attenuation * light_att_factor * (diffuse_light + specular_light);
        } else {
            light = ambient;
        }
    } else {
        float d = distance(light_pos, world_position);
        float attenuation = 1.0 / (d * d + 1);

        light = ambient + attenuation * (diffuse_light + specular_light);
    }

    return vec3(light);
}

void main() {
    world_normal2 = normalize(world_normal);
    // Compute contributions from both light sources
    vec3 light1_contribution = point_light_contribution(light_position, light_direction);
    vec3 light2_contribution = point_light_contribution(second_light_position, second_light_direction);

    // Combine the contributions from both lights
    vec3 total_light = light1_contribution + light2_contribution;

    // Write pixel out color
    out_color = vec4(object_color * total_light, 1.f);
}