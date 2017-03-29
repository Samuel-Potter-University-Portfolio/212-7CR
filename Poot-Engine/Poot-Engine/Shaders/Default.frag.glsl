R"( 
#version 420

in vec2 pass_uvs;
in vec3 pass_to_camera;
in vec3 pass_normal;

layout(binding = 0) uniform sampler2D texture0;
layout(binding = 1) uniform sampler2D phong_map;
uniform float shininess;
uniform float roughness;
uniform int using_phong_map;

const float minimum_brightness = 0.2;
uniform vec3 sun_direction;
uniform vec3 sun_colour;


out vec4 out_colour;


vec3 GetDiffuseLighting()
{
	//Surface may not be normalized, but sun direction already is
	vec3 surface_normal = normalize(pass_normal);
	float brightness = max(minimum_brightness, dot(surface_normal, -sun_direction));
	return brightness * sun_colour;
}

vec3 GetSpecularLighting()
{
	if(shininess == 0 || 1.0 - roughness == 0)
		return vec3(0);

	vec3 surface_normal = normalize(pass_normal);
	vec3 to_camera = normalize(pass_to_camera);

	vec3 reflection_light = reflect(sun_direction, surface_normal);
	float specular_factor = max(0.0, dot(reflection_light, to_camera));

	
	float rough_factor = 1.0f;
	float shiny_factor = 1.0f;

	if(using_phong_map != 0)
	{
		vec3 phong_map_colour = texture(phong_map, pass_uvs).rgb;
		rough_factor = phong_map_colour.r;
		shiny_factor = phong_map_colour.g;
	}

	return pow(specular_factor, shininess * shiny_factor) * sun_colour * (1.0 - roughness * rough_factor);
}


void main()
{
	vec3 texture_colour = texture(texture0, pass_uvs).rgb;
	
	out_colour.rgb = texture_colour * GetDiffuseLighting() + GetSpecularLighting();
	out_colour.w = 1.0;
}

)"