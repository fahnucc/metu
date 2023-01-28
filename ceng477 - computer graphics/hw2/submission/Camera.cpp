#include "Camera.h"
#include "Matrix4.h"
#include "Helpers.h"
#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

Camera::Camera() {}

Camera::Camera(int cameraId,
               int projectionType,
               Vec3 pos, Vec3 gaze,
               Vec3 u, Vec3 v, Vec3 w,
               double left, double right, double bottom, double top,
               double near, double far,
               int horRes, int verRes,
               string outputFileName)
{

    this->cameraId = cameraId;
    this->projectionType = projectionType;
    this->pos = pos;
    this->gaze = gaze;
    this->u = u;
    this->v = v;
    this->w = w;
    this->left = left;
    this->right = right;
    this->bottom = bottom;
    this->top = top;
    this->near = near;
    this->far = far;
    this->horRes = horRes;
    this->verRes = verRes;
    this->outputFileName = outputFileName;
}

Camera::Camera(const Camera &other)
{
    this->cameraId = other.cameraId;
    this->projectionType = other.projectionType;
    this->pos = other.pos;
    this->gaze = other.gaze;
    this->u = other.u;
    this->v = other.v;
    this->w = other.w;
    this->left = other.left;
    this->right = other.right;
    this->bottom = other.bottom;
    this->top = other.top;
    this->near = other.near;
    this->far = other.far;
    this->horRes = other.horRes;
    this->verRes = other.verRes;
    this->outputFileName = other.outputFileName;
}

ostream &operator<<(ostream &os, const Camera &c)
{
    const char *camType = c.projectionType ? "perspective" : "orthographic";

    os << fixed << setprecision(6) << "Camera " << c.cameraId << " (" << camType << ") => pos: " << c.pos << " gaze: " << c.gaze << endl
       << "\tu: " << c.u << " v: " << c.v << " w: " << c.w << endl
       << fixed << setprecision(3) << "\tleft: " << c.left << " right: " << c.right << " bottom: " << c.bottom << " top: " << c.top << endl
       << "\tnear: " << c.near << " far: " << c.far << " resolutions: " << c.horRes << "x" << c.verRes << " fileName: " << c.outputFileName;

    return os;
}

Matrix4 Camera::getCameraTransformation()
{
    double cameraTransformation[4][4] = {{this->u.x, this->u.y, this->u.z, (-1) * (this->u.x * this->pos.x + this->u.y * this->pos.y + this->u.z * this->pos.z)},
                                         {this->v.x, this->v.y, this->v.z, (-1) * (this->v.x * this->pos.x + this->v.y * this->pos.y + this->v.z * this->pos.z)},
                                         {this->w.x, this->w.y, this->w.z, (-1) * (this->w.x * this->pos.x + this->w.y * this->pos.y + this->w.z * this->pos.z)},
                                         {0, 0, 0, 1}};

    return Matrix4(cameraTransformation);
}

Matrix4 Camera::getProjectionTransMatrix()
{
    if (this->projectionType)
    {
        double perspective[4][4] = {{(2 * this->near) / (this->right - this->left), 0, (this->right + this->left) / (this->right - this->left), 0},
                                    {0, (2 * this->near) / (this->top - this->bottom), (this->top + this->bottom) / (this->top - this->bottom), 0},
                                    {0, 0, -((this->far + this->near) / (this->far - this->near)), -((2 * this->far * this->near) / (this->far - this->near))},
                                    {0, 0, -1, 0}};
        return Matrix4(perspective);
    }
    else
    {
        double ortho[4][4] = {{2 / (this->right - this->left), 0, 0, -((this->right + this->left) / (this->right - this->left))},
                              {0, 2 / (this->top - this->bottom), 0, -((this->top + this->bottom) / (this->top - this->bottom))},
                              {0, 0, -(2 / (this->far - this->near)), -((this->far + this->near) / (this->far - this->near))},
                              {0, 0, 0, 1}};
        return Matrix4(ortho);
    }
}

Matrix4 Camera::getViewportMatrix()
{
    double viewPort[4][4] = {{(double(this->horRes) / 2), 0, 0, ((double(this->horRes) - 1) / 2)},
                             {0, (double(this->verRes) / 2), 0, ((double(this->verRes) - 1) / 2)},
                             {0, 0, 0.5, 0.5},
                             {0, 0, 0, 1}};

    return Matrix4(viewPort);
}