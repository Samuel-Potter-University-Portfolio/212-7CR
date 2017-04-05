#pragma once
#include "API.h"
#include <GL/glew.h>


class POOT_API Model
{
private:
	const GLuint vao;
	const int index_count;

public:
	Model(const GLuint vao, const int index_count);

	inline const GLuint GetVAO() { return vao; }
	inline const int GetIndexCount() { return index_count; }

	inline bool operator==(const Model& other) const { return vao == other.vao; }
	inline bool operator!=(const Model& other) const { return vao != other.vao; }
};

