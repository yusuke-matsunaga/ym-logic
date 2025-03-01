
/// @file PyBdd.cc
/// @brief Python Bdd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyBdd.h"
#include "pym/PyBddMgr.h"
#include "pym/PyBddVarSet.h"
#include "pym/PyJsonValue.h"
#include "pym/PyModule.h"
#include "ym/BddVar.h"
#include "ym/BddLit.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
struct BddObject
{
  PyObject_HEAD
  Bdd mBdd;
};

// Python 用のタイプ定義
PyTypeObject BddType = {
  PyVarObject_HEAD_INIT(nullptr, 0)
};

// 生成関数
PyObject*
Bdd_new(
  PyTypeObject* type,
  PyObject* args,
  PyObject* kwds
)
{
  // 引数を受け取らないことを確認する．
  static const char* kw_list[] = {
    nullptr
  };
  PyObject* obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "",
				    const_cast<char**>(kw_list)) ) {
    return nullptr;
  }
  auto self = type->tp_alloc(type, 0);
  auto bdd_obj = reinterpret_cast<BddObject*>(self);
  new (&bdd_obj->mBdd) Bdd{}; // 不正値となる．
  return self;
}

// 終了関数
void
Bdd_dealloc(
  PyObject* self
)
{
  auto bdd_obj = reinterpret_cast<BddObject*>(self);
  bdd_obj->mBdd.~Bdd();
  Py_TYPE(self)->tp_free(self);
}

PyObject*
Bdd_invalid(
  PyObject* Py_UNUSED(self),
  PyObject* Py_UNUSED(args)
)
{
  auto bdd = Bdd::invalid();
  return PyBdd::ToPyObject(bdd);
}

PyObject*
Bdd_cofactor(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "",
    "inv",
    nullptr
  };
  PyObject* bdd_obj = nullptr;
  int inv_int = 0;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!|$p",
				    const_cast<char**>(kwlist),
				    PyBdd::_typeobject(), &bdd_obj,
				    &inv_int) ) {
    return nullptr;
  }

  auto inv = static_cast<bool>(inv_int);
  auto bdd = PyBdd::Get(self);
  auto bdd1 = PyBdd::Get(bdd_obj);
  if ( inv ) {
    bdd1 = ~bdd1;
  }
  try {
    auto ans_bdd = bdd.cofactor(bdd1);
    return PyBdd::ToPyObject(ans_bdd);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "argument 1 should be a cube type BDD");
    return nullptr;
  }
}

PyObject*
Bdd_ite(
  PyObject* Py_UNUSED(self),
  PyObject* args
)
{
  PyObject* obj1 = nullptr;
  PyObject* obj2 = nullptr;
  PyObject* obj3 = nullptr;
  if ( !PyArg_ParseTuple(args, "O!O!O!",
			 PyBdd::_typeobject(), &obj1,
			 PyBdd::_typeobject(), &obj2,
			 PyBdd::_typeobject(), &obj3) ) {
    return nullptr;
  }
  auto cond_bdd = PyBdd::Get(obj1);
  auto then_bdd = PyBdd::Get(obj2);
  auto else_bdd = PyBdd::Get(obj3);
  try {
    auto ans_bdd = Bdd::ite(cond_bdd, then_bdd, else_bdd);
    return PyBdd::ToPyObject(ans_bdd);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "invalid_argument");
    return nullptr;
  }
}

PyObject*
Bdd_compose(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "var",
    "opr",
    nullptr
  };
  PyObject* var_obj = nullptr;
  PyObject* opr_obj = nullptr;
  if ( PyArg_ParseTupleAndKeywords(args, kwds, "O!O!",
				   const_cast<char**>(kw_list),
				   PyBdd::_typeobject(), &var_obj,
				   PyBdd::_typeobject(), &opr_obj) ) {
    return nullptr;
  }
  auto var_bdd = PyBdd::Get(var_obj);
  auto var = BddVar::from_bdd(var_bdd);
  if ( var.is_invalid() ) {
    PyErr_SetString(PyExc_TypeError, "var should be a variable");
    return nullptr;
  }
  auto opr = PyBdd::Get(opr_obj);
  auto bdd = PyBdd::Get(self);
  try {
    auto ans_bdd = bdd.compose(var, opr);
    return PyBdd::ToPyObject(ans_bdd);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "invalid_argument");
    return nullptr;
  }
}

