#pragma once
#include <GL/glew.h>


class Model
{
private:
	const GLuint vao;
	const int index_count;

public:
	Model(const GLuint vao, const int index_count);

	inline const GLuint GetVAO() { return vao; }
	inline const int GetIndexCount() { return index_count; }
};

