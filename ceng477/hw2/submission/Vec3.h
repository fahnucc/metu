#ifndef __VEC3_H__
#define __VEC3_H__

#include "Translation.h"
#include "Rotation.h"
#include "Scaling.h"
#include <iostream>
using namespace std;

class Vec3
{
public:
    double x, y, z;
    int colorId;

    Vec3();
    Vec3(double x, double y, double z, int colorId);
    Vec3(const Vec3 &other);
    Vec3(const Vec3 &other, int colorId);

    double getElementAt(int index);

    friend std::ostream &operator<<(std::ostream &os, const Vec3 &v);

    Vec3 operator+(const Vec3 &other);
    Vec3 operator-(const Vec3 &other);
    Vec3 operator*(const double &other);
    Vec3 operator/(const double &other);
    Vec3 operator*(const Vec3 &other);
    Vec3 operator/(const Vec3 &other);

    double dotProduct(const Vec3 &other);
    Vec3 crossProduct(const Vec3 &other);
    double length();
    Vec3 normalize();
    Vec3 *translate(Translation *translation);
    Vec3 *rotate(Rotation *rotation);
    Vec3 *scale(Scaling *scaling);
};

#endif