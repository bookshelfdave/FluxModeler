#include "FluxDefaultInputHandler.h"

#include <QList>
#include <QSet>

#include <math.h>

#include "FluxCore.h"
#include "FluxContext.h"
#include "FluxGLWidget.h"
#include "FluxSelection.h"
#include "FluxModelOps.h"
#include "FluxMath3d.h"

using namespace FluxEventHandlers;

DefaultInputHandler::DefaultInputHandler(Context* newCtx) : InputHandler(newCtx) {
    this->installMouseHandler(new SceneRotateHandler());
    this->installMouseHandler(new SceneTranslateHandler());
    this->installMouseHandler(new SelectionHandler());
    this->installMouseHandler(new GeomManipHandler());

    this->installKeyboardHandler(new DeselectHandler());
    this->installKeyboardHandler(new SplitFaceHandler());
    this->installKeyboardHandler(new SelectEdgeRingHandler());
    this->installKeyboardHandler(new SelectEdgeLoopHandler());

    this->installKeyboardHandler(new GeomTranslateHandler());
    this->installKeyboardHandler(new GeomRotateHandler());
    this->installKeyboardHandler(new GeomScaleHandler());


}


SceneRotateHandler::SceneRotateHandler() {
    mouseDownInputState=isSelect;
    mouseUpInputState=isSceneManip;
    keyModifiers = Qt::ControlModifier;
    mouseButtons = Qt::LeftButton;
    respondToMouseMove = true;
    lastPosition = new QPoint(0,0);
}

SceneRotateHandler::~SceneRotateHandler() {
    delete lastPosition;
}

void SceneRotateHandler::buttonDown(QMouseEvent* event,Context* ctx) {
    ctx->getScene()->getActiveSceneView()->getInputStateStack()->pushState(isSceneManip);
    lastPosition->setX(event->pos().x());
    lastPosition->setY(event->pos().y());
}


void SceneRotateHandler::buttonUp(QMouseEvent* event,Context* ctx) {
    ctx->getScene()->getActiveSceneView()->getInputStateStack()->popState();
}


void SceneRotateHandler::mouseMove(QMouseEvent* event,Context* ctx) {
    int dx = event->x() - lastPosition->x();
    int dy = lastPosition->y() - event->y();
    FluxGLWidget* glWidget = ctx->getScene()->getActiveGLWidget();
    // TODO: change to doubles?
    float thetaY = 360.0 * ((float)dx / ((float)glWidget->width()));
    float thetaX = 360.0 * ((float)dy / ((float)glWidget->height()));
    V3* rot = ctx->getScene()->getActiveSceneView()->getRotation();

    rot->x = rot->x - thetaX;
    rot->y = rot->y + thetaY;
    glWidget->updateGL();
    lastPosition->setX(event->pos().x());
    lastPosition->setY(event->pos().y());
}

void SceneRotateHandler::mouseWheel(QWheelEvent* event,Context* ctx) {
    double scale = ctx->getScene()->getActiveSceneView()->getScale();

    if(event->delta() <0.0) {
        ctx->getScene()->getActiveSceneView()->setScale(scale - 0.3);
    } else {
        ctx->getScene()->getActiveSceneView()->setScale(scale + 0.3);
    }

    FluxGLWidget* glWidget = ctx->getScene()->getActiveGLWidget();
    glWidget->updateGL();
}



SceneTranslateHandler::SceneTranslateHandler() {
    mouseDownInputState=isSelect;
    mouseUpInputState=isSceneManip;
    keyModifiers = Qt::ControlModifier;
    mouseButtons = Qt::RightButton;
    respondToMouseMove = true;
    lastPosition = new QPoint(0,0);
}

SceneTranslateHandler::~SceneTranslateHandler() {
    delete lastPosition;
}


void SceneTranslateHandler::buttonDown(QMouseEvent* event,Context* ctx) {
    ctx->getScene()->getActiveSceneView()->getInputStateStack()->pushState(isSceneManip);
    lastPosition->setX(event->pos().x());
    lastPosition->setY(event->pos().y());
}

void SceneTranslateHandler::buttonUp(QMouseEvent* event,Context* ctx) {
    ctx->getScene()->getActiveSceneView()->getInputStateStack()->popState();
}

