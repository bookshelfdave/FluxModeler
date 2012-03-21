#ifndef FLUXMODELOPS_H
#define FLUXMODELOPS_H

#include "FluxCommon.h"
class Context;
class Face;

class ModelOps {
    public:
    ModelOps();
    void splitFaceAtVerts(Context* ctx, Face* f);
private:
    DISALLOW_COPY_AND_ASSIGN(ModelOps);
};
#endif // FLUXMODELOPS_H
