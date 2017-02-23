#include "World.h"
#include "Game.h"
#include "DefaultShader.h"
#include "UIShader.h"
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
	in_logic_tick = true;

	//Handle, if world if currently being destroyed
	if (is_being_destroyed)
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

	//Tick physics
	if (physics_scene)
		physics_scene->Tick(delta_time);

	in_logic_tick = false;
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

	//Try and load new entitites
	while (new_entities.size() != 0)
	{
		if (in_logic_tick)
			break;

		Entity* entity = new_entities.front();
		InternalAddEntity(entity);
		new_entities.pop();
	}
	

	//Tick entities
	for (Entity* entity : entities)
		entity->HandleWindowTick(delta_time);
	
	if (renderer)
	{
		RenderSettings render_settings;
		render_settings.aspect_ratio = window->GetAspectRatio();
		render_settings.camera = main_camera;

		renderer->AddRenderTarget(render_settings);
		renderer->FullRender();
	}
}

void World::LoadLogicResources(GameLogic* game_logic) 
{
	physics_scene = new PhysicsScene;
	physics_scene->Link(this);
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
	
	//UI - Quad
	{
		ModelMesh mesh(
		{
			-50.0, -50.0, 0.0,	-50.0, 50.0, 0.0,
			50.0, 50.0, 0.0,	50.0, -50.0, 0.0,
		},
		{
			0.0, 0.0, 0.0, 1.0,
			1.0, 1.0, 1.0, 0.0
		},
		{},
		{
			2,1,0, 3,2,0,
		}
		);
		window->GetModelLoader().RegisterModel("ui_quad", mesh);
	}

	//Cube
	{
		std::vector<float> verts{
			0.0, 0.0, 0.0,	0.0, 1.0, 0.0,
			1.0, 1.0, 0.0,	1.0, 0.0, 0.0,

			0.0, 0.0, 1.0,	0.0, 1.0, 1.0,
			1.0, 1.0, 1.0,	1.0, 0.0, 1.0,
		};

		const float r3 = (float)sqrt(3);
		std::vector<float> normals;

		for (float& f : verts)
		{
			if (f == 0.0f)
				normals.push_back(r3);
			else
				normals.push_back(-r3);

			f -= 0.5f;
		}


		ModelMesh mesh(
			verts,
			{
				0.0, 0.0, 0.0, 1.0,
				1.0, 1.0, 1.0, 0.0,

				1.0, 1.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0,
			},
			normals,
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
	window->GetShaderLoader().RegisterShader("ui", new UIShader);
}

void World::UnloadLogicResources(GameLogic* game_logic)
{
	for (Entity* entity : entities)
		entity->LogicDestroy();

	if (physics_scene)
	{
		physics_scene->CleanUp();
		delete physics_scene;
		physics_scene = nullptr;
	}

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

	if (in_logic_tick)
		new_entities.push(entity);
	else
		InternalAddEntity(entity);
}

void World::InternalAddEntity(Entity* entity) 
{
	entities.push_back(entity);
	entity->SetWorld(this);

	if (!main_camera && entity->GetTags() & E_TAG_PLAYER)
	{
		main_camera = entity->GetComponent<CameraComponent>();

		if (main_camera)
			LOG(Log, "Found Camera component to use as main");
	}

	if (!sun_light)
	{
		sun_light = entity->GetComponent<DirectionalLightComponent>();

		if (sun_light)
			LOG(Log, "Found directional light component to use as sun");
	}

	if (renderer)
		renderer->AddEntityToQueue(entity);

	if (physics_scene)
		physics_scene->AddEntityToLevel(entity);
}