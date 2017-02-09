#include "BoatWorld.h"

#include "TestEnt.h"

#include "SkyBox.h"
#include "WaterSurface.h"
#include "Mesh.h"
#include "WaterShader.h"

#include "Window.h"
#include "Logger.h"

BoatWorld::BoatWorld()
{
}

void BoatWorld::LoadLogicResources(GameLogic* game_logic) 
{
	World::LoadLogicResources(game_logic);
	AddEntity(new SkyBox);
	AddEntity(new WaterSurface);
	AddEntity(new TestEnt);
}

void BoatWorld::LoadWindowResources(Window* window) 
{
	World::LoadWindowResources(window);

	//Water model
	{
		std::vector<float> verts;
		std::vector<float> uvs;
		std::vector<float> normals;
		std::vector<int> indices;

		const int resolution = 50;
		const float scale = 5.0f;

		for (int y = -resolution/2; y < resolution/2; y++)
		{
			const int i = y + resolution / 2;

			for (int x = -resolution / 2; x < resolution / 2; x++)
			{
				const int n = x + resolution / 2;

				verts.push_back(x * scale);//x
				verts.push_back(0);//y
				verts.push_back(y * scale);//z

				uvs.push_back(resolution - n);//x
				uvs.push_back(i);//y

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

	//Load shaders
	window->GetShaderLoader().RegisterShader("water", new WaterShader);
}

void BoatWorld::UnloadLogicResources(GameLogic* game_logic) 
{
	World::UnloadLogicResources(game_logic);
}

void BoatWorld::UnloadWindowResources(Window* window) 
{
	World::UnloadWindowResources(window);
}