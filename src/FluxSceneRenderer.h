#ifndef FLUXSCENERENDERER_H
#define FLUXSCENERENDERER_H
#include "FluxCommon.h"
#include <QColor>
#include <QtOpenGL>

class FluxGLWidget;
class Context;
class SceneView;
class Face;
class Object3d;

class SceneRenderer {
public:
    SceneRenderer();
    void render(FluxGLWidget* gl, Context* ctx, SceneView* sv);

private:
    GLuint displayListId;

    DISALLOW_COPY_AND_ASSIGN(SceneRenderer);
};

#endif // FLUXSCENERENDERER_H
