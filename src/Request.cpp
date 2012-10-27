/*

Session(host, port)
Session.start()
Session.openService(serviceName)
Service = Session.getService(serviceName)
Session.sendRequest(request)
Event = Session.nextEvent()
Message ...

Request = Service.createRequest(requestType)
Element = Request.getElement(elementName)
Request.set(name, value)

Element.appendValue(value)

*/

#include "Python.h"

#include <blpapi_request.h>
using namespace BloombergLP;
using namespace blpapi;

#include "Element.h"

typedef struct {
    PyObject_HEAD
    Request * _request;
} RequestObject;

extern PyTypeObject Request_Type;

#define RequestObject_Check(v)      (Py_TYPE(v) == &Request_Type)

extern PyObject *
newRequestObject(void * request)
{
    RequestObject *self;

    self = PyObject_New(RequestObject, &Request_Type);
    if (self == NULL)
        return NULL;

    self->_request = (Request *)request;

    return (PyObject *)self;
}

/* Request methods */

static void
Request_dealloc(RequestObject *self)
{
  delete self->_request;
  PyObject_Del(self);
}

static PyObject * Request_getElement(RequestObject *self, PyObject *args)
{
  const char * elementName;
  if (!PyArg_ParseTuple(args, "s", &elementName))
    return NULL;

  Element * element = new Element(self->_request->getElement(elementName));

  // TODO: return None if there is none

  PyObject * elementObject = newElementObject((void *)element);

  Py_INCREF(elementObject);
  return elementObject;
}

static PyObject * Request_set(RequestObject *self, PyObject *args)
{
  const char * key, * value;
  if (!PyArg_ParseTuple(args, "ss", &key, &value))
    return NULL;

  self->_request->set(key, value);

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject * Request_asElement(RequestObject *self, PyObject *args)
{
  Element * element = new Element(self->_request->asElement());

  PyObject * elementObject = newElementObject((void *)element);

  Py_INCREF(elementObject);
  return elementObject;
}

static PyMethodDef Request_methods[] = {
    {"getElement", (PyCFunction)Request_getElement, METH_VARARGS,
        PyDoc_STR("")},
    {"asElement", (PyCFunction)Request_asElement, METH_VARARGS,
        PyDoc_STR("")},
    {"set", (PyCFunction)Request_set, METH_VARARGS,
        PyDoc_STR("")},
    {NULL,              NULL}           /* sentinel */
};

static PyObject *
Request_getattr(RequestObject *self, char *name)
{
    return Py_FindMethod(Request_methods, (PyObject *)self, name);
}

extern PyTypeObject Request_Type = {
    /* The ob_type field must be initialized in the module init function
     * to be portable to Windows without using C++. */
    PyVarObject_HEAD_INIT(NULL, 0)
    "blpapi.Request",             /*tp_name*/
    sizeof(RequestObject),          /*tp_basicsize*/
    0,                          /*tp_itemsize*/
    /* methods */
    (destructor)Request_dealloc, /*tp_dealloc*/
    0,                          /*tp_print*/
    (getattrfunc)Request_getattr, /*tp_getattr*/
    0,                          /*tp_setattr*/
    0,                          /*tp_compare*/
    0,                          /*tp_repr*/
    0,                          /*tp_as_number*/
    0,                          /*tp_as_sequence*/
    0,                          /*tp_as_mapping*/
    0,                          /*tp_hash*/
    0,                      /*tp_call*/
    0,                      /*tp_str*/
    0,                      /*tp_getattro*/
    0,                      /*tp_setattro*/
    0,                      /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT,     /*tp_flags*/
    0,                      /*tp_doc*/
    0,                      /*tp_traverse*/
    0,                      /*tp_clear*/
    0,                      /*tp_richcompare*/
    0,                      /*tp_weaklistoffset*/
    0,                      /*tp_iter*/
    0,                      /*tp_iternext*/
    0,                      /*tp_methods*/
    0,                      /*tp_members*/
    0,                      /*tp_getset*/
    0,                      /*tp_base*/
    0,                      /*tp_dict*/
    0,                      /*tp_descr_get*/
    0,                      /*tp_descr_set*/
    0,                      /*tp_dictoffset*/
    0,                      /*tp_init*/
    0,                      /*tp_alloc*/
    0,                      /*tp_new*/
    0,                      /*tp_free*/
    0,                      /*tp_is_gc*/
};
/* --------------------------------------------------------------------- */

