#include "Python.h"

#include "Session.h"
#include "Service.h"
#include "Request.h"
#include "Element.h"
#include "Event.h"

static PyObject *ErrorObject;

static PyMethodDef blpapi_methods[] = {
    {NULL,              NULL}           /* sentinel */
};

PyDoc_STRVAR(module_doc, "Python binding for Bloomberg BLPAPI");

PyMODINIT_FUNC
initblpapi(void)
{
    PyObject *m;

    if (PyType_Ready(&Session_Type) < 0)
        return;
    if (PyType_Ready(&Service_Type) < 0)
        return;
    if (PyType_Ready(&Request_Type) < 0)
        return;
    if (PyType_Ready(&Element_Type) < 0)
        return;
    if (PyType_Ready(&Event_Type) < 0)
        return;

    m = Py_InitModule3("blpapi", blpapi_methods, module_doc);
    if (m == NULL)
        return;

    if (ErrorObject == NULL) {
        ErrorObject = PyErr_NewException("blpapi.error", NULL, NULL);
        if (ErrorObject == NULL)
            return;
    }
    Py_INCREF(ErrorObject);
    PyModule_AddObject(m, "error", ErrorObject);

    PyModule_AddObject(m, "Session", (PyObject *)&Session_Type);
    PyModule_AddObject(m, "Service", (PyObject *)&Service_Type);
    PyModule_AddObject(m, "Request", (PyObject *)&Request_Type);
    PyModule_AddObject(m, "Element", (PyObject *)&Element_Type);
    PyModule_AddObject(m, "Event", (PyObject *)&Event_Type);
}
