#include "FluxCore.h"
#include <Python.h>
#include <limits>
#include <math.h>
#include <QtGlobal>

double V3::operator *(const V3& v) const {
    return x*v.x + y*v.y + z*v.z;
}


V3 V3::operator %(const V3& v) const {
    return (V3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x));
}

V3 V3::operator -(const V3& p) const {
    // Difference between two points is a vector
    return (V3(x - p.x, y - p.y, z - p.z));
}

V3 V3::operator +(const V3& p) const {
    // Difference between two points is a vector
    return (V3(x + p.x, y + p.y, z + p.z));
}


double V3::length() {
    return (sqrt(x*x + y*y + z*z));
}

V3 V3::scale(double s) {
    return V3(x*s,y*s,z*s);
}

V3 V3::normalize(void) {
    double l = length();
    return V3(x /= l, y /= l, z /= l);
}


QString V3::asString() {
    // safe to return this
    QString out = QString("V3(%1,%2,%3)").arg(x).arg(y).arg(z);
    return out;
}






Vert::~Vert() {

}

bool Vert::isSelected() {
    return selected;
}

void Vert::select() {
    selected = true;
}

void Vert::deselect() {
    selected = false;
}

VertIdType Vert::getId() {
    return vid;
}






//Edge Edge::getReversedEdge() {
//    /// OOOH -1 is dangerous!!
//    return Edge(-1,a,b);
//}

bool Edge::operator==(Edge other) {
    if(other.a == this->a && other.b == this->b) {
        return true;
    } else {
        return false;
    }
}

QString Edge::getEdgeName() {
    return QString::number(a->getId()) + "-" + QString::number(b->getId());
}


bool Edge::isSelected() {
    return selected;
}

void Edge::select() {
    selected = true;
}

void Edge::deselect() {
    selected = false;
}




Face::Face(Object3d* p,FaceIdType newid) : id(newid) {
    parent = p;
    verts = new QList<Vert*>();
    edgeids = new QList<EdgeIdType>();
    triangles = new QList<Triangle*>();
    selected = false;
}

Face::~Face() {
    delete verts;
    delete edgeids;
    foreach(Triangle* t, *triangles) {
        delete t;
    }

    delete triangles;
}

QList<EdgeIdType>* Face::getUndirectedEdgeIds() {
    return edgeids;
}

QList<Triangle*>* Face::getTriangles() {
    return triangles;
}

QList<Vert*>* Face::getVerts() {
    return verts;
}

FaceIdType Face::getId() {
    return id;
}


Object3d* Face::getParent() {
    return parent;
}

void Face::addVert(Vert* v) {
    verts->append(v);
}


void Face::build() {
    makeEdges();
    calculateNormal();
    triangulate();
    calculateCentroid();
}

void Face::makeEdges() {
    int index = 0;
    foreach(Vert* v, *verts) {
        if(index == (verts->length()-1)) {
            EdgeIdType eid = parent->addEdge(v,verts->first());
            edgeids->append(eid);
            //qDebug(QString("Adding edge: %1 %2").arg(v->getId()).arg(verts->first()->getId()).toAscii().data());
        } else {
            EdgeIdType eid = parent->addEdge(v,verts->at(index+1));
            edgeids->append(eid);
            //qDebug(QString("Adding edge: %1 %2").arg(v->getId()).arg(verts->at(index+1)->getId()).toAscii().data());
        }

        index++;
    }

}

void Face::triangulate() {
    // clear out any previous triangles...
    // is this needed here. will i ever have any previous triangles??
    foreach(Triangle* t, *triangles) {
        delete t;
    }
    triangles->clear();
    triangulatePolygon(verts, normal, triangles);
}


void Face::calculateNormal() {
    int index = 0;

    do {
        normal = (*(verts->at(0+index)) - *(verts->at(1+index))) %
                 ( *(verts->at(1+index)) - *(verts->at(2+index)));

        if(index == verts->length() - 2) {
            // ERROR!
            qDebug("Error calculating normal. Is this a straight line???");
            break;
        }

        index++;
    } while(normal.x == 0.0 && normal.y == 0.0 && normal.z == 0.0);
}


