// Anıl Berdan Ceylan 2304277
// Fahri Numan Eskici 2309995

#ifndef __HW1__PARSER__
#define __HW1__PARSER__

#include <string>
#include <vector>
#include <cmath>

namespace parser
{
    // Notice that all the structures are as simple as possible
    // so that you are not enforced to adopt any style or design.
    struct Vec3f
    {
        float x, y, z;

        Vec3f(float a = 0, float b = 0, float c = 0)
        {
            x = a;
            y = b;
            z = c;
        }

        Vec3f &operator=(const Vec3f &b)
        {
            x = b.x;
            y = b.y;
            z = b.z;
            return *this;
        }

        Vec3f operator+(const Vec3f &b) const
        {
            Vec3f result;
            result.x = x + b.x;
            result.y = y + b.y;
            result.z = z + b.z;
            return result;
        }

        Vec3f &operator+=(const Vec3f &b)
        {
            x += b.x;
            y += b.y;
            z += b.z;
            return *this;
        }

        Vec3f operator*(const Vec3f &b) const
        {
            Vec3f result;
            result.x = x * b.x;
            result.y = y * b.y;
            result.z = z * b.z;
            return result;
        }

        Vec3f operator*(const float &b) const
        {
            Vec3f result;
            result.x = x * b;
            result.y = y * b;
            result.z = z * b;
            return result;
        }

        Vec3f operator/(const Vec3f &b) const
        {
            Vec3f result;
            result.x = x / b.x;
            result.y = y / b.y;
            result.z = z / b.z;
            return result;
        }

        Vec3f operator/(const float &b) const
        {
            Vec3f result;
            result.x = x / b;
            result.y = y / b;
            result.z = z / b;
            return result;
        }

        Vec3f operator-(const Vec3f &b) const
        {
            Vec3f result;
            result.x = x - b.x;
            result.y = y - b.y;
            result.z = z - b.z;
            return result;
        }

        bool operator==(const Vec3f &b) const
        {
            return (x == b.x && y == b.y && z == b.z);
        }

        float dotProduct(const Vec3f &b) const
        {
            Vec3f multp = *this * b;
            return multp.x + multp.y + multp.z;
        }

        Vec3f crossProduct(const Vec3f &b) const
        {
            Vec3f result(
                y * b.z - z * b.y,
                z * b.x - x * b.z,
                x * b.y - y * b.x);

            return result;
        }

        float length() const
        {
            Vec3f multp = *this * *this;
            return sqrt(multp.x + multp.y + multp.z);
        }

        float calculateDistance(const Vec3f &b) const
        {
            return (b - *this).length();
        }

        Vec3f normalize() const
        {
            Vec3f result;
            float len = length();
            result.x = x / len;
            result.y = y / len;
            result.z = z / len;
            return result;
        }

        float determinant(const Vec3f &v1, const Vec3f &v2) const
        {
            return x * (v1.y * v2.z - v2.y * v1.z) + y * (v2.x * v1.z - v1.x * v2.z) + z * (v1.x * v2.y - v1.y * v2.x);
        }
    };

    struct Vec3i
    {
        int x, y, z;
    };

    struct Vec4f
    {
        float x, y, z, w;
    };

    struct Ray
    {
        Vec3f origin;
        Vec3f direction;

        Ray(Vec3f o = Vec3f(), Vec3f d = Vec3f())
        {
            origin = o;
            direction = d;
        };

        Vec3f getTPoint(float t) const
        {
            Vec3f result;
            result.x = origin.x + t * direction.x;
            result.y = origin.y + t * direction.y;
            result.z = origin.z + t * direction.z;
            return result;
        }
    };

    struct Hit
    {
        bool trueHit;
        int obj_gid;
        int material_id;
        float t;
        Vec3f intersectionPoint;
        Vec3f surfaceNormal;
    };

    struct Camera
    {
        Vec3f position;
        Vec3f gaze;
        Vec3f up;
        Vec4f near_plane;
        float near_distance;
        int image_width, image_height;
        std::string image_name;

