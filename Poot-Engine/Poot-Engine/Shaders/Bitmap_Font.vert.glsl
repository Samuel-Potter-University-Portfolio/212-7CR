R"( 
#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_uv;

uniform mat3 model_matrix;
uniform vec2 frame_size;
uniform vec2 anchor;

uniform vec2 letter_location;

out vec2 pass_uvs;


void main()
{	
	vec2 clip_step = 1.0 / frame_size;

	vec3 clip_position = model_matrix * vec3(in_position.xy + letter_location, 1);
	clip_position.xy *= clip_step;
	clip_position.xy += anchor;
	gl_Position = vec4(clip_position, 1.0);
	
	pass_uvs = in_uv;
}

)"