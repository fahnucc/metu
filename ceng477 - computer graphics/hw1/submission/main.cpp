// Anıl Berdan Ceylan 2304277
// Fahri Numan Eskici 2309995

#include <iostream>
#include "parser.h"
#include "ppm.h"
#include <cmath>
#include <thread>
#include <limits>

using namespace parser;
using namespace std;

void raytracer(Scene scene, Camera cam, unsigned char *image, int minWidth, int maxWidth, int width, int height)
{
	for (int j = 0; j < height; j++)
	{
		for (int i = minWidth; i < maxWidth; i++)
		{
			int pixelNumber = 3 * (j * width + i);

			// Generate ray
			Ray ray = cam.generateRay(j, i);

			// Find hit result
			Hit hitResult = scene.calculateHit(ray);

			Vec3f color;

			// If hit happened
			if (hitResult.trueHit)
				color = scene.coloring(hitResult, cam, ray, scene.max_recursion_depth);
			// If not, assign color to background color.
			else
			{
				color = Vec3f(scene.background_color.x,
							  scene.background_color.y,
							  scene.background_color.z);
			}

			image[pixelNumber++] = round(color.x);
			image[pixelNumber++] = round(color.y);
			image[pixelNumber++] = round(color.z);
		}
	}
};

int main(int argc, char *argv[])
{
	Scene scene;
	scene.loadFromXml(argv[1]);

	// Create image for each camera
	for (int cameraId = 0; cameraId < scene.cameras.size(); cameraId++)
	{
		Camera cam = scene.cameras[cameraId];
		int width = cam.image_width;
		int height = cam.image_height;

		unsigned char *image = new unsigned char[3 * width * height];

		const auto processor_count = thread::hardware_concurrency();

		if (processor_count == 0)
			raytracer(scene, cam, image, 0, width, width, height);
		else
		{
			thread *threads = new thread[processor_count];
			for (int i = 0; i < processor_count; i++)
			{
				int minWidth = (width / processor_count) * i;
				int maxWidth = minWidth + (width / processor_count);
				threads[i] = thread(raytracer, scene, cam, image, minWidth, maxWidth, width, height);
			}
			for (int i = 0; i < processor_count; i++)
				threads[i].join();
		}

		write_ppm(cam.image_name.c_str(), image, width, height);
	}
}