// centroid: http://local.wasp.uwa.edu.au/~pbourke/geometry/polyarea/
void Face::calculateCentroid() {
    V3 num = V3(0.0,0.0,0.0);
    double denom = 0.0;

    foreach(Triangle* tri, *triangles) {
        V3* v0 = tri->index[0];
        V3* v1 = tri->index[1];
        V3* v2 = tri->index[2];

        V3 r0 = V3(v0->x + v1->x + v2->x,
                  v0->y + v1->y + v2->y,
                  v0->z + v1->z + v2->z);
        V3 r = r0.scale(1.0/3.0);
        double a = ((*v1 - *v0) % (*v2 - *v0)).length();

        num = num + r.scale(a);
        denom += a;
    }
    centroid = num.scale(1.0/denom);
}


long Face::GetNextActive(long x, long vertexCount,const bool* active) {
    for (;;) {
        if (++x == vertexCount) x = 0;

        if (active[x]) return (x);
    }
}

long Face::GetPrevActive(long x, long vertexCount,const bool* active) {
    for (;;) {
        if (--x == -1) x = vertexCount - 1;

        if (active[x]) return (x);
    }
}

// This code is from the book
// Mathematics for 3D Game Programming & Computer Graphics
// Second Edition
// by Eric Lengyel
// (c) 2004 Charles River Media, Inc.
void Face::triangulatePolygon(QList<Vert*> *vertex, V3& normal, QList<Triangle*> *triangles) {
    long vertexCount = vertex->size();

    bool* active = new bool[vertexCount];

    for (long a = 0; a < vertexCount; a++)
        active[a] = true;

    long triangleCount = 0;
    long start = 0;
    long p1 = 0;
    long p2 = 1;
    long m1 = vertexCount - 1;
    long m2 = vertexCount - 2;

    bool lastPositive = false;

    for (;;)    {
        if (p2 == m2) {
            // Only three vertices remain
            Triangle* triangle = new Triangle();

            triangle->index[0] = this->verts->at(m1);
            triangle->index[1] = this->verts->at(p1);
            triangle->index[2] = this->verts->at(p2);
            triangleCount++;
            triangles->append(triangle);
            break;
        }

        V3* vp1 = vertex->at(p1);
        V3* vp2 = vertex->at(p2);
        V3* vm1 = vertex->at(m1);
        V3* vm2 = vertex->at(m2);
        bool positive = false;
        bool negative = false;

        // Determine whether vp1, vp2, and vm1 form
        // a valid triangle
        V3 n1 = normal % (*vm1 - *vp2).normalize();

        if (n1 * (*vp1 - *vp2) > epsilon) {

            positive = true;
            V3 n2 = (normal % (*vp1 - *vm1).normalize());
            V3 n3 = (normal % (*vp2 - *vp1).normalize());

            for (long a = 0; a < vertexCount; a++) {
                // Look for other vertices inside the triangle

                if ((active[a])
                        && (a != p1) && (a != p2) && (a != m1)) {
                    V3* v = vertex->at(a);

                    if ((n1 * (*v - *vp2).normalize() > -epsilon)
                            && (n2 * (*v - *vm1).normalize() > -epsilon)
                            && (n3 * (*v - *vp1).normalize() > -epsilon)) {

                        positive = false;
                        break;
                    }
                }
            }
        }

        // Determine whether vm1, vm2, and vp1 form
        // a valid triangle
        n1 = normal % (*vm2 - *vp1).normalize();

        if (n1 * (*vm1 - *vp1) > epsilon) {
            negative = true;
            V3 n2 = (normal % (*vm1 - *vm2).normalize());
            V3 n3 = (normal % (*vp1 - *vm1).normalize());

            for (long a = 0; a < vertexCount; a++) {
                // Look for other vertices inside the triangle

                if ((active[a])
                        && (a != m1) && (a != m2) && (a != p1)) {
                    V3* v = vertex->at(a);

                    if ((n1 * (*v - *vp1).normalize() > -epsilon)
                            && (n2 * (*v - *vm2).normalize() > -epsilon)
                            && (n3 * (*v - *vm1).normalize() > -epsilon)) {
                        negative = false;
                        break;
                    }
                }
            }
        }

        // If both triangles valid, choose the one
        // having the larger smallest angle
        if ((positive) && (negative)) {
            float pd = (*vp2 - *vm1).normalize() *
                       (*vm2 - *vm1).normalize();
            float md = (*vm2 - *vp1).normalize() *
                       (*vp2 - *vp1).normalize();

            if (fabs(pd - md) < epsilon) {
                if (lastPositive) positive = false;
                else negative = false;
            } else {
                if (pd < md) negative = false;
                else positive = false;
            }

        }

        if (positive) {
            // Output the triangle m1, p1, p2

            active[p1] = false;
            Triangle* triangle = new Triangle();
            triangle->index[0] = this->verts->at(m1);
            triangle->index[1] = this->verts->at(p1);
            triangle->index[2] = this->verts->at(p2);
            triangleCount++;
            triangles->append(triangle);
            //triangle++;

            p1 = GetNextActive(p1, vertexCount, active);
            p2 = GetNextActive(p2, vertexCount, active);
            lastPositive = true;
            start = -1;
        } else if (negative) {
            // Output the triangle m2, m1, p1

            active[m1] = false;
            Triangle* triangle = new Triangle();
            triangle->index[0] = this->verts->at(m2);
            triangle->index[1] = this->verts->at(m1);
            triangle->index[2] = this->verts->at(p1);
            triangleCount++;
            triangles->append(triangle);

            //triangle++;

            m1 = GetPrevActive(m1, vertexCount, active);
            m2 = GetPrevActive(m2, vertexCount, active);
            lastPositive = false;
            start = -1;
        } else {

            // Exit if we've gone all the way around the
            // polygon without finding a valid triangle
            if (start == -1) start = p2;
            else if (p2 == start) break;

            // Advance working set of vertices
            m2 = m1;
            m1 = p1;
            p1 = p2;
            p2 = GetNextActive(p2, vertexCount, active);
        }
    }

    delete[] active;
//    QString countOut = QString("Triangle count = %1").arg(triangleCount);
//    qDebug(countOut.toAscii().data());
}