        Ray generateRay(int i, int j)
        {
            float left = near_plane.x;
            float right = near_plane.y;
            float bottom = near_plane.z;
            float top = near_plane.w;

            Vec3f gazeN = gaze.normalize();

            float su = (right - left) * (j + 0.5) / image_width;
            float sv = (top - bottom) * (i + 0.5) / image_height;

            Vec3f m = position + (gaze * near_distance);
            Vec3f u = (gazeN.crossProduct(up)).normalize();
            Vec3f v = u.crossProduct(gazeN);
            Vec3f q = m + (u * left) + (v * top);
            Vec3f s = q + (u * su) - (v * sv);

            Ray ray;
            ray.origin = position;
            ray.direction = (s - position).normalize();

            return ray;
        }
    };

    struct Material
    {
        bool is_mirror;
        Vec3f ambient;
        Vec3f diffuse;
        Vec3f specular;
        Vec3f mirror;
        float phong_exponent;

        bool isMirror() const
        {
            if (mirror.x > 0 || mirror.y > 0 || mirror.z > 0)
                return true;
            return false;
        }

        Vec3f calculateAmbient(const Vec3f &ambientLight) const
        {
            Vec3f result = ambient * ambientLight;
            return result;
        }
    };

    struct PointLight
    {
        Vec3f position;
        Vec3f intensity;

        Vec3f findDiffuse(const Material &material, const Vec3f &normal, const Vec3f &intersectionPoint)
        {
            Vec3f l = (position - intersectionPoint).normalize();

            float cosTheta = fmax(0.0f, l.dotProduct(normal));
            Vec3f irradiance = findIrradiance(intersectionPoint);

            Vec3f diffuse = irradiance * cosTheta * material.diffuse;
            return diffuse;
        };

        Vec3f findSpecular(const Material &material, const Ray &ray, const Vec3f &normal, const Vec3f &intersectionPoint)
        {
            Vec3f irradiance = findIrradiance(intersectionPoint);

            Vec3f wi = (position - intersectionPoint).normalize();

            Vec3f h = (wi - ray.direction).normalize();

            float dotPro = fmax(0, normal.dotProduct(h));

            Vec3f specular = material.specular * irradiance * pow(dotPro, material.phong_exponent);

            return specular;
        }

        Vec3f findIrradiance(const Vec3f &intersectionPoint)
        {
            float lightDistance = (position - intersectionPoint).length();
            Vec3f irr = intensity / (lightDistance * lightDistance);
            return irr;
        }
    };

    struct Face
    {
        int v0_id;
        int v1_id;
        int v2_id;
        Vec3f v0;
        Vec3f v1;
        Vec3f v2;
        Vec3f normal;

        Face(int a = 0, int b = 0, int c = 0, Vec3f va = Vec3f(), Vec3f vb = Vec3f(), Vec3f vc = Vec3f(), Vec3f norm = Vec3f())
        {
            v0_id = a;
            v1_id = b;
            v2_id = c;
            v0 = va;
            v1 = vb;
            v2 = vc;
            normal = norm;
        };

        void findVertices(const std::vector<Vec3f> &vertex_data)
        {
            v0 = vertex_data[v0_id - 1];
            v1 = vertex_data[v1_id - 1];
            v2 = vertex_data[v2_id - 1];
        }

        void calculateNormal()
        {
            normal = ((v1 - v0).crossProduct((v2 - v0))).normalize();
        }
    };

    struct Triangle
    {
        int material_id;
        Face indices;
        int gid;

        Triangle(Face ind = Face(), int mat = 0)
        {
            indices = ind;
            material_id = mat;
        };

        Hit calculateIntersection(const Ray &ray, int obj_gid) const
        {
            Hit hit;
            hit.trueHit = false;

            Vec3f o = ray.origin;
            Vec3f d = ray.direction;

            Vec3f a = indices.v0;
            Vec3f b = indices.v1;
            Vec3f c = indices.v2;

            float detA = (a - b).determinant((a - c), d);
            if (detA == 0.0)
            {
                return hit;
            }

            float t = (a - b).determinant((a - c), (a - o)) / detA;
            if (t <= 0.0)
            {
                return hit;
            }

            float gamma = (a - b).determinant((a - o), d) / detA;
            if (gamma < 0 || gamma > 1)
            {
                return hit;
            }

            float beta = (a - o).determinant((a - c), d) / detA;
            if (beta < 0 || beta > (1 - gamma))
            {
                return hit;
            }

            hit.trueHit = true;
            hit.obj_gid = obj_gid;
            hit.material_id = material_id;
            hit.t = t;
            hit.intersectionPoint = ray.getTPoint(t);
            hit.surfaceNormal = indices.normal;

            return hit;
        }
    };

