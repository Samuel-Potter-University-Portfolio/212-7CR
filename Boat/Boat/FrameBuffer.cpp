#include "FrameBuffer.h"
#include "Game.h"
#include "Logger.h"


FrameBuffer::FrameBuffer(const unsigned int width, const unsigned int height, const bool store_depth)
	: width(width), height(height), aspect_ratio((float)width/(float)height), store_depth(store_depth)
{
}


FrameBuffer::~FrameBuffer()
{
	CleanUp();
}

void FrameBuffer::CleanUp()
{
	if (!cleaned_up)
	{
		glDeleteFramebuffers(1, &fbo);
		glDeleteTextures(1, &texture_id);
		cleaned_up = true;
	}
}

void FrameBuffer::Create()
{
	glEnable(GL_TEXTURE_2D);

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	//Colour texture0
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, width, height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture_id, 0);
	
	if (!store_depth)
	{
		//Depth
		glGenRenderbuffers(1, &render_buffer);
		glBindRenderbuffer(GL_RENDERBUFFER, render_buffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, render_buffer);
	}
	else
	{
		//Depth texture
		glGenTextures(1, &depth_texture_id);
		glBindTexture(GL_TEXTURE_2D, depth_texture_id);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32, width, height);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_texture_id, 0);
	}
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		LOG(Error, "Could not completely setup framebuffer %i", glCheckFramebufferStatus(GL_FRAMEBUFFER));

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);	
}

void FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glViewport(0, 0, width, height);

	GLenum draw_buffers[1]{ GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, draw_buffers);
}

void FrameBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Window* window = g_game ? g_game->GetWindow() : nullptr;

	if(window)
		glViewport(0, 0, window->GetWidth(), window->GetHeight());
}
