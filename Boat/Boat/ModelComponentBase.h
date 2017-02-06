#pragma once
#include "Component.h"
#include "Entity.h"
#include "Model.h"
#include "Shader.h"
#include "Transform.h"

class ModelComponentBase : public Component
{
private:
	bool is_visable = true;

	//16 Avaliable texture units
	GLuint textures[16]{0};

public:
	virtual Shader* GetShader() = 0;
	virtual Model* GetModel() = 0;

	inline const bool IsVisable() { return IsActive() && (parent ? parent->IsVisable() && is_visable : is_visable); }
	inline void SetVisable(const bool visable) { is_visable = visable; }

	inline void SetTextureUnit(unsigned int i, GLuint texture_id) { textures[i] = texture_id; }
	inline GLuint GetTextureUnit(unsigned int i) { if (i < 16) return textures[i]; else return 0; }
};

