
/// @file PyBdd.cc
/// @brief Python Bdd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyBdd.h"
#include "pym/PyBddVarSet.h"
#include "pym/PyLiteral.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
struct BddObject
{
  PyObject_HEAD
  Bdd* mVal;
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
  bdd_obj->mVal = new Bdd{}; // 不正値となる．
  return self;
}

// 終了関数
void
Bdd_dealloc(
  PyObject* self
)
{
  auto bdd_obj = reinterpret_cast<BddObject*>(self);
  delete bdd_obj->mVal;
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
  PyObject* obj1 = nullptr;
  int inv = 0;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!$p",
				    const_cast<char**>(kwlist),
				    &obj1, &inv) ) {
    return nullptr;
  }

  auto bdd = PyBdd::_get(self);
  if ( PyLong_Check(obj1) ) {
    SizeType varid = PyLong_AsLong(obj1);
    auto lit = Literal{varid, static_cast<bool>(inv)};
    auto ans_bdd = bdd.cofactor(varid, static_cast<bool>(inv));
    return PyBdd::ToPyObject(ans_bdd);
  }
  if ( PyLiteral::_check(obj1) ) {
    if ( inv ) {
      PyErr_SetString(PyExc_TypeError, "'inv' keyword is not allowed with 'Literal' type");
      return nullptr;
    }
    auto lit = PyLiteral::_get(obj1);
    auto ans_bdd = bdd.cofactor(lit);
    return PyBdd::ToPyObject(ans_bdd);
  }
  if ( PyBdd::_check(obj1) ) {
    if ( inv ) {
      PyErr_SetString(PyExc_TypeError, "'inv' keyword is not allowed with 'Bdd' type");
      return nullptr;
    }
    auto bdd1 = PyBdd::_get(obj1);
    try {
      auto ans_bdd = bdd.cofactor(bdd1);
      return PyBdd::ToPyObject(ans_bdd);
    }
    catch ( std::invalid_argument ) {
      PyErr_SetString(PyExc_ValueError, "argument 1 must be a cube type BDD");
      return nullptr;
    }
  }

  PyErr_SetString(PyExc_TypeError, "argument 1 must be an int or Literal");
  return nullptr;
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
  auto cond_bdd = PyBdd::_get(obj1);
  auto then_bdd = PyBdd::_get(obj2);
  auto else_bdd = PyBdd::_get(obj3);
  try {
    auto ans_bdd = ite(cond_bdd, then_bdd, else_bdd);
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
  PyObject* args
)
{
  SizeType index = 0;
  PyObject* obj1 = nullptr;
  if ( PyArg_ParseTuple(args, "kO!", &index, PyBdd::_typeobject(), &obj1) ) {
    return nullptr;
  }
  auto bdd1 = PyBdd::_get(obj1);
  auto bdd = PyBdd::_get(self);
  try {
    auto ans_bdd = bdd.compose(index, bdd1);
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
  unordered_map<SizeType, Bdd> compose_map;
  for ( SizeType i = 0; i < n; ++ i ) {
    auto obj1 = PyList_GetItem(values, i);
    SizeType index = 0;
    PyObject* val_obj = nullptr;
    if ( !PyArg_ParseTuple(obj1, "kO!", &index,
			   PyBdd::_typeobject(), &val_obj) ) {
      Py_DecRef(values);
      return nullptr;
    }
    auto bdd1 = PyBdd::_get(val_obj);
    compose_map.emplace(index, bdd1);
  }
  Py_DecRef(values);
  auto bdd = PyBdd::_get(self);
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
  unordered_map<SizeType, Literal> var_map;
  for ( SizeType i = 0; i < n; ++ i ) {
    auto obj1 = PyList_GetItem(values, i);
    SizeType index = 0;
    PyObject* obj2 = nullptr;
    if ( !PyArg_ParseTuple(obj1, "kO!", &index,
			   PyLiteral::_typeobject(), &obj2) ) {
      return nullptr;
    }
    auto lit = PyLiteral::_get(obj2);
    var_map.emplace(index, lit);
  }
  auto bdd = PyBdd::_get(self);
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
  auto bdd = PyBdd::_get(self);
  auto r = bdd.is_valid();
  return PyBool_FromLong(r);
}

PyObject*
Bdd_is_invalid(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto bdd = PyBdd::_get(self);
  auto r = bdd.is_invalid();
  return PyBool_FromLong(r);
}

PyObject*
Bdd_is_zero(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto bdd = PyBdd::_get(self);
  auto r = bdd.is_zero();
  return PyBool_FromLong(r);
}

PyObject*
Bdd_is_one(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto bdd = PyBdd::_get(self);
  auto r = bdd.is_one();
  return PyBool_FromLong(r);
}

PyObject*
Bdd_is_cube(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto bdd = PyBdd::_get(self);
  auto r = bdd.is_cube();
  return PyBool_FromLong(r);
}

PyObject*
Bdd_is_posicube(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto bdd = PyBdd::_get(self);
  auto r = bdd.is_posicube();
  return PyBool_FromLong(r);
}

PyObject*
Bdd_check_sup(
  PyObject* self,
  PyObject* args
)
{
  SizeType var = 0;
  if ( !PyArg_ParseTuple(args, "k", &var) ) {
    return nullptr;
  }
  auto bdd = PyBdd::_get(self);
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
  SizeType var1 = 0;
  SizeType var2 = 0;
  int inv = 0;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "kk|$p",
				    const_cast<char**>(kw_list),
				    &var1, &var2, &inv) ) {
    return nullptr;
  }
  auto bdd = PyBdd::_get(self);
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
  auto bdd = PyBdd::_get(self);
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
  auto bdd = PyBdd::_get(self);
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
  auto bdd = PyBdd::_get(self);
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
  auto bdd = PyBdd::_get(self);
  try {
    Bdd f0;
    Bdd f1;
    auto index = bdd.root_decomp(f0, f1);
    auto obj1 = PyBdd::ToPyObject(f0);
    auto obj2 = PyBdd::ToPyObject(f1);
    return Py_BuildValue("kOO", index, obj1, obj2);
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
  auto bdd = PyBdd::_get(self);
  try {
    auto index = bdd.root_var();
    return PyLong_FromLong(index);
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
  auto bdd = PyBdd::_get(self);
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
  auto bdd = PyBdd::_get(self);
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
Bdd_to_litlist(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto bdd = PyBdd::_get(self);
  try {
    auto litlist = bdd.to_litlist();
    SizeType n = litlist.size();
    auto ans_obj = PyList_New(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      auto lit = litlist[i];
      auto lit_obj = PyLiteral::ToPyObject(lit);
      PyList_SetItem(ans_obj, i, lit_obj);
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
  PyObject* args
)
{
  SizeType n = 0;
  if ( !PyArg_ParseTuple(args, "k", &n) ) {
    return nullptr;
  }
  auto bdd = PyBdd::_get(self);
  try {
    auto str = bdd.to_truth(n);
    return Py_BuildValue("s", str.c_str());
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "invalid_argument");
    return nullptr;
  }
}

// メソッド定義
PyMethodDef Bdd_methods[] = {
  {"invalid", Bdd_invalid, METH_STATIC | METH_NOARGS,
   PyDoc_STR("make an invalid BDD")},
  {"cofactor", reinterpret_cast<PyCFunction>(Bdd_cofactor),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return a cofactor BDD")},
  {"ite", Bdd_ite, METH_STATIC | METH_VARARGS,
   PyDoc_STR("ITE operation")},
  {"compose", Bdd_compose, METH_VARARGS,
   PyDoc_STR("compose operation")},
  {"multi_compose", Bdd_multi_compose, METH_VARARGS,
   PyDoc_STR("multiway compose operation")},
  {"remap_vars", Bdd_remap_vars, METH_VARARGS,
   PyDoc_STR("variable index remapping")},
  {"is_valid", Bdd_is_valid, METH_NOARGS,
   PyDoc_STR("True if valid BDD")},
  {"is_invalid", Bdd_is_invalid, METH_NOARGS,
   PyDoc_STR("True if invalid BDD")},
  {"is_zero", Bdd_is_zero, METH_NOARGS,
   PyDoc_STR("True if Zero BDD")},
  {"is_one", Bdd_is_one, METH_NOARGS,
   PyDoc_STR("True if One BDD")},
  {"is_cube", Bdd_is_cube, METH_NOARGS,
   PyDoc_STR("True if cube BDD")},
  {"is_posicube", Bdd_is_posicube, METH_NOARGS,
   PyDoc_STR("True if positive cube BDD")},
  {"check_sup", Bdd_check_sup, METH_VARARGS,
   PyDoc_STR("check if the specified variable is a true support")},
  {"check_sym", reinterpret_cast<PyCFunction>(Bdd_check_sym),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("check if the specified variables are symmetric")},
  {"get_support", Bdd_get_support, METH_NOARGS,
   PyDoc_STR("return the support")},
  {"get_onepath", Bdd_get_onepath, METH_NOARGS,
   PyDoc_STR("return a BDD representing a One path")},
  {"get_zeropath", Bdd_get_zeropath, METH_NOARGS,
   PyDoc_STR("return a BDD representing a Zero path")},
  {"root_decomp", Bdd_root_decomp, METH_NOARGS,
   PyDoc_STR("return the root index, 0's cofactor, and 1'cofactor")},
  {"root_var", Bdd_root_var, METH_NOARGS,
   PyDoc_STR("return the root index")},
  {"root_cofactor0", Bdd_root_cofactor0, METH_NOARGS,
   PyDoc_STR("return the 0's cofactor of the root index")},
  {"root_cofactor1", Bdd_root_cofactor1, METH_NOARGS,
   PyDoc_STR("return the 1's cofactor of the root index")},
  {"to_litlist", Bdd_to_litlist, METH_NOARGS,
   PyDoc_STR("convert to the list of Literals")},
  {"to_truth", Bdd_to_truth, METH_VARARGS,
   PyDoc_STR("convert to the truth-table string")},
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
Bdd_size(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto bdd = PyBdd::_get(self);
  auto val = bdd.size();
  return PyLong_FromLong(val);
}

// get/set 関数定義
PyGetSetDef Bdd_getset[] = {
  {"size", Bdd_size, nullptr, PyDoc_STR("size(node count)"), nullptr},
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
  if ( PyBdd::_check(self) &&
       PyBdd::_check(other) ) {
    auto val1 = PyBdd::_get(self);
    auto val2 = PyBdd::_get(other);
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
  if ( PyBdd::_check(self) ) {
    auto val = PyBdd::_get(self);
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
  if ( PyBdd::_check(self) && PyBdd::_check(other) ) {
    auto val1 = PyBdd::_get(self);
    auto val2 = PyBdd::_get(other);
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
  if ( PyBdd::_check(self) && PyBdd::_check(other) ) {
    auto val1 = PyBdd::_get(self);
    auto val2 = PyBdd::_get(other);
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
  if ( PyBdd::_check(self) && PyBdd::_check(other) ) {
    auto val1 = PyBdd::_get(self);
    auto val2 = PyBdd::_get(other);
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
  if ( PyBdd::_check(self) && PyBdd::_check(other) ) {
    auto val1 = PyBdd::_get(self);
    auto val2 = PyBdd::_get(other);
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
  auto val = PyBdd::_get(self);
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
  bdd_obj->mVal = new Bdd{val};
  return obj;
}

// @brief PyObject が Bdd タイプか調べる．
bool
PyBdd::_check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, _typeobject());
}

// @brief Bdd を表す PyObject から Bdd を取り出す．
Bdd
PyBdd::_get(
  PyObject* obj
)
{
  auto bdd_obj = reinterpret_cast<BddObject*>(obj);
  return *bdd_obj->mVal;
}

// @brief Bdd を表すオブジェクトの型定義を返す．
PyTypeObject*
PyBdd::_typeobject()
{
  return &BddType;
}

END_NAMESPACE_YM