V3 Face::getCentroid() {
    return centroid;
}

V3 Face::getNormal() {
    return normal;
}

bool Face::isSelected() {
    return selected;
}

void Face::select() {
    selected = true;
}

void Face::deselect() {
    selected = false;
}





BoundingBox3d::BoundingBox3d() {
//    xmin = ymin = zmin = std::numeric_limits<double>::min() ;
//    xmax = ymax = zmax = std::numeric_limits<double>::max() ;

}

BoundingBox3d::~BoundingBox3d() {
}


bool BoundingBox3d::pointInBB(V3* point) {
    // scale it up a bit for a little bit of fluff
    V3 newmin = bbmin.scale(1.1);
    V3 newmax = bbmax.scale(1.1);

    if(point->x >= newmin.x && point->x <= newmax.x &&
            point->y >= newmin.y && point->y <= newmax.y &&
            point->z >= newmin.z && point->z <= newmax.z) {
        return true;
    } else {
        return false;
    }
}



Object3d::Object3d(int oid) {
    objectId = oid;
    faces = new QList<Face*>();
    edges = new QList<Edge*>();
    edgemap = new QMap<QString,Edge*>();
    verts = new QList<Vert*> ();
    halfEdges = new QMap<QString,HalfEdge*>();
    bb = new BoundingBox3d();
}

Object3d::~Object3d() {
    foreach(V3 *v, *verts) {
        delete v;
    }
    foreach(Edge *e, *edges) {
        delete e;
    }
    foreach(Face *f, *faces) {
        delete f;
    }

    foreach(HalfEdge* he, *halfEdges) {
        delete he;
    }

    delete faces;
    delete edges;
    delete edgemap;
    delete verts;
    delete halfEdges;
    delete bb;
}


