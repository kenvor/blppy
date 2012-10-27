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

#include <sstream>

#include <blpapi_element.h>
using namespace BloombergLP;
using namespace blpapi;

typedef struct {
    PyObject_HEAD
    Element * _element;
} ElementObject;

extern PyTypeObject Element_Type;

#define ElementObject_Check(v)      (Py_TYPE(v) == &Element_Type)

extern PyObject *
newElementObject(void * element)
{
    ElementObject *self;

    self = PyObject_New(ElementObject, &Element_Type);
    if (self == NULL)
        return NULL;

    self->_element = (Element *)element;

    return (PyObject *)self;
}

/* Element methods */

static void
Element_dealloc(ElementObject *self)
{
  delete self->_element;
  PyObject_Del(self);
}

static PyObject * Element_appendValue(ElementObject *self, PyObject *args)
{
  const char * value;
  if (!PyArg_ParseTuple(args, "s", &value))
    return NULL;

  self->_element->appendValue(value);

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject * Element_appendElement(ElementObject *self, PyObject *args)
{
  Element * element = new Element(self->_element->appendElement());

  PyObject * elementObject = newElementObject((void *)element);

  Py_INCREF(elementObject);
  return elementObject;
}

static PyObject * Element_setElement(ElementObject *self, PyObject *args)
{
  const char * key, * value;
  if (!PyArg_ParseTuple(args, "ss", &key, &value))
    return NULL;

  self->_element->setElement(key, value);

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject * Element_getElementAsString(ElementObject *self, PyObject *args)
{
  const char * elementName;
  if (!PyArg_ParseTuple(args, "s", &elementName))
    return NULL;  

  // TODO: deal with elementNames that can't be found - currently handled by Exceptions
  PyObject * pystr;
  const char * e = self->_element->getElementAsString(elementName);
  if (e == NULL) {
    pystr = Py_None;
  } else {
    pystr = Py_BuildValue("s", e);
  }

  Py_INCREF(pystr);
  return pystr;
}

static PyObject * Element_getElement(ElementObject *self, PyObject *args)
{
  const char * elementName;
  int position;
  PyObject * ret;

  PyObject * arg = PyTuple_GetItem(args, 0);

  if (PyNumber_Check(arg) == 1) { // is the first argument a number?
    PyArg_ParseTuple(args, "i", &position);
    Element * element = new Element();
    if (self->_element->getElement(element, position) == 0) {
      ret = newElementObject((void *)element);
    } else {
      delete element;
      ret = Py_None;
    }
  } else if (PyArg_ParseTuple(args, "s", &elementName)) { // ok, it's probably a string, but it might not be
    Element * element = new Element();
    if (self->_element->getElement(element, elementName) == 0) {
      ret = newElementObject((void *)element);
    } else {
      delete element;
      ret = Py_None;
    }
  } else {
    return NULL;
  }

  Py_INCREF(ret);
  return ret;
}

static PyObject * Element_datatype(ElementObject *self, PyObject *args)
{
  PyObject * ret = Py_BuildValue("i", self->_element->datatype());

  Py_INCREF(ret);
  return ret;
}

static PyObject * Element_numValues(ElementObject *self, PyObject *args)
{
  int n = self->_element->numValues();

  PyObject * ret = Py_BuildValue("i", n);

  Py_INCREF(ret);
  return ret;
}

static PyObject * Element_numElements(ElementObject *self, PyObject *args)
{
  int n = self->_element->numElements();

  PyObject * ret = Py_BuildValue("i", n);

  Py_INCREF(ret);
  return ret;
}

static PyObject * Element_getValueAsElement(ElementObject *self, PyObject *args)
{
  int index;
  if (!PyArg_ParseTuple(args, "i", &index))
    return NULL;  

  Element * element = new Element(self->_element->getValueAsElement(index));

  PyObject * ret = newElementObject((void *)element);

  Py_INCREF(ret);
  return ret;  
}

static PyObject * Element_getValueAsString(ElementObject *self, PyObject *args)
{
  PyObject * ret;

  if (PyTuple_Size(args) == 1) { 
    int index;
    if (!PyArg_ParseTuple(args, "i", &index))
      return NULL;
    ret = Py_BuildValue("s", self->_element->getValueAsString(index));
  } else {
    ret = Py_BuildValue("s", self->_element->getValueAsString());
  }

  Py_INCREF(ret);
  return ret;  
}

static PyObject * Element_name(ElementObject *self, PyObject *args)
{
  PyObject * ret = Py_BuildValue("s", self->_element->name().string());

  Py_INCREF(ret);
  return ret;  
}

static PyMethodDef Element_methods[] = {
    {"appendValue", (PyCFunction)Element_appendValue, METH_VARARGS,
        PyDoc_STR("")},
    {"appendElement", (PyCFunction)Element_appendElement, METH_VARARGS,
        PyDoc_STR("")},
    {"setElement", (PyCFunction)Element_setElement, METH_VARARGS,
        PyDoc_STR("")},
    {"getElementAsString", (PyCFunction)Element_getElementAsString, METH_VARARGS,
        PyDoc_STR("")},
    {"getElement", (PyCFunction)Element_getElement, METH_VARARGS,
        PyDoc_STR("")},
    {"datatype", (PyCFunction)Element_datatype, METH_VARARGS,
        PyDoc_STR("")},
    {"numValues", (PyCFunction)Element_numValues, METH_VARARGS,
        PyDoc_STR("")},
    {"numElements", (PyCFunction)Element_numElements, METH_VARARGS,
        PyDoc_STR("")},
    {"getValueAsElement", (PyCFunction)Element_getValueAsElement, METH_VARARGS,
        PyDoc_STR("")},
    {"getValueAsString", (PyCFunction)Element_getValueAsString, METH_VARARGS,
        PyDoc_STR("")},
    {"name", (PyCFunction)Element_name, METH_VARARGS,
        PyDoc_STR("")},
    {NULL,              NULL}           /* sentinel */
};

static PyObject * Element_str(ElementObject * self)
{
  std::ostringstream ss;

  self->_element->print(ss);

  PyObject * ret = Py_BuildValue("s", ss.str().c_str());

  Py_INCREF(ret);
  return ret;  
}

static PyObject *
Element_getattr(ElementObject *self, char *name)
{
    return Py_FindMethod(Element_methods, (PyObject *)self, name);
}

extern PyTypeObject Element_Type = {
    /* The ob_type field must be initialized in the module init function
     * to be portable to Windows without using C++. */
    PyVarObject_HEAD_INIT(NULL, 0)
    "blpapi.Element",             /*tp_name*/
    sizeof(ElementObject),          /*tp_basicsize*/
    0,                          /*tp_itemsize*/
    /* methods */
    (destructor)Element_dealloc, /*tp_dealloc*/
    0,                          /*tp_print*/
    (getattrfunc)Element_getattr, /*tp_getattr*/
    0,                          /*tp_setattr*/
    0,                          /*tp_compare*/
    0,                          /*tp_repr*/
    0,                          /*tp_as_number*/
    0,                          /*tp_as_sequence*/
    0,                          /*tp_as_mapping*/
    0,                          /*tp_hash*/
    0,                      /*tp_call*/
    (reprfunc)Element_str,                      /*tp_str*/
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

