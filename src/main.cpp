#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include "stb_image_write.h"

static const char *IMG_PATH = "C:\\Users\\George\\Desktop\\img.png";

int main()
{
	int nx = 200;
	int ny = 100;
	std::vector<uint8_t> img(nx * ny * 3);
	size_t cc = 0;

	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			glm::vec3 color(float(i) / float(nx), float(j) / float(ny), 0.2f);
			img[cc++] = uint8_t(255.99f*color.r);
			img[cc++] = uint8_t(255.99f*color.g);
			img[cc++] = uint8_t(255.99f*color.b);
		}
	}

	stbi_write_png(IMG_PATH, nx, ny, 3, img.data(), 0);
	return 0;
}

