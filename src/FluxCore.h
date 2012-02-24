#ifndef FLUXCORE_H
#define FLUXCORE_H

#include <algorithm>
#include <QList>
#include <QString>
#include <QStringBuilder>
#include <QMap>
#include <QSet>
#include "FluxCommon.h"

typedef unsigned int VertIdType;
typedef unsigned int EdgeIdType;
typedef unsigned int FaceIdType;
typedef unsigned int ObjIdType;







// point / vector all wrapped up in one
class V3 {
public:
    V3() : x(0.0), y(0.0), z(0.0) {}
    V3(double vx, double vy,double vz) : x(vx), y(vy), z(vz) {}
    virtual ~V3() {}
    double x,y,z;
    // vector ops
    double operator *(const V3& v) const;
    V3 operator %(const V3& v) const;
    V3 operator -(const V3& p) const;
    V3 operator +(const V3& p) const;
    double length();
    V3 scale(double s);
    V3 normalize(void);
    QString asString();
};


class M4 {
public:
    M4() : x(0.0), y(0.0), z(0.0) {}
    M4(double vx, double vy,double vz) : x(vx), y(vy), z(vz),w(1.0) {}
    M4(V3 v) : x(v.x), y(v.y), z(v.z),w(1.0) {}
    double x,y,z,w;
};

// vector with selection and id info. I suppose other stuff will go here too
class Vert : public V3 {
public:
    Vert(VertIdType newid, double vx, double vy,double vz) :
        V3(vx,vy,vz), vid(newid), selected(false) {
    }
    ~Vert();
    bool isSelected();
    void select();
    void deselect();
    VertIdType getId();

private:
    VertIdType vid;
    bool selected;
};


class Triangle {
public:
    Vert* index[3];
};




/* ideas:
 1) An edge is always stored with lowest vertid as a, and the highest vertid as b
 2) Half edge just stored vertid's as a and b, with a pointer to the edge and
    a bool called "reversed".
*/
class Edge {
public:

    Edge(EdgeIdType eid, Vert* va, Vert* vb) : id(eid),selected(false) {
        a = std::min(va,vb);
        b = std::max(va,vb);
    }
    //Edge getReversedEdge();
    bool operator==(Edge other);
    QString getEdgeName();

    static QString makeEdgeName(Vert* a, Vert* b) {
        return QString("%1-%2").arg(a->getId()).arg(b->getId());
    }

    static QString makeEdgeName(VertIdType a, VertIdType b) {
        return QString("%1-%2").arg(a).arg(b);
    }



    bool isSelected();
    void select();
    void deselect();

    EdgeIdType getId() {
        return id;
    }

    Vert* getA() {
        return a;
    }
    Vert* getB() {
        return b;
    }

private:
    EdgeIdType id;
    Vert* a;
    Vert* b;
    bool selected;
    DISALLOW_COPY_AND_ASSIGN(Edge);
};

class Object3d;

const float epsilon = 0.001F;

class Face {
public:
    Face(Object3d* p, FaceIdType newid);
    ~Face();

    void addVert(Vert* v);
    void build();

    QList<EdgeIdType> *getUndirectedEdgeIds();
    QList<Triangle*> *getTriangles();
    QList<Vert*> *getVerts();
    FaceIdType getId();
    Object3d* getParent();
    // TODO: should this be a pointer?
    V3 getCentroid();
    bool isSelected();
    void select();
    void deselect();

protected:
    void calculateNormal();
    void calculateCentroid();
    void triangulate();
    void makeEdges();
    DISALLOW_COPY_AND_ASSIGN(Face);

private:
    void triangulatePolygon(QList<Vert*> *vertex, V3& normal, QList<Triangle*> *tris);
    static long GetNextActive(long x, long vertexCount,const bool* active);
    static long GetPrevActive(long x, long vertexCount,const bool* active);

    Object3d* parent;

    QList<EdgeIdType> *edgeids;
    QList<Vert*> *verts;
    QList<Triangle*> *triangles;
    V3 normal;
    V3 centroid;
    FaceIdType id;
    bool selected;
};

//class MutableFace : Face {

//};


class HalfEdge  {
public:
    Vert* vert;
    Face* face;
    Edge* normalizedEdge;
    bool reversedEdge;
    HalfEdge*   pair;
    HalfEdge*   prev;
    HalfEdge*   next;

    QString getDirectedEdgeId() {
        if(reversedEdge) {
            return Edge::makeEdgeName(normalizedEdge->getB(),normalizedEdge->getA());
        } else {
            return Edge::makeEdgeName(normalizedEdge->getA(),normalizedEdge->getB());
        }

    }

    EdgeIdType getFirst() {
        if(reversedEdge) {
            return normalizedEdge->getB()->getId();
        } else {
            return normalizedEdge->getA()->getId();
        }
    }

    EdgeIdType getSecond() {
        if(reversedEdge) {
            return normalizedEdge->getA()->getId();
        } else {
            return normalizedEdge->getB()->getId();
        }
    }
};

class BoundingBox3d {
public:
    BoundingBox3d();
    ~BoundingBox3d();
    bool pointInBB(V3* point);
    V3 bbmin;
    V3 bbmax;
};


class Object3d {
public:
    Object3d();
    ~Object3d();

    VertIdType addVert(float x, float y, float z);
    EdgeIdType addEdge(Vert* a, Vert* b);
    FaceIdType addFace(QList<VertIdType>*);

    void initialBuild();
    // don't even know why i bothered to use getters and setters for these
    QList<Face*> *getFaces();
    //QMap<QString,Edge*> getEdgeMap();
    QList<Edge*> *getEdges();
    QList<Vert*> *getVerts();

    QMap<QString,HalfEdge*> *getHalfEdges();
    BoundingBox3d* getBB();

    void deselectAllVerts();
    void deselectAllEdges();
    void deselectAllFaces();
    //QSet<FaceIdType>* getFaceSelection();
    //QSet<EdgeIdType>* getEdgeSelection();
    //QSet<VertIdType>* getVertSelection();

private:

    QList<Face*> *faces;
    QList<Edge*> *edges;
    QMap<QString,Edge*> *edgemap;
    QList<Vert*> *verts;
    QMap<QString,HalfEdge*> *halfEdges;
    BoundingBox3d* bb;

    //QSet<FaceIdType> *selectedFaces;
    //QSet<EdgeIdType> *selectedEdges;
    //QSet<VertIdType> *selectedVerts;


    //GLuint displayListId;
    DISALLOW_COPY_AND_ASSIGN(Object3d);
};

#endif // CORE_H
