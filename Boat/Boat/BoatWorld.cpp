#include "BoatWorld.h"
#include <random>

#include "PlayerBoat.h"
#include "PlayerEntry.h"

#include "SkyBox.h"
#include "Sun.h"
#include "FloatBarrier.h"
#include "OceanFloor.h"
#include "WaterSurface.h"
#include "Mesh.h"
#include "WaterShader.h"
#include "ClipShader.h"
#include "BeachBall.h"

#include "Window.h"
#include "Logger.h"

BoatWorld::BoatWorld()
{
}

void BoatWorld::LoadLogicResources(GameLogic* game_logic) 
{
	World::LoadLogicResources(game_logic);

	Sun* sun = new Sun;
	sun->directional_light->SetDirection(glm::vec3(1, -1, 1));
	AddEntity(sun);


	AddEntity(new SkyBox);
	AddEntity(new OceanFloor);

	//Build track
#define ADD_SEGMENT(x,z, direction) { FloatBarrier* box = new FloatBarrier(direction); box->transform.location = glm::vec3(x, 0, z) * 22.0f; AddEntity(box); }
#define ADD_BALL(x,z) { BeachBall* ball = new BeachBall; ball->transform.location = glm::vec3(x, 5.0f/22.0f, z) * 22.0f; AddEntity(ball); }

	//Initial stretch
	for (int i = -5; i < 5; i++)
	{
		ADD_SEGMENT(1, i, Vertical);
		ADD_SEGMENT(-1, i, Vertical);
	}

	//Corner
	ADD_SEGMENT(1, 5, Vertical);
	ADD_SEGMENT(1, 6, Vertical);
	ADD_SEGMENT(1, 6, Horizontal);
	ADD_SEGMENT(0, 6, Horizontal);
	ADD_SEGMENT(1, 6, Horizontal);
	//ADD_SEGMENT(-1, i, Vertical);

	ADD_BALL(0,1)


	AddEntity(new WaterSurface);

	//Ball tower
	/*
	{
		BeachBall* ball = new BeachBall;
		ball->transform.location = glm::vec3(0, 54, 50);
		AddEntity(ball);
		ball = new BeachBall;
		ball->transform.location = glm::vec3(-2, 50, 50);
		AddEntity(ball);
		ball = new BeachBall;
		ball->transform.location = glm::vec3(2, 50, 50);
		AddEntity(ball);

		ball = new BeachBall;
		ball->transform.location = glm::vec3(-4, 46, 50);
		AddEntity(ball);
		ball = new BeachBall;
		ball->transform.location = glm::vec3(0, 46, 50);
		AddEntity(ball);
		ball = new BeachBall;
		ball->transform.location = glm::vec3(4, 46, 50);
		AddEntity(ball);

		ball = new BeachBall;
		ball->transform.location = glm::vec3(-6, 42, 50);
		AddEntity(ball);
		ball = new BeachBall;
		ball->transform.location = glm::vec3(-2, 42, 50);
		AddEntity(ball);
		ball = new BeachBall;
		ball->transform.location = glm::vec3(2, 42, 50);
		AddEntity(ball);
		ball = new BeachBall;
		ball->transform.location = glm::vec3(6, 42, 50);
		AddEntity(ball);
	}
	*/
	AddEntity(new PlayerEntry);
}

void BoatWorld::LoadWindowResources(Window* window) 
{
	World::LoadWindowResources(window);
	srand(0);

	//Water model
	{
		std::vector<float> verts;
		std::vector<float> uvs;
		std::vector<float> normals;
		std::vector<int> indices;

		const int resolution = 50;
		const float scale = 5000.0f;
		const float uv_scale = 25.0f / scale;

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

		const int resolution = 50;
		const int deviation = 5;

		float heights[resolution * 2][resolution * 2]{ {0} };

		heights[24][23] = 31 + rand() % deviation;
		heights[24][24] = 31 + rand() % deviation;
		heights[24][25] = 31 + rand() % deviation;
		heights[24][26] = 31 + rand() % deviation;
		heights[24][27] = 31 + rand() % deviation;

		heights[23][23] = 31 + rand() % deviation;
		heights[23][24] = 31 + rand() % deviation;
		heights[23][25] = 31 + rand() % deviation;
		heights[23][26] = 31 + rand() % deviation;
		heights[23][27] = 31 + rand() % deviation;


		heights[25][22] = 25;
		heights[24][22] = 25;
		heights[23][22] = 25;
		heights[22][22] = 25;

		heights[25][28] = 25;
		heights[24][28] = 25;
		heights[23][28] = 25;
		heights[22][28] = 25;

		heights[25][23] = 25;
		heights[25][24] = 25;
		heights[25][25] = 25;
		heights[25][26] = 25;
		heights[25][27] = 25;

		heights[22][23] = 25;
		heights[22][24] = 25;
		heights[22][25] = 25;
		heights[22][26] = 25;
		heights[22][27] = 25;

		const float scale = 50.0f;
		const float uv_scale = 25.0f / scale;

		for (int y = -resolution / 2; y < resolution / 2; y++)
		{
			const int i = y + resolution / 2;

			for (int x = -resolution / 2; x < resolution / 2; x++)
			{
				const int n = x + resolution / 2;

				verts.push_back(x * scale);//x
				verts.push_back(heights[n][i]);//y
				verts.push_back(y * scale);//z

				uvs.push_back((resolution - n) / uv_scale);//x
				uvs.push_back((i) / uv_scale);//y

				normals.push_back(x * scale);//x
				normals.push_back(heights[n][i]);//y
				normals.push_back(y * scale);//z

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