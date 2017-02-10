#include "Renderer.h"
#include "Game.h"
#include "ModelComponentBase.h"
#include "FrameBuffer.h"
#include "Logger.h"

Renderer::Renderer()
{
	LOG(Log, "Instanced Renderer constructed");
}


Renderer::~Renderer()
{
	LOG(Log, "Instanced Renderer destroyed");
}

void Renderer::AddEntityToQueue(Entity* entity) 
{
	std::vector<ModelComponentBase*> model_comps = entity->GetAllComponents<ModelComponentBase>();
	
	for (ModelComponentBase* model_comp: model_comps)
	{
		//Ignore if invalid component
		if (!model_comp)
			continue;

		//Temporarily store component, if waiting on model load
		if (!model_comp->GetModel())
		{
			pending_loads.push_back(model_comp);
			continue;
		}

		AddComponentToQueue(model_comp);
	}
}

void Renderer::AddComponentToQueue(ModelComponentBase* model_comp) 
{
	//Doesn't contain queue so make one
	if (render_queue.find(model_comp->GetModel()) == render_queue.end())
		render_queue[model_comp->GetModel()] = { model_comp };

	//Add to existing queue
	else
		render_queue[model_comp->GetModel()].push_back(model_comp);
}

void Renderer::Update() 
{
	tick_time = g_game->GetGameLogic()->GetNormalizedTickTime();
}

void Renderer::CleanUp() 
{
}

void Renderer::Render(RenderSettings& render_settings)
{
	if (render_settings.frame_buffer)
		render_settings.frame_buffer->Bind();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//If camera is null, use main camera
	CameraComponent* camera = render_settings.camera;

	if(!camera)
		camera = g_game->GetWorld()->GetMainCamera();

	//Don't render, if camera is still null
	if (!camera)
		return;

	//Check camera is up to date 
	camera->BuildProjectionMatrix(render_settings.aspect_ratio);

	//Check components to see if they have loaded
	for (int i = 0; i < pending_loads.size(); i++)
	{
		//Model has now loaded
		if (pending_loads[i]->GetModel())
		{
			AddComponentToQueue(pending_loads[i]);
			pending_loads.erase(pending_loads.begin() + i);
			i--;
		}
	}

	//Check for shader override
	bool is_shader_overridden = false;
	
	if (render_settings.shader_override)
	{
		render_settings.shader_override->Start();
		is_shader_overridden = true;
	}


	//Instanced rendering
	for (auto it = render_queue.begin(); it != render_queue.end(); ++it)
	{
		Model* model = it->first;
		std::vector<ModelComponentBase*>& components = it->second;

		glBindVertexArray(model->GetVAO());


		//Render all components of that model
		for (ModelComponentBase* comp : components)
		{
			//Don't render if invisible
			if (!comp || !comp->IsVisable())
				continue;

			//Only render if tags are valid
			Entity* parent = comp->GetParent();
			Tags tags = parent ? parent->GetTags() : E_TAG_NONE;

			if (render_settings.whitelist != E_TAG_ALL && !(tags & render_settings.whitelist))
				continue;

			if (render_settings.blacklist != E_TAG_NONE && (tags & render_settings.blacklist))
				continue;


			//Render to override shader
			if(is_shader_overridden)
				render_settings.shader_override->Render(camera, comp, tick_time);


			//Render to component shader
			else
			{
				//Load shader and render, if valid
				Shader* shader = comp->GetShader();

				if (!shader)
					continue;

				shader->Start();
				shader->Render(camera, comp, tick_time);
				shader->Stop();
			}
		}

		glBindVertexArray(0);
	}

	if (is_shader_overridden)
		render_settings.shader_override->Stop();

	if (render_settings.frame_buffer)
		render_settings.frame_buffer->Unbind();
}