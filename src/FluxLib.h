#ifndef FLUXLIB_H
#define FLUXLIB_H

#include <Python.h>
#include "structmember.h"

class V3;
class Edge;

extern "C" int getSceneObjectCount(void* ctx);

//extern "C" V3 getVert(void* ctx, int objId, int vertId);
//extern "C" int getVertCount(void* ctx, int soId);

extern "C" Edge* getEdge(void *ctx, int objId, int edgeId);
extern "C" int getEdgeCount(void *ctx, int objId);

extern "C" void pushInputState(void* ctx, int objId, int vertId);



typedef struct {    
    PyObject_HEAD
    int number;
} App;

static void* appCtx;

 void App_dealloc(App* self);
 PyObject *App_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
 int App_init(App *self, PyObject *args, PyObject *kwds);

 static PyMemberDef App_members[] = {
    {NULL}  /* Sentinel */
};

PyObject *App_sceneObjectCount(App* self);
PyObject *App_getVert(App* self, PyObject* args);
PyObject *App_vertCount(App* self, PyObject* args);

PyObject *App_getEdge(App* self, PyObject* args);
PyObject *App_edgeCount(App* self, PyObject* args);

PyObject *App_faceCount(App* self, PyObject* args);
PyObject *App_getFace(App* self, PyObject* args);


PyObject *App_halfEdgeCount(App* self, PyObject* args);
PyObject *App_getHalfEdge(App* self, PyObject* args);


PyObject *App_createSceneObject(App* self, PyObject* args);
PyObject *App_createVert(App* self, PyObject* args);
PyObject *App_createFace(App* self, PyObject* args);
PyObject *App_buildSceneObject(App* self, PyObject* args);


static PyMethodDef App_methods[] = {    
    {"sceneObjectCount", (PyCFunction)App_sceneObjectCount, METH_NOARGS,"Return the scene object count"},
    {"getVert", (PyCFunction)App_getVert, METH_VARARGS,"Return a vert"},        
    {"vertCount", (PyCFunction)App_vertCount, METH_VARARGS,"Return the vert count"},
    {"getEdge", (PyCFunction)App_getEdge, METH_VARARGS,"Return an edge"},
    {"edgeCount", (PyCFunction)App_edgeCount, METH_VARARGS,"Return the edge count"},
    {"getFace", (PyCFunction)App_getFace, METH_VARARGS,"Return an edge"},
    {"faceCount", (PyCFunction)App_faceCount, METH_VARARGS,"Return the edge count"},
    {"getHalfEdge", (PyCFunction)App_getHalfEdge, METH_VARARGS,"Return an edge"},
    {"halfEdgeCount", (PyCFunction)App_halfEdgeCount, METH_VARARGS,"Return the edge count"},
    {"createSceneObject", (PyCFunction)App_createSceneObject, METH_VARARGS,"Create a Scene Object"},
    {"createVert", (PyCFunction)App_createVert, METH_VARARGS,"Create a vertex in the current Scene Object"},
    {"createFace", (PyCFunction)App_createFace, METH_VARARGS,"Create a face in the current Scene Object"},
    {"buildSceneObject", (PyCFunction)App_buildSceneObject, METH_VARARGS,"Complete the scene object build"},
    {NULL}  /* Sentinel */
};

static PyTypeObject AppType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "flux.root.App",             /*tp_name*/
    sizeof(App),               /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)App_dealloc,   /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "App objects",           /* tp_doc */
    0,		               /* tp_traverse */
    0,		               /* tp_clear */
    0,		               /* tp_richcompare */
    0,		               /* tp_weaklistoffset */
    0,		               /* tp_iter */
    0,		               /* tp_iternext */
    App_methods,             /* tp_methods */
    App_members,             /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)App_init,      /* tp_init */
    0,                         /* tp_alloc */
    App_new,                 /* tp_new */
};

static PyMethodDef module_methods[] = {
    {NULL}  /* Sentinel */
};

#ifndef PyMODINIT_FUNC	/* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif




PyMODINIT_FUNC initFluxApp(void*);


#endif // FLUXLIB_H
