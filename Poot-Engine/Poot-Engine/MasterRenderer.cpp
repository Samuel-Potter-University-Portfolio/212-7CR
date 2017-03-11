#include "MasterRenderer.h"
#include "InstancedRenderer.h"
#include "GUIRenderer.h"
#include "Logger.h"

#include "World.h"
#include "FrameBuffer.h"
#include "Game.h"


void MasterRenderer::Register(World* world) 
{
	this->world = world;
	instanced_renderer = new InstancedRenderer;
	gui_renderer = new GUIRenderer;

	//TEMP - Add default requests
	RenderRequest world_request;
	RenderRequest gui_request;
	world_request.render_mode = RenderRequest::RenderMode::World;
	gui_request.render_mode = RenderRequest::RenderMode::GUI;

	requests.push_back(world_request);
	requests.push_back(gui_request);

	LOG(Log, "Registered Master Renderer");
}

void MasterRenderer::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (RenderRequest& request : requests)
		Render(request);
}

std::string MasterRenderer::GetStatusString()
{
	return instanced_renderer->GetStatusString();
}

void MasterRenderer::HandleNewComponent(Component* component)
{
	instanced_renderer->HandleNewComponent(component);
	gui_renderer->HandleNewComponent(component);
}

void MasterRenderer::Render(RenderRequest& request)
{
	if (request.frame_buffer)
	{
		request.frame_buffer->Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}


	//If camera is null, use main camera
	if (!request.camera)
		request.camera = world->GetMainCamera();

	//Don't render, if camera is still null
	if (!request.camera)
		return;

	//Fetch desired render size
	if (request.frame_buffer)
	{
		request.frame_width = request.frame_buffer->GetWidth();
		request.frame_height = request.frame_buffer->GetHeight();
	}
	else
	{
		request.frame_width = g_game->GetWindow()->GetWidth();
		request.frame_height = g_game->GetWindow()->GetHeight();
	}

	if (request.frame_width == 0.0f)
		request.frame_width = 1.0f;
	if (request.frame_height == 0.0f)
		request.frame_height = 1.0f;

	//Check camera is up to date 
	request.camera->BuildProjectionMatrix(request.frame_width, request.frame_height);
	
	if (request.pre_render)
		request.pre_render();


	if (request.render_mode == RenderRequest::RenderMode::World)
		instanced_renderer->Render(request);
	
	if (request.render_mode == RenderRequest::RenderMode::GUI)
		gui_renderer->Render(request);
		

	if (request.post_render)
		request.post_render();


	if (request.frame_buffer)
		request.frame_buffer->Unbind();
}

void MasterRenderer::CleanUp() 
{
	delete instanced_renderer;
	delete gui_renderer;
	LOG(Log, "Cleaned up Master Renderer");
}