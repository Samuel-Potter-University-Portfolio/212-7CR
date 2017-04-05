#pragma once
#include "API.h"
#include "Component.h"
#include "MasterRenderer.h"
#include "ModelComponentBase.h"

#include <map>
#include <vector>
#include <string>


class POOT_API InstancedRenderer
{
private:
	struct ShaderModel
	{
		Model* model;
		Shader* shader;

		inline int GetID() const
		{
			return model->GetVAO() * 1000000 + shader->GetProgramID();
		}

		inline bool operator==(const ShaderModel& other) const
		{
			return other.model == model && other.shader == shader;
		}
		inline bool operator<(const ShaderModel& other) const
		{
			return GetID() < other.GetID();
		}
	};

	std::map<ShaderModel, std::vector<ModelComponentBase*>> render_queue;
	std::vector<ModelComponentBase*> pending_loads;

	void HandlePendingLoads();
	void AddLoadedComponent(ModelComponentBase* model_comp);

public:
	void HandleNewComponent(Component* component);
	void Render(struct RenderRequest& request);

	std::string GetStatusString();
};

