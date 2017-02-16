R"( 
#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_uv;

uniform vec3 anchor;
uniform vec3 scale;
uniform vec2 resolution;


out vec2 pass_uvs;


void main()
{	
	vec3 position = in_position * scale;
	position.x /= resolution.x;
	position.y /= resolution.y;

	position *= resolution.x / 800;

	gl_Position = vec4(position, 1.0);

	pass_uvs = in_uv;
}

)"