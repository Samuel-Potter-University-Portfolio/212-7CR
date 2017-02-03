R"( 
#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;


out vec4 pass_colour;


void main()
{	
	vec4 world_position = model_matrix * vec4(in_position, 1.0);
	gl_Position = projection_matrix * view_matrix * world_position;

	pass_colour = vec4(in_normal, 1.0);
}

)"