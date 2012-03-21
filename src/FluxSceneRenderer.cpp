#include "FluxSceneRenderer.h"

#include <QtGlobal>
#include "FluxGLWidget.h"
#include "FluxCore.h"
#include "FluxContext.h"
#include "FluxOptions.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

SceneRenderer::SceneRenderer() {
    qDebug("Init SceneRenderer");
    displayListId = 0;
}

void SceneRenderer::render(FluxGLWidget* glWidget, Context* ctx, SceneView* sceneView) {
    //if(displayListId == 0) {
    //float polycolor[] = {6.0/255.0,23.0/255.0,61.0/255.0};
    //float polycolor[] = {37.0/255.0,50.0/255.0,250.0/255.0};
    float polycolor[] = {89.0/255.0,89.0/255.0,89.0/255.0};

    //float edgecolor[] ={88.0/255.0, 92.0/255.0, 90.0/255.0};
    float edgecolor[] = {113.0/255.0, 121.0/255.0, 251.0/255.0};


    float pointcolor[] = {1.0,1.0,1.0};
    float selectedpointcolor[] = {0.0,1.0,1.0};

    bool showTris    = ctx->getApplicationOptions()->getBoolOption("showtris",false);
    bool showCenters = ctx->getApplicationOptions()->getBoolOption("showcenters",false);





    foreach(Object3d* so,ctx->getScene()->getSceneObjects()) {
        int faceid = 0;
        foreach(Face* face, (*so->getFaces())) {
            faceid++;
            glBegin(GL_POLYGON);

            if(face->isSelected()) {
                glColor3f(1.0,0.0,0.0);
            } else {
                glColor3f(polycolor[0],polycolor[1],polycolor[2]);
            }

            foreach(Vert* v, *(face->getVerts())) {
                glVertex3d(v->x,v->y,v->z);
            }
            glEnd();


            if(showCenters) {
                glPointSize(5.0);
                glColor3f(0.0,1.0,0.0);
                glBegin(GL_POINTS);
                V3 centroid = face->getCentroid();
                glVertex3d(centroid.x, centroid.y, centroid.z);
                glEnd();
            }



            if(showTris) {
                glColor3f(1.0,1.0,1.0);
                QList<Triangle*> *tris = face->getTriangles();                                
                    // TODO: Move centroid calculation out of here!!
                foreach(Triangle* tri, *tris) {
                    glBegin(GL_LINE_LOOP);
                    V3* v0 = tri->index[0];
                    V3* v1 = tri->index[1];
                    V3* v2 = tri->index[2];
                    glVertex3d(v0->x,v0->y,v0->z);
                    glVertex3d(v1->x,v1->y,v1->z);
                    glVertex3d(v2->x,v2->y,v2->z);
                    glEnd();

                }

            }
        }

        glBegin(GL_LINES);
        foreach(Edge* edge, (*so->getEdges())) {
            if(edge->isSelected()) {
                glColor3f(1.0,0.0,0.0);
            } else {
                glColor3f(edgecolor[0],edgecolor[1],edgecolor[2]);
            }

            glVertex3d(edge->getA()->x,edge->getA()->y,edge->getA()->z);
            glVertex3d(edge->getB()->x,edge->getB()->y,edge->getB()->z);
        }
        glEnd();

        glPointSize(3.0);
        glBegin(GL_POINTS);
        foreach(Vert* vert, (*so->getVerts())) {
            if(vert->isSelected()) {
                glColor3f(selectedpointcolor[0],selectedpointcolor[1],selectedpointcolor[2]);
            } else {
                glColor3f(pointcolor[0],pointcolor[1],pointcolor[2]);
            }

            glVertex3d(vert->x,vert->y,vert->z);
        }
        glEnd();

    }

}





//        glEndList();
//        qDebug("Done creating display list");
//    }
//    qDebug("Calling display list");
//    glCallList(displayListId);




//glPointSize(3.0);
//qDebug("Creating display list");
//displayListId = glGenLists(1);
//glNewList(displayListId, GL_COMPILE);


