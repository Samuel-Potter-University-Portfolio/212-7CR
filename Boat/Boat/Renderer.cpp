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

void Renderer::Render(Window* window)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	CameraComponent* main_camera = g_game->GetWorld()->GetMainCamera();
	float lerp_time = g_game->GetGameLogic()->GetNormalizedTickTime();


	//Check camera is up to date 
	if (main_camera && main_camera->GetAspectRatio() != window->GetAspectRatio())
		main_camera->BuildProjectionMatrix(window);


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
			if (!comp || !comp->IsVisable())
				continue;

			Shader* shader = comp->GetShader();

			if (!shader)
				continue;

			shader->Start();
			shader->Render(main_camera, comp, lerp_time);
			shader->Stop();
		}

		glBindVertexArray(0);
	}
}