void SceneTranslateHandler::mouseMove(QMouseEvent* event,Context* ctx) {
    int dx = event->x() - lastPosition->x();
    int dy = lastPosition->y() - event->y();
    FluxGLWidget* glWidget = ctx->getScene()->getActiveGLWidget();
    // TODO: change to doubles?
    float thetaY = 360.0 * ((float)dx / ((float)glWidget->width()));
    float thetaX = 360.0 * ((float)dy / ((float)glWidget->height()));
    V3* loc = ctx->getScene()->getActiveSceneView()->getLocation();
    loc->x = loc->x + (thetaY / 10.0);
    loc->y = loc->y + (thetaX / 10.0);
    glWidget->updateGL();
    lastPosition->setX(event->pos().x());
    lastPosition->setY(event->pos().y());
}

void SceneTranslateHandler::mouseWheel(QWheelEvent* event,Context* ctx) {

}






SelectionHandler::SelectionHandler() {
    mouseDownInputState=isSelect;
    mouseUpInputState=isGeomManip;
    keyModifiers = Qt::NoModifier;
    mouseButtons = Qt::LeftButton;
    respondToMouseMove = true;
}

SelectionHandler::~SelectionHandler() {
}

void SelectionHandler::buttonDown(QMouseEvent* event,Context* ctx) {
    Selection selection;

    switch(ctx->getScene()->getActiveSceneView()->getSelectionMode()) {
    case smVerts:
        selection.selectVert(ctx,event->x(), event->y());
        break;
    case smEdges:
        selection.selectEdge(ctx,event->x(), event->y());
        break;
    case smFaces:
        selection.selectFace(ctx,event->x(), event->y());
        break;
    case smObjects:
        break;
    }

    ctx->getScene()->getActiveGLWidget()->updateGL();
}


void SelectionHandler::buttonUp(QMouseEvent* event,Context* ctx) {
}


void SelectionHandler::mouseMove(QMouseEvent* event,Context* ctx) {

}

void SelectionHandler::mouseWheel(QWheelEvent* event,Context* ctx) {

}




GeomManipHandler::GeomManipHandler() {
    mouseDownInputState=isSelect;
    mouseUpInputState=isGeomManip;
    keyModifiers = Qt::ShiftModifier;
    mouseButtons = Qt::LeftButton;
    respondToMouseMove = true;
    lastPosition = new QPoint(0,0);
}

GeomManipHandler::~GeomManipHandler() {
    delete lastPosition;
}

void GeomManipHandler::buttonDown(QMouseEvent* event,Context* ctx) {
    ctx->getScene()->getActiveSceneView()->getInputStateStack()->pushState(isGeomManip);
    lastPosition->setX(event->pos().x());
    lastPosition->setY(event->pos().y());
    lastVert = Math3d::unProject(event->pos().x(), event->pos().y());
}


void GeomManipHandler::buttonUp(QMouseEvent* event,Context* ctx) {
    ctx->getScene()->getActiveSceneView()->getInputStateStack()->popState();
}


void GeomManipHandler::mouseMove(QMouseEvent* event,Context* ctx) {
    qDebug("Moving!");
    // TODO
    Object3d* so = ctx->getScene()->getSceneObjects().first();
    BoundingBox3d* bb = so->getBB();

    QList<Vert*> vertSelection;
    // TODO: Move this out of here!!
    foreach(Vert* v, *(so->getVerts())) {
        if(v->isSelected()) {
            qDebug("Adding selected vert");
            vertSelection.append(v);
        }
    }

    V3 currVert  = Math3d::unProject(event->pos().x(), event->pos().y());

    double incx = fabs(bb->bbmax.x - bb->bbmin.x) / 100;
    double incy = fabs(bb->bbmax.y - bb->bbmin.y) / 100;
    double incz = fabs(bb->bbmax.z - bb->bbmin.z) / 100;
    if(incx == 0.0) {
        incx = 0.01;
    }
    if(incy == 0.0) {
        incy = 0.01;
    }
    if(incz == 0.0) {
        incz = 0.01;
    }


    qDebug(QString("Incs %1 %2 %3").arg(incx).arg(incy).arg(incz).toAscii().data());
    //int diffx = event->x() - lastPosition->x();
    //int diffy = event->x() - lastPosition->y();

    foreach(Vert* v, vertSelection) {
        if(currVert.x > lastVert.x) {
            v->x += incx;
        }
        if(currVert.x < lastVert.x) {
            v->x -= incx;
        }


        if(currVert.y > lastVert.y) {
            v->y += incy;
        }
        if(currVert.y < lastVert.y) {
            v->y -= incy;
        }


        if(currVert.z > lastVert.z) {
            v->z += incz;
        }
        if(currVert.z < lastVert.z) {
            v->z -= incz;
        }
    }

    ctx->getScene()->getActiveGLWidget()->updateGL();
    lastPosition->setX(event->pos().x());
    lastPosition->setY(event->pos().y());
    //lastVert = Math3d::unProject(event->pos().x(), event->pos().y());
}

