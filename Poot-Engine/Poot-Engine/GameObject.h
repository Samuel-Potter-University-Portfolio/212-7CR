#pragma once
#include "API.h"
#include "Object.h"
#include "Component.h"
#include "Local.h"

#include <GL/glew.h>
#include <vector>
#include <string>


/*
Serves as a base for all world tickable classes,
World handles ALL memory management surrounding game objects.
*/
class POOT_API GameObject : public Object
{
private:
	class World* world;

	std::vector<Component*> components;
	bool components_built;

protected:
	virtual void BuildComponents();


	//virtual void LogicBegin() {}
	virtual void WindowBegin() override;

	virtual void LogicTick(float delta_time) override;
	virtual void WindowTick(float delta_time) override;

	virtual void Begin() {}
	virtual void Tick(float delta_time) {}

public:
	GameObject();
	virtual ~GameObject();
	
	inline World* GetWorld() { return world; }

	virtual void SetOwner(World* world);
	virtual void SetOwner(Object* object) override;

	static class Model* LoadModelAsset(std::string key);
	static class Shader* LoadShaderAsset(std::string key);
	static GLuint LoadTextureAsset(std::string key);
	static GLuint LoadCubeMapAsset(std::string key);

	virtual void OnCollide(class Collider* collider, struct HitInfo& hit_info) {}
	
	/* Components */
private:
	void AddComponent(Component* component);

protected:

	template<typename Type>
	inline Type* MakeComponent()
	{
		Type* new_comp = new Type;
		AddComponent(new_comp);
		return new_comp;
	}

public:
	//Component fetching
	template<typename Type>
	Type* GetComponent()
	{
		for (Component* comp : components)
		{
			Type* actual_comp = Cast<Type>(comp);
			if (actual_comp)
				return actual_comp;
		}
		return nullptr;
	}

	template<typename Type>
	std::vector<Type*> GetAllComponents()
	{
		std::vector<Type*> actual_comps;

		for (Component* comp : components)
		{
			Type* actual_comp = Cast<Type*>(comp);
			if (actual_comp)
				actual_comps.push_back(actual_comp);
		}
		return actual_comps;
	}

	inline const std::vector<Component*>& GetAllComponents() { return components; }
};


class POOT_API GameObject3D : public GameObject, public Local3D
{
public:
	virtual void SetOwner(Object* object) override;
protected:
	float frame_timestep = 1.0f;

	virtual void WindowBegin() override;
	virtual void LogicTick(float delta_time) override;
	virtual void WindowTick(float delta_time) override;
};


class POOT_API GameObject2D : public GameObject, public Local2D
{
public:
	virtual void SetOwner(Object* object) override;
protected:
	float frame_timestep = 1.0f;

	virtual void WindowBegin() override;
	virtual void LogicTick(float delta_time) override;
	virtual void WindowTick(float delta_time) override;
};

