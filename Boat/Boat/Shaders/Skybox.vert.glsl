R"( 
#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;

uniform vec3 camera_position;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;


out vec3 pass_uvs;


void main()
{	
	vec4 world_position = vec4(in_position + camera_position, 1.0);
	gl_Position = projection_matrix * view_matrix * world_position;

	pass_uvs = -in_position;
}

)"