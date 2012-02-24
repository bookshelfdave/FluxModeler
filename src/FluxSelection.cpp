#include "FluxSelection.h"

#include <math.h>
#include <QtOpenGL>
#include <QMap>
#include "FluxCore.h"
#include "FluxContext.h"
#include "FluxGLWidget.h"
#include "FluxMath3d.h"

Selection::Selection() {
}


Selection::~Selection() {

}

void Selection::selectFace(Context* ctx, int x, int y) {
    V3 point3d = Math3d::unProject(x,y);
    V3 near3d = Math3d::getPointOnNearPlane(x,y);
    V3 far3d = Math3d::getPointOnFarPlane(x,y);
    QList<Object3d*> sos = ctx->getScene()->getSceneObjects();
    QMap<FaceIdType, double> hits;

    foreach(Object3d* so, sos) {
        if(so->getBB()->pointInBB(&point3d)) {
            foreach(Face* face, *(so->getFaces())) {
                foreach(Triangle* tri, *(face->getTriangles())) {

                    // so why do near and far have to be backwards...?
                    V3 intersect;
                    int result = intersect_RayTriangle(&far3d, &near3d, tri, intersect);

                    if(result > 0) {
                        double distance = (intersect - near3d).length();
                        hits.insert(face->getId(),distance);
                        qDebug() << "Triangle hit:" << intersect.asString();
                    }
                }
            }
        }

        // TODO: This won't work right with more than 1 object
        if(hits.size() > 0) {
            double mindistance = 10000000.0;
            QMapIterator<FaceIdType, double> i(hits);
            FaceIdType foundFace = -1; // lame magic number

            while (i.hasNext()) {
                i.next();

                if(i.value() < mindistance) {
                    mindistance = i.value();
                    foundFace = i.key();
                }
            }

            so->getFaces()->at(foundFace)->select();
            qDebug() << "Found a face:" << foundFace;
        }
    }

    if(hits.size() > 0) {
        ctx->getScene()->getActiveGLWidget()->updateGL();
    }
}


void Selection::selectEdge(Context* ctx, int x, int y) {
    qDebug("Edge selection");
    V3 point3d = Math3d::unProject(x,y);
    V3 near3d = Math3d::getPointOnNearPlane(x,y);
    V3 far3d = Math3d::getPointOnFarPlane(x,y);
    QList<Object3d*> sos = ctx->getScene()->getSceneObjects();

    double min = 100000.00;
    Edge* selectedEdge = NULL; // dangerous magic constant

    //QList<V3*> selectSet;
    foreach(Object3d* so, sos) {
        if(so->getBB()->pointInBB(&point3d)) {
            QList<Edge*> *edges = so->getEdges();
            foreach(Edge* edge, *edges) {
                float distance = this->dist_Point_to_Segment( &point3d, edge->getA(), edge->getB());

                //qDebug(QString("Distance to edge = %1").arg(distance).toAscii().data());
                if(distance < min && distance <= 0.05) {
                    min = distance;
                    selectedEdge = edge;
                }
            }
        }
    }

    if(selectedEdge != NULL) {
        qDebug(QString("Selected: %1").arg(selectedEdge->getId()).toAscii().data());
        selectedEdge->select();
    }

}


void Selection::selectVert(Context* ctx, int x, int y) {
    V3 point3d = Math3d::unProject(x,y);
    V3 near3d = Math3d::getPointOnNearPlane(x,y);
    V3 far3d = Math3d::getPointOnFarPlane(x,y);
    QList<Object3d*> sos = ctx->getScene()->getSceneObjects();
    double min = 100000.00;
    int index = -1;
    //QList<V3*> selectSet;
    foreach(Object3d* so, sos) {
        if(so->getBB()->pointInBB(&point3d)) {
            QList<Vert*> *verts = so->getVerts();

            foreach(Vert* vert, *verts) {
                //http://mathworld.wolfram.com/Point-LineDistance3-Dimensional.html
                /*
                 |(x0 - x1) x (x0 - x2)|
                 -----------------------
                         |x2-x1|
                 */
                V3 num = (*vert- far3d) % (*vert - near3d);
                V3 denom = (near3d - far3d);
                double distance = num.length() / denom.length();

                //qDebug(QString("Distance %1").arg(distance).toAscii().data());
                if(distance < min && distance <= 0.05) {
                    min = distance;
                    index = vert->getId();
                }

            }
        }

        qDebug(QString("Found %1 at %2").arg(index).arg(min).toAscii().data());

        if(index > -1) {
            so->getVerts()->at(index)->select();
        }
    }

}




/* EVERYTHING BELOW IS FROM SOFTSURFER */

// Copyright 2001, softSurfer (www.softsurfer.com)
// This code may be freely used and modified for any purpose
// providing that this copyright notice is included with it.
// SoftSurfer makes no warranty for this code, and cannot be held
// liable for any real or imagined damage resulting from its use.
// Users of this code must verify correctness for their application.

#define SMALL_NUM  0.00000001 // anything that avoids division overflow

// intersect_RayTriangle(): intersect a ray with a 3D triangle
//    Input:  a ray R, and a triangle T
//    Output: *I = intersection point (when it exists)
//    Return: -1 = triangle is degenerate (a segment or point)
//             0 = disjoint (no intersect)
//             1 = intersect in unique point I1
//             2 = are in the same plane

