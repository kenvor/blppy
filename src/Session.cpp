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
#include <blpapi_event.h>
using namespace BloombergLP;
using namespace blpapi;

typedef struct {
    PyObject_HEAD
    Request * _request;
} RequestObject;

#include "Service.h"
#include "Event.h"

typedef struct {
    PyObject_HEAD
    Session * _session;
} SessionObject;

extern PyTypeObject Session_Type;

#define SessionObject_Check(v)      (Py_TYPE(v) == &Session_Type)

static SessionObject *
newSessionObject(PyObject *arg)
{
    SessionObject *self;

    const char * host;
    int port;
    
    if (!PyArg_ParseTuple(arg, "si", &host, &port))
        return NULL;

    self = PyObject_New(SessionObject, &Session_Type);
    if (self == NULL)
        return NULL;

    SessionOptions sessionOptions;
    sessionOptions.setServerHost(host);
    sessionOptions.setServerPort(port);

    self->_session = new Session(sessionOptions);

    return self;
}

/* Session methods */

static PyObject *
Session_new(PyTypeObject *subtype, PyObject *args, PyObject *kwds)
{
  return (PyObject *)newSessionObject(args);
}

static void
Session_dealloc(SessionObject *self)
{
  delete self->_session;
  PyObject_Del(self);
}

static PyObject * Session_start(SessionObject *self, PyObject *args)
{
  self->_session->start();

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject * Session_openService(SessionObject *self, PyObject *args)
{
  const char * serviceName;
  if (!PyArg_ParseTuple(args, "s", &serviceName))
    return NULL;

  self->_session->openService(serviceName);

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject * Session_getService(SessionObject *self, PyObject *args)
{
  const char * serviceName;
  if (!PyArg_ParseTuple(args, "s", &serviceName))
    return NULL;

  Service * service = new Service(self->_session->getService(serviceName));

  PyObject * serviceObject = newServiceObject((void *)service);

  Py_INCREF(serviceObject);
  return serviceObject;
}

static PyObject * Session_sendRequest(SessionObject *self, PyObject *args)
{
  PyObject * request;
  if (!PyArg_ParseTuple(args, "O", &request))
    return NULL;
  
  RequestObject * r = (RequestObject *)request;

  self->_session->sendRequest(*(r->_request));

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject * Session_tryNextEvent(SessionObject *self, PyObject *args)
{
  Event * event = new Event();

  PyObject * ret;
  if (self->_session->tryNextEvent(event) == 0) {
    ret = newEventObject((void *)event);
  } else {
    delete event;
    ret = Py_None;
  }

  Py_INCREF(ret);
  return ret;
}

static PyObject * Session_nextEvent(SessionObject *self, PyObject *args)
{
  int timeout = 0;
  if (!PyArg_ParseTuple(args, "|i", &timeout))
    return NULL;

  Event * event = new Event(self->_session->nextEvent(timeout));

  PyObject * ret;
  if (event->eventType() != Event::TIMEOUT) {
    ret = newEventObject((void *)event);
  } else {
    delete event;
    ret = Py_None;
  }

  Py_INCREF(ret);
  return ret;  
}

static PyMethodDef Session_methods[] = {
    {"start", (PyCFunction)Session_start, METH_VARARGS,
        PyDoc_STR("")},
    {"openService", (PyCFunction)Session_openService, METH_VARARGS,
        PyDoc_STR("")},
    {"getService", (PyCFunction)Session_getService, METH_VARARGS,
        PyDoc_STR("")},
    {"sendRequest", (PyCFunction)Session_sendRequest, METH_VARARGS,
        PyDoc_STR("")},
    {"nextEvent", (PyCFunction)Session_nextEvent, METH_VARARGS,
        PyDoc_STR("")},
    {"tryNextEvent", (PyCFunction)Session_tryNextEvent, METH_VARARGS,
        PyDoc_STR("")},
    {NULL,              NULL}           /* sentinel */
};

static PyObject *
Session_getattr(SessionObject *self, char *name)
{
    return Py_FindMethod(Session_methods, (PyObject *)self, name);
}

extern PyTypeObject Session_Type = {
    /* The ob_type field must be initialized in the module init function
     * to be portable to Windows without using C++. */
    PyVarObject_HEAD_INIT(NULL, 0)
    "blpapi.Session",             /*tp_name*/
    sizeof(SessionObject),          /*tp_basicsize*/
    0,                          /*tp_itemsize*/
    /* methods */
    (destructor)Session_dealloc, /*tp_dealloc*/
    0,                          /*tp_print*/
    (getattrfunc)Session_getattr, /*tp_getattr*/
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
    (newfunc)Session_new,                      /*tp_new*/
    0,                      /*tp_free*/
    0,                      /*tp_is_gc*/
};
/* --------------------------------------------------------------------- */

