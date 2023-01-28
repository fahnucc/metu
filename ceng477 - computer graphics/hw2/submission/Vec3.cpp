#include "Vec3.h"
#include <iomanip>
#include <cmath>
#include "Translation.h"
#include "Rotation.h"
#include "Scaling.h"
#include "Helpers.h"

using namespace std;

Vec3::Vec3()
{
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
    this->colorId = -1;
}

Vec3::Vec3(double x, double y, double z, int colorId)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->colorId = colorId;
}

Vec3::Vec3(const Vec3 &other)
{
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    this->colorId = other.colorId;
}

Vec3::Vec3(const Vec3 &other, int colorId)
{
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    this->colorId = colorId;
}

double Vec3::getElementAt(int index)
{
    switch (index)
    {
    case 0:
        return this->x;

    case 1:
        return this->y;

    case 2:
        return this->z;

    default:
        return this->z;
    }
}

ostream &operator<<(ostream &os, const Vec3 &v)
{

    os << fixed << setprecision(6) << "[" << v.x << ", " << v.y << ", " << v.z << "]";

    return os;
}

Vec3 Vec3::operator+(const Vec3 &other)
{
    return Vec3(this->x + other.x, this->y + other.y, this->z + other.z, this->colorId);
}

Vec3 Vec3::operator-(const Vec3 &other)
{
    return Vec3(this->x - other.x, this->y - other.y, this->z - other.z, this->colorId);
}

Vec3 Vec3::operator*(const double &other)
{
    return Vec3(this->x * other, this->y * other, this->z * other, this->colorId);
}

Vec3 Vec3::operator/(const double &other)
{
    return Vec3(this->x / other, this->y / other, this->z / other, this->colorId);
}

Vec3 Vec3::operator*(const Vec3 &other)
{
    return Vec3(this->x * other.x, this->y * other.y, this->z * other.z, this->colorId);
}

Vec3 Vec3::operator/(const Vec3 &other)
{
    return Vec3(this->x / other.x, this->y / other.y, this->z / other.z, this->colorId);
}

double Vec3::dotProduct(const Vec3 &other)
{
    return this->x * other.x + this->y * other.y + this->z * other.z;
}

Vec3 Vec3::crossProduct(const Vec3 &other)
{
    return Vec3(this->y * other.z - this->z * other.y, this->z * other.x - this->x * other.z, this->x * other.y - this->y * other.x, this->colorId);
}

double Vec3::length()
{
    return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

Vec3 Vec3::normalize()
{
    double length = this->length();
    return Vec3(this->x / length, this->y / length, this->z / length, this->colorId);
}

Vec3 *Vec3::translate(Translation *translation)
{
    return new Vec3(this->x + translation->tx, this->y + translation->ty, this->z + translation->tz, this->colorId);
}

Vec3 *Vec3::rotate(Rotation *rotation)
{
    double x = rotation->ux;
    double y = rotation->uy;
    double z = rotation->uz;
    double ix = this->x;
    double iy = this->y;
    double iz = this->z;
    double squx = x * x;
    double squy = y * y;
    double squz = z * z;
    double sqrtL = sqrt(squx + squy + squz);
    double angle = (rotation->angle * 3.14159) / 180;
    double cosa = cos(angle);
    double sina = sin(angle);
    double divisionL = 1.0f / (squx + squy + squz);
    double m[4][4] = {{(squx + (squy + squz) * cosa) * divisionL, (x * y * (1 - cosa) - z * sqrtL * sina) * divisionL, (x * z * (1 - cosa) + y * sqrtL * sina) * divisionL, 0},
                      {(x * y * (1 - cosa) + z * sqrtL * sina) * divisionL, (squy + (squx + squz) * cosa) * divisionL, (y * z * (1 - cosa) - x * sqrtL * sina) * divisionL, 0},
                      {(x * z * (1 - cosa) - y * sqrtL * sina) * divisionL, (y * z * (1 - cosa) + x * sqrtL * sina) * divisionL, (squz + (squx + squy) * cosa) * divisionL, 0},
                      {0, 0, 0, 1}};
    Matrix4 matrix = Matrix4(m);
    Vec4 v = Vec4(ix, iy, iz, 1, this->colorId);
    Vec4 r = multiplyMatrixWithVec4(matrix, v);
    Vec3 *result = new Vec3();
    result->x = r.x;
    result->y = r.y;
    result->z = r.z;
    result->colorId = this->colorId;
    return result;
}

Vec3 *Vec3::scale(Scaling *scaling)
{
    Vec3 *r = new Vec3();
    r->x = this->x * scaling->sx;
    r->y = this->y * scaling->sy;
    r->z = this->z * scaling->sz;
    r->colorId = this->colorId;
    return r;
}