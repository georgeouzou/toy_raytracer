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
static const int DEPTH = 16;

static glm::vec3 output_color(const Ray &r, const Hitable *world, int depth, RandomGenerator<float> &generator)
{
	HitRecord rec;
	if (world->hit(r, 0.001f, std::numeric_limits<float>::max(), rec)) {
		Ray scattered;
		glm::vec3 attenuation;
		if (depth < DEPTH && rec.mat_ptr->scatter(r, rec, generator, attenuation, scattered)) {
			return attenuation * output_color(scattered, world, depth + 1, generator);
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

	glm::vec3 cam_pos(13.0f, 2.0f, 3.0f);
	glm::vec3 lookat(0.0f, 0.0f, 0.0f);
	float dist_to_focus = 10.0f; 
	float aperture = 0.1f;

	Camera cam(cam_pos, lookat, glm::vec3(0.0f, 1.0f, 0.0f),
		20, float(nx) / float(ny), aperture, dist_to_focus);

	std::vector<std::shared_ptr<Material>> materials = {
		std::make_shared<Dielectric>(1.5f),
		std::make_shared<Lambertian>(glm::vec3(0.4f, 0.2f, 0.1f)),
		std::make_shared<Metal>(glm::vec3(0.7, 0.6, 0.5), 0.0f),
		std::make_shared<Lambertian>(glm::vec3(0.5, 0.5, 0.5))
	};

	std::vector<std::unique_ptr<Hitable>> objects;
	objects.emplace_back(std::make_unique<Sphere>(glm::vec3(0.0f, -1000.0f, 0.0f), 1000.0f, materials[3]));
	objects.emplace_back(std::make_unique<Sphere>(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f, materials[0]));
	objects.emplace_back(std::make_unique<Sphere>(glm::vec3(-4.0f, 1.0f, 0.0f), 1.0f, materials[1]));
	objects.emplace_back(std::make_unique<Sphere>(glm::vec3(4.0f, 1.0f, 0.0f), 1.0f, materials[2]));

	RandomGenerator<float> rand;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			float choose_mat = rand.gen();
			glm::vec3 center(a + 0.9f*rand.gen(), 0.2f, b + 0.9f*rand.gen());
			if ((center - glm::vec3(4.0f, 0.2f, 0.0f)).length() > 0.9) {
				if (choose_mat < 0.8) { // diffuse
					objects.emplace_back(std::make_unique<Sphere>(
						center, 0.2f, std::make_shared<Lambertian>(
							glm::vec3(rand.gen()*rand.gen(), rand.gen()*rand.gen(), rand.gen()*rand.gen()))));
				} else if (choose_mat < 0.95) { // metal
					objects.emplace_back(std::make_unique<Sphere>(
						center, 0.2f, std::make_shared<Metal>(
							glm::vec3(0.5*(1 + rand.gen()), 0.5*(1 + rand.gen()), 0.5*(1 + rand.gen())), 0.5 *rand.gen())));
				} else { // glass
					objects.emplace_back(std::make_unique<Sphere>(
						center, 0.2f, std::make_shared<Dielectric>(1.5f)));
				}
			}
		}
	}
	HitableList world(std::move(objects));

	const int num_samples = 128;

    #pragma omp parallel
	{
		RandomGenerator<float> rand;
        #pragma omp for schedule(dynamic, 1)
		for (int j = 0; j < ny; j++) {
			for (int i = 0; i < nx; i++) {
				const int idx = (ny - 1 - j) * nx + i;
				glm::vec3 color(0.0f, 0.0f, 0.0f);
				for (int s = 0; s < num_samples; s++) {
					float u = (float(i) + rand.gen()) / float(nx);
					float v = (float(j) + rand.gen()) / float(ny);
					Ray ray = cam.generate_ray(u, v, rand);
					color += output_color(ray, &world, 0, rand);
				}
				// super sampling averaging
				color /= float(num_samples);
				// gamma correct it
				color = glm::vec3(glm::sqrt(color));
				img[3 * idx + 0] = uint8_t(255.99f*color.r);
				img[3 * idx + 1] = uint8_t(255.99f*color.g);
				img[3 * idx + 2] = uint8_t(255.99f*color.b);
			}
		}
	}

	stbi_write_png(IMG_PATH, nx, ny, 3, img.data(), 0);
	return 0;
}

