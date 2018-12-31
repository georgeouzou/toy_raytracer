#include <iostream>
#include <vector>
#include <limits>
#include <random>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include "stb_image_write.h"
#include "ray.h"
#include "hitable.h"
#include "camera.h"
#include "material.h"

static const char *IMG_PATH = "C:\\Users\\George\\Desktop\\img.png";

static glm::vec3 output_color(const Ray &r, const Hitable *world, int depth)
{
	HitRecord rec;
	if (world->hit(r, 0.001f, std::numeric_limits<float>::max(), rec)) {
		Ray scattered;
		glm::vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			return attenuation * output_color(scattered, world, depth + 1);
		} else {
			return glm::vec3(0.0f);
		}
	} else {
		glm::vec3 unit_dir = glm::normalize(r.direction());
		float t = 0.5f * (unit_dir.y + 1.0f);
		return (1.0f - t) * glm::vec3(1.0f, 1.0f, 1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
	}
}

int main()
{
	int nx = 200;
	int ny = 100;
	std::vector<uint8_t> img(nx * ny * 3);
	size_t idx = 0;

	Camera cam;
	std::shared_ptr<RandomGenerator<float>> rand_gen = std::make_shared<RandomGenerator<float>>();

	std::vector<std::shared_ptr<Material>> materials = {
		std::make_shared<Lambertian>(glm::vec3(0.8f, 0.3f, 0.3f), rand_gen),
		std::make_shared<Lambertian>(glm::vec3(0.8f, 0.8f, 0.0f), rand_gen),
		std::make_shared<Metal>(glm::vec3(0.8f, 0.6f, 0.2f), 1.0, rand_gen),
		std::make_shared<Metal>(glm::vec3(0.8f, 0.8f, 0.8f), 0.3, rand_gen),
		std::make_shared<Dielectric>(1.5f, rand_gen),
	};

	std::vector<std::unique_ptr<Hitable>> objects;
	objects.emplace_back(std::make_unique<Sphere>(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f, materials[0]));
	objects.emplace_back(std::make_unique<Sphere>(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f, materials[1]));
	objects.emplace_back(std::make_unique<Sphere>(glm::vec3(1.0f, 0.0f, -1.0f), 0.5f, materials[2]));
	objects.emplace_back(std::make_unique<Sphere>(glm::vec3(-1.0f, 0.0f, -1.0f), -0.45f, materials[4]));
	HitableList world(std::move(objects));

	const int num_samples = 100;

	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			glm::vec3 color(0.0f, 0.0f, 0.0f);
			for (int s = 0; s < num_samples; s++) {
				float u = (float(i) + rand_gen->gen()) / float(nx);
				float v = (float(j) + rand_gen->gen()) / float(ny);
				Ray ray = cam.generate_ray(u, v);
				color += output_color(ray, &world, 0);
			}
			// super sampling averaging
			color /= float(num_samples);
			// gamma correct it
			color = glm::vec3(glm::sqrt(color));
			img[idx++] = uint8_t(255.99f*color.r);
			img[idx++] = uint8_t(255.99f*color.g);
			img[idx++] = uint8_t(255.99f*color.b);
		}
	}

	stbi_write_png(IMG_PATH, nx, ny, 3, img.data(), 0);
	return 0;
}

