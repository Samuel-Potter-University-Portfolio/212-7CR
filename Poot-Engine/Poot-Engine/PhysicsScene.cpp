#include "PhysicsScene.h"
#include "Logger.h"

#include <sstream>

void PhysicsScene::Register(World* world) 
{
	this->world = world;
	LOG(Log, "Registered Physics Scene");
}

void PhysicsScene::HandleNewComponent(Component* component)
{
	Collider* collider = Cast<Collider>(component);
	if (collider)
	{
		colliders.push_back(collider);
		collider->AddToScene(this);
	}

	Body* body = Cast<Body>(component);
	if (body)
	{
		bodies.push_back(body);
		body->AddToScene(this);
	}
}

std::string PhysicsScene::GetStatusString() 
{
	std::stringstream message;
	message << "Colliders: " << colliders.size() << '\n';

	unsigned int sleeping_total = 0;

	for (int i = 0; i < bodies.size(); i++)
		if(!bodies[i]->IsAwake())
			sleeping_total++;

	message << "Awake: " << (bodies.size() - sleeping_total) << '\n';
	message << "Sleeping: " << sleeping_total;
	return message.str();
}

void PhysicsScene::PhysicsTick(float delta_time) 
{
	check_buffer.clear();

	for (Body* body : bodies) 
	{
		if (!body->IsAwake())
			continue;

		body->UpdateVelocity(delta_time);

		//Check for collisions
		Collider* player_collider = body->GetCollider();
		
		if (player_collider)
		{
			//Store broad phase
			for (Collider* collider : colliders)
				if (collider != player_collider && collider->DoesBroadPhaseCollide(player_collider, body->velocity))
					check_buffer.push_back(collider);


			if (check_buffer.size() != 0)
			{
				//Check required number of times
				for (int i = 0; i < settings.checks_per_tick; i++)
				{
					HitInfo closet_hit_info;
					Collider* closet_hit = nullptr;

					//Check for narrow phase
					for (Collider* collider : check_buffer)
					{
						//Store closets narrow phase collisions
						HitInfo hit_info;
						if (collider->DoesNarrowPhaseCollide(player_collider, body->velocity, hit_info)
							&& (closet_hit == nullptr || hit_info.distance < closet_hit_info.distance)
							)
						{
							closet_hit = collider;
							closet_hit_info = hit_info;
						}
					}

					//No hit found
					if (!closet_hit)
						break;

					//Resolve closest hit
					closet_hit->ResolveCollision(body, closet_hit_info);
				}
			}

			check_buffer.clear();
		}

		body->UpdateTransform(delta_time);
	}
}

void PhysicsScene::CleanUp() 
{
	LOG(Log, "Cleaned up Physics Scene");
}
