#include "FluxGLWidget.h"

#include <QWidget>
#include <QGLWidget>
#include <QColor>
#include "FluxCommon.h"
#include "FluxCore.h"
#include "FluxSceneRenderer.h"
#include "FluxContext.h"
#include "FluxInputHandler.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

FluxGLWidget::FluxGLWidget(Context* appCtx, QWidget* parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent), ctx(appCtx) {

    bgColor = QColor::fromRgb(70,70,70);
    // need to set
    setFocusPolicy(Qt::StrongFocus);
}

FluxGLWidget::~FluxGLWidget() {

}

void FluxGLWidget::setSceneView(SceneView* sv) {
    sceneView = sv;
}



void FluxGLWidget::initializeGL() {
    qglClearColor(bgColor);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_MULTISAMPLE);
//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT0);

//    static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
//    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    glEnable(GL_POINT_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable (GL_LINE_SMOOTH);

    //float z = (float)width() / (float)height();
    //int side = qMin(width, height);
    //glViewport((width - side) / 2, (height - side) / 2, side, side);
    glViewport(0,0,width(),height());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glDepthRange(0.0,100.0);
    //glFrustum(-1.0,1.0,-z,z,4.0,60.0);
    gluPerspective(45.0f,((GLfloat)width()/(GLfloat)height()),1.0f,50.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // does this even need to be here anymore?
    V3* loc = sceneView->getLocation();
    glTranslatef(loc->x, loc->y, loc->z);

}

void FluxGLWidget::resizeGL(int w, int h) {
    glViewport(0,0,width(),height());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glDepthRange(0.0,100.0);
    gluPerspective(45.0f,((GLfloat)width()/(GLfloat)height()),1.0f,50.0f);

    glMatrixMode(GL_MODELVIEW);
}

void FluxGLWidget::paintGL() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    V3* loc = sceneView->getLocation();
    V3* rot = sceneView->getRotation();
    double scale = sceneView->getScale();

    glTranslatef(loc->x,loc->y,loc->z);
    glRotatef(rot->x,1.0f,0.0f,0.0f);
    glRotatef(rot->y,0.0f,1.0f,0.0f);
    glRotatef(rot->z,0.0f,0.0f,1.0f);

    glScalef(scale,scale,scale);

    ctx->getSceneRenderer()->render(this,ctx,sceneView);
}


void FluxGLWidget::mousePressEvent(QMouseEvent* event) {
    ctx->getInputHandler()->handleMousePressEvent(event);
}

void FluxGLWidget::mouseReleaseEvent(QMouseEvent* event) {
    ctx->getInputHandler()->handleMouseReleaseEvent(event);
}


void FluxGLWidget::mouseMoveEvent(QMouseEvent* event) {
    ctx->getInputHandler()->handleMouseMoveEvent(event);
}

void FluxGLWidget::wheelEvent(QWheelEvent* event) {
    ctx->getInputHandler()->handleWheelEvent(event);
}

void FluxGLWidget::keyPressEvent(QKeyEvent* event) {
    ctx->getInputHandler()->handleKeyPressEvent(event);
}

void FluxGLWidget::keyReleaseEvent(QKeyEvent* event) {
    ctx->getInputHandler()->handleKeyReleaseEvent(event);
}
