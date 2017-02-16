R"( 
#version 330

in vec2 pass_uvs;

uniform sampler2D texture0;

out vec4 out_colour;

void main()
{
	out_colour = texture(texture0, pass_uvs);

	if(out_colour.w < 1.0f)
		discard;
}

)"