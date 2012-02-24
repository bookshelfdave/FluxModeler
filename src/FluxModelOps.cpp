#include "FluxModelOps.h"
#include "FluxCore.h"
#include "math.h"

ModelOps::ModelOps() {
}


// need a circular buffer class...

void ModelOps::splitFaceAtVerts(Context* ctx, Face* f) {
    QList<Vert*> selectedVerts;
    QList<int> selectedIndices;
    int indexInFace = 0;
    qDebug("----------------");
    foreach(Vert* v, *(f->getVerts())) {
        if(v->isSelected()) {
            selectedVerts.append(v);
            selectedIndices.append(indexInFace);
        }

        indexInFace++;
    }

    if(selectedVerts.count() != 2) {
        qDebug("Not enough selected verts");
        return;
    }

    int index0 = fmin(selectedIndices.at(0),selectedIndices.at(1));
    int index1 = fmax(selectedIndices.at(0),selectedIndices.at(1));
    qDebug(QString("Index 0: %1").arg(index0).toAscii().data());
    qDebug(QString("Index 1: %1").arg(index1).toAscii().data());

    int diff = fabs(index1 - index0);

    if(diff == 1) {
        qDebug("Cannot split on adjacent verts");
        return;
    }

    int lastVertIndex = f->getVerts()->size()-1;

    if((index0 == 0 && index1 == lastVertIndex) || (index0 == lastVertIndex && index1 == 0)) {
        qDebug("Cannot split on adjacent verts 2");
        return;
    }

    qDebug("Shape 1");

    for(int i = index0; i <= index1; i++) {
        Edge* edge = f->getParent()->getEdges()->at(i);
        qDebug(QString("Edge: %1").arg(edge->getEdgeName()).toAscii().data());
    }

//    qDebug("Shape 2");
//    for(int i = index1; i <= index1; i++) {
//        Edge *edge = f->getParent()->getEdges()->at(i);
//        qDebug(QString("Edge: %1").arg(edge->getEdgeName()).toAscii().data());
//    }

    //HalfEdge* he = f->getParent()->getHalfEdges()->value("1-2");

}
