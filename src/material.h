#ifndef MATERIAL_H
#define MATERIAL_H

#include <memory>
#include <random>
#include <glm/glm.hpp>
#include "ray.h"
#include "hitable.h"

template<typename T>
class RandomGenerator
{
private:
	std::random_device m_rd;
	std::mt19937 m_engine;
	std::uniform_real_distribution<T> m_distribution;

public:
	RandomGenerator(T min = T(0), T max = T(1)) :
		m_engine(m_rd()),
		m_distribution(min, max)
	{}

	T gen() { return m_distribution(m_engine); }
};


namespace detail
{

static inline glm::vec3 random_in_unit_sphere(RandomGenerator<float> *generator)
{
	glm::vec3 p;
	do {
		p = 2.0f * glm::vec3(generator->gen(), generator->gen(), generator->gen()) - glm::vec3(1.0f);
	} while (glm::length2(p) >= 1.0f);
	return p;
}

}

class Material
{
public:
	virtual bool scatter(const Ray &ray_in, const HitRecord &rec, 
		glm::vec3 &attenuation, Ray &scattered) const = 0;
};


class Lambertian : public Material
{
public:
	Lambertian(const glm::vec3 &a, std::shared_ptr<RandomGenerator<float>> generator)
		: m_albedo(a), m_generator(generator) {}

	virtual bool scatter(const Ray &ray_in, const HitRecord &rec,
		glm::vec3 &attenuation, Ray &scattered) const override
	{
		glm::vec3 target = rec.p + rec.normal + detail::random_in_unit_sphere(m_generator.get());
		scattered = Ray(rec.p, target - rec.p);
		attenuation = m_albedo;
		return true;
	}

private:
	glm::vec3 m_albedo;
	std::shared_ptr<RandomGenerator<float>> m_generator;
};

class Metal : public Material
{
public:
	Metal(const glm::vec3 &a, float fuzz, std::shared_ptr<RandomGenerator<float>> generator) 
		: m_albedo(a), m_fuzz(fuzz), m_generator(generator) {}

	virtual bool scatter(const Ray &ray_in, const HitRecord &rec,
		glm::vec3 &attenuation, Ray &scattered) const override
	{
		glm::vec3 reflected = glm::reflect(glm::normalize(ray_in.direction()), rec.normal);
		scattered = Ray(rec.p, reflected + m_fuzz * detail::random_in_unit_sphere(m_generator.get()));
		attenuation = m_albedo;
		return (glm::dot(scattered.direction(), rec.normal) > 0.0f);
	}

private:
	glm::vec3 m_albedo;
	float m_fuzz;
	std::shared_ptr<RandomGenerator<float>> m_generator;
};


#endif