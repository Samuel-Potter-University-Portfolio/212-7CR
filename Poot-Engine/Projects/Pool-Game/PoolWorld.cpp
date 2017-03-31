#include "PoolWorld.h"
#include <Logger.h>
#include <Sun.h>
#include <SkyBox.h>
#include <Window.h>
#include "PoolDebugPlayer.h"

#include "PoolBall.h"
#include "PoolTable.h"
#include "BasicPlane.h"


#define PI 3.141592653589793f


PoolWorld::PoolWorld()
{
}

void PoolWorld::LoadLogicResources(GameLogic* game_logic)
{
	Super::LoadLogicResources(game_logic);

	AddObject(new PoolTable);
	AddObject(new BasicPlane);
	AddObject(new SkyBox("Resources/Skybox/TropicalSunnyDay.png"));

	AddObject(new PoolDebugPlayer);
	Sun* sun = new Sun(glm::vec3(0, -1, 0));
	AddObject(sun);
	
#define ADD_BALL(num, x, y) \
	{ \
		PoolBall* ball = new PoolBall(num); \
		ball->local_transform.location = glm::vec3(x * 2.02f, 38.0f, 12.0f + y * 1.75f); \
		AddObject(ball); \
	}

	ADD_BALL(0, 0, -17);

	ADD_BALL(1, 0, 0);

	ADD_BALL(2, 0.5f, 1);
	ADD_BALL(9, -0.5f, 1);

	ADD_BALL(10, 1, 2);
	ADD_BALL(8, 0, 2);
	ADD_BALL(3, -1, 2);

	ADD_BALL(12, 1.5f, 3);
	ADD_BALL(5, 0.5f, 3);
	ADD_BALL(11, -0.5f, 3);
	ADD_BALL(4, -1.5f, 3);

	ADD_BALL(15, 2, 4);
	ADD_BALL(14, 1, 4);
	ADD_BALL(7, 0, 4);
	ADD_BALL(13, -1, 4);
	ADD_BALL(6, -2, 4);

	//ADD_BALL(8, 0, 1.65f * 2);
}

void PoolWorld::LoadWindowResources(Window* window)
{
	Super::LoadWindowResources(window);

	//Ball
	{
		//Unit sphere
		std::vector<float> verts;
		std::vector<float> uvs;
		const int stacks = 20;
		const int slices = 20;

		for (int s = 0; s <= stacks; s++)
		{
			//Height of layer
			float theta = s / (float)stacks * PI;

			//Layer
			for (int l = 0; l <= slices; l++)
			{
				float phi = l / (float)slices * PI * 2.0f;

				//Spherical coordinates
				verts.push_back(sinf(theta) * cosf(phi));
				verts.push_back(sinf(theta) * sinf(phi));
				verts.push_back(cosf(theta));

				//Cylindrical sphere uvs https://mft-dev.dk/uv-mapping-sphere/
				uvs.push_back((l / (float)slices) * -2.0f);
				uvs.push_back((s / (float)stacks));
			}
		}


		std::vector<float> normals;

		for (int i = 0; i < verts.size(); i += 3)
		{
			const float x = verts[i];
			const float y = verts[i + 1];
			const float z = verts[i + 2];
			const float length = 1.0f;//sqrtf(x*x + y*y + z*z); //With current setup, lenght is always 1 anyway

			normals.push_back(x / length);
			normals.push_back(y / length);
			normals.push_back(z / length);
		}

		std::vector<int> indices;

		for (int i = 0; i <= stacks; i++)
		{
			const int this_layer_start = i * slices;
			const int next_layer_start = (i + 1) * slices;

			for (int s = 0; s < slices; s++)
			{
				indices.push_back(this_layer_start + s);
				indices.push_back(next_layer_start + s);
				indices.push_back(next_layer_start + s + 1);

				indices.push_back(this_layer_start + s);
				indices.push_back(next_layer_start + s + 1);
				indices.push_back(this_layer_start + s + 1);
			}
		}


		ModelMesh mesh(
			verts,
			uvs,
			normals,
			indices
		);
		//TODO - Optimize mesh
		window->GetModelLoader().RegisterModel("ball", mesh);
	}
}

void PoolWorld::UnloadLogicResources(GameLogic* game_logic)
{
	Super::UnloadLogicResources(game_logic);
}

void PoolWorld::UnloadWindowResources(Window* window)
{
	Super::UnloadWindowResources(window);
}