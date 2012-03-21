#include "FluxLib.h"
#include "FluxCore.h"
#include "FluxContext.h"


int getSceneObjectCount(void* ctx) {
    if(ctx) {
        Scene* scene = ((Context*)ctx)->getScene();
        return scene->getSceneObjects().count();
    } else {       
        return 0;
    }

}


//V3 getVert(void* ctx, int objId, int vertId) {
//    if(ctx) {
//        Scene* scene = ((Context*)ctx)->getScene();
//        V3* v = scene->getSceneObjects()[objId]->getVerts()->at(vertId);
//        // a copy every time :-(
//        return V3(v->x,v->y,v->z);
//    } else {
//        return V3(0.0,0.0,0.0);
//    }
//}

//int getVertCount(void* ctx, int soId) {
//    if(ctx) {
//        Scene* scene = ((Context*)ctx)->getScene();
//        return scene->getSceneObjects()[soId]->getVerts()->count();
//    } else {
//        return 0;
//    }

//}


Edge* getEdge(void *ctx, int soId, int edgeId) {
    if(ctx) {
        Scene* scene = ((Context*)ctx)->getScene();
        Edge* edge = scene->getSceneObjects()[soId]->getEdges()->at(edgeId);
        return edge;
    } else {
        return NULL;
    }
}

int getEdgeCount(void *ctx, int soId) {
    if(ctx) {
        Scene* scene = ((Context*)ctx)->getScene();
        return scene->getSceneObjects()[soId]->getEdges()->count();
    } else {
        return 0;
    }
}




 void App_dealloc(App* self)
{
    self->ob_type->tp_free((PyObject*)self);
}

 PyObject *App_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    App *self;

    self = (App *)type->tp_alloc(type, 0);
//    if (self != NULL) {
//        self->first = PyString_FromString("");
//        if (self->first == NULL)
//          {
//            Py_DECREF(self);
//            return NULL;
//          }

//        self->last = PyString_FromString("");
//        if (self->last == NULL)
//          {
//            Py_DECREF(self);
//            return NULL;
//          }

//        self->number = 0;
//    }
//    void* v = PythonTest_new(1.0,2.0,3.0);
//    PythonTest_delete(v);
    return (PyObject *)self;
}

 int App_init(App *self, PyObject *args, PyObject *kwds)
{
//    PyObject *first=NULL, *last=NULL, *tmp;

//    static char *kwlist[] = {"first", "last", "number", NULL};

//    if (! PyArg_ParseTupleAndKeywords(args, kwds, "|OOi", kwlist,
//                                      &first, &last,
//                                      &self->number))
//        return -1;

//    if (first) {
//        tmp = self->first;
//        Py_INCREF(first);
//        self->first = first;
//        Py_XDECREF(tmp);
//    }

//    if (last) {
//        tmp = self->last;
//        Py_INCREF(last);
//        self->last = last;
//        Py_XDECREF(tmp);
//    }

    return 0;
}



PyObject *App_vertCount(App* self, PyObject* args)
{
    PyObject *result;
    int soId;
    if (!PyArg_ParseTuple(args, "i",&soId))
        return NULL;
    //int total = getVertCount(appCtx,soId);
    Scene* scene = ((Context*)appCtx)->getScene();
    int total = scene->getSceneObjects()[soId]->getVerts()->count();
    result = Py_BuildValue("i",total);
    return result;
}

PyObject *App_sceneObjectCount(App* self) {
    PyObject *result;
    int total = getSceneObjectCount(appCtx);    
    result = Py_BuildValue("i",total);
    return result;
}

