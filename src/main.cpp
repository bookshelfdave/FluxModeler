/*
  Copyright 2010 Dave Parfitt
*/
#include <stdlib.h>
#include <QApplication>
#include <QDesktopWidget>
#include <QtGlobal>
#include <QString>
#include <Python.h>

#include "FluxWindow.h"
#include "FluxObjParser.h"
#include "FluxCore.h"
#include "FluxData.h"
#include "FluxInputStateStack.h"
#include "FluxInputHandler.h"
#include "FluxContext.h"

#include "FluxLib.h"

int main(int argc, char* argv[]) {


    // initialize this first!
    qDebug("Starting...");
    Context* ctx = new Context();


    ObjParser p;
    Object3d* so = p.parseObject("c:\\src\\grid.obj");
    so->initialBuild();
    //so->addToPython();
    ctx->getScene()->addSceneObject(so);
    //qDebug("Done loading obj file");
    //return 0;



    char pySearchPath[] = "c:\\tools\\Python27";
    Py_SetPythonHome(pySearchPath);
    //qDebug(Py_GetPath());
    //qDebug(QDir::currentPath().toAscii().data());
    Py_Initialize( );
    qDebug("Python initialized");
    initFluxApp(ctx);
    PyRun_SimpleString("import sys\nsys.path.append(\"c:\\src\\metasrc\\FluxModeler\\lib\")\n");
    PyRun_SimpleString("import flux.mode\nflux.mode.standalone=False\n");
    PyRun_SimpleString("from flux.core import *\n");
    PyRun_SimpleString("import flux.root\n");
    PyRun_SimpleString("from flux.root import *\n");    

    SceneView* sv = ctx->getScene()->getActiveSceneView();
    V3* loc = sv->getLocation();
    loc->z = -20.0;

    V3* rot = sv->getRotation();
    rot->x = 20.0;
    rot->y = 20.0;

    sv->setScale(4.0);



    QApplication app(argc, argv);

    FluxWindow window(ctx);
    window.resize(1024,768);
    int desktopArea = QApplication::desktop()->width() *
                      QApplication::desktop()->height();
    int widgetArea = window.width() * window.height();

    if (((float)widgetArea / (float)desktopArea) < 0.75f)
        window.show();
    else
        window.showMaximized();

    return app.exec();
}
