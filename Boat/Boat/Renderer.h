#pragma once
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
	class Shader* shader_override = nullptr;
};


class Renderer
{
private:
	std::map<Model*, std::vector<ModelComponentBase*>> render_queue;
	std::vector<ModelComponentBase*> pending_loads;
	float tick_time;

	void AddComponentToQueue(ModelComponentBase* model_comp);

public:
	Renderer();
	~Renderer();

	void AddEntityToQueue(Entity* entity);

	void Update();
	void Render(RenderSettings& render_settings);
	void CleanUp();
};

