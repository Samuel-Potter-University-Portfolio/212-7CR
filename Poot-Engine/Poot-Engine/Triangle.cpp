#include "Triangle.h"


Triangle::Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c)
	: a(a), b(b), c(c)
{
	normal = glm::normalize(glm::cross(b - a, c - a));

	//ax + by + cz + d = 0 
	plane_d = -glm::dot(normal, a);
}

bool Triangle::IsInside(const glm::vec3& point) const 
{
	//Source: http://www.peroxide.dk/papers/collision/collision.pdf
	glm::vec3 E10 = b - a;
	glm::vec3 E20 = c - a;

	float A = glm::dot(E10, E10);
	float B = glm::dot(E10, E20);
	float C = glm::dot(E20, E20);
	float AC_BB = (A*C) - (B*B);
	glm::vec3 VP(point.x - a.x, point.y - a.y, point.z - a.z);

	float D = glm::dot(VP, E10);
	float E = glm::dot(VP, E20);
	float X = (D*C) - (E*B);
	float Y = (E*A) - (D*B);
	float Z = X + Y - AC_BB;

	typedef unsigned int uint;
	return (((uint&)Z) & ~(((uint&)X) | ((uint&)Y))) & 0x80000000;
}