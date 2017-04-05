R"( 
#version 420 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;


out vec2 pass_uvs;
out vec3 pass_to_camera;
out vec3 pass_normal;


void main()
{	
	vec4 world_position = model_matrix * vec4(in_position, 1.0);
	gl_Position = projection_matrix * view_matrix * world_position;

	vec3 camera_position = (inverse(view_matrix) * vec4(0,0,0,1)).xyz;

	pass_uvs = in_uv;
	pass_to_camera = camera_position - world_position.xyz;
	pass_normal = (model_matrix * vec4(in_normal, 0.0)).rgb;
}

)"