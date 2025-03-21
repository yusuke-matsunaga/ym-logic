
/// @file PyBddVarSet.cc
/// @brief Python BddVarSet の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyBddVarSet.h"
#include "pym/PyBddMgr.h"
#include "pym/PyBdd.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
struct BddVarSetObject
{
  PyObject_HEAD
  BddVarSet mVal;
};

// Python 用のタイプ定義
PyTypeObject BddVarSetType = {
  PyVarObject_HEAD_INIT(nullptr, 0)
};

// 生成関数
PyObject*
BddVarSet_new(
  PyTypeObject* type,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "mgr",
    "varset",
    nullptr
  };
  PyObject* mgr_obj = nullptr;
  PyObject* varset_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "|O!O!",
				    const_cast<char**>(kw_list),
				    PyBddMgr::_typeobject(), &mgr_obj,
				    &PyList_Type, &varset_obj) ) {
    return nullptr;
  }
  auto self = type->tp_alloc(type, 0);
  auto bdd_obj = reinterpret_cast<BddVarSetObject*>(self);
  if ( mgr_obj != nullptr ) {
    vector<BddVar> var_set;
    if ( varset_obj != nullptr ) {
      SizeType n = PyList_Size(varset_obj);
      var_set.reserve(n);
      for ( SizeType i = 0; i < n; ++ i ) {
	auto obj1 = PyList_GetItem(varset_obj, i);
	if ( !PyBdd::Check(obj1) ) {
	  PyErr_SetString(PyExc_TypeError, "argument 1 must be a list of 'BddVar'");
	  return nullptr;
	}
	auto& bdd = PyBdd::_get_ref(obj1);
	auto var = BddVar::from_bdd(bdd);
	if ( var.is_invalid() ) {
	  PyErr_SetString(PyExc_TypeError, "argument 1 must be a list of 'BddVar'");
	  return nullptr;
	}
	var_set.push_back(var);
      }
    }
    auto& mgr = PyBddMgr::_get_ref(mgr_obj);
    new (&bdd_obj->mVal) BddVarSet{mgr, var_set};
  }
  else {
    new (&bdd_obj->mVal) BddVarSet{};
  }
  return self;
}

// 終了関数
void
BddVarSet_dealloc(
  PyObject* self
)
{
  auto bdd_obj = reinterpret_cast<BddVarSetObject*>(self);
  bdd_obj->mVal.~BddVarSet();
  Py_TYPE(self)->tp_free(self);
}

PyObject*
BddVarSet_to_varlist(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& bdd = PyBddVarSet::_get_ref(self);
  try {
    auto varlist = bdd.to_varlist();
    SizeType n = varlist.size();
    auto ans_obj = PyList_New(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      auto var = varlist[i];
      auto var_obj = PyBdd::ToPyObject(var);
      PyList_SET_ITEM(ans_obj, i, var_obj);
    }
    return ans_obj;
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError, "invalid_argument");
    return nullptr;
  }
}

PyObject*
BddVarSet_bdd(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& bdd = PyBddVarSet::_get_ref(self);
  auto ans_bdd = bdd.bdd();
  return PyBdd::ToPyObject(ans_bdd);
}

