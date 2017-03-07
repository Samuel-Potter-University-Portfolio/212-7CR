#include "Renderer.h"
#include "Game.h"
#include "ModelComponentBase.h"
#include "FrameBuffer.h"
#include "TextComponent.h"
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
		if (!model_comp->GetModel() || !model_comp->GetShader())
		{
			pending_loads.push_back(model_comp);
			continue;
		}

		AddComponentToQueue(model_comp);
	}
}

void Renderer::AddComponentToQueue(ModelComponentBase* model_comp) 
{
	ShaderModel model_meta;
	model_meta.model = model_comp->GetModel();
	model_meta.shader = model_comp->GetShader();

	//Doesn't contain queue so make one
	if (render_queue.find(model_meta) == render_queue.end())
		render_queue[model_meta] = { model_comp };

	//Add to existing queue
	else
		render_queue[model_meta].push_back(model_comp);
}

void Renderer::FullRender()
{
	tick_time = g_game->GetGameLogic()->GetNormalizedTickTime();

	for (RenderSettings& render_settings : render_targets)
		Render(render_settings);

	render_targets.clear();
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

	if (!camera)
		camera = g_game->GetWorld()->GetMainCamera();

	//Don't render, if camera is still null
	if (!camera)
		return;

	//Check camera is up to date 
	camera->BuildProjectionMatrix(render_settings.aspect_ratio);


	//Check components to see if they have loaded
	for (int i = 0; i < pending_loads.size(); i++)
	{
		//Model and Shader has now loaded
		if (pending_loads[i]->GetModel() && pending_loads[i]->GetShader())
		{
			AddComponentToQueue(pending_loads[i]);
			pending_loads.erase(pending_loads.begin() + i);
			i--;
		}
	}


	//Check for shader override
	bool is_shader_overridden = false;

	if (render_settings.shader_override)
		is_shader_overridden = render_settings.shader_override_tags != E_TAG_NONE;


	//Instanced rendering
	for (auto it = render_queue.begin(); it != render_queue.end(); ++it)
	{
		const ShaderModel model_meta = it->first;
		std::vector<ModelComponentBase*>& components = it->second;

		//Bind model and shader
		glBindVertexArray(model_meta.model->GetVAO());
		Shader* current_shader;

		if (!is_shader_overridden || true)
			current_shader = model_meta.shader;
		else
			current_shader = render_settings.shader_override;
		current_shader->Start();


		//Render all components of that model + shader pair
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

			//TODO - Remove; TEMP don't render text to overriden shader
			if (is_shader_overridden)
			{
				TextComponent* text = dynamic_cast<TextComponent*>(comp);
				if(text)
					continue;
			}

			//If should be overridden when active
			if (is_shader_overridden)
			{
				const bool use_override = (tags & render_settings.shader_override_tags);

				//Make sure current shader is shader override
				if (use_override && current_shader != render_settings.shader_override)
				{
					current_shader = render_settings.shader_override;
					current_shader->Start();
				}

				//Make sure current shader is default shader
				else if (!use_override && current_shader != model_meta.shader)
				{
					current_shader = model_meta.shader;
					current_shader->Start();
				}
			}

			current_shader->Render(camera, comp, tick_time);
		}

		current_shader->Stop();
		glBindVertexArray(0);
	}

	if (render_settings.frame_buffer)
		render_settings.frame_buffer->Unbind();
}