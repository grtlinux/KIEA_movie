/*///////////////////////////////////////////////////////
squaretype.c
�־��� �ε����� ������ �ѱ�� ������ Ŭ������ �����Ѵ�.
������ ������ ����.

import square

s = square.Square(10)
for k in s:
    print k
print len(s)
print s.middle()

__getitem__, __len__, middle �޽�带 �����Ѵ�.

�̰���	2002.1
///////////////////////////////////////////////////////*/

#include "Python.h"

static PyObject *ErrorObject;   
#define onError(message) \
       { PyErr_SetString(ErrorObject, message); return NULL; }


/*///////////////////////////////////////////////////////
              Square �� ������
///////////////////////////////////////////////////////*/

typedef struct {        /* Square �ν��Ͻ� ��ü */
    PyObject_HEAD       /* ���̽� ��� : ���۷��� ī��Ʈ�� �������� ���´� */
    int limit;			/* �ν��Ͻ� ��ü ����� ������� �����Ѵ� */
} squareobject;

staticforward PyTypeObject Squaretype;	/* �ڿ��� ���ǵȴٴ� ���� */

#define is_squareobject(v)  ((v)->ob_type == &Squaretype)


/*///////////////////////////////////////////////////////
              �߰��� �Լ���
///////////////////////////////////////////////////////*/

static PyObject* square_middle(squareobject* self, PyObject* args)
{
    if (!PyArg_ParseTuple(args, ""))
        return NULL;
    return Py_BuildValue("i", self->limit / 2);
}

static struct PyMethodDef square_inst_methods[] = {     /* �߰��� �޽��� */
 {"middle",       square_middle,     METH_VARARGS, "middle point"},
 {NULL,         NULL,	0, NULL}		/* ������ ǥ�� */
};

/*///////////////////////////////////////////////////////
              �⺻ ���� �Լ���
///////////////////////////////////////////////////////*/

static squareobject* newsquareobject(int limit)
{
    squareobject *self;
    self = PyObject_NEW(squareobject, &Squaretype); /* squareobject ��ü ���� */
    if (self == NULL)	/* ��ü ������ �����ϸ� ���� �߻� */
        return NULL;
    self->limit = limit; /* �����ڿ� ���޵� �μ� ó�� */
    return self;		/* ���ο� squareobject �� ���� */
}

static void square_dealloc(squareobject* self)
{
    PyObject_Del(self);
}

static PyObject* square_getattr(squareobject* self, char* name)
{
    return Py_FindMethod(square_inst_methods, (PyObject *)self, name);
}


/*///////////////////////////////////////////////////////
              ������ �ڷ��� �޽���
///////////////////////////////////////////////////////*/

static int square_length(squareobject* self)
{
    return self->limit;		/* ���� ������ ������ �׳� �ѱ�� �ȴ� */
}

static PyObject* square_getitem(squareobject* self, int index)
{  
    if (index < 0 || index >= self->limit) { 
        PyErr_SetString(PyExc_IndexError, "index out-of-bounds"); 
        return NULL;
    } 
    return Py_BuildValue("i", index * index);
}

static PyObject* square_slice(squareobject* self, int low, int high)
{
    onError("�����̽��� ���� �������� �ʾҽ��ϴ�")
}


/*///////////////////////////////////////////////////////
              �ڷ����� ������ ���� �޽���
///////////////////////////////////////////////////////*/

static PySequenceMethods square_sequence = {
      (inquiry)       square_length,            /* len(x)   */
      (binaryfunc)    0,                        /* x + y    */
      (intargfunc)    0,                        /* x * n    */
      (intargfunc)    square_getitem,           /* x[i], in */
      (intintargfunc) square_slice,             /* x[i:j]   */
      (intobjargproc)     0,                    /* x[i] = v */
      (intintobjargproc)  0,                    /* x[i:j]=v */
      (objobjproc) 0,                           /* in */
      /* Added in release 2.0 */
      (binaryfunc) 0,
      (intargfunc) 0
};

static PyTypeObject Squaretype = {      
      /* Ÿ�� ��� */                    /* ��� �ν��Ͻ��� �����Ѵ� */
      PyObject_HEAD_INIT(&PyType_Type)     
      0,                               /* ob_size */
      "Square",                        /* tp_name */
      sizeof(squareobject),            /* tp_basicsize */
      0,                               /* tp_itemsize */

      /* ǥ�� �޽��� */
      (destructor)  square_dealloc,	/* tp_dealloc */
      (printfunc)   0,				/* tp_print */
      (getattrfunc) square_getattr,	/* tp_getattr */
      (setattrfunc) 0,				/* tp_setattr */
      (cmpfunc)     0,				/* tp_compare */
      (reprfunc)    0,				/* tp_repr */

      /* �ڷ��� ���� */
      0,                             /* ��ġ�� �޽��� */
      &square_sequence,              /* �������� �޽��� */
      0,                             /* ������ �޽��� */

      /* ��Ÿ �޽��� */
      (hashfunc)   0,                /* tp_hash */
      (binaryfunc) 0,                /* tp_call */
      (reprfunc)   0,                /* tp_str  */

};  /* �߰��� �ٸ� �޽����� object.h ���� */


/*/////////////////////*/
/* ��� ���� �۾���    */
/*/////////////////////*/
static PyObject* square_new(PyObject *self, PyObject *args) /* ������ */
{
    int limit;
    if (!PyArg_ParseTuple(args, "i", &limit)) /* args�� ������ �μ� */
        return NULL;
    return (PyObject *)newsquareobject(limit); /* �ν��Ͻ� ��ü�� �Ѱ��ش� */
} 

static struct PyMethodDef square_methods[] = {
    {"Square",  square_new,  METH_VARARGS, "Create new Square object"},	/* Ŭ���� �̸��� �Լ� �ּ� */
    {NULL,     NULL,	0,	NULL}              
};

void initsquare(void)
{
    PyObject *m, *d;
	Squaretype.ob_type = &PyType_Type;
    m = Py_InitModule("square", square_methods);   /* ��� �ʱ�ȭ */

    d = PyModule_GetDict(m);	/* ����� �̸������� �����´� */
    ErrorObject = Py_BuildValue("s", "square error");	/* ���� ��ü�� ����� */
    PyDict_SetItemString(d, "error", ErrorObject); /* ���� ��ü�� �̸� ������ ����Ѵ� */
    if (PyErr_Occurred())
        Py_FatalError("can't initialize module square");
}