PyObject*
Bdd_multi_compose(
  PyObject* self,
  PyObject* args
)
{
  PyObject* obj1 = nullptr;
  if ( !PyArg_ParseTuple(args, "O!", &PyDict_Type, &obj1) ) {
    return nullptr;
  }
  auto values = PyDict_Values(obj1);
  SizeType n = PyList_Size(values);
  unordered_map<BddVar, Bdd> compose_map;
  for ( SizeType i = 0; i < n; ++ i ) {
    auto obj1 = PyList_GetItem(values, i);
    PyObject* var_obj = nullptr;
    PyObject* opr_obj = nullptr;
    if ( !PyArg_ParseTuple(obj1, "O!O!",
			   PyBdd::_typeobject(), &var_obj,
			   PyBdd::_typeobject(), &opr_obj) ) {
      Py_DecRef(values);
      return nullptr;
    }
    auto var_bdd = PyBdd::Get(var_obj);
    auto var = BddVar::from_bdd(var_bdd);
    if ( var.is_invalid() ) {
      PyErr_SetString(PyExc_TypeError, "var should be a variable");
      return nullptr;
    }
    auto opr = PyBdd::Get(opr_obj);
    compose_map.emplace(var, opr);
  }
  Py_DecRef(values);
  auto bdd = PyBdd::Get(self);
  try {
    auto ans_bdd = bdd.multi_compose(compose_map);
    return PyBdd::ToPyObject(ans_bdd);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "invalid_argument");
    return nullptr;
  }
}

PyObject*
Bdd_remap_vars(
  PyObject* self,
  PyObject* args
)
{
  PyObject* obj1 = nullptr;
  if ( !PyArg_ParseTuple(args, "O!", &PyDict_Type, &obj1) ) {
    return nullptr;
  }
  auto values = PyDict_Values(obj1);
  SizeType n = PyList_Size(values);
  unordered_map<BddVar, BddLit> var_map;
  for ( SizeType i = 0; i < n; ++ i ) {
    auto obj1 = PyList_GetItem(values, i);
    PyObject* var_obj = nullptr;
    PyObject* lit_obj = nullptr;
    if ( !PyArg_ParseTuple(obj1, "O!O!",
			   PyBdd::_typeobject(), &var_obj,
			   PyBdd::_typeobject(), &lit_obj) ) {
      return nullptr;
    }
    auto var_bdd = PyBdd::Get(var_obj);
    auto var = BddVar::from_bdd(var_bdd);
    if ( var.is_invalid() ) {
      PyErr_SetString(PyExc_TypeError, "var should be a variable");
      return nullptr;
    }
    auto lit_bdd = PyBdd::Get(lit_obj);
    auto lit = BddLit::from_bdd(lit_bdd);
    if ( lit.is_invalid() ) {
      PyErr_SetString(PyExc_TypeError, "operand should be a literal");
      return nullptr;
    }
    var_map.emplace(var, lit);
  }
  auto bdd = PyBdd::Get(self);
  try {
    auto ans_bdd = bdd.remap_vars(var_map);
    return PyBdd::ToPyObject(ans_bdd);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "invalid_argument");
    return nullptr;
  }
}

PyObject*
Bdd_is_valid(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto bdd = PyBdd::Get(self);
  auto r = bdd.is_valid();
  return PyBool_FromLong(r);
}

PyObject*
Bdd_is_invalid(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto bdd = PyBdd::Get(self);
  auto r = bdd.is_invalid();
  return PyBool_FromLong(r);
}

PyObject*
Bdd_is_zero(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto bdd = PyBdd::Get(self);
  auto r = bdd.is_zero();
  return PyBool_FromLong(r);
}

PyObject*
Bdd_is_one(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto bdd = PyBdd::Get(self);
  auto r = bdd.is_one();
  return PyBool_FromLong(r);
}

PyObject*
Bdd_is_cube(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto bdd = PyBdd::Get(self);
  auto r = bdd.is_cube();
  return PyBool_FromLong(r);
}

PyObject*
Bdd_is_posicube(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto bdd = PyBdd::Get(self);
  auto r = bdd.is_posicube();
  return PyBool_FromLong(r);
}

PyObject*
Bdd_check_sup(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "var",
    nullptr
  };
  PyObject* var_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!",
				    const_cast<char**>(kw_list),
				    PyBdd::_typeobject(), &var_obj) ) {
    return nullptr;
  }
  auto var_bdd = PyBdd::Get(var_obj);
  auto var = BddVar::from_bdd(var_bdd);
  if ( var.is_invalid() ) {
    PyErr_SetString(PyExc_TypeError, "var should be a variable");
    return nullptr;
  }
  auto bdd = PyBdd::Get(self);
  try {
    auto r = bdd.check_sup(var);
    return PyBool_FromLong(r);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "invalid_argument");
    return nullptr;
  }
}

