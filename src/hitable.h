#ifndef HITABLE_H
#define HITABLE_H

#include <vector>
#include <memory>
#include "ray.h"

class Material;

struct HitRecord 
{
	float t;
	glm::vec3 p;
	glm::vec3 normal;
	Material *mat_ptr;
};

class Hitable
{
public:
	virtual bool hit(const Ray &r, float tmin, float tmax, HitRecord &rec) const = 0;
};

class Sphere : public Hitable
{
public:
	Sphere(glm::vec3 c, float r, std::shared_ptr<Material> mat) : m_center(c), m_radius(r), m_material(mat) {}
	virtual bool hit(const Ray &r, float tmin, float tmax, HitRecord &rec) const override;

private:
	glm::vec3 m_center;
	float m_radius;
	std::shared_ptr<Material> m_material;
};

bool Sphere::hit(const Ray &r, float tmin, float tmax, HitRecord &rec) const
{
	// ray = A + t*B
    // t*t*dot(B,B) + 2*t*dot(B,A-C) + dot(A-C,A-C) - R*R = 0
	glm::vec3 oc = r.origin() - m_center;
	float a = glm::dot(r.direction(), r.direction());
	float b = glm::dot(oc, r.direction());
	float c = glm::dot(oc, oc) - m_radius * m_radius;
	float discr = b * b - a*c;
	if (discr > 0) {
		float temp = (-b - std::sqrt(discr)) / a;
		if (temp < tmax && temp > tmin) {
			rec.t = temp;
			rec.p = r.pt(temp);
			rec.normal = (rec.p - m_center) / m_radius;
			rec.mat_ptr = m_material.get();
			return true;
		}
		temp = (-b + std::sqrt(discr)) / a;
		if (temp < tmax && temp > tmin) {
			rec.t = temp;
			rec.p = r.pt(temp);
			rec.normal = (rec.p - m_center) / m_radius;
			rec.mat_ptr = m_material.get();
			return true;
		}
	}
	return false;
}

class HitableList: public Hitable
{
public:
	HitableList(std::vector<std::unique_ptr<Hitable>> hitables) : m_hitables(std::move(hitables)) {}
	virtual bool hit(const Ray &r, float tmin, float tmax, HitRecord &rec) const override;

private:
	std::vector<std::unique_ptr<Hitable>> m_hitables;
};

bool HitableList::hit(const Ray &r, float tmin, float tmax, HitRecord &rec) const
{
	HitRecord temp_rec;
	bool hit_any = false;
	float closest_so_far = tmax;
	for (size_t i = 0; i < m_hitables.size(); ++i) {
		if (m_hitables[i]->hit(r, tmin, closest_so_far, temp_rec)) {
			hit_any = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}
	return hit_any;
}



#endif