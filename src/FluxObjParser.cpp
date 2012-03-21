#include "FluxObjParser.h"
#include <QtGlobal>
#include <QFile>
#include <QTextStream>
#include <QStringList>

ObjParser::ObjParser() {
}


// will only parse a single object from a file exported with Wings3d :-)
Object3d* ObjParser::parseObject(QString s) {
    QString msg = "Parsing file " + s;
    qDebug(msg.toAscii().data());
    QFile* data = new QFile(s);
    // only 1 object allowed right now
    Object3d* so = new Object3d(0);

    if (data->open(QFile::ReadOnly)) {
        QTextStream stream(data);
        QString line;

        do {
            line = stream.readLine();

            if(!line.startsWith("#")) {
                QStringList chunks = line.split(" ");
                QString first = chunks.first();

                //qDebug(line.toAscii().data());
                if(first == "o") {
                    qDebug("Found an object");
                } else if (first == "v") {
                    parseVert(so,chunks);
                } else if (first == "vn") {
                    //qDebug("Found an vn");
                } else if (first == "f") {
                    parseFace(so,chunks);
                }
            }
        } while (!line.isNull());
    }

    delete(data);
    return so;
}

void ObjParser::parseVert(Object3d* so,QStringList vertList) {
    QString x = vertList.at(1);
    QString y = vertList.at(2);
    QString z = vertList.at(3);
    so->addVert(x.toFloat(),y.toFloat(),z.toFloat());
}

void ObjParser::parseFace(Object3d* so, QStringList faceList) {
    //f 1//1 6//6 7//7 2//2
    QStringListIterator it(faceList);
    it.next(); // skip "f"

    QList<VertIdType>* vertids = new QList<VertIdType>();

    while(it.hasNext()) {
        QString f = it.next();
        QStringList chunks = f.split("/");
        QString vid = chunks.first();
        vertids->append(vid.toUInt()-1);
    }

    so->addFace(vertids);
    delete vertids;
}

