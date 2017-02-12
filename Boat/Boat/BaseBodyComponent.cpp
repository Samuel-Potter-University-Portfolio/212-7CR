#include "BaseBodyComponent.h"


void BaseBodyComponent::JoinScene(PhysicsScene* physics_scene) 
{
	this->physics_scene = physics_scene;
}