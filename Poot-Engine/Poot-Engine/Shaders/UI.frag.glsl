R"( 
#version 330

in vec2 pass_uvs;

uniform vec4 colour;

uniform sampler2D texture0;

out vec4 out_colour;


void main()
{
	out_colour = texture(texture0, pass_uvs);
	out_colour *= colour;
}

)"