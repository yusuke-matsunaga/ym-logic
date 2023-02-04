
/// @file PyTvFunc.cc
/// @brief Python TvFunc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyTvFunc.h"
#include "pym/PyLiteral.h"
#include "pym/PyNpnMap.h"
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
    "",
    "",
    nullptr
  };
  SizeType ni = -1;
  PyObject* vect_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "|kO",
				    const_cast<char**>(kwlist),
				    &ni, &vect_obj) ) {
    return nullptr;
  }

  TvFunc func;
  if ( ni == -1 ) {
    func = TvFunc::make_invalid();
  }
  else if ( vect_obj == nullptr ) {
    func = TvFunc::make_zero(ni);
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
  auto& func = PyTvFunc::_get(self);
  // func から 文字列を作る．
  const char* tmp_str = nullptr;
  return Py_BuildValue("s", tmp_str);
}

PyObject*
TvFunc_make_invalid(
  PyObject* Py_UNUSED(self),
  PyObject* Py_UNUSED(args)
)
{
  return PyTvFunc::ToPyObject(TvFunc::make_invalid());
}

PyObject*
TvFunc_make_zero(
  PyObject* Py_UNUSED(self),
  PyObject* args
)
{
  SizeType ni = 0;
  if ( !PyArg_ParseTuple(args, "k", &ni) ) {
    return nullptr;
  }
  return PyTvFunc::ToPyObject(TvFunc::make_zero(ni));
}

PyObject*
TvFunc_make_one(
  PyObject* Py_UNUSED(self),
  PyObject* args
)
{
  SizeType ni = 0;
  if ( !PyArg_ParseTuple(args, "k", &ni) ) {
    return nullptr;
  }
  return PyTvFunc::ToPyObject(TvFunc::make_one(ni));
}

PyObject*
TvFunc_make_literal(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "",
    "",
    "inv",
    nullptr
  };
  SizeType ni = 0;
  PyObject* obj1 = nullptr;
  int inv = 0;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "kO|$p",
				    const_cast<char**>(kwlist),
				    &ni, &obj1, &inv) ) {
    return nullptr;
  }
  TvFunc func;
  if ( PyLong_Check(obj1) ) {
    SizeType var = PyLong_AsLong(obj1);
    func = TvFunc::make_literal(ni, var, static_cast<bool>(inv));
  }
  else if ( PyLiteral::_check(obj1) ) {
    auto lit = PyLiteral::_get(obj1);
    func = TvFunc::make_literal(ni, lit);
  }
  else {
    PyErr_SetString(PyExc_TypeError, "argument 2 must be an int or a Literal");
    return nullptr;
  }
  return PyTvFunc::ToPyObject(std::move(func));
}

PyObject*
TvFunc_make_posi_literal(
  PyObject* Py_UNUSED(self),
  PyObject* args
)
{
  SizeType ni = -1;
  SizeType varid = -1;
  if ( !PyArg_ParseTuple(args, "kk", &ni, &varid) ) {
    return nullptr;
  }
  return PyTvFunc::ToPyObject(TvFunc::make_posi_literal(ni, varid));
}

PyObject*
TvFunc_make_nega_literal(
  PyObject* Py_UNUSED(self),
  PyObject* args
)
{
  SizeType ni = -1;
  SizeType varid = -1;
  if ( !PyArg_ParseTuple(args, "kk", &ni, &varid) ) {
    return nullptr;
  }
  return PyTvFunc::ToPyObject(TvFunc::make_nega_literal(ni, varid));
}

PyObject*
TvFunc_cofactor(
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
  int inv = false;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O|$p",
				    const_cast<char**>(kwlist),
				    &obj1, &inv) ) {
    return nullptr;
  }
  SizeType var;
  if ( PyLong_Check(obj1) ) {
    var = PyLong_AsLong(obj1);
  }
  else if ( PyLiteral::_check(obj1) ) {
    auto lit = PyLiteral::_get(obj1);
    var = lit.varid();
    inv = lit.is_negative();
  }
  else {
    PyErr_SetString(PyExc_TypeError, "argument 1 must be an in or a Literal");
    return nullptr;
  }
  auto& func = PyTvFunc::_get(self);
  auto ans = func.cofactor(var, static_cast<bool>(inv));
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
    "",
    "inv",
    nullptr
  };
  PyObject* obj1 = nullptr;
  int inv = false;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O|$p",
				    const_cast<char**>(kwlist),
				    &obj1, &inv) ) {
    return nullptr;
  }
  SizeType var;
  if ( PyLong_Check(obj1) ) {
    var = PyLong_AsLong(obj1);
  }
  else if ( PyLiteral::_check(obj1) ) {
    auto lit = PyLiteral::_get(obj1);
    var = lit.varid();
    inv = lit.is_negative();
  }
  else {
    PyErr_SetString(PyExc_TypeError, "argument 1 must be an in or a Literal");
    return nullptr;
  }
  auto& func = PyTvFunc::_get(self);
  func.cofactor_int(var, static_cast<bool>(inv));
  Py_INCREF(self);
  return self;
}

