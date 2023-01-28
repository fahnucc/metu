// Anil Berdan Ceylan 2304277
// Fahri Numan Eskici 2309995

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <cmath>

#include "Scene.h"
#include "Camera.h"
#include "Color.h"
#include "Mesh.h"
#include "Rotation.h"
#include "Scaling.h"
#include "Translation.h"
#include "Triangle.h"
#include "Vec3.h"
#include "tinyxml2.h"
#include "Helpers.h"

using namespace tinyxml2;
using namespace std;

Vec3 Scene::viewingTransformations(Camera *camera, Vec3 *point)
{
	Matrix4 cameraTransformation = camera->getCameraTransformation();
	Matrix4 projectionMatrix = camera->getProjectionTransMatrix();
	Matrix4 viewPort = camera->getViewportMatrix();

	Vec4 realWorld = Vec4(point->x, point->y, point->z, 1, 0);
	Vec4 realToCamera = multiplyMatrixWithVec4(cameraTransformation, realWorld);

	Vec4 cameraToProjection = multiplyMatrixWithVec4(projectionMatrix, realToCamera);
	if (camera->projectionType)
		cameraToProjection = cameraToProjection / cameraToProjection.t;

	Vec4 viewportTrans = multiplyMatrixWithVec4(viewPort, cameraToProjection);

	return Vec3(round(viewportTrans.x), round(viewportTrans.y), round(viewportTrans.z), -1);
}