PyObject *App_getVert(App* self,PyObject *args){
    int vertId;
    int objId;
    if (!PyArg_ParseTuple(args, "ii",&objId, &vertId))
        return NULL;
    Py_DECREF(args);

    //V3 vert = getVert(appCtx,objId,vertId);
    Scene* scene = ((Context*)appCtx)->getScene();
    V3* vert = scene->getSceneObjects()[objId]->getVerts()->at(vertId);

    PyObject *tuple,*x, *y, *z;

    tuple = PyTuple_New(3);
    x = PyFloat_FromDouble(vert->x);
    y = PyFloat_FromDouble(vert->y);
    z = PyFloat_FromDouble(vert->z);
    PyTuple_SetItem(tuple,0,x);
    PyTuple_SetItem(tuple,1,y);
    PyTuple_SetItem(tuple,2,z);
    return tuple;
}


PyObject *App_getEdge(App* self, PyObject* args) {
    int soId;
    int edgeId;
    if (!PyArg_ParseTuple(args, "ii",&soId,&edgeId))
        return NULL;

    Scene* scene = ((Context*)appCtx)->getScene();
    Edge* edge = scene->getSceneObjects()[soId]->getEdges()->at(edgeId);

    PyObject *tuple,*a, *b;
    tuple = PyTuple_New(2);
    a = PyFloat_FromDouble(edge->getA()->getId());
    b = PyFloat_FromDouble(edge->getB()->getId());
    PyTuple_SetItem(tuple,0,a);
    PyTuple_SetItem(tuple,1,b);
    return tuple;
}


PyObject *App_getFace(App* self, PyObject* args) {
    int soId;
    int faceId;
    if (!PyArg_ParseTuple(args, "ii",&soId,&faceId))
        return NULL;

    Scene* scene = ((Context*)appCtx)->getScene();
    Face* face = scene->getSceneObjects()[soId]->getFaces()->at(faceId);

    PyObject *tuple,*verts, *edges,*vid, *peid, *centroid, *normal, *triangles;

    tuple = PyTuple_New(5);

    verts = PyTuple_New(face->getVerts()->count());
    int vidCount = 0;
    foreach(Vert* vert, (*face->getVerts())) {
        vid = PyInt_FromLong((long)vert->getId());
        PyTuple_SetItem(verts,vidCount++,vid);
    }
    PyTuple_SetItem(tuple,0,verts);


    edges = PyTuple_New(face->getUndirectedEdgeIds()->count());
    int eidCount = 0;
    //qDebug(QString("Undirected edge count %1").arg(face->getUndirectedEdgeIds()->count()).toAscii().data());
    foreach(EdgeIdType eid, (*face->getUndirectedEdgeIds())) {
        peid = PyInt_FromLong((long)eid);
        PyTuple_SetItem(edges,eidCount++,peid);
    }
    PyTuple_SetItem(tuple,1,edges);

    centroid = PyTuple_New(3);
    PyTuple_SetItem(centroid,0,PyFloat_FromDouble((double)face->getCentroid().x));
    PyTuple_SetItem(centroid,1,PyFloat_FromDouble((double)face->getCentroid().y));
    PyTuple_SetItem(centroid,2,PyFloat_FromDouble((double)face->getCentroid().z));
    PyTuple_SetItem(tuple,2,centroid);

    normal = PyTuple_New(3);
    PyTuple_SetItem(normal,0,PyFloat_FromDouble((double)face->getNormal().x));
    PyTuple_SetItem(normal,1,PyFloat_FromDouble((double)face->getNormal().y));
    PyTuple_SetItem(normal,2,PyFloat_FromDouble((double)face->getNormal().z));
    PyTuple_SetItem(tuple,3,normal);

    triangles = PyTuple_New(face->getTriangles()->count());
    int triCount = 0;
    foreach(Triangle* tri, (*face->getTriangles())) {
        PyObject* triangle = PyTuple_New(3);
        PyTuple_SetItem(triangle,0,PyFloat_FromDouble(tri->index[0]->getId()));
        PyTuple_SetItem(triangle,1,PyFloat_FromDouble(tri->index[1]->getId()));
        PyTuple_SetItem(triangle,2,PyFloat_FromDouble(tri->index[2]->getId()));
        PyTuple_SetItem(triangles,triCount++,triangle);
    }
    PyTuple_SetItem(tuple,4,triangles);
    /*
    self.triangles =[]
    self.centroid = V3(0.0,0.0,0.0)
    self.normal = V3(0.0,0.0,0.0)
    self.selected = False
    */

    return tuple;
}


