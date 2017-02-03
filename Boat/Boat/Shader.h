#pragma once
#include <string>
#include <GL/glew.h>


class Shader
{
private:
	GLuint program_id;

protected:
	std::string debug_name;
	virtual void AttachShaders() = 0;
	virtual void CleanUp();

	static GLuint CreateShader(const GLenum shader_type, std::string source);
public:
	Shader(const std::string debug_name) : debug_name(debug_name) {};

	virtual void Start();
	virtual void Render(class CameraComponent* camera, class ModelComponentBase* component, float frame_time);
	virtual void Stop();

	virtual bool Load();
	inline GLuint GetProgramID() { return program_id; }
};

