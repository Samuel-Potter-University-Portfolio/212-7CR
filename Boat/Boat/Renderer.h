#pragma once
#include "Model.h"
#include "Entity.h"

#include <map>


class Renderer
{
private:
	//std::map<Model, Component*> render_queue;

public:
	Renderer();
	~Renderer();

	void AddEntityToQueue(Entity entity);

	void Render();
	void CleanUp();
};