/*
Edge* firstEdge = so->getEdges()->at(face->getEdgeIds()->first());
Vert* a = firstEdge->getA();
glVertex3d(a->x,a->y,a->z);
foreach(EdgeIdType eid, *(face->getEdgeIds())) {
    Edge* edge = so->getEdges()->at(eid);
    V3* b = edge->getB();
    glVertex3d(b->x,b->y,b->z);
}
glEnd();
*/


//            glBegin(GL_LINE_LOOP);
//            foreach(EdgeIdType eid, *(face->getUndirectedEdgeIds())) {
//                Edge* edge = so->getEdges()->at(eid);
//                Vert* a = edge->getA();
//                Vert* b = edge->getB();
//                if(edge->isSelected()) {
//                    glColor3f(1.0,0.0,0.0);
//                } else {
//                    glColor3f(edgecolor[0],edgecolor[1],edgecolor[2]);
//                }

//                glVertex3d(a->x,a->y,a->z);
//                glVertex3d(b->x,b->y,b->z);
//            }
//            glEnd();


/*
glBegin(GL_LINE_LOOP);
firstEdge = so->getEdges()->at(face->getEdgeIds()->first());
if(firstEdge->isSelected()) {
    glColor3f(1.0,0.0,0.0);
} else {
    glColor3f(edgecolor[0],edgecolor[1],edgecolor[2]);
}

a = firstEdge->getA();
glVertex3d(a->x,a->y,a->z);
foreach(EdgeIdType eid, *(face->getEdgeIds())) {
    Edge* edge = so->getEdges()->at(eid);
    if(edge->isSelected()) {
        glColor3f(1.0,0.0,0.0);
    } else {
        glColor3f(edgecolor[0],edgecolor[1],edgecolor[2]);
    }
    V3* b = edge->getB();
    glVertex3d(b->x,b->y,b->z);
}
glEnd();
*/


/*
glPointSize(3.0);
glBegin(GL_POINTS);
firstEdge = so->getEdges()->at(face->getEdgeIds()->first());
a = firstEdge->getA();
if(a->isSelected()) {
    glColor3f(selectedpointcolor[0],selectedpointcolor[1],selectedpointcolor[2]);
} else {
    glColor3f(pointcolor[0],pointcolor[1],pointcolor[2]);
}
glVertex3d(a->x,a->y,a->z);
foreach(EdgeIdType eid, *(face->getEdgeIds())) {
    Edge* edge = so->getEdges()->at(eid);
    Vert* b = edge->getB();
    if(b->isSelected()) {
        glColor3f(selectedpointcolor[0],selectedpointcolor[1],selectedpointcolor[2]);
    } else {
        glColor3f(pointcolor[0],pointcolor[1],pointcolor[2]);
    }
    glVertex3d(b->x,b->y,b->z);
}
glEnd();
*/

/*
glColor3f(1.0,0.0,0.0);
QList<Triangle*> *tris = face->getTriangles();
V3 num = V3(0.0,0.0,0.0);
double denom = 0.0;
    // TODO: Move centroid calculation out of here!!
foreach(Triangle* tri, *tris) {
    //glBegin(GL_LINE_LOOP);

    V3* v0 = tri->index[0];
    V3* v1 = tri->index[1];
    V3* v2 = tri->index[2];
    V3 r0 = V3(v0->x + v1->x + v2->x,
              v0->y + v1->y + v2->y,
              v0->z + v1->z + v2->z);
    V3 r = r0.scale(1.0/3.0);
    double a = ((*v1 - *v0) % (*v2 - *v0)).length();

    num = num + r.scale(a);
    denom += a;
    //glVertex3d(v0->x,v0->y,v0->z);
    //glVertex3d(v1->x,v1->y,v1->z);
    //glVertex3d(v2->x,v2->y,v2->z);
    //glEnd();
}


// centroid: http://local.wasp.uwa.edu.au/~pbourke/geometry/polyarea/
glPointSize(5.0);
glColor3f(0.0,1.0,0.0);
V3 centroid = num.scale(1.0/denom);
glBegin(GL_POINTS);
glVertex3d(centroid.x, centroid.y, centroid.z);
glEnd();
*/
