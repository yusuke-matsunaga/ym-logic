
/// @file PyTvFunc.cc
/// @brief Python TvFunc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyTvFunc.h"
#include "pym/PyLiteral.h"
#include "pym/PyNpnMap.h"
#include "pym/PyPrimType.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
struct TvFuncObject
{
  PyObject_HEAD
  TvFunc* mVal;
};

// Python 用のタイプ定義
PyTypeObject TvFuncType = {
  PyVarObject_HEAD_INIT(nullptr, 0)
};

// 生成関数
PyObject*
TvFunc_new(
  PyTypeObject* type,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "input_num",
    "val_list",
    nullptr
  };
  SizeType ni = -1;
  PyObject* vect_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "|$kO",
				    const_cast<char**>(kwlist),
				    &ni, &vect_obj) ) {
    return nullptr;
  }

  TvFunc func;
  if ( ni == -1 ) {
    func = TvFunc::invalid();
  }
  else if ( vect_obj == nullptr ) {
    func = TvFunc::zero(ni);
  }
  else if ( PySequence_Check(vect_obj) ) {
    SizeType n = PySequence_Size(vect_obj);
    if ( n != (1 << ni) ) {
      PyErr_SetString(PyExc_ValueError, "invalid vector size");
      return nullptr;
    }
    vector<int> values(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      auto obj1 = PySequence_GetItem(vect_obj, i);
      auto val = PyLong_AsLong(obj1);
      Py_XDECREF(obj1);
      if ( val == -1 && PyErr_Occurred() ) {
	PyErr_SetString(PyExc_ValueError, "argument 2 must be a sequence of ints");
	return nullptr;
      }
      values[i] = val;
    }
    func = TvFunc{ni, values};
  }
  else {
    PyErr_SetString(PyExc_ValueError, "argument 2 must be a sequence of ints");
    return nullptr;
  }
  return PyTvFunc::ToPyObject(std::move(func));
}

// 終了関数
void
TvFunc_dealloc(
  PyObject* self
)
{
  auto tv_obj = reinterpret_cast<TvFuncObject*>(self);
  delete tv_obj->mVal;
  Py_TYPE(self)->tp_free(self);
}

// repr() 関数
PyObject*
TvFunc_repr(
  PyObject* self
)
{
  auto& func = PyTvFunc::Get(self);
  // func から 文字列を作る．
  auto tmp_str = func.str();
  return Py_BuildValue("s", tmp_str.c_str());
}

PyObject*
TvFunc_invalid(
  PyObject* Py_UNUSED(self),
  PyObject* Py_UNUSED(args)
)
{
  return PyTvFunc::ToPyObject(TvFunc::invalid());
}

PyObject*
TvFunc_zero(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "input_num",
    nullptr
  };
  SizeType ni = 0;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
				    const_cast<char**>(kw_list),
				    &ni) ) {
    return nullptr;
  }
  return PyTvFunc::ToPyObject(TvFunc::zero(ni));
}

PyObject*
TvFunc_one(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "input_num",
    nullptr
  };
  SizeType ni = 0;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
				    const_cast<char**>(kw_list),
				    &ni) ) {
    return nullptr;
  }
  return PyTvFunc::ToPyObject(TvFunc::one(ni));
}

PyObject*
TvFunc_literal(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "input_num",
    "var",
    "inv",
    "literal",
    nullptr
  };
  SizeType ni = 0;
  SizeType var = -1;
  int inv_int = 0;
  PyObject* lit_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k|$kpO!",
				    const_cast<char**>(kwlist),
				    &ni, &var, &inv_int,
				    PyLiteral::_typeobject(), &lit_obj) ) {
    return nullptr;
  }
  TvFunc func;
  if ( var != -1 ) {
    if ( lit_obj != nullptr ) {
      PyErr_SetString(PyExc_ValueError,
		      "'var' and 'literal' are mutually exclusive.");
      return nullptr;
    }
    bool inv = static_cast<bool>(inv_int);
    func = TvFunc::literal(ni, var, inv);
  }
  else if ( lit_obj != nullptr ) {
    auto lit = PyLiteral::Get(lit_obj);
    if ( inv_int ) {
      lit = ~lit;
    }
    func = TvFunc::literal(ni, lit);
  }
  else {
    PyErr_SetString(PyExc_ValueError,
		    "either 'var' or 'literal' must be specified.");
    return nullptr;
  }
  return PyTvFunc::ToPyObject(std::move(func));
}