void GeomManipHandler::mouseWheel(QWheelEvent* event,Context* ctx) {

}






DeselectHandler::DeselectHandler() {
    key = Qt::Key_Space;
    keyModifiers = Qt::NoModifier;
    keyDownInputState=isSelect;
    keyUpInputState=isSelect;
}

DeselectHandler::~DeselectHandler() {

}

void DeselectHandler::keyPress(QKeyEvent* event, Context* ctx) {
    QList<Object3d*> sos = ctx->getScene()->getSceneObjects();

    foreach(Object3d* so, sos) {
        switch(ctx->getScene()->getActiveSceneView()->getSelectionMode()) {
        case smVerts:
            so->deselectAllVerts();
            break;
        case smEdges:
            so->deselectAllEdges();
            break;
        case smFaces:
            so->deselectAllFaces();
            break;

        case smObjects:
            break;
        }
    }
    ctx->getScene()->getActiveGLWidget()->updateGL();
}

void DeselectHandler::keyRelease(QKeyEvent* event, Context* ctx) {

}






SelectEdgeRingHandler::SelectEdgeRingHandler() {
    key = Qt::Key_R;
    keyModifiers = Qt::ShiftModifier;
    keyDownInputState=isSelect;
    keyUpInputState=isSelect;
}

SelectEdgeRingHandler::~SelectEdgeRingHandler() {

}

void SelectEdgeRingHandler::keyPress(QKeyEvent* event, Context* ctx) {
    QList<Object3d*> sos = ctx->getScene()->getSceneObjects();
    Edge* startingEdge;
    int selectCount = 0;

    foreach(Edge* e, *(sos.first()->getEdges())) {
        if(e->isSelected()) {
            startingEdge = e;
            selectCount++;
        }
    }

    if(selectCount != 1) {
        // although... at some point, it would be nice to be able to select an edge
        // ring with some of the segments already selected
        qDebug("Cannot select edge ring with more than 1 edge selected");
        return;
    }

    HalfEdge* startingHe = sos.first()->getHalfEdges()->value(startingEdge->getEdgeName());
//    if(startingHe->face->getVerts()->length() % 2 != 0) {
//        qDebug("Cannot select edge ring on a face with an odd amount of verts");
//        return;
//    }
    HalfEdge* ccw = startingHe->next->next->pair;
    HalfEdge* cw =  startingHe->prev->prev->pair;

    while(true) {
        if(ccw == cw) {
            cw->normalizedEdge->select();
        } else {
            break;
        }

        ccw = ccw->next->next->pair;
        cw = cw->prev->prev->pair;
    }


    ccw = startingHe->pair->next->next;
    cw =  startingHe->pair->prev->prev;

    while(true) {
        if(ccw == cw) {
            cw->normalizedEdge->select();
        } else {
            break;
        }

        ccw = ccw->pair->next->next;
        cw = cw->pair->prev->prev;
    }

    ctx->getScene()->getActiveGLWidget()->updateGL();
}

void SelectEdgeRingHandler::keyRelease(QKeyEvent* event, Context* ctx) {

}


SelectEdgeLoopHandler::SelectEdgeLoopHandler() {
    key = Qt::Key_L;
    keyModifiers = Qt::ShiftModifier;
    keyDownInputState=isSelect;
    keyUpInputState=isSelect;
}

SelectEdgeLoopHandler::~SelectEdgeLoopHandler() {

}

