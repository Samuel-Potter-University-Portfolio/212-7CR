#pragma once

#include <GL/glew.h>


class FrameBuffer
{	
private:
	const unsigned int width, height;
	const float aspect_ratio;
	GLuint fbo;
	GLuint render_buffer;

	GLuint texture_id;

	bool cleaned_up;

public:
	FrameBuffer(const unsigned int width, const unsigned int height);
	~FrameBuffer();

	void Bind();
	void Unbind();

	void Create();
	void CleanUp();

	inline GLuint GetTextureID() { return texture_id; }

	inline const int GetWidth() { return width; }
	inline const int GetHeight() { return height; }
	inline const float GetAspectRatio() { return aspect_ratio; }
};

