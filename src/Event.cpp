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

#include <blpapi_event.h>
using namespace BloombergLP;
using namespace blpapi;

#include "Message.h"

typedef struct {
    PyObject_HEAD
    Event * _event;
    MessageIterator * _messageiterator;
} EventObject;

extern PyTypeObject Event_Type;

#define EventObject_Check(v)      (Py_TYPE(v) == &Event_Type)

extern PyObject *
newEventObject(void * event)
{
    EventObject *self;

    self = PyObject_New(EventObject, &Event_Type);
    if (self == NULL)
        return NULL;

    self->_event = (Event *)event;
    self->_messageiterator = new MessageIterator(*(self->_event));

    return (PyObject *)self;
}

/* Event methods */

static void
Event_dealloc(EventObject *self)
{
  delete self->_event;
  delete self->_messageiterator;
  PyObject_Del(self);
}

static PyObject * Event_nextMessage(EventObject *self, PyObject *args)
{
  PyObject * messageObject;
  if (self->_messageiterator->next()) {
    Message * message = new Message(self->_messageiterator->message());
    messageObject = (PyObject *)newMessageObject((void *)message);
  } else {
    messageObject = Py_None;
  }

  Py_INCREF(messageObject);
  return messageObject;
}


static PyMethodDef Event_methods[] = {
    {"nextMessage", (PyCFunction)Event_nextMessage, METH_VARARGS,
     PyDoc_STR("")},
    {NULL,              NULL}           /* sentinel */
};

static PyObject *
Event_getattr(EventObject *self, char *name)
{
    return Py_FindMethod(Event_methods, (PyObject *)self, name);
}

extern PyTypeObject Event_Type = {
    /* The ob_type field must be initialized in the module init function
     * to be portable to Windows without using C++. */
    PyVarObject_HEAD_INIT(NULL, 0)
    "blpapi.Event",             /*tp_name*/
    sizeof(EventObject),          /*tp_basicsize*/
    0,                          /*tp_itemsize*/
    /* methods */
    (destructor)Event_dealloc, /*tp_dealloc*/
    0,                          /*tp_print*/
    (getattrfunc)Event_getattr, /*tp_getattr*/
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

