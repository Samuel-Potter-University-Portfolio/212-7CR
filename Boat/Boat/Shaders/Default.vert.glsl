R"( 
#version 330 core

layout(location = 0) in vec3 position;

out vec4 pass_colour;

void main()
{	
	gl_Position = vec4(position, 1.0);
	pass_colour = vec4(position, 1.0);
}

)"