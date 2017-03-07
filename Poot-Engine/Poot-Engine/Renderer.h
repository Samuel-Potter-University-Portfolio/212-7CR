#pragma once
#include "API.h"
#include "Model.h"
#include "ModelComponentBase.h"

#include <map>
#include <vector>


struct POOT_API RenderSettings
{
	float aspect_ratio = 1.0f;
	class CameraComponent* camera = nullptr;

	Tag whitelist = OBJ_TAG_ALL;
	Tag blacklist = OBJ_TAG_NONE;

	class FrameBuffer* frame_buffer = nullptr;

	Tag shader_override_tags = OBJ_TAG_ALL;
	class Shader* shader_override = nullptr;
};

struct POOT_API ShaderModel
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

class POOT_API Renderer
{
private:
	std::map<ShaderModel, std::vector<ModelComponentBase*>> render_queue;
	std::vector<ModelComponentBase*> pending_loads;
	std::vector<RenderSettings> render_targets;
	float tick_time;

	void AddComponentToQueue(ModelComponentBase* model_comp);
	void Render(RenderSettings& render_settings);

public:
	Renderer();
	~Renderer();

	void HandleNewComponent(Component* component);

	void CleanUp();

	inline void AddRenderTarget(RenderSettings render_settings) { render_targets.push_back(render_settings); }
	void FullRender();
};

