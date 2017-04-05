R"( 
#version 330

in vec3 pass_uvs;

uniform samplerCube cube_map;

out vec4 out_colour;

void main()
{
	vec3 tex_colour = texture(cube_map, pass_uvs).rgb;
	out_colour = vec4(tex_colour, 1.0);
}

)"