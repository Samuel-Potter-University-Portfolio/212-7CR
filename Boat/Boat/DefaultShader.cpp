#include "DefaultShader.h"
#include "Logger.h"

const char* vert_source =
#include "Shaders/Default.vert.glsl"
;
const char* frag_source =
#include "Shaders/Default.frag.glsl"
;


void DefaultShader::AttachShaders() 
{
	glAttachShader(GetProgramID(), CreateShader(GL_VERTEX_SHADER, vert_source));
	glAttachShader(GetProgramID(), CreateShader(GL_FRAGMENT_SHADER, frag_source));
}