VertIdType Object3d::addVert(float x, float y, float z) {
    Vert* v = new Vert(verts->size(),x,y,z);
    verts->append(v);

    if(x < bb->bbmin.x) {
        bb->bbmin.x = x;
    }

    if(x > bb->bbmax.x) {
        bb->bbmax.x = x;
    }

    if(y < bb->bbmin.y) {
        bb->bbmin.y = y;
    }

    if(y > bb->bbmax.y) {
        bb->bbmax.y = y;
    }

    if(z < bb->bbmin.z) {
        bb->bbmin.z = z;
    }

    if(z > bb->bbmax.z) {
        bb->bbmax.z = z;
    }

    return v->getId();
}

EdgeIdType Object3d::addEdge(Vert* a, Vert* b) {

    VertIdType va = std::min(a->getId(),b->getId());
    VertIdType vb = std::max(a->getId(),b->getId());
    QString edgeName = Edge::makeEdgeName(va,vb);

    Edge* e = NULL;

    if(!edgemap->contains(edgeName)) {
        e = new Edge(edges->length(),verts->at(va),verts->at(vb));
        edgemap->insert(e->getEdgeName(),e);
        edges->append(e);
    } else {
        e = edgemap->value(edgeName);
    }

    return e->getId();
}

FaceIdType Object3d::addFace(QList<VertIdType>* fvids) {
    FaceIdType newid = faces->length();
    Face* face = new Face(this,newid);
    foreach(VertIdType v, *fvids) {
        face->addVert(this->verts->at(v));
    }
    face->build();
    faces->append(face);
    return face->getId();
}



void Object3d::initialBuild() {

    qDebug("Building halfedges");
    foreach(Face* face,*faces) {
        QList<HalfEdge*> heList;
        int edgeIndex = 0;
        int vertIndex = 0;
        foreach(Vert* v, *(face->getVerts())) {
            HalfEdge* he = new HalfEdge();
            //qDebug(QString("Vert %1").arg(v->getId()).toAscii().data());
            he->face = face;
            he->vert = v;

            VertIdType va = v->getId();

            VertIdType vb = 0;

            if(vertIndex == (face->getVerts()->length()-1)) {
                vb = face->getVerts()->at(0)->getId();
            } else {
                vb = face->getVerts()->at(++vertIndex)->getId();
            }

            he->normalizedEdge = this->getEdges()->at(face->getUndirectedEdgeIds()->at(edgeIndex++));
            // qDebug(QString("Edge: %1 %2").arg(he->normalizedEdge->getA()->getId()).arg(he->normalizedEdge->getB()->getId()).toAscii().data());

            if(he->normalizedEdge->getA() == this->getVerts()->at(va)) {
                he->reversedEdge = false;
            } else if(he->normalizedEdge->getA() == this->getVerts()->at(vb)) {
                he->reversedEdge = true;
            } else {
                qDebug("Broken edge match in HalfEdge build");
            }

            //qDebug(QString("Pair %1 %2").arg(va).arg(vb).toAscii().data());
            QString key = Edge::makeEdgeName(va,vb);
            he->key = key;
            qDebug(key.toAscii().data());
            halfEdges->insert(key,he);
            heList.append(he);
        }

        int index = 0;
        foreach(HalfEdge* he, heList) {
            if(index == heList.length()-1) {
                he->next = heList.first();
            } else {
                he->next = heList.at(index+1);
            }

            if(index == 0) {
                he->prev = heList.last();
            } else {
                he->prev = heList.at(index-1);
            }

//            qDebug(QString("Curr %1").arg(he->getDirectedEdgeId()).toAscii().data());
//            qDebug(QString("Next %1").arg(he->next->getDirectedEdgeId()).toAscii().data());
//            qDebug(QString("Prev %1").arg(he->prev->getDirectedEdgeId()).toAscii().data());
//            qDebug("XXXXXXXXXXXXXXXXXXXXXX");
            index++;
        }
    }
    foreach(HalfEdge* he, halfEdges->values()) {
        if(he->pair == 0) {
            QString peerId = Edge::makeEdgeName(he->getSecond(),he->getFirst());
            //QString peerId = edges->at(he->edge->getId())->getReversedEdge().getEdgeName();
            //qDebug(QString("Peer: %1").arg(peerId).toAscii().data());

            if(halfEdges->contains(peerId)) {
                HalfEdge* peer = (*halfEdges)[peerId];
                he->pair = peer;
                peer->pair = he;
            } else {
                //qDebug("No peer");
            }
        }
    }

    foreach(HalfEdge* he, halfEdges->values()) {
        if(he->pair == 0) {
            qDebug(QString("Empty peer: %1").arg(he->getDirectedEdgeId()).toAscii().data());
        }
    }
}


