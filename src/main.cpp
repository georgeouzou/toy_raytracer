#include <iostream>
#include <vector>
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
			float r = float(i) / float(nx);
			float g = float(j) / float(ny);
			float b = 0.2f;
			img[cc++] = uint8_t(255.99f*r);
			img[cc++] = uint8_t(255.99f*g);
			img[cc++] = uint8_t(255.99f*b);
		}
	}

	stbi_write_png(IMG_PATH, nx, ny, 3, img.data(), 0);
	return 0;
}

