/*
  Copyright 2010 Dave Parfitt
*/
#include <QApplication>
#include <QDesktopWidget>
#include <QtGlobal>
#include <QString>

#include "FluxWindow.h"
#include "FluxObjParser.h"
#include "FluxCore.h"
#include "FluxData.h"
#include "FluxInputStateStack.h"
#include "FluxInputHandler.h"
#include "FluxContext.h"


int main(int argc, char* argv[]) {
    // initialize this first!
    Context* ctx = new Context();

    ObjParser p;
    Object3d* so = p.parseObject("c:\\src\\grid.obj");
    so->initialBuild();
    ctx->getScene()->addSceneObject(so);
    qDebug("Done loading obj file");
    //return 0;

    SceneView* sv = ctx->getScene()->getActiveSceneView();
    V3* loc = sv->getLocation();
    loc->z = -20.0;

    V3* rot = sv->getRotation();
    rot->x = 20.0;
    rot->y = 20.0;

    sv->setScale(4.0);

    qDebug("Starting...");
    QApplication app(argc, argv);
    FluxWindow window(ctx);
    window.resize(800,600);
    int desktopArea = QApplication::desktop()->width() *
                      QApplication::desktop()->height();
    int widgetArea = window.width() * window.height();

    if (((float)widgetArea / (float)desktopArea) < 0.75f)
        window.show();
    else
        window.showMaximized();

    return app.exec();
}
