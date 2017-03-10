#pragma once
#include "API.h"
#include "MasterRenderer.h"
#include <string>
#include <GL/glew.h>


class POOT_API Shader
{
private:
	GLuint program_id;

protected:
	virtual void AttachShaders() = 0;

	static GLuint CreateShader(const GLenum shader_type, std::string source);
public:
	virtual void CleanUp();

	virtual void Start();
	virtual void AmbiguousRender(const RenderRequest& request, class Component* component, float frame_time);
	virtual void Render(const RenderRequest& request, class ModelComponentBase* component, float frame_time);
	virtual void Stop();

	virtual bool Load();
	inline GLuint GetProgramID() { return program_id; }
};

