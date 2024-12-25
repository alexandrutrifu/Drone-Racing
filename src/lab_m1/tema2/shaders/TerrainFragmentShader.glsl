#version 330

// Input
// TODO(student): Get values from vertex shader
in vec3 frag_position;
in vec3 frag_normal;
in vec2 frag_texture;
in vec3 frag_color;
in float frag_noise;

// Output
layout(location = 0) out vec4 out_color;
layout(location = 1) out vec4 out_normal;
layout(location = 2) out vec3 out_texture;


void main()
{
    vec3 color = mix(vec3(0.078, 0.259, 0.247), frag_color, frag_noise);

    out_color	= vec4(color, 1.0);
	out_normal	= vec4(frag_normal, 1.0);
	out_texture = vec3(frag_texture, 1.0);
}
