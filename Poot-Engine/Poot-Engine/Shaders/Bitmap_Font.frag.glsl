R"( 
#version 330

in vec2 pass_uvs;


uniform vec4 colour;
uniform sampler2D texture0;
uniform int current_char;


out vec4 out_colour;


vec2 GetCharacterUVs()
{
	const float unit = 1.0/16.0;
	vec2 local_uvs = pass_uvs * unit;

	vec2 char_uvs = vec2(
		16 + mod(current_char, 16),
		15 - int(current_char)/16
	) * unit;

	return char_uvs + local_uvs;
}


void main()
{
	vec4 char_colour = texture(texture0, GetCharacterUVs());

	out_colour = char_colour;
	out_colour.a = char_colour.x;
	out_colour *= colour;

	if(out_colour.a < 0.3)
		discard;
}

)"