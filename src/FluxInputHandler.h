#ifndef FLUXINPUTHANDLER_H
#define FLUXINPUTHANDLER_H

#include <QMouseEvent>
#include <QList>
#include "FluxInputStateStack.h"

class Context;
class FluxContext;
class SceneView;

class KeyboardEventHandler {
protected:
    InputState keyDownInputState;
    InputState keyUpInputState;
    Qt::KeyboardModifiers keyModifiers;
    Qt::Key key;

public:
    KeyboardEventHandler();
    virtual ~KeyboardEventHandler()=0;
    InputState getKeyDownInputState();
    InputState getKeyUpInputState();
    Qt::KeyboardModifiers getKeyModifiers();
    Qt::Key getKey();
    virtual void keyPress( QKeyEvent* event,Context* ctx ) { }
    virtual void keyRelease( QKeyEvent* event,Context* ctx ) { }

private:
    DISALLOW_COPY_AND_ASSIGN(KeyboardEventHandler);

};


class MouseEventHandler: public KeyboardEventHandler {

protected:
    InputState mouseDownInputState;
    InputState mouseUpInputState;
    Qt::MouseButtons mouseButtons;
    bool respondToMouseMove;

public:
    MouseEventHandler();
    virtual ~MouseEventHandler()=0;
    virtual bool ignoreModifiersOnMouseUp() {
        return true;
    }

    InputState getMouseDownInputState();
    InputState getMouseUpInputState();
    Qt::MouseButtons getMouseButtons();
    bool getRespondToMouseMove();

    virtual void doubleClick() {}
    virtual void buttonUp(QMouseEvent* event,Context* ctx) = 0;
    virtual void buttonDown(QMouseEvent* event,Context* ctx)  = 0;
    virtual void mouseMove(QMouseEvent* event,Context* ctx)  { }
    virtual void mouseWheel(QWheelEvent* event,Context* ctx) { }
private:
    DISALLOW_COPY_AND_ASSIGN(MouseEventHandler);
};





class InputHandler {
public:
    InputHandler(Context* ctx);
    virtual ~InputHandler();

    void installMouseHandler(MouseEventHandler* handler);
    void installKeyboardHandler(KeyboardEventHandler* handler);
    void handleMousePressEvent(QMouseEvent* event);
    void handleMouseReleaseEvent(QMouseEvent* event);
    void handleMouseMoveEvent(QMouseEvent* event);
    void handleWheelEvent(QWheelEvent* event);
    void handleKeyPressEvent(QKeyEvent* event);
    void handleKeyReleaseEvent(QKeyEvent* event);

protected:
    Context* ctx;
    QList<MouseEventHandler*> mouseEventHandlers;
    QList<KeyboardEventHandler*> keyboardEventHandlers;
    MouseEventHandler* moveHandler;
private:
    DISALLOW_COPY_AND_ASSIGN(InputHandler);
};


#endif // FLUXINPUTHANDLER_H
