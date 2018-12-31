#include <iostream>
#include <vector>
#include <limits>
#include <random>

#include <glm/glm.hpp>
#include "stb_image_write.h"
#include "ray.h"
#include "hitable.h"
#include "camera.h"

static const char *IMG_PATH = "C:\\Users\\George\\Desktop\\img.png";

static glm::vec3 output_color(const Ray &r, const Hitable *world)
{
	HitRecord rec;
	if (world->hit(r, 0.0f, std::numeric_limits<float>::max(), rec)) {
		return 0.5f*(rec.normal + glm::vec3(1.0f));
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
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> dist(0.0f, 1.0f);

	std::vector<std::unique_ptr<Hitable>> objects;
	objects.emplace_back(std::make_unique<Sphere>(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f));
	objects.emplace_back(std::make_unique<Sphere>(glm::vec3(0.0f, -100.5f, -1.0f), 100));
	HitableList world(std::move(objects));

	const int num_samples = 100;

	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			glm::vec3 color(0.0f, 0.0f, 0.0f);
			for (int s = 0; s < num_samples; s++) {
				float u = (float(i) + dist(mt)) / float(nx);
				float v = (float(j) + dist(mt)) / float(ny);
				Ray ray = cam.generate_ray(u, v);
				color += output_color(ray, &world);
			}
			color /= float(num_samples);
			img[idx++] = uint8_t(255.99f*color.r);
			img[idx++] = uint8_t(255.99f*color.g);
			img[idx++] = uint8_t(255.99f*color.b);
		}
	}

	stbi_write_png(IMG_PATH, nx, ny, 3, img.data(), 0);
	return 0;
}

