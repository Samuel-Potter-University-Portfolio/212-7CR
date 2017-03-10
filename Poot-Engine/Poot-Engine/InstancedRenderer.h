#pragma once
#include "API.h"
#include "Component.h"
#include "MasterRenderer.h"
#include "ModelComponentBase.h"

#include <map>
#include <vector>


class POOT_API InstancedRenderer
{
private:
	struct ShaderModel
	{
		Model* model;
		Shader* shader;

		inline bool operator==(const ShaderModel& other) const
		{
			return other.model == model && other.shader == shader;
		}
		inline bool operator<(const ShaderModel& other) const
		{
			return this < &other;
		}
	};

	std::map<ShaderModel, std::vector<ModelComponentBase*>> render_queue;
	std::vector<ModelComponentBase*> pending_loads;

	void HandlePendingLoads();
	void AddLoadedComponent(ModelComponentBase* model_comp);

public:
	void HandleNewComponent(Component* component);
	void Render(struct RenderRequest& request);
};
