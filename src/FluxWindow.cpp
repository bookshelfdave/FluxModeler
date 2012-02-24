#include "FluxWindow.h"

#include <QtGlobal>
#include <QtGui>
#include <QActionGroup>
#include <QMenu>

#include "FluxGLWidget.h"
#include "FluxContext.h"
#include "FluxCore.h"
#include "FluxInputHandler.h"
#include "FluxOptions.h"

FluxWindow::FluxWindow(Context* appCtx, QWidget* parent) :
    QMainWindow(parent), ctx(appCtx) {
    this->setWindowTitle("FluxModeler");
    this->createActions();
    this->createMenus();
    this->statusBar()->showMessage("Welcome to FluxModeler");
    fluxGl = new FluxGLWidget(appCtx,this);

    // for now...
    fluxGl->setSceneView(ctx->getScene()->getActiveSceneView());
    ctx->getScene()->setActiveGLWidget(fluxGl);

    this->setCentralWidget(fluxGl);
}

FluxWindow::~FluxWindow() {
    delete newAct;
    delete openAct;
    delete exitAct;

    delete selectVert;
    delete selectEdge;
    delete selectFace;
    delete selectObject;
    delete deselectAll;
    delete selectGroup;

    delete viewTris;

    delete fileMenu;
    delete selectMenu;
    delete viewMenu;

    delete fluxGl;
}


void FluxWindow::createActions() {
    newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create new scene"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newScene()));

    openAct = new QAction(tr("&Open"), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open scene"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(openScene()));


    exitAct = new QAction(tr("&Exit"),this);
    exitAct->setShortcut(QKeySequence::Quit);
    exitAct->setStatusTip("Exit FluxModeler");
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));


    selectVert = new QAction(tr("Select Verts"),this);
    selectVert->setStatusTip(tr("Set selection mode to vertices"));
    selectVert->setCheckable(true);
    selectVert->setChecked(true);

    selectEdge = new QAction(tr("Select Edges"),this);
    selectEdge->setStatusTip(tr("Set selection mode to edges"));
    selectEdge->setCheckable(true);

    selectFace = new QAction(tr("Select Faces"),this);
    selectFace->setStatusTip(tr("Set selection mode to faces"));
    selectFace->setCheckable(true);

    selectObject = new QAction(tr("Select Objects"),this);
    selectObject->setStatusTip(tr("Set selection mode to objects"));
    selectObject->setCheckable(true);

    selectGroup = new QActionGroup(this);
    selectGroup->addAction(selectVert);
    selectGroup->addAction(selectEdge);
    selectGroup->addAction(selectFace);
    selectGroup->addAction(selectObject);
    connect(selectGroup,SIGNAL(triggered(QAction*)),this,SLOT(selectionMode(QAction*)));

    deselectAll = new QAction(tr("Deselect All"),this);
    connect(deselectAll,SIGNAL(triggered()),this,SLOT(deselect()));
    deselectAll->setStatusTip(tr("Deselect all selected items"));

    viewTris = new QAction(tr("Triangles"),this);
    viewTris->setCheckable(true);
    viewTris->setChecked(false);
    connect(viewTris,SIGNAL(triggered()),this,SLOT(viewOptionTris()));


    viewCenters = new QAction(tr("Centers"),this);
    viewCenters->setCheckable(true);
    viewCenters->setChecked(false);
    connect(viewCenters,SIGNAL(triggered()),this,SLOT(viewOptionCenters()));
}

void FluxWindow::createMenus() {
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(exitAct);

    selectMenu = menuBar()->addMenu(tr("Select"));
    selectMenu->addAction(selectVert);
    selectMenu->addAction(selectEdge);
    selectMenu->addAction(selectFace);
    selectMenu->addAction(selectObject);
    selectMenu->addSeparator();
    selectMenu->addAction(deselectAll);

    viewMenu = menuBar()->addMenu(tr("View"));
    viewMenu->addAction(viewTris);
    viewMenu->addAction(viewCenters);
}


void FluxWindow::newScene() {
    qDebug("New Scene");
}

void FluxWindow::openScene() {
    qDebug("Open Scene");
}

void FluxWindow::deselect() {
    foreach(Object3d* obj, ctx->getScene()->getSceneObjects()) {
        switch(ctx->getScene()->getActiveSceneView()->getSelectionMode()) {
        case smVerts:
            obj->deselectAllVerts();
            break;
        case smEdges:
            obj->deselectAllEdges();
            break;
        case smFaces:
            obj->deselectAllFaces();
            break;
        case smObjects:
            qDebug("Not implemented");
            break;
        }
    }
    ctx->getScene()->getActiveGLWidget()->updateGL();
}

void FluxWindow::selectionMode(QAction* action) {
    if(action == selectVert) {
        ctx->getScene()->getActiveSceneView()->setSelectionMode(smVerts);
    } else if(action == selectEdge) {
        ctx->getScene()->getActiveSceneView()->setSelectionMode(smEdges);
    } else if(action == selectFace) {
        ctx->getScene()->getActiveSceneView()->setSelectionMode(smFaces);
    } else if(action == selectObject) {
        ctx->getScene()->getActiveSceneView()->setSelectionMode(smObjects);
    }
}

void FluxWindow::viewOptionTris() {
    if(ctx->getApplicationOptions()->getBoolOption("showtris",false)) {
        qDebug("Setting to false");
        ctx->getApplicationOptions()->setBoolOption("showtris",false);
    } else {
        qDebug("Setting to true");
        ctx->getApplicationOptions()->setBoolOption("showtris",true);
    }

}

void FluxWindow::viewOptionCenters() {
    if(ctx->getApplicationOptions()->getBoolOption("showcenters",false)) {
        qDebug("Setting to false");
        ctx->getApplicationOptions()->setBoolOption("showcenters",false);
    } else {
        qDebug("Setting to true");
        ctx->getApplicationOptions()->setBoolOption("showcenters",true);
    }
}