PyObject*
Bdd_check_sym(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "",
    "",
    "inv",
    nullptr
  };
  PyObject* var1_obj = nullptr;
  PyObject* var2_obj = nullptr;
  int inv_int = 0;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!O!|$p",
				    const_cast<char**>(kw_list),
				    PyBdd::_typeobject(), &var1_obj,
				    PyBdd::_typeobject(), &var2_obj,
				    &inv_int) ) {
    return nullptr;
  }
  auto var1_bdd = PyBdd::Get(var1_obj);
  auto var1 = BddVar::from_bdd(var1_bdd);
  if ( var1.is_invalid() ) {
    PyErr_SetString(PyExc_TypeError, "var1 should be a variable");
    return nullptr;
  }
  auto var2_bdd = PyBdd::Get(var2_obj);
  auto var2 = BddVar::from_bdd(var2_bdd);
  if ( var2.is_invalid() ) {
    PyErr_SetString(PyExc_TypeError, "var2 should be a variable");
    return nullptr;
  }
  auto inv = static_cast<bool>(inv_int);
  auto bdd = PyBdd::Get(self);
  try {
    auto r = bdd.check_sym(var1, var2, inv);
    return PyBool_FromLong(r);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "invalid_argument");
    return nullptr;
  }
}

PyObject*
Bdd_get_support(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto bdd = PyBdd::Get(self);
  try {
    auto ans_bdd = bdd.get_support();
    return PyBddVarSet::ToPyObject(ans_bdd);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "invalid_argument");
    return nullptr;
  }
}

PyObject*
Bdd_get_onepath(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto bdd = PyBdd::Get(self);
  try {
    auto ans_bdd = bdd.get_onepath();
    return PyBdd::ToPyObject(ans_bdd);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "invalid_argument");
    return nullptr;
  }
}

PyObject*
Bdd_get_zeropath(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto bdd = PyBdd::Get(self);
  try {
    auto ans_bdd = bdd.get_zeropath();
    return PyBdd::ToPyObject(ans_bdd);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "invalid_argument");
    return nullptr;
  }
}

PyObject*
Bdd_root_decomp(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto bdd = PyBdd::Get(self);
  try {
    Bdd f0;
    Bdd f1;
    auto var = bdd.root_decomp(f0, f1);
    auto var_obj = PyBdd::ToPyObject(var);
    auto f0_obj = PyBdd::ToPyObject(f0);
    auto f1_obj = PyBdd::ToPyObject(f1);
    return Py_BuildValue("OOO", var_obj, f0_obj, f1_obj);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "invalid_argument");
    return nullptr;
  }
}

PyObject*
Bdd_root_var(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto bdd = PyBdd::Get(self);
  try {
    auto var = bdd.root_var();
    return PyBdd::ToPyObject(var);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "invalid_argument");
    return nullptr;
  }
}

PyObject*
Bdd_root_cofactor0(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto bdd = PyBdd::Get(self);
  try {
    auto ans_bdd = bdd.root_cofactor0();
    return PyBdd::ToPyObject(ans_bdd);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "invalid_argument");
    return nullptr;
  }
}

PyObject*
Bdd_root_cofactor1(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto bdd = PyBdd::Get(self);
  try {
    auto ans_bdd = bdd.root_cofactor1();
    return PyBdd::ToPyObject(ans_bdd);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "invalid_argument");
    return nullptr;
  }
}

PyObject*
Bdd_eval(
  PyObject* self,
  PyObject* args
)
{
  PyObject* inputs_obj = nullptr;
  if ( !PyArg_ParseTuple(args, "O", &inputs_obj) ) {
    return nullptr;
  }
  if ( !PyList_Check(inputs_obj) ) {
    PyErr_SetString(PyExc_TypeError, "1st argument should be a list of bool");
    return nullptr;
  }
  auto n = PyList_Size(inputs_obj);
  vector<bool> inputs(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto obj = PyList_GetItem(inputs_obj, i);
    if ( obj == Py_True ) {
      inputs[i] = true;
    }
    else if ( obj == Py_False ) {
      inputs[i] = false;
    }
    else {
      PyErr_SetString(PyExc_TypeError, "1st argument should be a list of bool");
      return nullptr;
    }
  }

  auto bdd = PyBdd::Get(self);
  auto val = bdd.eval(inputs);
  return PyBool_FromLong(val);
}

