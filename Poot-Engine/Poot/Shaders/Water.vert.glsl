R"( 
#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform vec3 camera_location;


out vec4 clip_position;
out vec3 to_camera;
out vec3 pass_normal;
out vec2 pass_uv;


void main()
{	
	vec4 world_position = model_matrix * vec4(in_position, 1.0);
	clip_position = projection_matrix * view_matrix * world_position;

	gl_Position = clip_position;
	pass_uv = in_uv;
	pass_normal = in_normal;
	to_camera = camera_location - world_position.xyz;
}

)"