void Scene::coloring(Vec3 first, Vec3 second, Vec3 third, int horSize, int verSize)
{
	int x1, x2, y1, y2;
	double d;
	Color *color1, *color2;
	for (int mid = 0; mid < 3; mid++)
	{
		switch (mid)
		{
		case 0:
			x1 = (int)first.x;
			x2 = (int)second.x;
			y1 = (int)first.y;
			y2 = (int)second.y;
			d = (second.y - first.y) / (second.x - first.x);
			color1 = colorsOfVertices[first.colorId - 1];
			color2 = colorsOfVertices[second.colorId - 1];
			break;
		case 1:
			x1 = (int)second.x;
			x2 = (int)third.x;
			y1 = (int)second.y;
			y2 = (int)third.y;
			d = (third.y - second.y) / (third.x - second.x);
			color1 = colorsOfVertices[second.colorId - 1];
			color2 = colorsOfVertices[third.colorId - 1];
			break;
		case 2:
			x1 = (int)third.x;
			x2 = (int)first.x;
			y1 = (int)third.y;
			y2 = (int)first.y;
			d = (first.y - third.y) / (first.x - third.x);
			color1 = colorsOfVertices[third.colorId - 1];
			color2 = colorsOfVertices[first.colorId - 1];
			break;
		default:
			break;
		}

		if (d < (-1))
		{
			if (x1 > x2)
			{
				int x = x1;
				int d = 2 * (x2 - x1) + (y2 - y1);
				double rr = (abs(color1->r - color2->r)) / (y2 - y1);
				double rg = (abs(color1->g - color2->g)) / (y2 - y1);
				double rb = (abs(color1->b - color2->b)) / (y2 - y1);
				for (int i = 0; i <= (y2 - y1); i++)
				{
					if (x >= 0 && x < horSize && y1 + i >= 0 && y1 + i < verSize)
					{
						if (color1->r < color2->r)
							image[x][y1 + i].r = color1->r + (i * rr);
						else
							image[x][y1 + i].r = color1->r - (i * rr);

						if (color1->g < color2->g)
							image[x][y1 + i].g = color1->g + (i * rg);
						else
							image[x][y1 + i].g = color1->g - (i * rg);

						if (color1->b < color2->b)
							image[x][y1 + i].b = color1->b + (i * rb);
						else
							image[x][y1 + i].b = color1->b - (i * rb);
					}
					if (d < 0)
					{
						x -= 1;
						d += 2 * ((x2 - x1) + (y2 - y1));
					}
					else
						d += 2 * (x2 - x1);
				}
			}
			else if (x2 > x1)
			{
				int x = x1;
				int d = 2 * (x1 - x2) + (y1 - y2);
				double rr = (abs(color1->r - color2->r)) / (y1 - y2);
				double rg = (abs(color1->g - color2->g)) / (y1 - y2);
				double rb = (abs(color1->b - color2->b)) / (y1 - y2);
				for (int i = 0; i <= (y1 - y2); i++)
				{
					if (x >= 0 && x < horSize && y1 - i >= 0 && y1 - i < verSize)
					{
						if (color1->r < color2->r)
							image[x][y1 - i].r = color1->r + (i * rr);
						else
							image[x][y1 - i].r = color1->r - (i * rr);

						if (color1->g < color2->g)
							image[x][y1 - i].g = color1->g + (i * rg);
						else
							image[x][y1 - i].g = color1->g - (i * rg);

						if (color1->b < color2->b)
							image[x][y1 - i].b = color1->b + (i * rb);
						else
							image[x][y1 - i].b = color1->b - (i * rb);
					}
					if (d < 0)
					{
						x += 1;
						d += 2 * ((x1 - x2) + (y1 - y2));
					}
					else
						d += 2 * (x1 - x2);
				}
			}
		}
		else if (d == 0)
		{
			if (x1 > x2)
			{
				double rr = (abs(color1->r - color2->r)) / (x1 - x2);
				double rg = (abs(color1->g - color2->g)) / (x1 - x2);
				double rb = (abs(color1->b - color2->b)) / (x1 - x2);
				for (int i = 0; i <= (x1 - x2); i++)
				{
					if (x1 - i >= 0 && x1 - i < horSize && y1 >= 0 && y1 < verSize)
					{
						if (color1->r < color2->r)
							image[x1 - i][y1].r = color1->r + (i * rr);
						else
							image[x1 - i][y1].r = color1->r - (i * rr);

						if (color1->g < color2->g)
							image[x1 - i][y1].g = color1->g + (i * rg);
						else
							image[x1 - i][y1].g = color1->g - (i * rg);

						if (color1->b < color2->b)
							image[x1 - i][y1].b = color1->b + (i * rb);
						else
							image[x1 - i][y1].b = color1->b - (i * rb);
					}
				}
			}
			else if (x2 > x1)
			{
				double rr = (abs(color1->r - color2->r)) / (x2 - x1);
				double rg = (abs(color1->g - color2->g)) / (x2 - x1);
				double rb = (abs(color1->b - color2->b)) / (x2 - x1);
				for (int i = 0; i <= (x2 - x1); i++)
				{
					if (x1 + i >= 0 && x1 + i < horSize && y1 >= 0 && y1 < verSize)
					{
						if (color1->r < color2->r)
							image[x1 + i][y1].r = color1->r + (i * rr);
						else
							image[x1 + i][y1].r = color1->r - (i * rr);

						if (color1->g < color2->g)
							image[x1 + i][y1].g = color1->g + (i * rg);
						else
							image[x1 + i][y1].g = color1->g - (i * rg);

						if (color1->b < color2->b)
							image[x1 + i][y1].b = color1->b + (i * rb);
						else
							image[x1 + i][y1].b = color1->b - (i * rb);
					}
				}
			}
		}
		else if (d > 1)
		{
			if (x1 > x2)
			{
				int x = x1;
				int d = 2 * (x1 - x2) + (y2 - y1);
				double rr = (abs(color1->r - color2->r)) / (y1 - y2);
				double rg = (abs(color1->g - color2->g)) / (y1 - y2);
				double rb = (abs(color1->b - color2->b)) / (y1 - y2);
				for (int i = 0; i <= (y1 - y2); i++)
				{
					if (x >= 0 && x < horSize && y1 - i >= 0 && y1 - i < verSize)
					{
						if (color1->r < color2->r)
							image[x][y1 - i].r = color1->r + (i * rr);
						else
							image[x][y1 - i].r = color1->r - (i * rr);

						if (color1->g < color2->g)
							image[x][y1 - i].g = color1->g + (i * rg);
						else
							image[x][y1 - i].g = color1->g - (i * rg);

						if (color1->b < color2->b)
							image[x][y1 - i].b = color1->b + (i * rb);
						else
							image[x][y1 - i].b = color1->b - (i * rb);
					}
					if (d < 0)
						d += 2 * (x1 - x2);
					else
					{
						x -= 1;
						d += 2 * ((x1 - x2) + (y2 - y1));
					}
				}
			}
			else if (x2 > x1)
			{
				int x = x1;
				int d = 2 * (x2 - x1) + (y1 - y2);
				double rr = (abs(color1->r - color2->r)) / (y2 - y1);
				double rg = (abs(color1->g - color2->g)) / (y2 - y1);
				double rb = (abs(color1->b - color2->b)) / (y2 - y1);
				for (int i = 0; i <= (y2 - y1); i++)
				{
					if (x >= 0 && x < horSize && y1 + i >= 0 && y1 + i < verSize)
					{
						if (color1->r < color2->r)
							image[x][y1 + i].r = color1->r + (i * rr);
						else
							image[x][y1 + i].r = color1->r - (i * rr);

						if (color1->g < color2->g)
							image[x][y1 + i].g = color1->g + (i * rg);
						else
							image[x][y1 + i].g = color1->g - (i * rg);

						if (color1->b < color2->b)
							image[x][y1 + i].b = color1->b + (i * rb);
						else
							image[x][y1 + i].b = color1->b - (i * rb);
					}
					if (d < 0)
						d += 2 * (x2 - x1);
					else
					{
						x += 1;
						d += 2 * ((x2 - x1) + (y1 - y2));
					}
				}
			}
		}
		else if (x1 == x2)
		{
			if (y1 > y2)
			{
				double rr = (abs(color1->r - color2->r)) / (y1 - y2);
				double rg = (abs(color1->g - color2->g)) / (y1 - y2);
				double rb = (abs(color1->b - color2->b)) / (y1 - y2);
				for (int i = 0; i <= (y1 - y2); i++)
				{
					if (x1 >= 0 && x1 < horSize && y1 - i >= 0 && y1 - i < verSize)
					{
						if (color1->r < color2->r)
							image[x1][y1 - i].r = color1->r + (i * rr);
						else
							image[x1][y1 - i].r = color1->r - (i * rr);

						if (color1->g < color2->g)
							image[x1][y1 - i].g = color1->g + (i * rg);
						else
							image[x1][y1 - i].g = color1->g - (i * rg);

						if (color1->b < color2->b)
							image[x1][y1 - i].b = color1->b + (i * rb);
						else
							image[x1][y1 - i].b = color1->b - (i * rb);
					}
				}
			}
			else if (y2 > y1)
			{
				double rr = (abs(color1->r - color2->r)) / (y2 - y1);
				double rg = (abs(color1->g - color2->g)) / (y2 - y1);
				double rb = (abs(color1->b - color2->b)) / (y2 - y1);
				for (int i = 0; i <= (y2 - y1); i++)
				{
					if (x1 >= 0 && x1 < horSize && y1 + i >= 0 && y1 + i < verSize)
					{
						if (color1->r < color2->r)
							image[x1][y1 + i].r = color1->r + (i * rr);
						else
							image[x1][y1 + i].r = color1->r - (i * rr);

						if (color1->g < color2->g)
							image[x1][y1 + i].g = color1->g + (i * rg);
						else
							image[x1][y1 + i].g = color1->g - (i * rg);

						if (color1->b < color2->b)
							image[x1][y1 + i].b = color1->b + (i * rb);
						else
							image[x1][y1 + i].b = color1->b - (i * rb);
					}
				}
			}
		}
		else if (d > 0 && d <= 1)
		{
			if (y1 > y2)
			{
				int y = y1;
				int d = 2 * (y2 - y1) + (x1 - x2);
				double rr = (abs(color1->r - color2->r)) / (x1 - x2);
				double rg = (abs(color1->g - color2->g)) / (x1 - x2);
				double rb = (abs(color1->b - color2->b)) / (x1 - x2);
				for (int i = 0; i <= (x1 - x2); i++)
				{
					if (x1 - i >= 0 && x1 - i < horSize && y >= 0 && y < verSize)
					{
						if (color1->r < color2->r)
							image[x1 - i][y].r = color1->r + (i * rr);
						else
							image[x1 - i][y].r = color1->r - (i * rr);

						if (color1->g < color2->g)
							image[x1 - i][y].g = color1->g + (i * rg);
						else
							image[x1 - i][y].g = color1->g - (i * rg);

						if (color1->b < color2->b)
							image[x1 - i][y].b = color1->b + (i * rb);
						else
							image[x1 - i][y].b = color1->b - (i * rb);
					}
					if (d < 0)
					{
						y -= 1;
						d += 2 * ((y2 - y1) + (x1 - x2));
					}
					else
						d += 2 * (y2 - y1);
				}
			}
			else if (y2 > y1)
			{
				int y = y1;
				int d = 2 * (y1 - y2) + (x2 - x1);
				double rr = (abs(color1->r - color2->r)) / (x2 - x1);
				double rg = (abs(color1->g - color2->g)) / (x2 - x1);
				double rb = (abs(color1->b - color2->b)) / (x2 - x1);
				for (int i = 0; i <= (x2 - x1); i++)
				{
					if (x1 + i >= 0 && x1 + i < horSize && y >= 0 && y < verSize)
					{
						if (color1->r < color2->r)
							image[x1 + i][y].r = color1->r + (i * rr);
						else
							image[x1 + i][y].r = color1->r - (i * rr);

						if (color1->g < color2->g)
							image[x1 + i][y].g = color1->g + (i * rg);
						else
							image[x1 + i][y].g = color1->g - (i * rg);

						if (color1->b < color2->b)
							image[x1 + i][y].b = color1->b + (i * rb);
						else
							image[x1 + i][y].b = color1->b - (i * rb);
					}
					if (d < 0)
					{
						y += 1;
						d += 2 * ((y1 - y2) + (x2 - x1));
					}
					else
						d += 2 * (y1 - y2);
				}
			}
		}
		else if (d >= (-1) && d < 0)
		{
			if (x1 > x2)
			{
				int y = y1;
				int d = 2 * (y2 - y1) + (x2 - x1);
				double rr = (abs(color1->r - color2->r)) / (x1 - x2);
				double rg = (abs(color1->g - color2->g)) / (x1 - x2);
				double rb = (abs(color1->b - color2->b)) / (x1 - x2);
				for (int i = 0; i <= (x1 - x2); i++)
				{
					if (x1 - i >= 0 && x1 - i < horSize && y >= 0 && y < verSize)
					{
						if (color1->r < color2->r)
							image[x1 - i][y].r = color1->r + (i * rr);
						else
							image[x1 - i][y].r = color1->r - (i * rr);

						if (color1->g < color2->g)
							image[x1 - i][y].g = color1->g + (i * rg);
						else
							image[x1 - i][y].g = color1->g - (i * rg);

						if (color1->b < color2->b)
							image[x1 - i][y].b = color1->b + (i * rb);
						else
							image[x1 - i][y].b = color1->b - (i * rb);
					}
					if (d < 0)
						d += 2 * (y2 - y1);
					else
					{
						y += 1;
						d += 2 * ((y2 - y1) + (x2 - x1));
					}
				}
			}
			else if (x2 > x1)
			{
				int y = y1;
				int d = 2 * (y1 - y2) + (x1 - x2);
				double rr = (abs(color1->r - color2->r)) / (x2 - x1);
				double rg = (abs(color1->g - color2->g)) / (x2 - x1);
				double rb = (abs(color1->b - color2->b)) / (x2 - x1);
				for (int i = 0; i <= (x2 - x1); i++)
				{
					if (x1 + i >= 0 && x1 + i < horSize && y >= 0 && y < verSize)
					{
						if (color1->r < color2->r)
							image[x1 + i][y].r = color1->r + (i * rr);
						else
							image[x1 + i][y].r = color1->r - (i * rr);

						if (color1->g < color2->g)
							image[x1 + i][y].g = color1->g + (i * rg);
						else
							image[x1 + i][y].g = color1->g - (i * rg);

						if (color1->b < color2->b)
							image[x1 + i][y].b = color1->b + (i * rb);
						else
							image[x1 + i][y].b = color1->b - (i * rb);
					}
					if (d < 0)
						d += 2 * (y1 - y2);

					else
					{
						y -= 1;
						d += 2 * ((y1 - y2) + (x1 - x2));
					}
				}
			}
		}
	}
}