PyObject*
Bdd_to_litlist(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto bdd = PyBdd::Get(self);
  try {
    auto litlist = bdd.to_litlist();
    SizeType n = litlist.size();
    auto ans_obj = PyList_New(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      auto lit = litlist[i];
      auto lit_obj = PyBdd::ToPyObject(lit);
      PyList_SET_ITEM(ans_obj, i, lit_obj);
    }
    return ans_obj;
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "invalid_argument");
    return nullptr;
  }
}

PyObject*
Bdd_to_truth(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "var_list",
    nullptr
  };
  PyObject* list_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O",
				    const_cast<char**>(kw_list),
				    &list_obj) ) {
    return nullptr;
  }
  if ( !PySequence_Check(list_obj) ) {
    PyErr_SetString(PyExc_TypeError, "'var_list' should be a list of 'BddVar'");
    return nullptr;
  }
  auto n = PySequence_Size(list_obj);
  vector<BddVar> var_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto var_obj = PySequence_GetItem(list_obj, i);
    if ( !PyBdd::Check(var_obj) ) {
      PyErr_SetString(PyExc_TypeError, "'var_list' should be a list of 'BddVar'");
      return nullptr;
    }
    auto var_bdd = PyBdd::Get(var_obj);
    auto var = BddVar::from_bdd(var_bdd);
    if ( var.is_invalid() ) {
      PyErr_SetString(PyExc_TypeError, "var should be a variable");
      return nullptr;
    }
    var_list[i] = var;
    Py_DECREF(var_obj);
  }
  auto bdd = PyBdd::Get(self);
  try {
    auto str = bdd.to_truth(var_list);
    return Py_BuildValue("s", str.c_str());
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "invalid_argument");
    return nullptr;
  }
}

PyObject*
Bdd_gen_dot(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  const static char* kw_list[] = {
    "filename",
    "option",
    nullptr
  };
  const char* filename = nullptr;
  PyObject* option_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "s|$O!",
				    const_cast<char**>(kw_list),
				    &filename,
				    PyJsonValue::_typeobject(), &option_obj) ) {
    return nullptr;
  }
  ofstream ofs{filename};
  if ( !ofs ) {
    ostringstream buf;
    buf << "Could not create file '" << filename << "'";
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
  JsonValue option;
  if ( option_obj != nullptr ) {
    option = PyJsonValue::Get(option_obj);
  }
  auto bdd = PyBdd::Get(self);
  bdd.gen_dot(ofs, option);
  Py_RETURN_NONE;
}

PyObject*
Bdd_gen_dot2(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  const static char* kw_list[] = {
    "filename",
    "bdd_list",
    "option",
    nullptr
  };
  PyObject* bdd_list_obj = nullptr;
  const char* filename = nullptr;
  PyObject* option_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "sO|$O!",
				    const_cast<char**>(kw_list),
				    &filename,
				    &bdd_list_obj,
				    PyJsonValue::_typeobject(), &option_obj) ) {
    return nullptr;
  }

  const char* emsg = "1st argument shuld be a list of Bdd";
  if ( !PySequence_Check(bdd_list_obj) ) {
    PyErr_SetString(PyExc_TypeError, emsg);
    return nullptr;
  }
  auto n = PySequence_Size(bdd_list_obj);
  vector<Bdd> bdd_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto bdd_obj = PySequence_GetItem(bdd_list_obj, i);
    if ( !PyBdd::Check(bdd_obj) ) {
      PyErr_SetString(PyExc_TypeError, emsg);
      return nullptr;
    }
    auto bdd = PyBdd::Get(bdd_obj);
    Py_DECREF(bdd_obj);
    bdd_list[i] = bdd;
  }

  ofstream ofs{filename};
  if ( !ofs ) {
    ostringstream buf;
    buf << "Could not create file '" << filename << "'";
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
  JsonValue option;
  if ( option_obj != nullptr ) {
    option = PyJsonValue::Get(option_obj);
  }
  Bdd::gen_dot(ofs, bdd_list, option);
  Py_RETURN_NONE;
}