PyObject*
TvFunc_posi_literal(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "input_num",
    "var",
    nullptr
  };
  SizeType ni = -1;
  SizeType varid = -1;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "kk",
				    const_cast<char**>(kw_list),
				    &ni, &varid) ) {
    return nullptr;
  }
  return PyTvFunc::ToPyObject(TvFunc::posi_literal(ni, varid));
}

PyObject*
TvFunc_nega_literal(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "input_num",
    "var",
    nullptr
  };
  SizeType ni = -1;
  SizeType varid = -1;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "kk",
				    const_cast<char**>(kw_list),
				    &ni, &varid) ) {
    return nullptr;
  }
  return PyTvFunc::ToPyObject(TvFunc::nega_literal(ni, varid));
}

PyObject*
TvFunc_cofactor(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "var",
    "inv",
    "literal",
    nullptr
  };
  SizeType var = -1;
  int inv_int = 0;
  PyObject* lit_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "|$kpO!",
				    const_cast<char**>(kwlist),
				    &var, &inv_int,
				    PyLiteral::_typeobject(), &lit_obj) ) {
    return nullptr;
  }
  auto& func = PyTvFunc::Get(self);
  TvFunc ans;
  if ( var != -1 ) {
    if ( lit_obj != nullptr ) {
      PyErr_SetString(PyExc_ValueError,
		      "'var' and 'literal' are mutually exclusive.");
      return nullptr;
    }
    ans = func.cofactor(var, static_cast<bool>(inv_int));
  }
  else if ( lit_obj != nullptr ) {
    auto lit = PyLiteral::Get(lit_obj);
    ans = func.cofactor(lit);
  }
  else {
    PyErr_SetString(PyExc_ValueError,
		    "either 'var' or 'literal' must be specified.");
    return nullptr;
  }
  return PyTvFunc::ToPyObject(std::move(ans));
}

PyObject*
TvFunc_cofactor_int(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "var",
    "inv",
    "literal",
    nullptr
  };
  SizeType var = -1;
  int inv_int = 0;
  PyObject* lit_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "|$kpO!",
				    const_cast<char**>(kwlist),
				    &var, &inv_int,
				    PyLiteral::_typeobject(), &lit_obj) ) {
    return nullptr;
  }
  auto& func = PyTvFunc::Get(self);
  if ( var != -1 ) {
    if ( lit_obj != nullptr ) {
      PyErr_SetString(PyExc_ValueError,
		      "'var' and 'literal' are mutually exclusive.");
      return nullptr;
    }
    func.cofactor_int(var, static_cast<bool>(inv_int));
  }
  else if ( lit_obj != nullptr ) {
    auto lit = PyLiteral::Get(lit_obj);
    func.cofactor_int(lit);
  }
  else {
    PyErr_SetString(PyExc_ValueError,
		    "either 'var' or 'literal' must be specified.");
    return nullptr;
  }
  Py_INCREF(self);
  return self;
}

PyObject*
TvFunc_xform(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "map",
    nullptr
  };
  PyObject* npnmap_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!",
				    const_cast<char**>(kw_list),
				    PyNpnMap::_typeobject(), &npnmap_obj) ) {
    return nullptr;
  }
  auto& npnmap = PyNpnMap::Get(npnmap_obj);
  auto& func = PyTvFunc::Get(self);
  auto ans = func.xform(npnmap);
  return PyTvFunc::ToPyObject(std::move(ans));
}

PyObject*
TvFunc_shrink_map(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& func = PyTvFunc::Get(self);
  auto npnmap = func.shrink_map();
  return PyNpnMap::ToPyObject(npnmap);
}

PyObject*
TvFunc_npn_cannonical_map(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& func = PyTvFunc::Get(self);
  auto npnmap = func.npn_cannonical_map();
  return PyNpnMap::ToPyObject(npnmap);
}

PyObject*
TvFunc_npn_cannonical_all_map(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& func = PyTvFunc::Get(self);
  auto npnmap_list = func.npn_cannonical_all_map();
  SizeType n = npnmap_list.size();
  auto ans_obj = PyList_New(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto& npnmap = npnmap_list[i];
    auto obj1 = PyNpnMap::ToPyObject(npnmap);
    PyList_SET_ITEM(ans_obj, i, obj1);
  }
  return ans_obj;
}

PyObject*
TvFunc_is_valid(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& func = PyTvFunc::Get(self);
  auto r = func.is_valid();
  return PyBool_FromLong(r);
}