// メソッド定義
PyMethodDef BddVarSet_methods[] = {
  {"to_varlist", BddVarSet_to_varlist, METH_NOARGS,
   PyDoc_STR("convert to the list of variables")},
  {"bdd", BddVarSet_bdd, METH_NOARGS,
   PyDoc_STR("convert to BDD")},
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
BddVarSet_size(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& bdd = PyBddVarSet::_get_ref(self);
  auto val = bdd.size();
  return PyLong_FromLong(val);
}

// get/set 関数定義
PyGetSetDef BddVarSet_getset[] = {
  {"size", BddVarSet_size, nullptr, PyDoc_STR("size"), nullptr},
  {nullptr, nullptr, nullptr, nullptr, nullptr},
};

// 比較関数
PyObject*
BddVarSet_richcmpfunc(
  PyObject* self,
  PyObject* other,
  int op
)
{
  if ( PyBddVarSet::Check(self) &&
       PyBddVarSet::Check(other) ) {
    auto& val1 = PyBddVarSet::_get_ref(self);
    auto& val2 = PyBddVarSet::_get_ref(other);
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

// 和
PyObject*
BddVarSet_add(
  PyObject* self,
  PyObject* other
)
{
  if ( PyBddVarSet::Check(self) &&
       PyBddVarSet::Check(other) ) {
    auto& val1 = PyBddVarSet::_get_ref(self);
    auto& val2 = PyBddVarSet::_get_ref(other);
    try {
      return PyBddVarSet::ToPyObject(val1 + val2);
    }
    catch ( std::invalid_argument ) {
      PyErr_SetString(PyExc_ValueError, "invalid_argument");
      return nullptr;
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// 差
PyObject*
BddVarSet_sub(
  PyObject* self,
  PyObject* other
)
{
  if ( PyBddVarSet::Check(self) &&
       PyBddVarSet::Check(other) ) {
    auto& val1 = PyBddVarSet::_get_ref(self);
    auto& val2 = PyBddVarSet::_get_ref(other);
    try {
      return PyBddVarSet::ToPyObject(val1 - val2);
    }
    catch ( std::invalid_argument ) {
      PyErr_SetString(PyExc_ValueError, "invalid_argument");
      return nullptr;
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// 積(共通集合)
PyObject*
BddVarSet_and(
  PyObject* self,
  PyObject* other
)
{
  if ( PyBddVarSet::Check(self) &&
       PyBddVarSet::Check(other) ) {
    auto& val1 = PyBddVarSet::_get_ref(self);
    auto& val2 = PyBddVarSet::_get_ref(other);
    try {
      return PyBddVarSet::ToPyObject(val1 & val2);
    }
    catch ( std::invalid_argument ) {
      PyErr_SetString(PyExc_ValueError, "invalid_argument");
      return nullptr;
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// 数値演算メソッド定義
PyNumberMethods BddVarSet_number = {
  .nb_add = BddVarSet_add,
  .nb_subtract = BddVarSet_sub,
  .nb_and = BddVarSet_and,
  .nb_inplace_add = BddVarSet_add,
  .nb_inplace_subtract = BddVarSet_sub,
  .nb_inplace_and = BddVarSet_and,
};

END_NONAMESPACE


// @brief 'BddVarSet' オブジェクトを使用可能にする．
bool
PyBddVarSet::init(
  PyObject* m
)
{
  BddVarSetType.tp_name = "BddVarSet";
  BddVarSetType.tp_basicsize = sizeof(BddVarSetObject);
  BddVarSetType.tp_itemsize = 0;
  BddVarSetType.tp_dealloc = BddVarSet_dealloc;
  BddVarSetType.tp_flags = Py_TPFLAGS_DEFAULT;
  BddVarSetType.tp_doc = PyDoc_STR("BddVarSet object");
  BddVarSetType.tp_richcompare = BddVarSet_richcmpfunc;
  BddVarSetType.tp_methods = BddVarSet_methods;
  BddVarSetType.tp_getset = BddVarSet_getset;
  BddVarSetType.tp_new = BddVarSet_new;
  BddVarSetType.tp_as_number = &BddVarSet_number;

  // 型オブジェクトの登録
  if ( !PyModule::reg_type(m, "BddVarSet", &BddVarSetType) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// @brief BddVarSet を PyObject に変換する．
PyObject*
PyBddVarSet::Conv::operator()(
  const BddVarSet& val
)
{
  auto obj = BddVarSetType.tp_alloc(&BddVarSetType, 0);
  auto bddvarset_obj = reinterpret_cast<BddVarSetObject*>(obj);
  new (&bddvarset_obj->mVal) BddVarSet{val};
  return obj;
}

// @brief PyObject* から BddVarSet を取り出す．
bool
PyBddVarSet::Deconv::operator()(
  PyObject* obj,
  BddVarSet& val
)
{
  if ( PyBddVarSet::Check(obj) ) {
    val = PyBddVarSet::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が BddVarSet タイプか調べる．
bool
PyBddVarSet::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, _typeobject());
}

// @brief BddVarSet を表す PyObject から BddVarSet を取り出す．
BddVarSet&
PyBddVarSet::_get_ref(
  PyObject* obj
)
{
  auto bddvarset_obj = reinterpret_cast<BddVarSetObject*>(obj);
  return bddvarset_obj->mVal;
}

// @brief BddVarSet を表すオブジェクトの型定義を返す．
PyTypeObject*
PyBddVarSet::_typeobject()
{
  return &BddVarSetType;
}

END_NAMESPACE_YM