QList<Face*> *Object3d::getFaces() {
    return faces;
}


QList<Edge*> *Object3d::getEdges() {
    return edges;
}

QList<Vert*> *Object3d::getVerts() {
    return verts;
}

QMap<QString,HalfEdge*> *Object3d::getHalfEdges() {
    return halfEdges;
}

BoundingBox3d* Object3d::getBB() {
    return bb;
}


void Object3d::deselectAllVerts() {
    foreach(Vert *vert, *(verts)) {
        vert->deselect();
    }
}

void Object3d::deselectAllEdges() {
    foreach(Edge *edge, *(edges)) {
        edge->deselect();
    }
}

void Object3d::deselectAllFaces() {
    foreach(Face *face, *(faces)) {
        face->deselect();
    }
}


void Object3d::addToPython() {
    qDebug("Adding to Python...");
    //PyRun_SimpleString("app.addSceneObject(0)\n");
    PyRun_SimpleString("app.sceneObjects.append(Object3d(0))\n");
    PyRun_SimpleString("so = app.sceneObjects[0]\n");
    foreach(Vert *v, *verts) {
        QString py = QString("so.verts.append(Vert(%1,%2,%3,%4,False))\n")
                .arg(v->getId())
                .arg(v->x)
                .arg(v->y)
                .arg(v->z);

        PyRun_SimpleString(py.toAscii().constData());
    }

    foreach(Edge *e, *edges) {
        QString py = QString("so.edges.append(Edge(%1,so.verts[%2],so.verts[%3],False))\n")
                .arg(e->getId()).arg(e->getA()->getId()).arg(e->getB()->getId());
        PyRun_SimpleString(py.toAscii().constData());
    }

    foreach(Face *f, *faces) {
        QString py = QString("so.faces.append(Face(%1))\n")
                .arg(f->getId());
        PyRun_SimpleString(py.toAscii().constData());
    }

    QMapIterator<QString,HalfEdge*> i(*halfEdges);
    while (i.hasNext()) {
        i.next();
        HalfEdge* he = i.value();
        QString reversedVal;
        if(he->reversedEdge) {
            reversedVal=QString("True");
        } else {
            reversedVal=QString("False");
        }
        QString py = QString("so.halfEdges['%1'] = HalfEdge('%1',so.verts[%2],so.faces[%3],so.edges[%4],%5)\n")
                .arg(he->key)
                .arg(he->vert->getId())
                .arg(he->face->getId())
                .arg(he->normalizedEdge->getId())
                .arg(reversedVal);
        //vert,face,normalizedEdge,reversedEdge
        PyRun_SimpleString(py.toAscii().constData());
    }


    // now setup the HalfEdge next, prev pointers
    i.toFront();
    while (i.hasNext()) {
        i.next();
        HalfEdge* he = i.value();
        QString py1 = QString("so.halfEdges['%1'].prev = so.halfEdges['%2']")
                .arg(he->key)
                .arg(he->prev->key);
        QString py2 = QString("so.halfEdges['%1'].pair = so.halfEdges['%2']")
                .arg(he->key)
                .arg(he->pair->key);
        QString py3 = QString("so.halfEdges['%1'].next = so.halfEdges['%2']")
                .arg(he->key)
                .arg(he->next->key);
        PyRun_SimpleString(py1.toAscii().constData());
        PyRun_SimpleString(py2.toAscii().constData());
        PyRun_SimpleString(py3.toAscii().constData());
    }

    qDebug("Added to Python :-)");
}

void Object3d::updatePython() {
    // not implemented!
}

int Object3d::getObjectId() {
    return objectId;
}