PyObject*
TvFunc_is_invalid(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& func = PyTvFunc::Get(self);
  auto r = func.is_invalid();
  return PyBool_FromLong(r);
}

PyObject*
TvFunc_is_zero(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& func = PyTvFunc::Get(self);
  auto r = func.is_zero();
  return PyBool_FromLong(r);
}

PyObject*
TvFunc_is_one(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& func = PyTvFunc::Get(self);
  auto r = func.is_one();
  return PyBool_FromLong(r);
}

PyObject*
TvFunc_value(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "pos",
    nullptr
  };
  SizeType pos = -1;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
				    const_cast<char**>(kw_list),
				    &pos) ) {
    return nullptr;
  }
  auto& func = PyTvFunc::Get(self);
  auto r = func.value(pos);
  return PyLong_FromLong(r);
}

PyObject*
TvFunc_count_zero(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& func = PyTvFunc::Get(self);
  auto r = func.count_zero();
  return PyLong_FromLong(r);
}

PyObject*
TvFunc_count_one(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& func = PyTvFunc::Get(self);
  auto r = func.count_one();
  return PyLong_FromLong(r);
}

PyObject*
TvFunc_walsh_0(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& func = PyTvFunc::Get(self);
  auto r = func.walsh_0();
  return PyLong_FromLong(r);
}

PyObject*
TvFunc_walsh_1(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "var",
    nullptr
  };
  SizeType varid = -1;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
				    const_cast<char**>(kw_list),
				    &varid) ) {
    return nullptr;
  }
  auto& func = PyTvFunc::Get(self);
  auto r = func.walsh_1(varid);
  return PyLong_FromLong(r);
}

PyObject*
TvFunc_walsh_2(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "var1",
    "var2",
    nullptr
  };
  SizeType varid1 = -1;
  SizeType varid2 = -1;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "kk",
				    const_cast<char**>(kw_list),
				    &varid1, &varid2) ) {
    return nullptr;
  }
  auto& func = PyTvFunc::Get(self);
  auto r = func.walsh_2(varid1, varid2);
  return PyLong_FromLong(r);
}

PyObject*
TvFunc_check_sup(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "var",
    nullptr
  };
  SizeType varid = -1;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
				    const_cast<char**>(kw_list),
				    &varid) ) {
    return nullptr;
  }
  auto& func = PyTvFunc::Get(self);
  auto r = func.check_sup(varid);
  return PyBool_FromLong(r);
}

PyObject*
TvFunc_check_unate(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "var",
    nullptr
  };
  SizeType varid = -1;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
				    const_cast<char**>(kw_list),
				    &varid) ) {
    return nullptr;
  }
  auto& func = PyTvFunc::Get(self);
  auto r = func.check_unate(varid);
  return PyBool_FromLong(r);
}

PyObject*
TvFunc_check_sym(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "var1",
    "var2"
    "inv",
    nullptr
  };
  SizeType varid1 = -1;
  SizeType varid2 = -1;
  int inv = false;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "kk|$p",
				    const_cast<char**>(kwlist),
				    &varid1, &varid2, &inv) ) {
    return nullptr;
  }
  auto& func = PyTvFunc::Get(self);
  auto r = func.check_sym(varid1, varid2, static_cast<bool>(inv));
  return PyBool_FromLong(r);
}

PyObject*
TvFunc_analyze(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& func = PyTvFunc::Get(self);
  auto ptype = func.analyze();
  return PyPrimType::ToPyObject(ptype);
}

