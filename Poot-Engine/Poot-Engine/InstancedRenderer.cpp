#include "InstancedRenderer.h"
#include "ModelComponentBase.h"
#include "Game.h"

#include <sstream>


void InstancedRenderer::HandleNewComponent(Component* component)
{
	ModelComponentBase* model_comp = Cast<ModelComponentBase>(component);

	if (model_comp)
		pending_loads.push_back(model_comp);
}

std::string InstancedRenderer::GetStatusString()
{
	std::stringstream message;
	message << "Pending: " << pending_loads.size() << '\n';
	message << "Batches: " << render_queue.size() << '\n';

	unsigned int total = 0;

	for (auto it = render_queue.begin(); it != render_queue.end(); ++it)
		total += it->second.size();

	message << "Saved: " << (total - render_queue.size());
	return message.str();
}

void InstancedRenderer::HandlePendingLoads() 
{
	if (pending_loads.size() == 0)
		return;

	for (int i = 0; i < pending_loads.size(); i++)
	{
		ModelComponentBase* model_comp = pending_loads[i];

		//Model and Shader has now loaded
		if (model_comp->GetModel() && model_comp->GetShader())
		{
			AddLoadedComponent(model_comp);
			pending_loads.erase(pending_loads.begin() + i);
			i--;
		}
	}
}

void InstancedRenderer::AddLoadedComponent(ModelComponentBase* model_comp)
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

void InstancedRenderer::Render(RenderRequest& request) 
{
	HandlePendingLoads();

	//Check for shader override
	bool is_shader_overridden = false;
	Shader* current_shader = nullptr;

	if (request.shader_override)
		is_shader_overridden = request.shader_override_tags != OBJ_TAG_NONE;


	//Instanced rendering
	for (auto it = render_queue.begin(); it != render_queue.end(); ++it)
	{
		const ShaderModel model_meta = it->first;
		std::vector<ModelComponentBase*>& components = it->second;

		//Bind model and shader
		glBindVertexArray(model_meta.model->GetVAO());

		if (!is_shader_overridden || true)
			current_shader = model_meta.shader;
		else
			current_shader = request.shader_override;
		current_shader->Start();


		//Render all components of that model + shader pair
		for (ModelComponentBase* comp : components)
		{
			//Don't render if invisible
			if (!comp || !comp->IsVisable())
				continue;

			//Only render if tags are valid
			if (request.whitelist != OBJ_TAG_ALL && !comp->HasTag(request.whitelist))
				continue;

			if (request.blacklist != OBJ_TAG_NONE && comp->HasTag(request.blacklist))
				continue;

			//If should be overridden when active
			if (is_shader_overridden)
			{
				const bool use_override = comp->HasTag(request.shader_override_tags);

				//Make sure current shader is shader override
				if (use_override && current_shader != request.shader_override)
				{
					current_shader = request.shader_override;
					current_shader->Start();
				}

				//Make sure current shader is default shader
				else if (!use_override && current_shader != model_meta.shader)
				{
					current_shader = model_meta.shader;
					current_shader->Start();
				}
			}

			current_shader->Render(request, comp);
		}

		glBindVertexArray(0);
	}

	if(current_shader)
		current_shader->Stop();
}