PyObject*
TvFunc_xform(
  PyObject* self,
  PyObject* args
)
{
  PyObject* npnmap_obj = nullptr;
  if ( !PyArg_ParseTuple(args, "O!",
			 PyNpnMap::_typeobject(), &npnmap_obj) ) {
    return nullptr;
  }
  auto& npnmap = PyNpnMap::_get(npnmap_obj);
  auto& func = PyTvFunc::_get(self);
  auto ans = func.xform(npnmap);
  return PyTvFunc::ToPyObject(std::move(ans));
}

PyObject*
TvFunc_shrink_map(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& func = PyTvFunc::_get(self);
  auto npnmap = func.shrink_map();
  return PyNpnMap::ToPyObject(npnmap);
}

PyObject*
TvFunc_npn_cannonical_map(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& func = PyTvFunc::_get(self);
  auto npnmap = func.npn_cannonical_map();
  return PyNpnMap::ToPyObject(npnmap);
}

PyObject*
TvFunc_npn_cannonical_all_map(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& func = PyTvFunc::_get(self);
  auto npnmap_list = func.npn_cannonical_all_map();
  SizeType n = npnmap_list.size();
  auto ans_obj = PyList_New(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto& npnmap = npnmap_list[i];
    auto obj1 = PyNpnMap::ToPyObject(npnmap);
    PyList_SetItem(ans_obj, i, obj1);
  }
  return ans_obj;
}

PyObject*
TvFunc_is_valid(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& func = PyTvFunc::_get(self);
  auto r = func.is_valid();
  return PyBool_FromLong(r);
}

PyObject*
TvFunc_is_invalid(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& func = PyTvFunc::_get(self);
  auto r = func.is_invalid();
  return PyBool_FromLong(r);
}

PyObject*
TvFunc_is_zero(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& func = PyTvFunc::_get(self);
  auto r = func.is_zero();
  return PyBool_FromLong(r);
}

PyObject*
TvFunc_is_one(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& func = PyTvFunc::_get(self);
  auto r = func.is_one();
  return PyBool_FromLong(r);
}

PyObject*
TvFunc_value(
  PyObject* self,
  PyObject* args
)
{
  SizeType pos = -1;
  if ( !PyArg_ParseTuple(args, "k", &pos) ) {
    return nullptr;
  }
  auto& func = PyTvFunc::_get(self);
  auto r = func.value(pos);
  return PyLong_FromLong(r);
}

PyObject*
TvFunc_count_zero(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& func = PyTvFunc::_get(self);
  auto r = func.count_zero();
  return PyLong_FromLong(r);
}

PyObject*
TvFunc_count_one(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& func = PyTvFunc::_get(self);
  auto r = func.count_one();
  return PyLong_FromLong(r);
}

PyObject*
TvFunc_walsh_0(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& func = PyTvFunc::_get(self);
  auto r = func.walsh_0();
  return PyLong_FromLong(r);
}

PyObject*
TvFunc_walsh_1(
  PyObject* self,
  PyObject* args
)
{
  SizeType varid = -1;
  if ( !PyArg_ParseTuple(args, "k", &varid) ) {
    return nullptr;
  }
  auto& func = PyTvFunc::_get(self);
  auto r = func.walsh_1(varid);
  return PyLong_FromLong(r);
}

PyObject*
TvFunc_walsh_2(
  PyObject* self,
  PyObject* args
)
{
  SizeType varid1 = -1;
  SizeType varid2 = -1;
  if ( !PyArg_ParseTuple(args, "kk", &varid1, &varid2) ) {
    return nullptr;
  }
  auto& func = PyTvFunc::_get(self);
  auto r = func.walsh_2(varid1, varid2);
  return PyLong_FromLong(r);
}

PyObject*
TvFunc_check_sup(
  PyObject* self,
  PyObject* args
)
{
  SizeType varid = -1;
  if ( !PyArg_ParseTuple(args, "k", &varid) ) {
    return nullptr;
  }
  auto& func = PyTvFunc::_get(self);
  auto r = func.check_sup(varid);
  return PyBool_FromLong(r);
}

PyObject*
TvFunc_check_unate(
  PyObject* self,
  PyObject* args
)
{
  SizeType varid = -1;
  if ( !PyArg_ParseTuple(args, "k", &varid) ) {
    return nullptr;
  }
  auto& func = PyTvFunc::_get(self);
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
    "",
    ""
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
  auto& func = PyTvFunc::_get(self);
  auto r = func.check_sym(varid1, varid2, static_cast<bool>(inv));
  return PyBool_FromLong(r);
}

