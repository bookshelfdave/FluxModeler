#include "FluxInputHandler.h"

#include <QtGlobal>
#include "FluxContext.h"


KeyboardEventHandler::KeyboardEventHandler() {

}

KeyboardEventHandler::~KeyboardEventHandler() {

}

Qt::KeyboardModifiers KeyboardEventHandler::getKeyModifiers() {
    return keyModifiers;
}

Qt::Key KeyboardEventHandler::getKey() {
    return key;
}


InputState KeyboardEventHandler::getKeyDownInputState() {
    return keyDownInputState;
}

InputState KeyboardEventHandler::getKeyUpInputState() {
    return keyUpInputState;
}






MouseEventHandler::MouseEventHandler() {

}

MouseEventHandler::~MouseEventHandler() {

}

InputState MouseEventHandler::getMouseDownInputState() {
    return mouseDownInputState;
}

InputState MouseEventHandler::getMouseUpInputState() {
    return mouseUpInputState;
}


Qt::MouseButtons MouseEventHandler::getMouseButtons() {
    return mouseButtons;
}

bool MouseEventHandler::getRespondToMouseMove() {
    return respondToMouseMove;
}






InputHandler::InputHandler(Context* newCtx) : ctx(newCtx) {
}

InputHandler::~InputHandler() {
    // maybe bad form deleting these in the base class...
    foreach(MouseEventHandler* handler,mouseEventHandlers) {
        delete handler;
    }
}


void InputHandler::installMouseHandler(MouseEventHandler* handler) {
    mouseEventHandlers.append(handler);
}

void InputHandler::installKeyboardHandler(KeyboardEventHandler* handler) {
    keyboardEventHandlers.append(handler);
}

void InputHandler::handleMousePressEvent(QMouseEvent* event) {
    InputState currentInputState = ctx->getScene()->getActiveSceneView()->getInputStateStack()->peek();
    foreach(MouseEventHandler* h, mouseEventHandlers) {
        if((h->getMouseDownInputState() == currentInputState) &&
                (event->buttons() == h->getMouseButtons()) &&
                event->modifiers() == h->getKeyModifiers()) {
            if(h->getRespondToMouseMove()) {
                moveHandler = h;
            }

            h->buttonDown(event,ctx);
        }
    }
}

void InputHandler::handleMouseReleaseEvent(QMouseEvent* event) {
    InputState currentInputState = ctx->getScene()->getActiveSceneView()->getInputStateStack()->peek();
    foreach(MouseEventHandler* h, mouseEventHandlers) {
        // note: call to event->buttons() doesn't seem to ever == getMouseButtons...
        if((h->getMouseUpInputState() == currentInputState) &&
                (event->button() == h->getMouseButtons())) {
            if(h->ignoreModifiersOnMouseUp()) {
                if(h->getRespondToMouseMove()) {
                    moveHandler = NULL;
                }

                h->buttonUp(event,ctx);
            } else {
                if(event->modifiers() == h->getKeyModifiers()) {
                    if(h->getRespondToMouseMove()) {
                        moveHandler = NULL;
                    }

                    h->buttonUp(event,ctx);
                }
            }
        }
    }
}

void InputHandler::handleMouseMoveEvent(QMouseEvent* event) {    
    if(moveHandler != NULL) {
        moveHandler->mouseMove(event,ctx);
    }
}

void InputHandler::handleWheelEvent(QWheelEvent* event) {
    InputState currentInputState = ctx->getScene()->getActiveSceneView()->getInputStateStack()->peek();
    foreach(MouseEventHandler* h, mouseEventHandlers) {
        // should I have a wheelinputstate??
        if(h->getMouseDownInputState() == currentInputState) {
            if(event->modifiers() == h->getKeyModifiers()) {
                h->mouseWheel(event,ctx);
            }
        }
    }
}

void InputHandler::handleKeyPressEvent(QKeyEvent* event) {
    InputState currentInputState = ctx->getScene()->getActiveSceneView()->getInputStateStack()->peek();
    foreach(KeyboardEventHandler* h, keyboardEventHandlers) {
        if((h->getKeyDownInputState() == currentInputState) &&
                (event->key() == h->getKey())) {
            if(event->modifiers() == h->getKeyModifiers()) {
                h->keyPress(event,ctx);
            }

        }
    }
}

void InputHandler::handleKeyReleaseEvent(QKeyEvent* event) {
    InputState currentInputState = ctx->getScene()->getActiveSceneView()->getInputStateStack()->peek();
    foreach(KeyboardEventHandler* h, keyboardEventHandlers) {
        if((h->getKeyUpInputState() == currentInputState) &&
                (event->key() == h->getKey())) {
            if(event->modifiers() == h->getKeyModifiers()) {
                h->keyRelease(event,ctx);
            }

        }
    }
}


