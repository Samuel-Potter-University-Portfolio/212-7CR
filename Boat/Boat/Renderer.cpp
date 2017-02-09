#include "Renderer.h"
#include "Game.h"
#include "ModelComponentBase.h"
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

void Renderer::CleanUp() 
{
}

void Renderer::Render(float aspect_ratio, CameraComponent* camera, Tags whitelist_tags, Tags blacklist_tags)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//If camera is null, use main camera
	if(!camera)
		camera = g_game->GetWorld()->GetMainCamera();

	float lerp_time = g_game->GetGameLogic()->GetNormalizedTickTime();

	//Check camera is up to date 
	camera->BuildProjectionMatrix(aspect_ratio);

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

			if (whitelist_tags != E_TAG_ALL && !(tags & whitelist_tags))
				continue;

			if (blacklist_tags != E_TAG_NONE && (tags & blacklist_tags))
				continue;


			//Load shader and render, if valid
			Shader* shader = comp->GetShader();

			if (!shader)
				continue;

			shader->Start();
			shader->Render(camera, comp, lerp_time);
			shader->Stop();
		}

		glBindVertexArray(0);
	}
}