/*
	Transformations, clipping, culling, rasterization are done here.
	You may define helper functions.
*/
void Scene::forwardRenderingPipeline(Camera *camera)
{
	int horSize = camera->horRes;
	int verSize = camera->verRes;

	for (auto curMesh : meshes) // each mesh
	{
		int triangleNumber = curMesh->numberOfTriangles;
		int transformationNumber = curMesh->numberOfTransformations;

		for (int i = 0; i < triangleNumber; i++) // each triangle
		{
			Vec3 *p1 = vertices[curMesh->triangles[i].getFirstVertexId() - 1];
			Vec3 *p2 = vertices[curMesh->triangles[i].getSecondVertexId() - 1];
			Vec3 *p3 = vertices[curMesh->triangles[i].getThirdVertexId() - 1];

			for (int k = 0; k < transformationNumber; k++) // apply transformations
			{
				switch (curMesh->transformationTypes[k])
				{
				case 't':
					p3 = p3->translate(translations[curMesh->transformationIds[k] - 1]);
					p2 = p2->translate(translations[curMesh->transformationIds[k] - 1]);
					p1 = p1->translate(translations[curMesh->transformationIds[k] - 1]);
					break;
				case 'r':
					p3 = p3->rotate(rotations[curMesh->transformationIds[k] - 1]);
					p2 = p2->rotate(rotations[curMesh->transformationIds[k] - 1]);
					p1 = p1->rotate(rotations[curMesh->transformationIds[k] - 1]);
					break;
				case 's':
					p3 = p3->scale(scalings[curMesh->transformationIds[k] - 1]);
					p2 = p2->scale(scalings[curMesh->transformationIds[k] - 1]);
					p1 = p1->scale(scalings[curMesh->transformationIds[k] - 1]);
					break;
				default:
					break;
				}
			}

			if (cullingEnabled)
			{
				Vec3 mid = (*p1 + *p2 + *p3) / 3;
				Vec3 vector = mid - camera->pos;

				if (isBackfaceCulled(p1, p2, p3, vector) == false)
					continue;
			}

			Vec3 res1 = Vec3(viewingTransformations(camera, p1), p1->colorId);
			Vec3 res2 = Vec3(viewingTransformations(camera, p2), p2->colorId);
			Vec3 res3 = Vec3(viewingTransformations(camera, p3), p3->colorId);

			if (curMesh->type == 0) // wireframe
				coloring(res1, res2, res3, horSize, verSize);
			else // solid
			{
				int minx = min(res1.x, min(res2.x, res3.x));
				int maxx = max(res1.x, max(res2.x, res3.x));
				int miny = min(res1.y, min(res2.y, res3.y));
				int maxy = max(res1.y, max(res2.y, res3.y));

				maxx = max(maxx, camera->horRes);
				maxy = max(maxy, camera->verRes);

				minx = (minx < 0) ? 0 : minx;
				miny = (miny < 0) ? 0 : miny;

				for (int j = miny; j < maxy; j++)
				{
					for (int i = minx; i < maxx; i++)
					{
						Vec3 point;
						point.x = i;
						point.y = j;

						double alpha = triangleRasterizer(res2, res3, res1, point);
						double beta = triangleRasterizer(res3, res1, res2, point);
						double gamma = triangleRasterizer(res1, res2, res3, point);

						if (alpha >= 0 && beta >= 0 && gamma >= 0 && alpha <= 1 && beta <= 1 && gamma <= 1)
						{
							Color color = colorMaker(alpha, beta, gamma, colorsOfVertices[res1.colorId - 1], colorsOfVertices[res2.colorId - 1], colorsOfVertices[res3.colorId - 1]);
							image[i][j].r = color.r;
							image[i][j].g = color.g;
							image[i][j].b = color.b;
						}
					}
				}
			}
		}
	}
}

