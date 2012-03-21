QT += opengl
CONFIG += console

INCLUDEPATH = c:/tools/python27/include
win32:LIBS += c:/tools/python27/libs/libpython27.a

QMAKE_CXXFLAGS_DEBUG += -Wno-write-strings
QMAKE_CXXFLAGS_RELEASE += -Wno-write-strings

HEADERS += \
    src/FluxObjParser.h \
    src/FluxWindow.h \
    src/FluxCore.h \
    src/FluxGLWidget.h \
    src/FluxInputStateStack.h \
    src/FluxContext.h \
    src/FluxInputHandler.h \
    src/FluxSelection.h \
    src/FluxCommon.h \
    src/FluxSceneRenderer.h \
    src/FluxDefaultInputHandler.h \
    src/FluxModelOps.h \
    src/FluxOptions.h \
    src/FluxMath3d.h \
    src/FluxGraphicsView.h \
    src/FluxLib.h \
    src/FluxGeomStatusBar.h \
    src/FluxPythonIO.h

SOURCES += \
    src/FluxObjParser.cpp \
    src/main.cpp \
    src/FluxWindow.cpp \
    src/FluxCore.cpp \
    src/FluxGLWidget.cpp \
    src/FluxInputStateStack.cpp \
    src/FluxContext.cpp \
    src/FluxInputHandler.cpp \
    src/FluxSelection.cpp \
    src/FluxSceneRenderer.cpp \
    src/FluxDefaultInputHandler.cpp \
    src/FluxModelOps.cpp \
    src/FluxOptions.cpp \
    src/FluxMath3d.cpp \
    src/FluxGraphicsView.cpp \
    src/FluxLib.cpp \
    src/FluxGeomStatusBar.cpp \    
    src/FluxPythonIO.cpp
