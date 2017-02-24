R"( 
#version 330

in vec2 pass_uvs;

uniform sampler2D texture0;

uniform vec3 colour;
uniform int char_index;

out vec4 out_colour;

vec2 GetUVs()
{
	const float unit = 1.0/16.0;

	vec2 local_uvs = pass_uvs * unit;

	vec2 char_uvs = vec2(
		16 + mod(char_index, 16),
		15 - int(char_index)/16
	) * unit;

	return char_uvs + local_uvs;
}


void main()
{
	vec4 letter_colour = texture(texture0, GetUVs());

	if(letter_colour.r == 0)
		discard;

	out_colour = vec4(colour, 1.0);
}

)"