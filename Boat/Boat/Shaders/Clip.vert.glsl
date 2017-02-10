R"( 
#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;


uniform vec4 cliping_plane;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;


out vec2 pass_uvs;
out vec3 pass_normal;


void main()
{	
	vec4 world_position = model_matrix * vec4(in_position, 1.0);
	gl_Position = projection_matrix * view_matrix * world_position;
	gl_ClipDistance[0] = dot(world_position, cliping_plane); 
		
	pass_uvs = in_uv;
	pass_normal = in_normal;
}

)"