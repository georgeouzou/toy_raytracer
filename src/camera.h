#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include "ray.h"

class Camera
{
public:
	Camera() :
		m_origin(0.0f, 0.0f, 0.0f),
		m_lower_left_corner(-2.0f, -1.0f, -1.0f),
		m_vertical(0.0f, 2.0f, 0.0f),
		m_horizontal(4.0f, 0.0f, 0.0f)
	{}

	Ray generate_ray(float u, float v) const 
	{
		return Ray(m_origin, m_lower_left_corner + u * m_horizontal + v * m_vertical - m_origin);
	}

private:
	glm::vec3 m_origin;
	glm::vec3 m_lower_left_corner;
	glm::vec3 m_horizontal;
	glm::vec3 m_vertical;
};

#endif