// メソッド定義
PyMethodDef Bdd_methods[] = {
  {"invalid", Bdd_invalid,
   METH_STATIC | METH_NOARGS,
   PyDoc_STR("make an invalid BDD")},
  {"cofactor", reinterpret_cast<PyCFunction>(Bdd_cofactor),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return a cofactor BDD")},
  {"ite", Bdd_ite,
   METH_STATIC | METH_VARARGS,
   PyDoc_STR("ITE operation")},
  {"compose", reinterpret_cast<PyCFunction>(Bdd_compose),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("compose operation")},
  {"multi_compose", Bdd_multi_compose,
   METH_VARARGS,
   PyDoc_STR("multiway compose operation")},
  {"remap_vars", Bdd_remap_vars,
   METH_VARARGS,
   PyDoc_STR("variable index remapping")},
  {"is_valid", Bdd_is_valid,
   METH_NOARGS,
   PyDoc_STR("True if valid BDD")},
  {"is_invalid", Bdd_is_invalid,
   METH_NOARGS,
   PyDoc_STR("True if invalid BDD")},
  {"is_zero", Bdd_is_zero,
   METH_NOARGS,
   PyDoc_STR("True if Zero BDD")},
  {"is_one", Bdd_is_one,
   METH_NOARGS,
   PyDoc_STR("True if One BDD")},
  {"is_cube", Bdd_is_cube,
   METH_NOARGS,
   PyDoc_STR("True if cube BDD")},
  {"is_posicube", Bdd_is_posicube,
   METH_NOARGS,
   PyDoc_STR("True if positive cube BDD")},
  {"check_sup", reinterpret_cast<PyCFunction>(Bdd_check_sup),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("check if the specified variable is a true support")},
  {"check_sym", reinterpret_cast<PyCFunction>(Bdd_check_sym),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("check if the specified variables are symmetric")},
  {"get_support", Bdd_get_support,
   METH_NOARGS,
   PyDoc_STR("return the support")},
  {"get_onepath", Bdd_get_onepath,
   METH_NOARGS,
   PyDoc_STR("return a BDD representing a One path")},
  {"get_zeropath", Bdd_get_zeropath,
   METH_NOARGS,
   PyDoc_STR("return a BDD representing a Zero path")},
  {"root_decomp", Bdd_root_decomp,
   METH_NOARGS,
   PyDoc_STR("return the root index, 0's cofactor, and 1'cofactor")},
  {"root_var", Bdd_root_var,
   METH_NOARGS,
   PyDoc_STR("return the root index")},
  {"root_cofactor0", Bdd_root_cofactor0,
   METH_NOARGS,
   PyDoc_STR("return the 0's cofactor of the root index")},
  {"root_cofactor1", Bdd_root_cofactor1,
   METH_NOARGS,
   PyDoc_STR("return the 1's cofactor of the root index")},
  {"eval", Bdd_eval,
   METH_VARARGS,
   PyDoc_STR("evaluate")},
  {"to_litlist", Bdd_to_litlist,
   METH_NOARGS,
   PyDoc_STR("convert to the list of Literals")},
  {"to_truth", reinterpret_cast<PyCFunction>(Bdd_to_truth),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("convert to the truth-table string")},
  {"gen_dot", reinterpret_cast<PyCFunction>(Bdd_gen_dot),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("generate DOT file")},
  {"gen_dot2", reinterpret_cast<PyCFunction>(Bdd_gen_dot2),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("generate DOT file")},
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
Bdd_size(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto bdd = PyBdd::Get(self);
  auto val = bdd.size();
  return PyLong_FromLong(val);
}

PyObject*
Bdd_mgr(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto bdd = PyBdd::Get(self);
  auto mgr = bdd.mgr();
  return PyBddMgr::ToPyObject(mgr);
}

// get/set 関数定義
PyGetSetDef Bdd_getset[] = {
  {"size", Bdd_size, nullptr, PyDoc_STR("size(node count)"), nullptr},
  {"mgr", Bdd_mgr, nullptr, PyDoc_STR("BddMgr"), nullptr},
  {nullptr, nullptr, nullptr, nullptr, nullptr},
};

// 比較関数
PyObject*
Bdd_richcmpfunc(
  PyObject* self,
  PyObject* other,
  int op
)
{
  if ( PyBdd::Check(self) &&
       PyBdd::Check(other) ) {
    auto val1 = PyBdd::Get(self);
    auto val2 = PyBdd::Get(other);
    if ( op == Py_EQ ) {
      return PyBool_FromLong(val1 == val2);
    }
    if ( op == Py_NE ) {
      return PyBool_FromLong(val1 != val2);
    }
  }
  Py_INCREF(Py_NotImplemented);
  return Py_NotImplemented;
}

// 否定演算
PyObject*
Bdd_invert(
  PyObject* self
)
{
  if ( PyBdd::Check(self) ) {
    auto val = PyBdd::Get(self);
    return PyBdd::ToPyObject(~val);
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// AND乗算
PyObject*
Bdd_and(
  PyObject* self,
  PyObject* other
)
{
  if ( PyBdd::Check(self) && PyBdd::Check(other) ) {
    auto val1 = PyBdd::Get(self);
    auto val2 = PyBdd::Get(other);
    try {
      return PyBdd::ToPyObject(val1 & val2);
    }
    catch ( std::invalid_argument ) {
      PyErr_SetString(PyExc_ValueError, "invalid_argument");
      return nullptr;
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// OR乗算
PyObject*
Bdd_or(
  PyObject* self,
  PyObject* other
)
{
  if ( PyBdd::Check(self) && PyBdd::Check(other) ) {
    auto val1 = PyBdd::Get(self);
    auto val2 = PyBdd::Get(other);
    try {
      return PyBdd::ToPyObject(val1 | val2);
    }
    catch ( std::invalid_argument ) {
      PyErr_SetString(PyExc_ValueError, "invalid_argument");
      return nullptr;
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// XOR乗算
PyObject*
Bdd_xor(
  PyObject* self,
  PyObject* other
)
{
  if ( PyBdd::Check(self) && PyBdd::Check(other) ) {
    auto val1 = PyBdd::Get(self);
    auto val2 = PyBdd::Get(other);
    try {
      return PyBdd::ToPyObject(val1 ^ val2);
    }
    catch ( std::invalid_argument ) {
      PyErr_SetString(PyExc_ValueError, "invalid_argument");
      return nullptr;
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// cofactor 演算
PyObject*
Bdd_div(
  PyObject* self,
  PyObject* other
)
{
  if ( PyBdd::Check(self) && PyBdd::Check(other) ) {
    auto val1 = PyBdd::Get(self);
    auto val2 = PyBdd::Get(other);
    try {
      return PyBdd::ToPyObject(val1 / val2);
    }
    catch ( std::invalid_argument ) {
      PyErr_SetString(PyExc_ValueError, "invalid_argument");
      return nullptr;
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// 数値演算メソッド定義
PyNumberMethods Bdd_number = {
  .nb_invert = Bdd_invert,
  .nb_and = Bdd_and,
  .nb_xor = Bdd_xor,
  .nb_or = Bdd_or,
  .nb_inplace_and = Bdd_and,
  .nb_inplace_xor = Bdd_xor,
  .nb_inplace_or = Bdd_or,
  .nb_true_divide = Bdd_div,
  .nb_inplace_true_divide = Bdd_div,
};

// ハッシュ関数
Py_hash_t
Bdd_hash(
  PyObject* self
)
{
  auto val = PyBdd::Get(self);
  return val.hash();
}

END_NONAMESPACE


// @brief 'Bdd' オブジェクトを使用可能にする．
bool
PyBdd::init(
  PyObject* m
)
{
  BddType.tp_name = "Bdd";
  BddType.tp_basicsize = sizeof(BddObject);
  BddType.tp_itemsize = 0;
  BddType.tp_dealloc = Bdd_dealloc;
  BddType.tp_flags = Py_TPFLAGS_DEFAULT;
  BddType.tp_doc = PyDoc_STR("Bdd object");
  BddType.tp_richcompare = Bdd_richcmpfunc;
  BddType.tp_methods = Bdd_methods;
  BddType.tp_getset = Bdd_getset;
  BddType.tp_new = Bdd_new;
  BddType.tp_as_number = &Bdd_number;
  BddType.tp_hash = Bdd_hash;

  // 型オブジェクトの登録
  if ( !PyModule::reg_type(m, "Bdd", &BddType) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// @brief Bdd を PyObject に変換する．
PyObject*
PyBdd::ToPyObject(
  const Bdd& val
)
{
  auto obj = BddType.tp_alloc(&BddType, 0);
  auto bdd_obj = reinterpret_cast<BddObject*>(obj);
  new (&bdd_obj->mBdd) Bdd{val};
  return obj;
}

// @brief PyObject が Bdd タイプか調べる．
bool
PyBdd::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, _typeobject());
}

// @brief Bdd を表す PyObject から Bdd を取り出す．
Bdd
PyBdd::Get(
  PyObject* obj
)
{
  auto bdd_obj = reinterpret_cast<BddObject*>(obj);
  return bdd_obj->mBdd;
}

// @brief Bdd を表すオブジェクトの型定義を返す．
PyTypeObject*
PyBdd::_typeobject()
{
  return &BddType;
}

END_NAMESPACE_YM
