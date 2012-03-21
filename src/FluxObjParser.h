#ifndef FLUXOBJPARSER_H
#define FLUXOBJPARSER_H
#include <QString>
#include <QStringList>
#include <QList>

#include "FluxCore.h"

class ObjParser {
public:
    ObjParser();
    Object3d* parseObject(QString s);

private:
    void parseVert(Object3d* so, QStringList sl);
    void parseFace(Object3d* so, QStringList faceList);
    DISALLOW_COPY_AND_ASSIGN(ObjParser);
};

#endif // OBJPARSER_H