PyObject *App_edgeCount(App* self, PyObject* args) {
    PyObject *result;
    int soId;
    if (!PyArg_ParseTuple(args, "i",&soId))
        return NULL;

    Scene* scene = ((Context*)appCtx)->getScene();
    int total = scene->getSceneObjects()[soId]->getEdges()->count();

    //int total = getEdgeCount(appCtx,soId);
    result = Py_BuildValue("i",total);
    return result;
}


PyObject *App_faceCount(App* self, PyObject* args) {
    PyObject *result;
    int soId;
    if (!PyArg_ParseTuple(args, "i",&soId))
        return NULL;

    Scene* scene = ((Context*)appCtx)->getScene();
    int total = scene->getSceneObjects()[soId]->getFaces()->count();
    result = Py_BuildValue("i",total);
    return result;
}


PyObject *App_halfEdgeCount(App* self, PyObject* args) {
    PyObject *result;
    int soId;
    if (!PyArg_ParseTuple(args, "i",&soId))
        return NULL;

    Scene* scene = ((Context*)appCtx)->getScene();
    int total = scene->getSceneObjects()[soId]->getHalfEdges()->count();

    //int total = getEdgeCount(appCtx,soId);
    result = Py_BuildValue("i",total);
    return result;
}



PyObject *App_getHalfEdge(App* self, PyObject* args) {
    int soId;
    const char *heId;
    if (!PyArg_ParseTuple(args, "is",&soId,&heId))
        return NULL;

    QString qheId = QString(heId);
    Scene* scene = ((Context*)appCtx)->getScene();
    if(!scene->getSceneObjects()[soId]->getHalfEdges()->contains(qheId)) {
        return Py_None;
    } else {
        HalfEdge* he = scene->getSceneObjects()[soId]->getHalfEdges()->value(qheId);
        //he->next->key.toAscii()
        //he->prev->key.toAscii()
        //he->pair->key.toAscii()
        PyObject *tuple;
        tuple = PyTuple_New(1);
        PyTuple_SetItem(tuple,0,PyString_FromString(he->key.toAscii().data()));
        return tuple;
    }
}



PyObject *App_createSceneObject(App* self, PyObject* args) {
   qDebug("Creating a Scene Object");
   return PyFloat_FromDouble(0);
}

PyObject *App_createVert(App* self, PyObject* args) {
    double x,y,z;
    if (!PyArg_ParseTuple(args, "ddd",&x,&y,&z))
        return NULL;
    //qDebug(QString("New vert: %1 %2 %3").arg(x).arg(y).arg(z).toAscii().data());
    Py_RETURN_NONE;
}


PyObject *App_createFace(App* self, PyObject* args) {
    PyObject* list = PyTuple_GetItem(args,0);
    int faceSize = PyList_Size(list);
    qDebug(QString("\nFace size = %1").arg(faceSize).toAscii().data());
    for(int i = 0; i < faceSize; i++) {
        PyObject* pvid = PyList_GetItem(list,i);
        int vid = (int)PyFloat_AsDouble(pvid);
        qDebug(QString("%1").arg(vid).toAscii().data());
    }
    Py_RETURN_NONE;
}


PyObject *App_buildSceneObject(App* self, PyObject* args) {
    qDebug("Building scene object");
    Py_RETURN_NONE;
}

#ifndef PyMODINIT_FUNC	/* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif

PyMODINIT_FUNC initFluxApp(void* ctx)
{
    appCtx = ctx;
    PyObject* m;    
    if (PyType_Ready(&AppType) < 0)
        return;

    m = Py_InitModule3("flux.root", module_methods,"Flux Application");

    if (m == NULL)
      return;

    Py_INCREF(&AppType);
    PyModule_AddObject(m, "App", (PyObject *)&AppType);
}


