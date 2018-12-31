#ifndef RANDOM_GENERATOR_H
#define RANDOM_GENERATOR_H

#include <random>
#include <glm/glm.hpp>

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

	inline T gen() { return m_distribution(m_engine); }

	inline glm::tvec3<T> random_in_unit_sphere()
	{
		glm::tvec3<T> p;
		do {
			p = 2.0f * glm::tvec3<T>(gen(), gen(), gen()) - glm::tvec3<T>(1);
		} while (glm::length2(p) >= T(1));
		return p;
	}

	inline glm::tvec3<T> random_in_unit_disk()
	{
		glm::tvec3<T> p;
		do {
			p = 2.0f * glm::tvec3<T>(gen(), gen(), T(0)) - glm::tvec3<T>(1, 1, 0);
		} while (glm::dot(p, p) >= T(1));
		return p;
	}

};

#endif //RANDOM_GENERATOR_H
