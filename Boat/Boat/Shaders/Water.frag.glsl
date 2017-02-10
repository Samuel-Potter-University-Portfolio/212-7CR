R"( 
#version 420

in vec4 clip_position;
in vec2 pass_uv;

const float wave_strength = 0.04f;

uniform vec2 dudv_uv_offset0;
uniform vec2 dudv_uv_offset1;
layout(binding = 0) uniform sampler2D reflection_texture;
layout(binding = 1) uniform sampler2D refraction_texture;
layout(binding = 2) uniform sampler2D dudv_texture;


out vec4 out_colour;

void main()
{
	//Get projective uvs
	vec2 normal_device_space = (clip_position.xy/clip_position.w)/2.0 + 0.5;
	vec2 reflection_uvs = vec2(normal_device_space.x, -normal_device_space.y);
	vec2 refraction_uvs = vec2(normal_device_space.x, normal_device_space.y);

	//Use 2 waves to simulate current
	vec4 dudv_colour = mix(texture(dudv_texture, pass_uv + dudv_uv_offset0), texture(dudv_texture, pass_uv + dudv_uv_offset1), 0.5);
	vec2 uv_offset = (dudv_colour * 2 - 1).xy * wave_strength;	
	
	reflection_uvs += uv_offset;
	refraction_uvs += uv_offset;
	
	refraction_uvs = clamp(refraction_uvs, 0.001, 0.999);
	reflection_uvs.x = clamp(reflection_uvs.x, 0.001, 0.999);
	reflection_uvs.y = clamp(reflection_uvs.y, -0.999, -0.001);
	
	vec4 reflection_colour = texture(reflection_texture, reflection_uvs);
	vec4 refraction_colour = texture(refraction_texture, refraction_uvs);
	
	
	out_colour = mix(reflection_colour, refraction_colour, 0.5);
}

)"