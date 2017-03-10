R"( 
#version 330

in vec2 pass_uvs;
in vec3 pass_to_camera;
in vec3 pass_normal;

uniform sampler2D texture0;
uniform float shininess;
uniform float roughness;

const float minimum_brightness = 0.2;
uniform vec3 sun_direction;
uniform vec3 sun_colour;


out vec4 out_colour;


void main()
{
	out_colour = vec4(1,1,1,1);
}

)"