/*
	Parses XML file
*/
Scene::Scene(const char *xmlPath)
{
	const char *str;
	XMLDocument xmlDoc;
	XMLElement *pElement;

	xmlDoc.LoadFile(xmlPath);

	XMLNode *pRoot = xmlDoc.FirstChild();

	// read background color
	pElement = pRoot->FirstChildElement("BackgroundColor");
	str = pElement->GetText();
	sscanf(str, "%lf %lf %lf", &backgroundColor.r, &backgroundColor.g, &backgroundColor.b);

	// read culling
	pElement = pRoot->FirstChildElement("Culling");
	if (pElement != NULL)
	{
		str = pElement->GetText();

		if (strcmp(str, "enabled") == 0)
		{
			cullingEnabled = true;
		}
		else
		{
			cullingEnabled = false;
		}
	}

	// read cameras
	pElement = pRoot->FirstChildElement("Cameras");
	XMLElement *pCamera = pElement->FirstChildElement("Camera");
	XMLElement *camElement;
	while (pCamera != NULL)
	{
		Camera *cam = new Camera();

		pCamera->QueryIntAttribute("id", &cam->cameraId);

		// read projection type
		str = pCamera->Attribute("type");

		if (strcmp(str, "orthographic") == 0)
		{
			cam->projectionType = 0;
		}
		else
		{
			cam->projectionType = 1;
		}

		camElement = pCamera->FirstChildElement("Position");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf", &cam->pos.x, &cam->pos.y, &cam->pos.z);

		camElement = pCamera->FirstChildElement("Gaze");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf", &cam->gaze.x, &cam->gaze.y, &cam->gaze.z);

		camElement = pCamera->FirstChildElement("Up");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf", &cam->v.x, &cam->v.y, &cam->v.z);

		cam->gaze = normalizeVec3(cam->gaze);
		cam->u = crossProductVec3(cam->gaze, cam->v);
		cam->u = normalizeVec3(cam->u);

		cam->w = inverseVec3(cam->gaze);
		cam->v = crossProductVec3(cam->u, cam->gaze);
		cam->v = normalizeVec3(cam->v);

		camElement = pCamera->FirstChildElement("ImagePlane");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf %lf %lf %lf %d %d",
			   &cam->left, &cam->right, &cam->bottom, &cam->top,
			   &cam->near, &cam->far, &cam->horRes, &cam->verRes);

		camElement = pCamera->FirstChildElement("OutputName");
		str = camElement->GetText();
		cam->outputFileName = string(str);

		cameras.push_back(cam);

		pCamera = pCamera->NextSiblingElement("Camera");
	}

	// read vertices
	pElement = pRoot->FirstChildElement("Vertices");
	XMLElement *pVertex = pElement->FirstChildElement("Vertex");
	int vertexId = 1;

	while (pVertex != NULL)
	{
		Vec3 *vertex = new Vec3();
		Color *color = new Color();

		vertex->colorId = vertexId;

		str = pVertex->Attribute("position");
		sscanf(str, "%lf %lf %lf", &vertex->x, &vertex->y, &vertex->z);

		str = pVertex->Attribute("color");
		sscanf(str, "%lf %lf %lf", &color->r, &color->g, &color->b);

		vertices.push_back(vertex);
		colorsOfVertices.push_back(color);

		pVertex = pVertex->NextSiblingElement("Vertex");

		vertexId++;
	}

	// read translations
	pElement = pRoot->FirstChildElement("Translations");
	XMLElement *pTranslation = pElement->FirstChildElement("Translation");
	while (pTranslation != NULL)
	{
		Translation *translation = new Translation();

		pTranslation->QueryIntAttribute("id", &translation->translationId);

		str = pTranslation->Attribute("value");
		sscanf(str, "%lf %lf %lf", &translation->tx, &translation->ty, &translation->tz);

		translations.push_back(translation);

		pTranslation = pTranslation->NextSiblingElement("Translation");
	}

	// read scalings
	pElement = pRoot->FirstChildElement("Scalings");
	XMLElement *pScaling = pElement->FirstChildElement("Scaling");
	while (pScaling != NULL)
	{
		Scaling *scaling = new Scaling();

		pScaling->QueryIntAttribute("id", &scaling->scalingId);
		str = pScaling->Attribute("value");
		sscanf(str, "%lf %lf %lf", &scaling->sx, &scaling->sy, &scaling->sz);

		scalings.push_back(scaling);

		pScaling = pScaling->NextSiblingElement("Scaling");
	}

	// read rotations
	pElement = pRoot->FirstChildElement("Rotations");
	XMLElement *pRotation = pElement->FirstChildElement("Rotation");
	while (pRotation != NULL)
	{
		Rotation *rotation = new Rotation();

		pRotation->QueryIntAttribute("id", &rotation->rotationId);
		str = pRotation->Attribute("value");
		sscanf(str, "%lf %lf %lf %lf", &rotation->angle, &rotation->ux, &rotation->uy, &rotation->uz);

		rotations.push_back(rotation);

		pRotation = pRotation->NextSiblingElement("Rotation");
	}

	// read meshes
	pElement = pRoot->FirstChildElement("Meshes");

	XMLElement *pMesh = pElement->FirstChildElement("Mesh");
	XMLElement *meshElement;
	while (pMesh != NULL)
	{
		Mesh *mesh = new Mesh();

		pMesh->QueryIntAttribute("id", &mesh->meshId);

		// read projection type
		str = pMesh->Attribute("type");

		if (strcmp(str, "wireframe") == 0)
		{
			mesh->type = 0;
		}
		else
		{
			mesh->type = 1;
		}

		// read mesh transformations
		XMLElement *pTransformations = pMesh->FirstChildElement("Transformations");
		XMLElement *pTransformation = pTransformations->FirstChildElement("Transformation");

		while (pTransformation != NULL)
		{
			char transformationType;
			int transformationId;

			str = pTransformation->GetText();
			sscanf(str, "%c %d", &transformationType, &transformationId);

			mesh->transformationTypes.push_back(transformationType);
			mesh->transformationIds.push_back(transformationId);

			pTransformation = pTransformation->NextSiblingElement("Transformation");
		}

		mesh->numberOfTransformations = mesh->transformationIds.size();

		// read mesh faces
		char *row;
		char *clone_str;
		int v1, v2, v3;
		XMLElement *pFaces = pMesh->FirstChildElement("Faces");
		str = pFaces->GetText();
		clone_str = strdup(str);

		row = strtok(clone_str, "\n");
		while (row != NULL)
		{
			int result = sscanf(row, "%d %d %d", &v1, &v2, &v3);

			if (result != EOF)
			{
				mesh->triangles.push_back(Triangle(v1, v2, v3));
			}
			row = strtok(NULL, "\n");
		}
		mesh->numberOfTriangles = mesh->triangles.size();
		meshes.push_back(mesh);

		pMesh = pMesh->NextSiblingElement("Mesh");
	}
}