    struct Mesh
    {
        int material_id;
        std::vector<Face> faces;
        int gid;

        Hit calculateIntersection(const Ray &ray, int obj_gid) const
        {
            float minT = -1.0;
            Hit hitResult;
            hitResult.trueHit = false;

            for (int faceNumber = 0; faceNumber < faces.size(); faceNumber++)
            {
                Vec3f v0 = faces[faceNumber].v0;
                Vec3f v1 = faces[faceNumber].v1;
                Vec3f v2 = faces[faceNumber].v2;

                Triangle face = Triangle(
                    Face(faces[faceNumber].v0_id,
                         faces[faceNumber].v1_id,
                         faces[faceNumber].v2_id,
                         v0,
                         v1,
                         v2),
                    material_id);
                Hit hit = face.calculateIntersection(ray, obj_gid);

                if (hit.trueHit && hit.t >= 0 && (minT == -1 || hit.t < minT))
                {
                    hit.surfaceNormal = faces[faceNumber].normal;

                    minT = hit.t;
                    hitResult = hit;
                }
            }

            return hitResult;
        }
    };

    struct Sphere
    {
        int material_id;
        int center_vertex_id;
        float radius;
        Vec3f center;
        int gid;

        void findCenter(const std::vector<Vec3f> &vertex_data)
        {
            center = vertex_data[center_vertex_id - 1];
        }

        Hit calculateIntersection(const Ray &ray, int obj_gid) const
        {
            Hit hit;

            const float A = ray.direction.dotProduct(ray.direction);
            Vec3f z = ray.origin - center;
            const float B = 2 * ray.direction.dotProduct((z));
            const float C = z.dotProduct(z) - (radius * radius);

            const float discriminant = B * B - 4 * A * C;

            if (discriminant < 0)
            {
                hit.trueHit = false;
            }
            else
            {
                const float t1 = (-1 * B + sqrtf(discriminant)) / 2 * A;
                const float t2 = (-1 * B - sqrtf(discriminant)) / 2 * A;

                hit.material_id = material_id;
                hit.trueHit = true;
                hit.obj_gid = obj_gid;

                const float t = fmin(t1, t2);
                hit.intersectionPoint = ray.getTPoint(t);
                hit.surfaceNormal = (hit.intersectionPoint - center) / radius;
                hit.t = t;
            }

            return hit;
        }
    };

    struct Scene
    {
        Vec3i background_color;
        float shadow_ray_epsilon;
        int max_recursion_depth;
        std::vector<Camera> cameras;
        Vec3f ambient_light;
        std::vector<PointLight> point_lights;
        std::vector<Material> materials;
        std::vector<Vec3f> vertex_data;
        std::vector<Mesh> meshes;
        std::vector<Triangle> triangles;
        std::vector<Sphere> spheres;

        // Functions
        void loadFromXml(const std::string &filepath);

        Hit calculateHit(const Ray &ray)
        {
            int numSphere = spheres.size();
            int numTri = triangles.size();
            int numMesh = meshes.size();

            Hit hitResult;
            hitResult.trueHit = false;
            float minT = -1.;

            for (int sphereInd = 0; sphereInd < numSphere; sphereInd++)
            {
                Hit hit = spheres[sphereInd].calculateIntersection(ray, spheres[sphereInd].gid);

                if (hit.trueHit && hit.t >= 0. && (minT == -1. || hit.t < minT))
                {
                    hitResult = hit;
                    minT = hit.t;
                }
            }

            for (int triangleInd = 0; triangleInd < numTri; triangleInd++)
            {
                Hit hit = triangles[triangleInd].calculateIntersection(ray, triangles[triangleInd].gid);

                if (hit.trueHit && hit.t >= 0. && (minT == -1. || hit.t < minT))
                {
                    hitResult = hit;
                    minT = hit.t;
                }
            }

            for (int meshNumber = 0; meshNumber < numMesh; meshNumber++)
            {
                Hit hit = meshes[meshNumber].calculateIntersection(ray, meshes[meshNumber].gid);

                if (hit.trueHit && hit.t >= 0. && (minT == -1. || hit.t < minT))
                {
                    hitResult = hit;
                    minT = hit.t;
                }
            }

            return hitResult;
        };

