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

#include <blpapi_message.h>
using namespace BloombergLP;
using namespace blpapi;

#include "Element.h"

#include <iostream>

typedef struct {
    PyObject_HEAD
    Message * _message;
} MessageObject;

extern PyTypeObject Message_Type;

#define MessageObject_Check(v)      (Py_TYPE(v) == &Message_Type)

extern PyObject *
newMessageObject(void * message)
{
    MessageObject *self;

    self = PyObject_New(MessageObject, &Message_Type);
    if (self == NULL)
        return NULL;

    self->_message = (Message *)message;

    return (PyObject *)self;
}

/* Message methods */

static void
Message_dealloc(MessageObject *self)
{
  delete self->_message;
  PyObject_Del(self);
}

static PyObject * Message_asElement(MessageObject *self, PyObject *args)
{
  Element * element = new Element(self->_message->asElement());

  PyObject * elementObject = newElementObject((void *)element);

  Py_INCREF(elementObject);
  return elementObject;
}

static PyMethodDef Message_methods[] = {
    {"asElement", (PyCFunction)Message_asElement, METH_VARARGS,
        PyDoc_STR("")},
    {NULL,              NULL}           /* sentinel */
};

static PyObject *
Message_getattr(MessageObject *self, char *name)
{
    return Py_FindMethod(Message_methods, (PyObject *)self, name);
}

extern PyTypeObject Message_Type = {
    /* The ob_type field must be initialized in the module init function
     * to be portable to Windows without using C++. */
    PyVarObject_HEAD_INIT(NULL, 0)
    "blpapi.Message",             /*tp_name*/
    sizeof(MessageObject),          /*tp_basicsize*/
    0,                          /*tp_itemsize*/
    /* methods */
    (destructor)Message_dealloc, /*tp_dealloc*/
    0,                          /*tp_print*/
    (getattrfunc)Message_getattr, /*tp_getattr*/
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

