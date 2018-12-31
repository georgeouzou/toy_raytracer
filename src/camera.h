#ifndef CAMERA_H
#define CAMERA_H

#include <cmath>
#include <glm/glm.hpp>
#include "ray.h"
#include "random_generator.h"

namespace detail 
{

float pi() { return std::atan(1.0f)*4.0f; }

}

class Camera
{
public:
	Camera(const glm::vec3 &position, const glm::vec3 &lookat, const glm::vec3 &up, 
		float vfov, float aspect, float aperture, float focus_disk) //vfov is top to bottom in degrees
	{
		m_lens_radius = aperture / 2.0f;
		float theta = vfov * detail::pi() / 180.0f;
		float half_height = std::tan(theta / 2.0f);
		float half_width = aspect * half_height;
		m_origin = position;
		m_w = glm::normalize(position - lookat);
		m_u = glm::normalize(glm::cross(up, m_w));
		m_v = glm::cross(m_w, m_u);
		m_lower_left_corner = m_origin - half_width * focus_disk * m_u - half_height * focus_disk * m_v -  focus_disk * m_w;
		m_horizontal = 2.0f * half_width * focus_disk * m_u;
		m_vertical = 2.0f * half_height * focus_disk * m_v;
	}

	Ray generate_ray(float s, float t, RandomGenerator<float> &generator) const 
	{
		glm::vec3 rd = m_lens_radius * generator.random_in_unit_disk();
		glm::vec3 offset = m_u * rd.x + m_v * rd.y;
		return Ray(m_origin + offset, m_lower_left_corner + s * m_horizontal + t * m_vertical - m_origin - offset);
	}

private:

private:
	glm::vec3 m_origin;
	glm::vec3 m_lower_left_corner;
	glm::vec3 m_horizontal;
	glm::vec3 m_vertical;
	glm::vec3 m_u, m_v, m_w;
	float m_lens_radius;
};

#endif