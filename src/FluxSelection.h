#ifndef FLUXSELECTION_H
#define FLUXSELECTION_H
#include "FluxCommon.h"


class V3;
class VTriangle;
class Context;
class Triangle;

class Selection {
public:
    Selection();
    ~Selection();

    void selectFace(Context* ctx, int x, int y);
    void selectEdge(Context* ctx, int x, int y);
    void selectVert(Context* ctx, int x, int y);

    int intersect_RayTriangle(V3* r0, V3* r1, Triangle* T,V3& p);
    float dist3D_Segment_to_Segment(const  V3* s1p0, const V3* s1p1, const V3* s2p0, const V3* s2p1 );
    float dist_Point_to_Segment( V3* P, V3* SP0, V3* SP1);
private:
    DISALLOW_COPY_AND_ASSIGN(Selection);

};

#endif // FLUXSELECTION_H
