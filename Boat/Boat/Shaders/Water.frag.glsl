R"( 
#version 420


in vec4 clip_position;

layout(binding = 0) uniform sampler2D reflection_texture;
layout(binding = 1) uniform sampler2D refraction_texture;

out vec4 out_colour;

void main()
{
	vec2 normal_device_space = (clip_position.xy/clip_position.w)/2.0 + 0.5;
	vec2 reflection_uvs = vec2(normal_device_space.x, -normal_device_space.y);
	vec2 refraction_uvs = normal_device_space;

	vec4 reflection_colour = texture(reflection_texture, reflection_uvs);
	vec4 refraction_colour = texture(refraction_texture, refraction_uvs);
	
	out_colour = mix(reflection_colour, refraction_colour, 0.5);
}

)"