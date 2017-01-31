#pragma once
#include "Model.h"
#include "Entity.h"
#include "ModelComponentBase.h"

#include <map>


class Renderer
{
private:
	std::map<Model*, std::vector<ModelComponentBase*>> render_queue;
	std::vector<ModelComponentBase*> pending_loads;

	void AddComponentToQueue(ModelComponentBase* model_comp);

public:
	Renderer();
	~Renderer();

	void AddEntityToQueue(Entity* entity);

	void Render();
	void CleanUp();
};

