R"( 
#version 330

in vec4 pass_colour;

out vec4 out_colour;

void main()
{
	out_colour = pass_colour;
}

)"