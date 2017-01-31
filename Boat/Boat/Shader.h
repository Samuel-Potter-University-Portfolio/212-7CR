#pragma once
#include <string>
#include <GL/glew.h>


class Shader
{
private:
	GLuint program_id;

protected:
	virtual void AttachShaders() = 0;
	virtual void CleanUp();

	virtual void Start();
	virtual void Stop();

	static GLuint CreateShader(const GLenum shader_type, std::string source);
public:

	virtual bool Load();
	inline GLuint GetProgramID() { return program_id; }
};

