#ifndef FLUXINPUTSTATESTACK_H
#define FLUXINPUTSTATESTACK_H

#include <QStack>
#include "FluxCommon.h"
enum InputState {
    isSelect,
    isSceneManip,    
    isObjManip,
    isGeomManip,
    isNone,
    isInvalid
};


class InputStateStack {
public:
    InputStateStack();
    explicit InputStateStack(InputState initialState);

    void pushState(InputState st);
    InputState popState();
    InputState peek();

private:
    QStack<InputState> stateStack;
    DISALLOW_COPY_AND_ASSIGN(InputStateStack);
};

#endif // FLUXINPUTSTATESTACK_H