/*
	Initializes image with background color
*/
void Scene::initializeImage(Camera *camera)
{
	if (this->image.empty())
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			vector<Color> rowOfColors;

			for (int j = 0; j < camera->verRes; j++)
			{
				rowOfColors.push_back(this->backgroundColor);
			}

			this->image.push_back(rowOfColors);
		}
	}
	else
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			for (int j = 0; j < camera->verRes; j++)
			{
				this->image[i][j].r = this->backgroundColor.r;
				this->image[i][j].g = this->backgroundColor.g;
				this->image[i][j].b = this->backgroundColor.b;
			}
		}
	}
}

/*
	If given value is less than 0, converts value to 0.
	If given value is more than 255, converts value to 255.
	Otherwise returns value itself.
*/
int Scene::makeBetweenZeroAnd255(double value)
{
	if (value >= 255.0)
		return 255;
	if (value <= 0.0)
		return 0;
	return (int)(value);
}

/*
	Writes contents of image (Color**) into a PPM file.
*/
void Scene::writeImageToPPMFile(Camera *camera)
{
	ofstream fout;

	fout.open(camera->outputFileName.c_str());

	fout << "P3" << endl;
	fout << "# " << camera->outputFileName << endl;
	fout << camera->horRes << " " << camera->verRes << endl;
	fout << "255" << endl;

	for (int j = camera->verRes - 1; j >= 0; j--)
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			fout << makeBetweenZeroAnd255(this->image[i][j].r) << " "
				 << makeBetweenZeroAnd255(this->image[i][j].g) << " "
				 << makeBetweenZeroAnd255(this->image[i][j].b) << " ";
		}
		fout << endl;
	}
	fout.close();
}

/*
	Converts PPM image in given path to PNG file, by calling ImageMagick's 'convert' command.
	os_type == 1 		-> Ubuntu
	os_type == 2 		-> Windows
	os_type == other	-> No conversion
*/
void Scene::convertPPMToPNG(string ppmFileName, int osType)
{
	string command;

	// call command on Ubuntu
	if (osType == 1)
	{
		command = "convert " + ppmFileName + " " + ppmFileName + ".png";
		system(command.c_str());
	}

	// call command on Windows
	else if (osType == 2)
	{
		command = "magick convert " + ppmFileName + " " + ppmFileName + ".png";
		system(command.c_str());
	}

	// default action - don't do conversion
	else
	{
	}
}