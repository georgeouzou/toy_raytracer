#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>

class Ray
{
public:
	Ray() {}
	Ray(const glm::vec3 &_a, const glm::vec3 &_b) { a = _a; b = _b; }
	glm::vec3 origin() const { return a; }
	glm::vec3 direction() const { return b; }
	glm::vec3 pt(float t) const { return a + t * b; }

	glm::vec3 a;
	glm::vec3 b;
};


#endif