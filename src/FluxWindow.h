#ifndef FLUXWINDOW_H
#define FLUXWINDOW_H

#include <QtGui>
#include "FluxContext.h"

class FluxGLWidget;
class GeomStatusBar;

class FluxWindow : public QMainWindow {
    Q_OBJECT
public:
    FluxWindow(Context* ctx, QWidget* parent = 0);
    ~FluxWindow();


signals:   

private slots:
    void newScene();
    void openScene();
    void selectionMode(QAction*);
    void viewOptionTris();
    void viewOptionCenters();
    void deselect();

private:

    void createActions();
    void createMenus();

    QAction* newAct;
    QAction* openAct;
    QAction* exitAct;

    QAction* selectVert;
    QAction* selectEdge;
    QAction* selectFace;
    QAction* selectObject;
    QAction* deselectAll;
    QActionGroup* selectGroup;

    // should probably be per object
    QAction* viewTris;
    QAction* viewCenters;

    QMenu* fileMenu;
    QMenu* selectMenu;
    QMenu* viewMenu;

    GeomStatusBar* geomStatusBar;
    FluxGLWidget* fluxGl;
    Context* ctx;
    DISALLOW_COPY_AND_ASSIGN(FluxWindow);
};

#endif // FLUXWINDOW_H