// メソッド定義
PyMethodDef TvFunc_methods[] = {
  {"make_invalid", TvFunc_make_invalid, METH_STATIC | METH_NOARGS,
   PyDoc_STR("make invalid function")},
  {"make_zero", TvFunc_make_zero, METH_STATIC | METH_VARARGS,
   PyDoc_STR("make ZERO function")},
  {"make_one", TvFunc_make_zero, METH_STATIC | METH_VARARGS,
   PyDoc_STR("make ONE function")},
  {"make_literal", reinterpret_cast<PyCFunction>(TvFunc_make_literal),
   METH_STATIC | METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("make literal function")},
  {"make_posi_literal", TvFunc_make_posi_literal, METH_STATIC | METH_VARARGS,
   PyDoc_STR("make positive literal function")},
  {"make_nega_literal", TvFunc_make_nega_literal, METH_STATIC | METH_VARARGS,
   PyDoc_STR("make positive literal function")},
  {"cofactor", reinterpret_cast<PyCFunction>(TvFunc_cofactor),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return the cofactor")},
  {"cofactor_int", reinterpret_cast<PyCFunction>(TvFunc_cofactor_int),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return the cofactor(inplace version)")},
  {"xform", TvFunc_xform, METH_VARARGS,
   PyDoc_STR("transform")},
  {"shrink_map", TvFunc_shrink_map, METH_NOARGS,
   PyDoc_STR("make the shrink map")},
  {"npn_cannonical_map", TvFunc_npn_cannonical_map, METH_NOARGS,
   PyDoc_STR("make the cannonical map for NPN-equivalence")},
  {"npn_cannonical_all_map", TvFunc_npn_cannonical_all_map, METH_NOARGS,
   PyDoc_STR("make the list of all cannonical maps for NPN-equivalence")},
  {"is_valid", TvFunc_is_valid, METH_NOARGS,
   PyDoc_STR("True if valid")},
  {"is_invalid", TvFunc_is_valid, METH_NOARGS,
   PyDoc_STR("True if not valid")},
  {"is_zero", TvFunc_is_zero, METH_NOARGS,
   PyDoc_STR("True if ZERO function")},
  {"is_one", TvFunc_is_one, METH_NOARGS,
   PyDoc_STR("True if ONE function")},
  {"value", TvFunc_value, METH_VARARGS,
   PyDoc_STR("return the value for the specified input")},
  {"count_zero", TvFunc_count_zero, METH_NOARGS,
   PyDoc_STR("return the 0's count")},
  {"count_one", TvFunc_count_one, METH_NOARGS,
   PyDoc_STR("return the 1's count")},
  {"walsh_0", TvFunc_walsh_0, METH_NOARGS,
   PyDoc_STR("return the Walsh 0th coefficient")},
  {"walsh_1", TvFunc_walsh_1, METH_VARARGS,
   PyDoc_STR("return the Walsh 1st coefficient")},
  {"walsh_2", TvFunc_walsh_2, METH_VARARGS,
   PyDoc_STR("return the Walsh 2nd coefficient")},
  {"check_sup", TvFunc_check_sup, METH_VARARGS,
   PyDoc_STR("check if the specified input is a support")},
  {"check_unate", TvFunc_check_unate, METH_VARARGS,
   PyDoc_STR("check the unateness on the specified input")},
  {"check_sym", reinterpret_cast<PyCFunction>(TvFunc_check_sym),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("check the symmetry between two inputs")},
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
TvFunc_input_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& func = PyTvFunc::_get(self);
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
  if ( PyTvFunc::_check(self) &&
       PyTvFunc::_check(other) ) {
    auto& val1 = PyTvFunc::_get(self);
    auto& val2 = PyTvFunc::_get(other);
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
  if ( PyTvFunc::_check(self) ) {
    auto& val = PyTvFunc::_get(self);
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
  if ( PyTvFunc::_check(self) && PyTvFunc::_check(other) ) {
    auto& val1 = PyTvFunc::_get(self);
    auto& val2 = PyTvFunc::_get(other);
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
  if ( PyTvFunc::_check(self) && PyTvFunc::_check(other) ) {
    auto& val1 = PyTvFunc::_get(self);
    auto& val2 = PyTvFunc::_get(other);
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
  if ( PyTvFunc::_check(self) && PyTvFunc::_check(other) ) {
    auto& val1 = PyTvFunc::_get(self);
    auto& val2 = PyTvFunc::_get(other);
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
  if ( PyTvFunc::_check(self) && PyTvFunc::_check(other) ) {
    auto& val1 = PyTvFunc::_get(self);
    auto& val2 = PyTvFunc::_get(other);
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
  if ( PyTvFunc::_check(self) && PyTvFunc::_check(other) ) {
    auto& val1 = PyTvFunc::_get(self);
    auto& val2 = PyTvFunc::_get(other);
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
  if ( PyTvFunc::_check(self) && PyTvFunc::_check(other) ) {
    auto& val1 = PyTvFunc::_get(self);
    auto& val2 = PyTvFunc::_get(other);
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
  auto val = PyTvFunc::_get(self);
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
PyTvFunc::_check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, _typeobject());
}

// @brief TvFunc を表す PyObject から TvFunc を取り出す．
TvFunc&
PyTvFunc::_get(
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
