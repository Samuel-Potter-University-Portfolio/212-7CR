#include "World.h"
#include "Game.h"
#include "DefaultShader.h"
#include "SkyboxShader.h"
#include "Logger.h"

World::~World() 
{
	for (Entity* entity : entities)
		delete entity;
}

void World::LogicBegin(GameLogic* game_logic) 
{
	LoadLogicResources(game_logic);
	logic_begun = true;
	LOG(Log, "World logic ready");
}

void World::WindowBegin(Window* window)
{
	//Let logic load first to ensure all resources are ready
	if (!logic_begun)
		return;

	LoadWindowResources(window);
	window_begun = true;
	LOG(Log, "World window ready");
}

void World::LogicTick(GameLogic* game_logic, float delta_time)
{
	//Handle, if world if currently being destroyed
	if(is_being_destroyed)
	{
		if (!logic_destroyed)
		{
			UnloadLogicResources(game_logic);
			LOG(Log, "World logic destroyed");
		}
		return;
	}

	//Handle world's first tick i.e. needs to be loaded in
	if (!logic_begun) 
	{
		LogicBegin(game_logic);
		return;
	}

	//Wait until both threads have been loaded
	if (!HasLoaded())
		return;

	//Tick entities
	for (Entity* entity : entities)
		entity->HandleLogicTick(delta_time);
}

void World::WindowTick(Window* window, float delta_time)
{
	//Handle, if world if currently being destroyed
	if (is_being_destroyed)
	{
		if (!window_destroyed)
		{
			UnloadWindowResources(window);
			LOG(Log, "World window destroyed");
		}
		return;
	}

	//Handle world's first tick i.e. needs to be loaded in
	if (!window_begun)
	{
		WindowBegin(window);
		return;
	}

	//Wait until both threads have been loaded
	if (!HasLoaded())
		return;
	
	//Tick entities
	for (Entity* entity : entities)
		entity->HandleWindowTick(delta_time);

	if (renderer)
		renderer->Render(window);
}

void World::LoadLogicResources(GameLogic* game_logic) 
{
}

void World::LoadWindowResources(Window* window)
{
	renderer = new Renderer;

	//Added already existing entities
	for(Entity* entity : entities)
		renderer->AddEntityToQueue(entity);


	//Load default models
	//Quad
	{
		ModelMesh mesh(
		{
			0.0, 0.0, 0.0,	0.0, 1.0, 0.0,
			1.0, 1.0, 0.0,	1.0, 0.0, 0.0,
		},
		{
			1.0, 0.0, 1.0, 1.0,
			0.0, 1.0, 0.0, 0.0
		}, 
		{
			0.0, 0.0, 1.0, 0.0, 0.0, 1.0,
			0.0, 0.0, 1.0, 0.0, 0.0, 1.0
		}, 
		{
			0,1,2, 0,2,3,
		}
		);
		window->GetModelLoader().RegisterModel("quad", mesh);
	}

	//Cube
	{
		std::vector<float> verts{
			0.0, 0.0, 0.0,	0.0, 1.0, 0.0,
			1.0, 1.0, 0.0,	1.0, 0.0, 0.0,

			0.0, 0.0, 1.0,	0.0, 1.0, 1.0,
			1.0, 1.0, 1.0,	1.0, 0.0, 1.0,
		};

		for (float& f : verts)
			f -= 0.5f;

		const float r3 = (float)sqrt(3);

		ModelMesh mesh(
			verts,
			{
				0.0, 0.0, 0.0, 1.0,
				1.0, 1.0, 1.0, 0.0,

				1.0, 1.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0,
			},
			{
				-r3, -r3, r3, -r3, r3, r3,
				r3, r3, r3, r3, -r3, r3,

				r3, r3, -r3, r3, -r3, -r3,
				-r3, -r3, -r3, -r3, r3, -r3,
			},
			{
				0,1,2, 0,2,3,
				6,5,4, 7,6,4,

				5,6,1, 6,2,1,
				7,4,0, 0,3,7,

				2,6,3, 6,7,3,
				5,1,0, 4,5,0
			}
		);
		window->GetModelLoader().RegisterModel("cube", mesh);
	}
	
	//Skybox
	{
		const float size = 700.0f;

		ModelMesh mesh(
		{
			-size, -size, -size,	-size, size, -size,
			size, size, -size,	size, -size, -size,

			-size, -size, size,	-size, size, size,
			size, size, size,	size, -size, size
		},
		{}, //UVs Generated from position in shader
		{}, //Normals not required
		{
			2,1,0, 3,2,0,
			4,5,6, 4,6,7,

			1,6,5, 1,2,6,
			0,4,7, 7,3,0,

			3,6,2, 3,7,6,
			0,1,5, 0,5,4
		}
		);
		window->GetModelLoader().RegisterModel("skybox", mesh);
	}

	//Load default shaders
	window->GetShaderLoader().RegisterShader("default", new DefaultShader);
	window->GetShaderLoader().RegisterShader("skybox", new SkyboxShader);
}

void World::UnloadLogicResources(GameLogic* game_logic)
{
	for (Entity* entity : entities)
		entity->LogicDestroy();

	logic_destroyed = true;
}

void World::UnloadWindowResources(Window* window) 
{
	for (Entity* entity : entities)
		entity->WindowDestroy();

	if (renderer)
	{
		renderer->CleanUp();
		delete renderer;
		renderer = nullptr;
	}

	window_destroyed = true;
}

void World::AddEntity(Entity* entity) 
{
	if (!entity)
		return;

	entities.push_back(entity);

	if (!main_camera)
	{
		main_camera = entity->GetComponent<CameraComponent>();

		if (main_camera)
			LOG(Log, "Found Camera component to use as main");
	}

	if(renderer)
		renderer->AddEntityToQueue(entity);
}