#ifndef FLUXMATH3D_H
#define FLUXMATH3D_H
#include "FluxCommon.h"
#include <QGLWidget>

class V3;

class Math3d
{
public:
    Math3d();
    static V3 unProject(int x, int y);
    static V3 getPointOnNearPlane(int x, int y);
    static V3 getPointOnFarPlane(int x, int y);
private:
    DISALLOW_COPY_AND_ASSIGN(Math3d);
};


#endif // FLUXMATH3D_H
