#include "Shader.h"
#include "Logger.h"
#include "CameraComponent.h"
#include "ModelComponentBase.h"

void Shader::Start() 
{
	glUseProgram(program_id);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
}

void Shader::Stop() 
{
	glDisable(GL_CULL_FACE);
	glUseProgram(0);
}

void Shader::AmbiguousRender(const RenderRequest& request, class Component* component)
{
	//Too vague, so leave implementation to children
}

void Shader::Render(const RenderRequest& request, ModelComponentBase* component)
{
	//Assume model has already been bound
	glDrawElements(GL_TRIANGLES, component->GetModel()->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
}

void Shader::CleanUp() 
{
	glDeleteProgram(program_id);
}

bool Shader::Load()
{
	program_id = glCreateProgram();
	AttachShaders();

	int link_result;
	glLinkProgram(program_id);
	glGetProgramiv(program_id, GL_LINK_STATUS, &link_result);

	if (link_result == GL_FALSE)
	{
		int log_length = 0;
		glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);

		char* log = new char[log_length];
		glGetProgramInfoLog(program_id, log_length, nullptr, log);


		LOG(Error, "Error when linking shader:\n\t-shader_type: %s\n\t-error_no: %i\n==Log==\n%s",
			"SHADER_PROGRAM",
			link_result,
			log
			);

		delete[] log;
		return false;
	}

	return true;
}

GLuint Shader::CreateShader(const GLenum shader_type, std::string source)
{
	GLuint shader_id = glCreateShader(shader_type);
	const int source_size = source.size();

	const GLchar* str_ptr = source.c_str();
	glShaderSource(shader_id, 1, &str_ptr, &source_size);
	glCompileShader(shader_id);

	//Check compilation
	int compile_result;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_result);

	if (compile_result == GL_FALSE)
	{
		//Log compilation error
		int log_length = 0;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);

		char* log = new char[log_length];
		glGetShaderInfoLog(shader_id, log_length, nullptr, log);

		std::string shader_type_str =
			shader_type == GL_FRAGMENT_SHADER ? "GL_FRAGMENT_SHADER" :
			shader_type == GL_VERTEX_SHADER ? "GL_VERTEX_SHADER" :
			shader_type == GL_GEOMETRY_SHADER ? "GL_GEOMETRY_SHADER" :
			shader_type == GL_COMPUTE_SHADER ? "GL_COMPUTE_SHADER" :
			shader_type == GL_TESS_CONTROL_SHADER ? "GL_TESS_CONTROL_SHADER " :
			shader_type == GL_TESS_EVALUATION_SHADER ? "GL_TESS_EVALUATION_SHADER " :
			"Unlisted";

		LOG(Error, "Error when creating shader:\n\t-shader_type: %s\n\t-error_no: %i\n==Log==\n%s",
			shader_type_str.c_str(),
			compile_result,
			log
			);

		delete[] log;
	}

	return shader_id;
}
