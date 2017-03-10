#pragma once
#include "API.h"
#include "Object.h"
#include "Component.h"

#include <functional>
#include <vector>



struct POOT_API RenderRequest 
{
	std::function<void()> pre_render;
	std::function<void()> post_render;

	class CameraComponent* camera = nullptr;
	class FrameBuffer* frame_buffer = nullptr;

	enum RenderMode
	{
		World, GUI
	} render_mode;

	Tag whitelist = OBJ_TAG_ALL;
	Tag blacklist = OBJ_TAG_NONE;

	Tag shader_override_tags = OBJ_TAG_NONE;
	class Shader* shader_override = nullptr;
};


class POOT_API MasterRenderer
{
private:
	class World* world = nullptr;

	class InstancedRenderer* instanced_renderer = nullptr;

	std::vector<RenderRequest> requests;
	void Render(RenderRequest& request);

public:
	void Register(World* world);
	void HandleNewComponent(Component* component);

	void Render();
	void CleanUp();
};
