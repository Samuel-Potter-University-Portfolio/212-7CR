R"( 
#version 330

in vec2 pass_uvs;

uniform sampler2D texture0;

out vec4 out_colour;

void main()
{
	vec3 tex_colour = texture(texture0, pass_uvs).rgb;
	out_colour = vec4(tex_colour, 1.0);
}

)"