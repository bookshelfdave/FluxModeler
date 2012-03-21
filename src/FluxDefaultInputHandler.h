#ifndef FLUXDEFAULTINPUTHANDLER_H
#define FLUXDEFAULTINPUTHANDLER_H

#include <QMouseEvent>
#include <QWheelEvent>
#include <QPoint>

#include "FluxInputHandler.h"
#include "FluxCore.h"
#include "FluxCommon.h"

class Context;

namespace FluxEventHandlers {
class DefaultInputHandler : public InputHandler {
public:
    DefaultInputHandler(Context*);

private:
    DISALLOW_COPY_AND_ASSIGN(DefaultInputHandler);
};


class SceneRotateHandler : public MouseEventHandler {
public:
    SceneRotateHandler();
    ~SceneRotateHandler();
    void buttonUp(QMouseEvent* event,Context* ctx);
    void buttonDown(QMouseEvent* event,Context* ctx);
    void mouseMove(QMouseEvent* event,Context* ctx);
    void mouseWheel(QWheelEvent* event,Context* ctx);
private:
    QPoint* lastPosition;
    DISALLOW_COPY_AND_ASSIGN(SceneRotateHandler);
};


class SceneTranslateHandler : public MouseEventHandler {
public:
    SceneTranslateHandler();
    ~SceneTranslateHandler();
    void buttonUp(QMouseEvent* event,Context* ctx);
    void buttonDown(QMouseEvent* event,Context* ctx);
    void mouseMove(QMouseEvent* event,Context* ctx);
    void mouseWheel(QWheelEvent* event,Context* ctx);
private:
    QPoint* lastPosition;
    DISALLOW_COPY_AND_ASSIGN(SceneTranslateHandler);
};


class SelectionHandler : public MouseEventHandler {
public:
    SelectionHandler();
    ~SelectionHandler();
    void buttonUp(QMouseEvent* event,Context* ctx);
    void buttonDown(QMouseEvent* event,Context* ctx);
    void mouseMove(QMouseEvent* event,Context* ctx);
    void mouseWheel(QWheelEvent* event,Context* ctx);
private:
    DISALLOW_COPY_AND_ASSIGN(SelectionHandler);
};


class GeomManipHandler: public MouseEventHandler {
public:
    GeomManipHandler();
    ~GeomManipHandler();
    void buttonUp(QMouseEvent* event,Context* ctx);
    void buttonDown(QMouseEvent* event,Context* ctx);
    void mouseMove(QMouseEvent* event,Context* ctx);
    void mouseWheel(QWheelEvent* event,Context* ctx);
private:
    DISALLOW_COPY_AND_ASSIGN(GeomManipHandler);
    QPoint* lastPosition;
    V3 lastVert;
};

class DeselectHandler: public KeyboardEventHandler {
public:
    DeselectHandler();
    ~DeselectHandler();
    void keyPress(QKeyEvent* event, Context* ctx);
    void keyRelease(QKeyEvent* event, Context* ctx);
};

class SelectEdgeRingHandler: public KeyboardEventHandler {
public:
    SelectEdgeRingHandler();
    ~SelectEdgeRingHandler();
    void keyPress(QKeyEvent* event, Context* ctx);
    void keyRelease(QKeyEvent* event, Context* ctx);
};

class SelectEdgeLoopHandler: public KeyboardEventHandler {
public:
    SelectEdgeLoopHandler();
    ~SelectEdgeLoopHandler();
    void keyPress(QKeyEvent* event, Context* ctx);
    void keyRelease(QKeyEvent* event, Context* ctx);
};



class SplitFaceHandler: public KeyboardEventHandler {
public:
    SplitFaceHandler();
    ~SplitFaceHandler();
    void keyPress(QKeyEvent* event, Context* ctx);
    void keyRelease(QKeyEvent* event, Context* ctx);
};




class GeomTranslateHandler : public KeyboardEventHandler {
public:
    GeomTranslateHandler();
    ~GeomTranslateHandler();
    void keyPress(QKeyEvent* event, Context* ctx);
    void keyRelease(QKeyEvent* event, Context* ctx);
private:
    QPoint* lastPosition;
    DISALLOW_COPY_AND_ASSIGN(GeomTranslateHandler);
};


class GeomRotateHandler : public KeyboardEventHandler {
public:
    GeomRotateHandler ();
    ~GeomRotateHandler ();
    void keyPress(QKeyEvent* event, Context* ctx);
    void keyRelease(QKeyEvent* event, Context* ctx);
private:
    QPoint* lastPosition;
    DISALLOW_COPY_AND_ASSIGN(GeomRotateHandler );
};


class GeomScaleHandler : public KeyboardEventHandler {
public:
    GeomScaleHandler();
    ~GeomScaleHandler();
    void keyPress(QKeyEvent* event, Context* ctx);
    void keyRelease(QKeyEvent* event, Context* ctx);
private:
    QPoint* lastPosition;
    DISALLOW_COPY_AND_ASSIGN(GeomScaleHandler);
};


class PythonTestHandler : public KeyboardEventHandler {
public:
    PythonTestHandler();
    ~PythonTestHandler();
    void keyPress(QKeyEvent* event, Context* ctx);
    void keyRelease(QKeyEvent* event, Context* ctx);
private:
    QPoint* lastPosition;
    DISALLOW_COPY_AND_ASSIGN(PythonTestHandler);
};



class PythonMouseHandler: public MouseEventHandler {
public:
    PythonMouseHandler();
    ~PythonMouseHandler();
    void buttonUp(QMouseEvent* event,Context* ctx);
    void buttonDown(QMouseEvent* event,Context* ctx);
    void mouseMove(QMouseEvent* event,Context* ctx);
    void mouseWheel(QWheelEvent* event,Context* ctx);
private:
    DISALLOW_COPY_AND_ASSIGN(PythonMouseHandler);
};



} // end namespace

#endif // FLUXDEFAULTINPUTHANDLER_H
