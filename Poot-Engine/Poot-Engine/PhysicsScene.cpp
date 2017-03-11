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
		colliders.push_back(collider);

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
	message << "Bodies: " << bodies.size() << '\n';

	unsigned int sleeping_total = 0;

	for (int i = 0; i < bodies.size(); i++)
		if(!bodies[i]->IsAwake())
			sleeping_total++;

	message << "Sleeping: " << sleeping_total;
	return message.str();
}

void PhysicsScene::PhysicsTick(float delta_time) 
{
	for (Body* body : bodies) 
	{
		if (!body->IsAwake())
			continue;

		body->UpdateVelocity(delta_time);
		//Check against colliders
		body->UpdateTransform(delta_time);
	}
}

void PhysicsScene::CleanUp() 
{
	LOG(Log, "Cleaned up Physics Scene");
}
