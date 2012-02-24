#include "FluxInputStateStack.h"
#include <QString>

InputStateStack::InputStateStack() {
    stateStack.push(isSelect);
}

InputStateStack::InputStateStack(InputState initialState) {
    stateStack.push(initialState);
}

void InputStateStack::pushState(InputState st) {
    stateStack.push(st);
    qDebug(QString("State pushed:%1").arg(st).toAscii().data());
}

InputState InputStateStack::popState() {
    if(stateStack.size() > 1) {        
        InputState st = stateStack.pop();
        qDebug(QString("State popped:%1").arg(st).toAscii().data());
        return st;
    } else {
        // throw an exception???
        return isInvalid;
    }
}

InputState InputStateStack::peek() {
    return stateStack.top();
}
