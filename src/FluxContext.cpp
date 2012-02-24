#include "FluxCore.h"
#include "FluxContext.h"
#include "FluxSceneRenderer.h"
#include "FluxDefaultInputHandler.h"
#include "FluxSceneRenderer.h"
#include "FluxInputStateStack.h"
#include "FluxOptions.h"

Context::Context() {
    scene = new Scene();
    inputHandler = new FluxEventHandlers::DefaultInputHandler(this);
    sceneRenderer = new SceneRenderer();
    options = new Options();
}

Context::~Context() {
    delete scene;
    delete inputHandler;
    delete sceneRenderer;
    delete options;
}

Scene* Context::getScene() {
    return scene;
}

InputHandler* Context::getInputHandler() {
    return inputHandler;
}

SceneRenderer* Context::getSceneRenderer() {
    return sceneRenderer;
}


Options* Context::getApplicationOptions() {
    return options;
}




Scene::Scene() {
    SceneView* sv = createSceneView();
    activeSceneView = sv;
}

Scene::~Scene() {
    foreach(Object3d* so,sceneObjects) {
        delete so;
    }
    foreach(SceneView* sv, sceneViews) {
        delete sv;
    }
}

void Scene::addSceneObject(Object3d* so) {
    sceneObjects.append(so);
    qDebug("Setting active so");
    activeSceneObject = so;
}

QList<Object3d*> Scene::getSceneObjects() {
    return sceneObjects;
}


SceneView* Scene::getActiveSceneView() {
    return activeSceneView;
}

SceneView* Scene::createSceneView() {
    SceneView* sv = new SceneView(sceneViews.length());
    sceneViews.append(sv);
    return sv;
}

void Scene::setActiveGLWidget(FluxGLWidget* gl) {
    activeGLWidget = gl;
}

FluxGLWidget* Scene::getActiveGLWidget() {
    return activeGLWidget;
}



SceneView::SceneView(int newid) : id(newid) {
    this->location = new V3(0.0,0.0,0.0);
    this->rotation = new V3(0.0,0.0,0.0);
    inputStateStack = new InputStateStack();
    this->selectionMode = smVerts;
    this->geomManipMode = gmmTranslate;
}

SceneView::~SceneView() {
    delete location;
    delete rotation;
    delete inputStateStack;
}


Object3d* SceneView::getActiveSceneObject() {
    return activeSceneObject;
}

void SceneView::setActiveSceneObject(Object3d* o) {
    activeSceneObject = o;
}

V3* SceneView::getLocation() {
    return location;
}

V3* SceneView::getRotation() {
    return rotation;
}

double SceneView::getScale() {
    return scale;
}

void SceneView::setScale(double d) {
    scale = d;
}

InputStateStack* SceneView::getInputStateStack() {
    return inputStateStack;
}

SelectionMode SceneView::getSelectionMode() {
    return selectionMode;
}

void SceneView::setSelectionMode(SelectionMode newMode) {
    selectionMode = newMode;
}


GeomManipMode SceneView::getGeomManipMode() {
    return geomManipMode;
}

void SceneView::setGeomManipMode(GeomManipMode mode) {
    geomManipMode = mode;
}
