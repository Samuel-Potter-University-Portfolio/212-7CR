R"( 
#version 420

in vec4 clip_position;
in vec3 to_camera;
in vec3 pass_normal;
in vec2 pass_uv;

const float wave_strength = 0.04f;
const float reflectivity = 2;

uniform float camera_near_plane;
uniform float camera_far_plane;

uniform vec2 dudv_uv_offset0;
uniform vec2 dudv_uv_offset1;
layout(binding = 0) uniform sampler2D reflection_texture;
layout(binding = 1) uniform sampler2D refraction_texture;
layout(binding = 2) uniform sampler2D depth_texture;
layout(binding = 3) uniform sampler2D dudv_texture;


out vec4 out_colour;


vec2 GetWaveOffset()
{
	//Use 2 waves to simulate current
	vec4 dudv_colour = mix(texture(dudv_texture, pass_uv + dudv_uv_offset0), texture(dudv_texture, pass_uv + dudv_uv_offset1), 0.5);
	return (dudv_colour * 2 - 1).xy * wave_strength;	
}

float FromDepthToDistance(float value)
{
	//Convert from exp storage to linear
	return 2.0 * camera_near_plane * camera_far_plane / (camera_far_plane + camera_near_plane - (2.0 * value - 1.0) * (camera_far_plane - camera_near_plane));
}

void main()
{
	//Get projective uvs
	vec2 normal_device_space = (clip_position.xy/clip_position.w)/2.0 + 0.5;
	vec2 reflection_uvs = vec2(normal_device_space.x, -normal_device_space.y);
	vec2 refraction_uvs = vec2(normal_device_space.x, normal_device_space.y);

	float under_water_distance = FromDepthToDistance(texture(depth_texture, refraction_uvs).x);
	float water_distance = FromDepthToDistance(gl_FragCoord.z);
	float water_depth = under_water_distance - water_distance;
	
	vec2 uv_offset = GetWaveOffset();
	reflection_uvs += uv_offset;
	refraction_uvs += uv_offset;
	
	//Prevent texture from loop at top and bottom
	refraction_uvs = clamp(refraction_uvs, 0.001, 0.999);
	reflection_uvs.x = clamp(reflection_uvs.x, 0.001, 0.999);
	reflection_uvs.y = clamp(reflection_uvs.y, -0.999, -0.001);
	
	vec4 reflection_colour = texture(reflection_texture, reflection_uvs);
	vec4 refraction_colour = texture(refraction_texture, refraction_uvs);
	
	
	//Fresnel effect
	float refraction_factor = dot(normalize(to_camera), normalize(pass_normal));
	refraction_factor = pow(refraction_factor, reflectivity);
	
	vec4 water_colour = mix(reflection_colour, refraction_colour, refraction_factor);
	
	//Fade between refraction texture to give soft edges
	out_colour = mix(texture(refraction_texture, normal_device_space), water_colour, clamp(water_depth/1.5, 0.0, 1.0));
	//out_colour.a = water_depth; //Alt. fade between background
}

)"