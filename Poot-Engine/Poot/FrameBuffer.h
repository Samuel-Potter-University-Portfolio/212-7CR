#pragma once

#include <GL/glew.h>


class FrameBuffer
{	
private:
	const unsigned int width, height;
	const float aspect_ratio;
	const bool store_depth;
	GLuint fbo;

	GLuint texture_id;

	GLuint render_buffer;
	GLuint depth_texture_id;

	bool cleaned_up;

public:
	FrameBuffer(const unsigned int width, const unsigned int height, const bool store_depth = false);
	~FrameBuffer();

	void Bind();
	void Unbind();

	void Create();
	void CleanUp();

	inline GLuint GetTextureID() { return texture_id; }
	inline GLuint GetDepthTextureID() { return depth_texture_id; }

	inline const int GetWidth() { return width; }
	inline const int GetHeight() { return height; }
	inline const float GetAspectRatio() { return aspect_ratio; }
};

