#pragma once
#include "API.h"
#include "Model.h"
#include "Entity.h"
#include "ModelComponentBase.h"

#include <map>


struct RenderSettings 
{
	float aspect_ratio = 1.0f;
	class CameraComponent* camera = nullptr;

	Tags whitelist = E_TAG_ALL;
	Tags blacklist = E_TAG_NONE;

	class FrameBuffer* frame_buffer = nullptr;

	Tags shader_override_tags = E_TAG_ALL;
	class Shader* shader_override = nullptr;
};

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

	void AddEntityToQueue(Entity* entity);

	void CleanUp();

	inline void AddRenderTarget(RenderSettings render_settings) { render_targets.push_back(render_settings); }
	void FullRender();
};

