#ifndef CAMERA_H
#define CAMERA_H

#include <cmath>
#include <glm/glm.hpp>
#include "ray.h"

namespace detail 
{

float pi() { return std::atan(1.0f)*4.0f; }

}

class Camera
{
public:
	Camera(const glm::vec3 &position, const glm::vec3 &lookat, const glm::vec3 &up, float vfov, float aspect) //vfov is top to bottom in degrees
	{
		glm::vec3 u, v, w;
		float theta = vfov * detail::pi() / 180.0f;
		float half_height = std::tan(theta / 2.0f);
		float half_width = aspect * half_height;
		m_origin = position;
		w = glm::normalize(m_origin - lookat);
		u = glm::normalize(glm::cross(up, w));
		v = glm::cross(w, u);
		m_lower_left_corner = glm::vec3(-half_width, -half_height, -1.0f);
		m_lower_left_corner = m_origin - half_width * u - half_height * v - w;
		m_horizontal = 2.0f *half_width * u;
		m_vertical = 2.0f * half_height * v;
	}

	Ray generate_ray(float u, float v) const 
	{
		return Ray(m_origin, m_lower_left_corner + u * m_horizontal + v * m_vertical - m_origin);
	}

private:

private:
	glm::vec3 m_origin;
	glm::vec3 m_lower_left_corner;
	glm::vec3 m_horizontal;
	glm::vec3 m_vertical;
};

#endif