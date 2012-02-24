#ifndef FLUXGLWIDGET_H
#define FLUXGLWIDGET_H

#include <QWidget>
#include <QGLWidget>
#include <QColor>
#include "FluxCommon.h"

class Context;
class SceneView;

class FluxGLWidget : public QGLWidget {
    Q_OBJECT
public:
    FluxGLWidget(Context* ctx, QWidget* parent = 0);
    ~FluxGLWidget();
    void setSceneView(SceneView*);
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void wheelEvent(QWheelEvent*);
    void keyPressEvent(QKeyEvent*);
    void keyReleaseEvent(QKeyEvent*);

signals:

public slots:

private:
    QColor bgColor;
    SceneView* sceneView;
    Context* ctx;
    DISALLOW_COPY_AND_ASSIGN(FluxGLWidget);
};

#endif // FLUXGLWIDGET_H
