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

#include <blpapi_session.h>
#include <blpapi_request.h>
using namespace BloombergLP;
using namespace blpapi;

#include "Request.h"

typedef struct {
    PyObject_HEAD
    Service * _service;
} ServiceObject;

extern PyTypeObject Service_Type;

#define ServiceObject_Check(v)      (Py_TYPE(v) == &Service_Type)

extern PyObject *
newServiceObject(void * service)
{
    ServiceObject *self;

    self = PyObject_New(ServiceObject, &Service_Type);
    if (self == NULL)
        return NULL;

    self->_service = (Service *)service;

    return (PyObject *)self;
}

/* Service methods */

static void
Service_dealloc(ServiceObject *self)
{
  delete self->_service;
  PyObject_Del(self);
}

static PyObject * Service_createRequest(ServiceObject *self, PyObject *args)
{
  const char * requestType;
  if (!PyArg_ParseTuple(args, "s", &requestType))
    return NULL;

  Request * request = new Request(self->_service->createRequest(requestType));

  PyObject * requestObject = newRequestObject((void *)request);

  Py_INCREF(requestObject);
  return requestObject;
}

static PyMethodDef Service_methods[] = {
    {"createRequest", (PyCFunction)Service_createRequest, METH_VARARGS,
        PyDoc_STR("")},
    {NULL,              NULL}           /* sentinel */
};

static PyObject *
Service_getattr(ServiceObject *self, char *name)
{
    return Py_FindMethod(Service_methods, (PyObject *)self, name);
}

extern PyTypeObject Service_Type = {
    /* The ob_type field must be initialized in the module init function
     * to be portable to Windows without using C++. */
    PyVarObject_HEAD_INIT(NULL, 0)
    "blpapi.Service",             /*tp_name*/
    sizeof(ServiceObject),          /*tp_basicsize*/
    0,                          /*tp_itemsize*/
    /* methods */
    (destructor)Service_dealloc, /*tp_dealloc*/
    0,                          /*tp_print*/
    (getattrfunc)Service_getattr, /*tp_getattr*/
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