int Selection::intersect_RayTriangle(V3* r0, V3* r1, Triangle* T, V3& intersectPoint) {
    V3    u, v, n;             // triangle V3s
    V3    dir, w0, w;          // ray V3s
    float     r, a, b;             // params to calc ray-plane intersect

    // get triangle edge V3s and plane normal
    u = *(T->index[1]) - *(T->index[0]);
    v = *(T->index[2]) - *(T->index[0]);
    n = u % v;             // cross product

    if (n.x == 0.0 && n.y == 0.0 && n.z == 0.0)            // triangle is degenerate
        return -1;                 // do not deal with this case

    dir = *r1 - *r0;             // ray direction V3
    w0 = *r0 - *(T->index[0]);
    a = - (n * w0);
    b = n * dir;

    if (fabs(b) < SMALL_NUM) {     // ray is parallel to triangle plane
        if (a == 0)                // ray lies in triangle plane
            return 2;
        else return 0;             // ray disjoint from plane
    }

    // get intersect point of ray with triangle plane
    r = a / b;

    if (r < 0.0)                   // ray goes away from triangle
        return 0;                  // => no intersect

    // for a segment, also test if (r > 1.0) => no intersect


    intersectPoint = *r0 + dir.scale(r);           // intersect point of ray and plane

    // is I inside T?
    float    uu, uv, vv, wu, wv, D;
    uu = u * u;
    uv = u * v;
    vv = v * v;
    w = intersectPoint - *(T->index[0]);
    wu = w * u;
    wv = w * v;
    D = uv * uv - uu * vv;

    // get and test parametric coords
    float s, t;
    s = (uv * wv - vv * wu) / D;

    if (s < 0.0 || s > 1.0)        // I is outside T
        return 0;

    t = (uv * wu - uu * wv) / D;

    if (t < 0.0 || (s + t) > 1.0)  // I is outside T
        return 0;

    return 1;                      // I is in T
}



// http://softsurfer.com/Archive/algorithm_0106/algorithm_0106.htm#dist3D_Segment_to_Segment()
// dist3D_Segment_to_Segment():
//    Input:  two 3D line segments S1 and S2
//    Return: the shortest distance between S1 and S2
float Selection::dist3D_Segment_to_Segment( const V3* s1p0, const V3* s1p1, const V3* s2p0, const V3* s2p1 ) {
    V3   u = *s1p1 - *s1p0;
    V3   v = *s2p1 - *s2p0;
    V3   w = *s1p0 - *s2p0;

    float    a = u * u;        // always >= 0
    float    b = u * v;
    float    c = v *v;        // always >= 0
    float    d = u * w;
    float    e = v *w;


    float    D = a*c - b*b;       // always >= 0
    float    sc, sN, sD = D;      // sc = sN / sD, default sD = D >= 0
    float    tc, tN, tD = D;      // tc = tN / tD, default tD = D >= 0

    // compute the line parameters of the two closest points
    if (D < SMALL_NUM) { // the lines are almost parallel
        sN = 0.0;        // force using point P0 on segment S1
        sD = 1.0;        // to prevent possible division by 0.0 later
        tN = e;
        tD = c;
    } else {              // get the closest points on the infinite lines
        sN = (b*e - c*d);
        tN = (a*e - b*d);

        if (sN < 0.0) {       // sc < 0 => the s=0 edge is visible
            sN = 0.0;
            tN = e;
            tD = c;
        } else if (sN > sD) { // sc > 1 => the s=1 edge is visible
            sN = sD;
            tN = e + b;
            tD = c;
        }
    }

    if (tN < 0.0) {           // tc < 0 => the t=0 edge is visible
        tN = 0.0;

        // recompute sc for this edge
        if (-d < 0.0)
            sN = 0.0;
        else if (-d > a)
            sN = sD;
        else {
            sN = -d;
            sD = a;
        }
    } else if (tN > tD) {    // tc > 1 => the t=1 edge is visible
        tN = tD;

        // recompute sc for this edge
        if ((-d + b) < 0.0)
            sN = 0;
        else if ((-d + b) > a)
            sN = sD;
        else {
            sN = (-d + b);
            sD = a;
        }
    }

    // finally do the division to get sc and tc
    sc = (abs(sN) < SMALL_NUM ? 0.0 : sN / sD);
    tc = (abs(tN) < SMALL_NUM ? 0.0 : tN / tD);

    // get the difference of the two closest points
    V3 dP = w + (u.scale(sc)) - (v.scale(sc));  // = S1(sc) - S2(tc)

    return dP.length();   // return the closest distance
}

// http://softsurfer.com/Archive/algorithm_0102/algorithm_0102.htm#dist_Point_to_Segment()
float Selection::dist_Point_to_Segment( V3* P, V3* SP0, V3* SP1) {
    V3 v = *SP1 - *SP0;
    V3 w = *P - *SP0;

    double c1 = w * v;

    if ( c1 <= 0 )
        return (*P - *SP0).length();

    double c2 = v * v;

    if ( c2 <= c1 )
        return (*P - *SP1).length();

    double b = c1 / c2;
    V3 Pb = *SP0 + v.scale(b);
    return (*P - Pb).length();

}