void SelectEdgeLoopHandler::keyPress(QKeyEvent* event, Context* ctx) {
    qDebug("Selecting edge loop");
    QList<Object3d*> sos = ctx->getScene()->getSceneObjects();
    Edge* startingEdge;
    int selectCount = 0;

    foreach(Edge* e, *(sos.first()->getEdges())) {
        if(e->isSelected()) {
            startingEdge = e;
            selectCount++;
        }
    }

    if(selectCount != 1) {
        // although... at some point, it would be nice to be able to select an edge
        // ring with some of the segments already selected
        qDebug("Cannot select edge ring with more than 1 edge selected");
        return;
    }

    HalfEdge* startingHe = sos.first()->getHalfEdges()->value(startingEdge->getEdgeName());
    HalfEdge* ccw = startingHe->next->pair->next;
    HalfEdge* cw =  startingHe->pair->prev->pair->prev->pair;

    while(true) {
        if(ccw->normalizedEdge == cw->normalizedEdge) {
            cw->normalizedEdge->select();
        } else {
            break;
        }

        ccw = ccw->next->pair->next;
        cw =  cw->pair->prev->pair->prev->pair;
    }


    ccw = startingHe->prev->pair->prev;
    cw =  startingHe->pair->next->pair->next->pair;

    while(true) {
        if(ccw->normalizedEdge == cw->normalizedEdge) {
            cw->normalizedEdge->select();
        } else {
            break;
        }

        ccw = ccw->prev->pair->prev;
        cw =  cw->pair->next->pair->next->pair;
    }



    ctx->getScene()->getActiveGLWidget()->updateGL();
}

void SelectEdgeLoopHandler::keyRelease(QKeyEvent* event, Context* ctx) {

}













SplitFaceHandler::SplitFaceHandler() {
    key = Qt::Key_X;
    keyModifiers = Qt::NoModifier;
    keyDownInputState=isSelect;
    keyUpInputState=isSelect;
}

SplitFaceHandler::~SplitFaceHandler() {

}

void SplitFaceHandler::keyPress(QKeyEvent* event, Context* ctx) {
    QList<Object3d*> sos = ctx->getScene()->getSceneObjects();
    QSet<Face*> facesToSplit;
    QMap<Face*,int> faceSelectCount;

    foreach(Face* f, *(sos.first()->getFaces())) {
        foreach(Vert* v, *(f->getVerts())) {
            if(v->isSelected()) {
                if(!faceSelectCount.contains(f)) {
                    faceSelectCount.insert(f,1);
                } else {
                    int count =faceSelectCount[f];
                    faceSelectCount.insert(f,count+1);
                }
            }
        }
    }

    ModelOps ops;

    foreach(Face* f, faceSelectCount.keys()) {
        int count = faceSelectCount[f];

        if(count > 1) {
            ops.splitFaceAtVerts(ctx,f);
        }
    }

    ctx->getScene()->getActiveGLWidget()->updateGL();
}

void SplitFaceHandler::keyRelease(QKeyEvent* event, Context* ctx) {

}



GeomTranslateHandler::GeomTranslateHandler() {
    key = Qt::Key_W;
    keyModifiers = Qt::NoModifier;
    keyDownInputState=isSelect;
    keyUpInputState=isSelect;
}
GeomTranslateHandler::~GeomTranslateHandler() {

}
void GeomTranslateHandler::keyPress(QKeyEvent* event, Context* ctx) {
    ctx->getScene()->getActiveSceneView()->setGeomManipMode(gmmTranslate);
    qDebug("Geometry translate mode");
}

void GeomTranslateHandler::keyRelease(QKeyEvent* event, Context* ctx) {
}




GeomRotateHandler::GeomRotateHandler() {
    key = Qt::Key_R;
    keyModifiers = Qt::NoModifier;
    keyDownInputState=isSelect;
    keyUpInputState=isSelect;
}
GeomRotateHandler::~GeomRotateHandler() {

}
void GeomRotateHandler::keyPress(QKeyEvent* event, Context* ctx) {
    ctx->getScene()->getActiveSceneView()->setGeomManipMode(gmmRotate);
    qDebug("Geometry rotate mode");
}

void GeomRotateHandler::keyRelease(QKeyEvent* event, Context* ctx) {
}





GeomScaleHandler::GeomScaleHandler() {
    key = Qt::Key_E;
    keyModifiers = Qt::NoModifier;
    keyDownInputState=isSelect;
    keyUpInputState=isSelect;
}
GeomScaleHandler::~GeomScaleHandler() {

}
void GeomScaleHandler::keyPress(QKeyEvent* event, Context* ctx) {
    ctx->getScene()->getActiveSceneView()->setGeomManipMode(gmmScale);
    qDebug("Geometry scale mode");
}

void GeomScaleHandler::keyRelease(QKeyEvent* event, Context* ctx) {
}


