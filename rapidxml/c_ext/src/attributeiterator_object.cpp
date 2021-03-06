/*
** -*- coding: utf-8 -*-
**
** File: attributeiterator_object.cpp
** by Arzaroth Lekva
** arzaroth@arzaroth.com
**
*/

#include <Python.h>
#include <structmember.h>
#include <rapidxml.hpp>

#include <common.h>

static void rapidxml_AttributeIteratorObject_dealloc(rapidxml_AttributeIteratorObject* self) {
  Py_XDECREF(self->parent);
  Py_TYPE(self)->tp_free(reinterpret_cast<PyObject*>(self));
}

static PyObject* rapidxml_AttributeIteratorObject_new(PyTypeObject* type,
                                                      PyObject* args,
                                                      PyObject* kwds) {
  rapidxml_AttributeIteratorObject* self;

  self = reinterpret_cast<rapidxml_AttributeIteratorObject*>(type->tp_alloc(type, 0));
  if (self != NULL) {
    self->attribute = NULL;
    self->parent = NULL;
    if (PyArg_ParseTuple(args, "O", &self->parent) &&
        self->parent != NULL && IS_NODE(self->parent)) {
      Py_INCREF(self->parent);
      self->attribute = static_cast<rapidxml::xml_node<>*>
        (reinterpret_cast<rapidxml_NodeObject*>(self->parent)
         ->base.underlying_obj)->first_attribute();
    } else {
      Py_DECREF(self);
      return NULL;
    }
  }
  return reinterpret_cast<PyObject*>(self);
}

static PyObject* rapidxml_AttributeIteratorObject_iter(rapidxml_AttributeIteratorObject* self) {
  Py_INCREF(self);
  return reinterpret_cast<PyObject*>(self);
}

static PyObject* rapidxml_AttributeIteratorObject_iternext(rapidxml_AttributeIteratorObject* self) {
  PyObject* res;

  if (self->attribute == NULL || self->parent == NULL) {
    return NULL;
  }
  res = _bind_result(reinterpret_cast<rapidxml_BaseObject*>(self->parent),
                     self->attribute, &rapidxml_AttributeType);
  self->attribute = self->attribute->next_attribute();
  return res;
}

PyTypeObject rapidxml_AttributeIteratorType = {
  PyVarObject_HEAD_INIT(NULL, 0)
  "rapidxml.c_ext.AttributeIterator", /* tp_name */
  sizeof(rapidxml_AttributeIteratorObject), /* tp_basicsize */
  0,                               /* tp_itemsize */
  reinterpret_cast<destructor>(rapidxml_AttributeIteratorObject_dealloc), /* tp_dealloc */
  0,                               /* tp_print */
  0,                               /* tp_getattr */
  0,                               /* tp_setattr */
  0,                               /* tp_reserved */
  0,                               /* tp_repr */
  0,                               /* tp_as_number */
  0,                               /* tp_as_sequence */
  0,                               /* tp_as_mapping */
  0,                               /* tp_hash  */
  0,                               /* tp_call */
  0,                               /* tp_repr */
  0,                               /* tp_getattro */
  0,                               /* tp_setattro */
  0,                               /* tp_as_buffer */
  Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_ITER, /* tp_flags */
  "class for iterating over node's attributes", /* tp_doc */
  0,                               /* tp_traverse */
  0,                               /* tp_clear */
  0,                               /* tp_richcompare */
  0,                               /* tp_weaklistoffset */
  reinterpret_cast<getiterfunc>(rapidxml_AttributeIteratorObject_iter), /* tp_iter */
  reinterpret_cast<iternextfunc>(rapidxml_AttributeIteratorObject_iternext), /* tp_iternext */
  0,                               /* tp_methods */
  0,                               /* tp_members */
  0,                               /* tp_getset */
  0,                               /* tp_base */
  0,                               /* tp_dict */
  0,                               /* tp_descr_get */
  0,                               /* tp_descr_set */
  0,                               /* tp_dictoffset */
  0,                               /* tp_init */
  0,                               /* tp_alloc */
  reinterpret_cast<newfunc>(rapidxml_AttributeIteratorObject_new), /* tp_new */
  0,                               /* tp_free */
  0,                               /* tp_is_gc */
  0,                               /* tp_bases */
  0,                               /* tp_mro */
  0,                               /* tp_cache */
  0,                               /* tp_subclasses */
  0,                               /* tp_weaklist */
};
