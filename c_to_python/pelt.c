#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <Python.h>
#include <numpy/arrayobject.h>

double calculate_cost(const double* data, int start, int end) {
    double sum = 0.0;
    for (int i = start; i < end; i++) {
        sum += data[i];
    }
    return sum;
}

static PyObject* pelt_cost(PyObject* self, PyObject* args) {
    PyObject* py_data;
    int start, end;

    if (!PyArg_ParseTuple(args, "Oii", &py_data, &start, &end))
        return NULL;

    PyArrayObject* np_array = (PyArrayObject*)PyArray_FROM_OTF(py_data, NPY_DOUBLE, NPY_ARRAY_IN_ARRAY);
    if (np_array == NULL)
        return NULL;

    // Check if np_array is a one-dimensional array
    if (PyArray_NDIM(np_array) != 1) {
        Py_DECREF(np_array);
        PyErr_SetString(PyExc_TypeError, "Input must be a one-dimensional numpy array");
        return NULL;
    }

    // Check if start and end indices are valid for the array length
    int len = PyArray_SIZE(np_array);
    if (start < 0 || end > len || start > end) {
        Py_DECREF(np_array);
        PyErr_SetString(PyExc_IndexError, "Invalid start or end index for the numpy array");
        return NULL;
    }

    double* data = (double*)PyArray_DATA(np_array);
    double cost = calculate_cost(data, start, end);

    Py_DECREF(np_array);

    return Py_BuildValue("d", cost);
}

static PyMethodDef pelt_methods[] = {
    {"cost", pelt_cost, METH_VARARGS, "Calculate the cost for a given range."},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef pelt_module = {
    PyModuleDef_HEAD_INIT,
    "pelt",
    NULL,
    -1,
    pelt_methods
};

PyMODINIT_FUNC PyInit_pelt(void) {
    if (_import_array() < 0) {
        PyErr_SetString(PyExc_ImportError, "numpy.core.multiarray failed to import");
        return NULL;
    }
    return PyModule_Create(&pelt_module);
}
