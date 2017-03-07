#pragma once
#include "API.h"
#include "Component.h"
#include "Model.h"
#include "Shader.h"
#include "Transform.h"


#define MAX_MODEL_UNITS 16


class POOT_API ModelComponentBase : public Component3D
{
private:
	bool is_visable = true;

	//Avaliable units per textures, integers, and floats
	GLuint textures[MAX_MODEL_UNITS]{ 0 };
	int integers[MAX_MODEL_UNITS]{ 0 };
	float floats[MAX_MODEL_UNITS]{ 0 };

public:
	virtual Shader* GetShader() = 0;
	virtual Model* GetModel() = 0;

	inline const bool IsVisable() { return IsActive() && is_visable; }
	inline void SetVisable(const bool visable) { is_visable = visable; }

	inline void SetTextureUnit(unsigned int i, GLuint texture_id) { if (i < MAX_MODEL_UNITS) textures[i] = texture_id; }
	inline GLuint GetTextureUnit(unsigned int i) { if (i < MAX_MODEL_UNITS) return textures[i]; else return 0; }

	inline void SetIntUnit(unsigned int i, int value) { if (i < MAX_MODEL_UNITS) integers[i] = value; }
	inline int GetIntUnit(unsigned int i) { if (i < MAX_MODEL_UNITS) return integers[i]; else return 0; }

	inline void SetFloatUnit(unsigned int i, float value) { if (i < MAX_MODEL_UNITS) floats[i] = value; }
	inline float GetFloatUnit(unsigned int i) { if (i < MAX_MODEL_UNITS) return floats[i]; else return 0.0f; }
};

