#include "World.h"
#include "Game.h"
#include "DefaultShader.h"
#include "SkyboxShader.h"
#include "Logger.h"

World::World()
{
	//physics_scene = nullptr;
	main_camera = nullptr;
	sun_light = nullptr;

	logic_begun = false;
	window_begun = false;

	in_logic_tick = false;

	is_being_destroyed = false;
	logic_destroyed = false;
	window_destroyed = false;
}

World::~World()
{
	while (new_objects.size() != 0)
	{
		GameObject* object = new_objects.front();
		delete object;
		new_objects.pop();
	}

	for (GameObject* object : game_objects)
		delete object;
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

	//Tick objects
	for (GameObject* object : game_objects)
		object->HandleLogicTick(delta_time);

	//Tick physics
	//if (physics_scene)
	//	physics_scene->Tick(delta_time);

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
	while (new_objects.size() != 0)
	{
		if (in_logic_tick)
			break;

		GameObject* object = new_objects.front();
		InternalSpawnObject(object);
		new_objects.pop();
	}
	

	//Tick entities
	for (GameObject* object : game_objects)
		object->HandleWindowTick(delta_time);
	
	master_renderer.Render();
}

void World::LoadLogicResources(GameLogic* game_logic) 
{
	//TODO - UPDATE
	//physics_scene = new PhysicsScene;
	//physics_scene->Link(this);
}

void World::LoadWindowResources(Window* window)
{
	master_renderer.Register(this);

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
				normals.push_back(-r3);
			else
				normals.push_back(r3);

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
}

void World::UnloadLogicResources(GameLogic* game_logic)
{
	//if (physics_scene)
	//{
	//	physics_scene->CleanUp();
	//	delete physics_scene;
	//	physics_scene = nullptr;
	//}

	logic_destroyed = true;
}

void World::UnloadWindowResources(Window* window) 
{
	master_renderer.CleanUp();
	window_destroyed = true;
}

void World::AddObject(GameObject* object)
{
	if (!object)
		return;

	new_objects.push(object);
}

void World::InternalSpawnObject(GameObject* object)
{
	game_objects.push_back(object);
	object->SetOwner(this);

	for (Component* component : object->GetAllComponents())
		if (component != nullptr)
			InternalAddComponent(component);
}

void World::InternalAddComponent(Component* component) 
{
	//Check for camera
	if(!main_camera && component->HasTag(OBJ_TAG_PLAYER))
	{
		CameraComponent* camera = Cast<CameraComponent>(component);

		if (camera)
		{
			LOG(Log, "Found player Camera to use as main");
			main_camera = camera;
		}
	}

	//Check for sun
	if(!sun_light)
	{
		DirectionalLightComponent* sun = Cast<DirectionalLightComponent>(component);

		if (sun)
		{
			LOG(Log, "Found light to use as sun");
			sun_light = sun;
		}
	}

	//Add to components
	master_renderer.HandleNewComponent(component);

	g_game->GetWindow()->GetMouse().HandleNewComponent(component);
	g_game->GetWindow()->GetKeyboard().HandleNewComponent(component);

	/*
	if (physics_scene)
		physics_scene->AddEntityToLevel(entity);
	*/
}