// メソッド定義
PyMethodDef TvFunc_methods[] = {
  {"invalid", TvFunc_invalid,
   METH_STATIC | METH_NOARGS,
   PyDoc_STR("make invalid function")},
  {"zero", reinterpret_cast<PyCFunction>(TvFunc_zero),
   METH_STATIC | METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("make ZERO function")},
  {"one", reinterpret_cast<PyCFunction>(TvFunc_zero),
   METH_STATIC | METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("make ONE function")},
  {"literal", reinterpret_cast<PyCFunction>(TvFunc_literal),
   METH_STATIC | METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("make literal function")},
  {"posi_literal", reinterpret_cast<PyCFunction>(TvFunc_posi_literal),
   METH_STATIC | METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("make positive literal function")},
  {"nega_literal", reinterpret_cast<PyCFunction>(TvFunc_nega_literal),
   METH_STATIC | METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("make positive literal function")},
  {"cofactor", reinterpret_cast<PyCFunction>(TvFunc_cofactor),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return the cofactor")},
  {"cofactor_int", reinterpret_cast<PyCFunction>(TvFunc_cofactor_int),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return the cofactor(inplace version)")},
  {"xform", reinterpret_cast<PyCFunction>(TvFunc_xform),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("transform")},
  {"shrink_map", TvFunc_shrink_map,
   METH_NOARGS,
   PyDoc_STR("make the shrink map")},
  {"npn_cannonical_map", TvFunc_npn_cannonical_map,
   METH_NOARGS,
   PyDoc_STR("make the cannonical map for NPN-equivalence")},
  {"npn_cannonical_all_map", TvFunc_npn_cannonical_all_map,
   METH_NOARGS,
   PyDoc_STR("make the list of all cannonical maps for NPN-equivalence")},
  {"is_valid", TvFunc_is_valid,
   METH_NOARGS,
   PyDoc_STR("True if valid")},
  {"is_invalid", TvFunc_is_valid,
   METH_NOARGS,
   PyDoc_STR("True if not valid")},
  {"is_zero", TvFunc_is_zero,
   METH_NOARGS,
   PyDoc_STR("True if ZERO function")},
  {"is_one", TvFunc_is_one,
   METH_NOARGS,
   PyDoc_STR("True if ONE function")},
  {"value", reinterpret_cast<PyCFunction>(TvFunc_value),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return the value for the specified input")},
  {"count_zero", TvFunc_count_zero,
   METH_NOARGS,
   PyDoc_STR("return the 0's count")},
  {"count_one", TvFunc_count_one,
   METH_NOARGS,
   PyDoc_STR("return the 1's count")},
  {"walsh_0", TvFunc_walsh_0,
   METH_NOARGS,
   PyDoc_STR("return the Walsh 0th coefficient")},
  {"walsh_1", reinterpret_cast<PyCFunction>(TvFunc_walsh_1),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return the Walsh 1st coefficient")},
  {"walsh_2", reinterpret_cast<PyCFunction>(TvFunc_walsh_2),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return the Walsh 2nd coefficient")},
  {"check_sup", reinterpret_cast<PyCFunction>(TvFunc_check_sup),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("check if the specified input is a support")},
  {"check_unate", reinterpret_cast<PyCFunction>(TvFunc_check_unate),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("check the unateness on the specified input")},
  {"check_sym", reinterpret_cast<PyCFunction>(TvFunc_check_sym),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("check the symmetry between two inputs")},
  {"analyze", TvFunc_analyze, METH_NOARGS,
   PyDoc_STR("check if this function is a primitive function")},
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
TvFunc_input_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& func = PyTvFunc::Get(self);
  auto val = func.input_num();
  return PyLong_FromLong(val);
}

// get/set 定義
PyGetSetDef TvFunc_getset[] = {
  {"input_num", TvFunc_input_num, nullptr, PyDoc_STR("input num"), nullptr},
  {nullptr, nullptr, nullptr, nullptr, nullptr}
};

// 比較関数
PyObject*
TvFunc_richcmpfunc(
  PyObject* self,
  PyObject* other,
  int op
)
{
  if ( PyTvFunc::Check(self) &&
       PyTvFunc::Check(other) ) {
    auto& val1 = PyTvFunc::Get(self);
    auto& val2 = PyTvFunc::Get(other);
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

// 否定演算(単項演算の例)
PyObject*
TvFunc_invert(
  PyObject* self
)
{
  if ( PyTvFunc::Check(self) ) {
    auto& val = PyTvFunc::Get(self);
    return PyTvFunc::ToPyObject(~val);
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// AND演算
PyObject*
TvFunc_and(
  PyObject* self,
  PyObject* other
)
{
  if ( PyTvFunc::Check(self) && PyTvFunc::Check(other) ) {
    auto& val1 = PyTvFunc::Get(self);
    auto& val2 = PyTvFunc::Get(other);
    return PyTvFunc::ToPyObject(val1 & val2);
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// OR演算
PyObject*
TvFunc_or(
  PyObject* self,
  PyObject* other
)
{
  if ( PyTvFunc::Check(self) && PyTvFunc::Check(other) ) {
    auto& val1 = PyTvFunc::Get(self);
    auto& val2 = PyTvFunc::Get(other);
    return PyTvFunc::ToPyObject(val1 | val2);
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// XOR演算
PyObject*
TvFunc_xor(
  PyObject* self,
  PyObject* other
)
{
  if ( PyTvFunc::Check(self) && PyTvFunc::Check(other) ) {
    auto& val1 = PyTvFunc::Get(self);
    auto& val2 = PyTvFunc::Get(other);
    return PyTvFunc::ToPyObject(val1 ^ val2);
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// inplace AND演算
PyObject*
TvFunc_iand(
  PyObject* self,
  PyObject* other
)
{
  if ( PyTvFunc::Check(self) && PyTvFunc::Check(other) ) {
    auto& val1 = PyTvFunc::Get(self);
    auto& val2 = PyTvFunc::Get(other);
    val1 &= val2;
    Py_INCREF(self);
    return self;
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// inplace OR演算
PyObject*
TvFunc_ior(
  PyObject* self,
  PyObject* other
)
{
  if ( PyTvFunc::Check(self) && PyTvFunc::Check(other) ) {
    auto& val1 = PyTvFunc::Get(self);
    auto& val2 = PyTvFunc::Get(other);
    val1 |= val2;
    Py_INCREF(self);
    return self;
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// inplace XOR演算
PyObject*
TvFunc_ixor(
  PyObject* self,
  PyObject* other
)
{
  if ( PyTvFunc::Check(self) && PyTvFunc::Check(other) ) {
    auto& val1 = PyTvFunc::Get(self);
    auto& val2 = PyTvFunc::Get(other);
    val1 ^= val2;
    Py_INCREF(self);
    return self;
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// 数値演算メソッド定義
PyNumberMethods TvFunc_number = {
  .nb_invert = TvFunc_invert,
  .nb_and = TvFunc_and,
  .nb_xor = TvFunc_xor,
  .nb_or = TvFunc_or,
  .nb_inplace_and = TvFunc_iand,
  .nb_inplace_xor = TvFunc_ixor,
  .nb_inplace_or = TvFunc_ior
};

// ハッシュ関数
Py_hash_t
TvFunc_hash(
  PyObject* self
)
{
  auto val = PyTvFunc::Get(self);
  return val.hash();
}

END_NONAMESPACE


// @brief 'TvFunc' オブジェクトを使用可能にする．
bool
PyTvFunc::init(
  PyObject* m
)
{
  TvFuncType.tp_name = "TvFunc";
  TvFuncType.tp_basicsize = sizeof(TvFuncObject);
  TvFuncType.tp_itemsize = 0;
  TvFuncType.tp_dealloc = TvFunc_dealloc;
  TvFuncType.tp_flags = Py_TPFLAGS_DEFAULT;
  TvFuncType.tp_doc = PyDoc_STR("TvFunc object");
  TvFuncType.tp_richcompare = TvFunc_richcmpfunc;
  TvFuncType.tp_methods = TvFunc_methods;
  TvFuncType.tp_getset = TvFunc_getset;
  TvFuncType.tp_new = TvFunc_new;
  TvFuncType.tp_repr = TvFunc_repr;
  TvFuncType.tp_as_number = &TvFunc_number;
  TvFuncType.tp_hash = TvFunc_hash;

  // 型オブジェクトの登録
  if ( !PyModule::reg_type(m, "TvFunc", &TvFuncType) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// @brief TvFunc を PyObject に変換する．
PyObject*
PyTvFunc::ToPyObject(
  const TvFunc& val
)
{
  auto obj = TvFuncType.tp_alloc(&TvFuncType, 0);
  ASSERT_COND( obj != nullptr );
  auto tvfunc_obj = reinterpret_cast<TvFuncObject*>(obj);
  tvfunc_obj->mVal = new TvFunc{val};
  return obj;
}

// @brief TvFunc を PyObject に変換する．
PyObject*
PyTvFunc::ToPyObject(
  TvFunc&& val
)
{
  auto obj = TvFuncType.tp_alloc(&TvFuncType, 0);
  auto tvfunc_obj = reinterpret_cast<TvFuncObject*>(obj);
  tvfunc_obj->mVal = new TvFunc{std::move(val)};
  ASSERT_COND( obj != nullptr );
  return obj;
}

// @brief PyObject が TvFunc タイプか調べる．
bool
PyTvFunc::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, _typeobject());
}

// @brief TvFunc を表す PyObject から TvFunc を取り出す．
TvFunc&
PyTvFunc::Get(
  PyObject* obj
)
{
  auto tvfunc_obj = reinterpret_cast<TvFuncObject*>(obj);
  return *tvfunc_obj->mVal;
}

// @brief TvFunc を表すオブジェクトの型定義を返す．
PyTypeObject*
PyTvFunc::_typeobject()
{
  return &TvFuncType;
}

END_NAMESPACE_YM
