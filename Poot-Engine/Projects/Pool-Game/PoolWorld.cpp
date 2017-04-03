#include "PoolWorld.h"
#include <Logger.h>
#include <Sun.h>
#include <SkyBox.h>
#include <Window.h>
#include "PoolDebugPlayer.h"
#include "PoolEntry.h"

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

	AddObject(new PoolEntry);
	Sun* sun = new Sun(glm::vec3(0, -1, 0));
	AddObject(sun);

	for (int i = 0; i <= 15; i++)
		AddObject(new PoolBall(i));
}

void PoolWorld::LoadWindowResources(Window* window)
{
	Super::LoadWindowResources(window);

	//Ball
	{
		//Unit sphere
		std::vector<float> verts;
		std::vector<float> normals;
		std::vector<float> uvs;
		std::vector<int> indices;
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
				const float x = sinf(theta) * cosf(phi);
				const float y = sinf(theta) * sinf(phi);
				const float z = cosf(theta);

				verts.push_back(x);
				verts.push_back(y);
				verts.push_back(z);

				//Length is already 1
				normals.push_back(x);
				normals.push_back(y);
				normals.push_back(z);

				//Cylindrical sphere uvs https://mft-dev.dk/uv-mapping-sphere/
				uvs.push_back((l / (float)slices) * -2.0f);
				uvs.push_back((s / (float)stacks));
			}
		}

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

	//Cue
	{
		//Unit sphere
		std::vector<float> verts;
		std::vector<float> normals;
		std::vector<float> uvs;
		std::vector<int> indices;
		const int length = 20;
		const int resolution = 10;
		const float front_radius = 0.25f;
		const float back_radius = 0.4f;

		//Back
		for (int i = 0; i <= resolution; i++)
		{
			float theta = i / (float)resolution * PI * 2.0f;

			const float x = cosf(theta);
			const float y = sinf(theta);

			verts.push_back(x * back_radius);
			verts.push_back(y * back_radius);
			verts.push_back(-length);

			normals.push_back(x);
			normals.push_back(y);
			normals.push_back(-0.01f);

			uvs.push_back((i / (float)resolution));
			uvs.push_back(0);
		}

		//Front
		for (int i = 0; i <= resolution; i++)
		{
			float theta = i / (float)resolution * PI * 2.0f;

			const float x = cosf(theta);
			const float y = sinf(theta);

			verts.push_back(x * front_radius);
			verts.push_back(y * front_radius);
			verts.push_back(0.0f);

			normals.push_back(x);
			normals.push_back(y);
			normals.push_back(0.01f);

			uvs.push_back((i / (float)resolution));
			uvs.push_back(1);
		}

		for (int i = 0; i < resolution; i++)
		{
			//Cylinder
			indices.push_back(i);
			indices.push_back(i + 1);
			indices.push_back(resolution + i);

			indices.push_back(resolution + i);
			indices.push_back(i + 1);
			indices.push_back(resolution + i + 1);

			//Back face
			indices.push_back(resolution - 1);
			indices.push_back(i + 1);
			indices.push_back(i);

			//Front face
			indices.push_back(resolution + resolution - 1);
			indices.push_back(resolution + i);
			indices.push_back(resolution + i + 1);

		}

		ModelMesh mesh(
			verts,
			uvs,
			normals,
			indices
		);
		//TODO - Optimize mesh
		window->GetModelLoader().RegisterModel("cue", mesh);
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