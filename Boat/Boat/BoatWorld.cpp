#include "BoatWorld.h"
#include <random>
#include <time.h>

#include "TestEnt.h"

#include "SkyBox.h"
#include "BoxProp.h"
#include "OceanFloor.h"
#include "WaterSurface.h"
#include "Mesh.h"
#include "WaterShader.h"
#include "ClipShader.h"

#include "Window.h"
#include "Logger.h"

BoatWorld::BoatWorld()
{
}

void BoatWorld::LoadLogicResources(GameLogic* game_logic) 
{
	World::LoadLogicResources(game_logic);
	AddEntity(new TestEnt);
	AddEntity(new SkyBox);
	AddEntity(new WaterSurface);
	AddEntity(new OceanFloor);

	for (int x = -1; x <= 1; x++)
			for (int z = -1; z <= 1; z++)
			{	
				if (!x && !z)
					continue;

				BoxProp* box = new BoxProp;
				box->transform.location = glm::vec3(x, 0, z) * 75.0f;
				AddEntity(box);
			}
}

void BoatWorld::LoadWindowResources(Window* window) 
{
	World::LoadWindowResources(window);
	srand(time(nullptr));

	//Water model
	{
		std::vector<float> verts;
		std::vector<float> uvs;
		std::vector<float> normals;
		std::vector<int> indices;

		const int resolution = 50;
		const float scale = 5.0f;
		const float uv_scale = 5.0f;

		for (int y = -resolution/2; y < resolution/2; y++)
		{
			const int i = y + resolution / 2;

			for (int x = -resolution / 2; x < resolution / 2; x++)
			{
				const int n = x + resolution / 2;

				verts.push_back(x * scale);//x
				verts.push_back(0);//y
				verts.push_back(y * scale);//z

				uvs.push_back((resolution - n)/ uv_scale);//x
				uvs.push_back((i)/ uv_scale);//y

				normals.push_back(0);//x
				normals.push_back(1);//y
				normals.push_back(0);//z

				if (i != 0 && n != resolution - 1)
				{
					indices.push_back(0 + n + resolution * (i-1));
					indices.push_back(0 + n + resolution * i);
					indices.push_back(1 + n + resolution * i);

					indices.push_back(0 + n + resolution * (i - 1));
					indices.push_back(1 + n + resolution * i);
					indices.push_back(1 + n + resolution * (i - 1));
				}
			}
		}
		window->GetModelLoader().RegisterModel("water_surface", ModelMesh(verts, uvs, normals, indices));
	}

	//Sand model
	{
		std::vector<float> verts;
		std::vector<float> uvs;
		std::vector<float> normals;
		std::vector<int> indices;

		const int resolution = 10;
		const float scale = 50.0f;

		for (int y = -resolution / 2; y < resolution / 2; y++)
		{
			const int i = y + resolution / 2;

			for (int x = -resolution / 2; x < resolution / 2; x++)
			{
				const int n = x + resolution / 2;

				verts.push_back(x * scale);//x
				verts.push_back(rand() % 20 - 10);//y
				verts.push_back(y * scale);//z

				uvs.push_back(resolution - n);//x
				uvs.push_back(i);//y

				normals.push_back(0);//x
				normals.push_back(1);//y
				normals.push_back(0);//z

				if (i != 0 && n != resolution - 1)
				{
					indices.push_back(0 + n + resolution * (i - 1));
					indices.push_back(0 + n + resolution * i);
					indices.push_back(1 + n + resolution * i);

					indices.push_back(0 + n + resolution * (i - 1));
					indices.push_back(1 + n + resolution * i);
					indices.push_back(1 + n + resolution * (i - 1));
				}
			}
		}
		window->GetModelLoader().RegisterModel("ocean_floor", ModelMesh(verts, uvs, normals, indices));
	}

	//Load shaders
	window->GetShaderLoader().RegisterShader("water", new WaterShader);
	window->GetShaderLoader().RegisterShader("reflection_clip", new ClipShader);
	window->GetShaderLoader().RegisterShader("refraction_clip", new ClipShader);
}

void BoatWorld::UnloadLogicResources(GameLogic* game_logic) 
{
	World::UnloadLogicResources(game_logic);
}

void BoatWorld::UnloadWindowResources(Window* window) 
{
	World::UnloadWindowResources(window);
}