        Vec3f coloring(const Hit &hitResult, const Camera &currentCamera, const Ray &ray, int depth)
        {
            int numLight = point_lights.size();
            int numSphere = spheres.size();
            int numTri = triangles.size();
            int numMesh = meshes.size();

            Vec3f calculatedPixel(0., 0., 0.);

            // Add ambient
            Vec3f ambient = materials[hitResult.material_id - 1].calculateAmbient(ambient_light);
            calculatedPixel += ambient;

            if (hitResult.trueHit)
            {
                for (int lightInd = 0; lightInd < numLight; lightInd++)
                {
                    PointLight light = point_lights[lightInd];

                    // Move the point up by epsilon
                    Vec3f wi = (light.position - hitResult.intersectionPoint).normalize();
                    Vec3f wiEpsilon = wi * shadow_ray_epsilon;

                    Ray shadowRay((hitResult.intersectionPoint + wiEpsilon), wi);

                    float lightT = (light.position - shadowRay.origin).x / shadowRay.direction.x;

                    Hit shadowHit;
                    bool isInShadow = false;

                    for (int sphereInd = 0; sphereInd < numSphere; sphereInd++)
                    {
                        shadowHit = spheres[sphereInd].calculateIntersection(shadowRay, spheres[sphereInd].gid);

                        if (shadowHit.trueHit && lightT > shadowHit.t && shadowHit.t >= 0)
                        {
                            isInShadow = true;
                            break;
                        }
                    }

                    if (!isInShadow)
                    {
                        for (int triangleInd = 0; triangleInd < numTri; triangleInd++)
                        {
                            shadowHit = triangles[triangleInd].calculateIntersection(shadowRay, triangles[triangleInd].gid);

                            if (shadowHit.trueHit && lightT > shadowHit.t && shadowHit.t >= 0)
                            {
                                isInShadow = true;
                                break;
                            }
                        }
                    }

                    if (!isInShadow)
                    {
                        for (int meshNumber = 0; meshNumber < numMesh; meshNumber++)
                        {
                            shadowHit = meshes[meshNumber].calculateIntersection(shadowRay, meshes[meshNumber].gid);

                            if (shadowHit.trueHit && lightT > shadowHit.t && shadowHit.t >= 0)
                            {
                                isInShadow = true;
                                break;
                            }
                        }
                    }

                    if (!isInShadow)
                    {
                        Vec3f diffuse = light.findDiffuse(materials[hitResult.material_id - 1], hitResult.surfaceNormal, hitResult.intersectionPoint);
                        Vec3f specular = light.findSpecular(materials[hitResult.material_id - 1], ray, hitResult.surfaceNormal, hitResult.intersectionPoint);

                        calculatedPixel += diffuse + specular;
                    }
                }

                if (depth > 0 && materials[hitResult.material_id - 1].isMirror())
                {
                    float wi = -2. * ray.direction.dotProduct(hitResult.surfaceNormal);
                    Vec3f wiNormal = ((hitResult.surfaceNormal * wi) + ray.direction).normalize();
                    Vec3f wiEpsilon = wiNormal * shadow_ray_epsilon;

                    Ray reflectionRay = Ray((hitResult.intersectionPoint + wiEpsilon), wiNormal);

                    Hit hitRes = calculateHit(reflectionRay);

                    if (hitRes.trueHit && hitRes.obj_gid != hitResult.obj_gid)
                    {
                        Vec3f reflection = coloring(hitRes, currentCamera, reflectionRay, --depth);
                        calculatedPixel += reflection * materials[hitResult.material_id - 1].mirror;
                    }
                }
            }
            else if (depth < max_recursion_depth)
            {
                calculatedPixel = Vec3f(0, 0, 0);
            }

            Vec3f pixelColor(
                fmin(fmax(0.f, calculatedPixel.x), 255.f),
                fmin(fmax(0.f, calculatedPixel.y), 255.f),
                fmin(fmax(0.f, calculatedPixel.z), 255.f));

            return pixelColor;
        };
    };

}

#endif
