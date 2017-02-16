R"( 
#version 330

in vec2 pass_uvs;
in vec3 pass_normal;

uniform sampler2D texture0;


const float minimum_brightness = 0.5;
uniform vec3 sun_direction;
uniform vec3 sun_colour;


out vec4 out_colour;


vec3 GetLighting()
{
	//Surface may not be normalized, but sun direction already is
	vec3 surface_normal = normalize(pass_normal);

	float brightness = max(minimum_brightness, dot(surface_normal, -sun_direction));
	return brightness * sun_colour;
}


void main()
{
	vec3 texture_colour = texture(texture0, pass_uvs).rgb;
	out_colour = vec4(texture_colour * GetLighting(), 1.0);
}

)"