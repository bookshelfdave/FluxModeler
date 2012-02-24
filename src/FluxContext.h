#ifndef FLUXCONTEXT_H
#define FLUXCONTEXT_H
#include <QList>

#include "FluxCommon.h"
#include "FluxSelection.h"
/*
    Note: I may need to rethink linking sceneviews to glwdigets...

     Context
        |-> InputHandler
        |-> SceneRenderer
        |-> Scene
            |-> Object3d+
            |-> ActiveSceneView
            |-> SceneViews+
                |-> ActiveSceneObject
                |-> SelectionMode
                |-> Scene Loc, Rot, Scale
                |-> InputStateStack (could get tricky if scene view is changed)
                    (cant change sceneview if statestack depth > 1?)
*/

class V3;
class Object3d;
class SceneView;
class SceneRenderer;
class FluxGLWidget;
class InputStateStack;
class InputHandler;
class Options;


enum SelectionMode {
    smVerts,
    smEdges,
    smFaces,
    smObjects
};

enum ObjManipMode {
    ommTranslate,
    ommRotate,
    ommScale
};


enum GeomManipMode {
    gmmTranslate,
    gmmRotate,
    gmmScale
};


class Scene {
public:
    Scene();
    ~Scene();
    void addSceneObject(Object3d* so);
    QList<Object3d*> getSceneObjects();

    SceneView* createSceneView();
    SceneView* getActiveSceneView();

    void setActiveGLWidget(FluxGLWidget*);
    FluxGLWidget*  getActiveGLWidget();

private:
    QList<Object3d*> sceneObjects;
    Object3d* activeSceneObject;

    QList<SceneView*> sceneViews;
    SceneView* activeSceneView;

    FluxGLWidget* activeGLWidget;
    DISALLOW_COPY_AND_ASSIGN(Scene);
};


/*
|-> GLWidget
|-> ActiveSceneObject
|-> SelectionMode
|-> Scene Loc, Rot, Scale
|-> InputStateStack (could get tricky if scene view is changed)
    (cant change sceneview if statestack depth > 1?)
*/

class SceneView {
public:
    SceneView(int newid);
    ~SceneView();
    Object3d* getActiveSceneObject();
    void setActiveSceneObject(Object3d*);
    V3* getLocation();
    V3* getRotation();
    double getScale();
    void setScale(double d);
    InputStateStack* getInputStateStack();
    SelectionMode getSelectionMode();
    void setSelectionMode(SelectionMode);

    GeomManipMode getGeomManipMode();
    void setGeomManipMode(GeomManipMode mode);
private:
    int id;
    Object3d* activeSceneObject;

    V3* location;
    V3* rotation;
    double scale;

    SelectionMode selectionMode;
    GeomManipMode geomManipMode;
    InputStateStack* inputStateStack;
    DISALLOW_COPY_AND_ASSIGN(SceneView);
};


class Context {
public:
    Context();
    ~Context();
    Scene* getScene();
    InputHandler* getInputHandler();
    SceneRenderer* getSceneRenderer();    
    Options* getApplicationOptions();

private:
    Scene* scene;
    InputHandler* inputHandler;
    SceneRenderer* sceneRenderer;
    DISALLOW_COPY_AND_ASSIGN(Context);
    Options* options;
};




#endif // FLUXCONTEXT_H
