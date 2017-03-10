#include "MasterRenderer.h"
#include "InstancedRenderer.h"
#include "GUIRenderer.h"

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
}

void MasterRenderer::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (RenderRequest& request : requests)
		Render(request);
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
	float width = 1;
	float height = 1;

	if (!request.camera)
		request.camera = world->GetMainCamera();

	//Don't render, if camera is still null
	if (!request.camera)
		return;

	//Fetch desired render size
	if (request.frame_buffer)
	{
		width = request.frame_buffer->GetWidth();
		height = request.frame_buffer->GetHeight();
	}
	else
	{
		width = g_game->GetWindow()->GetWidth();
		height = g_game->GetWindow()->GetHeight();
	}

	//Check camera is up to date 
	request.camera->BuildProjectionMatrix(height != 0.0f ? width/height : 1.0f);
	
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
}