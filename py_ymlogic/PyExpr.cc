
/// @file PyExpr.cc
/// @brief Python Expr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/PyExpr.h"
#include "ym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
struct ExprObject
{
  PyObject_HEAD
  Expr* mPtr;
};

// Python 用のタイプ定義
PyTypeObject ExprType = {
  PyVarObject_HEAD_INIT(nullptr, 0)
};

// 生成関数
PyObject*
Expr_new(
  PyTypeObject* type,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "",
    nullptr
  };
  const char* expr_str = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "|s",
				    const_cast<char**>(kwlist),
				    &expr_str) ) {
    return nullptr;
  }
  auto obj = ExprType.tp_alloc(&ExprType, 0);
  auto expr_obj = reinterpret_cast<ExprObject*>(obj);
  Expr src_expr;
  if ( expr_str != nullptr ) {
    try {
      src_expr = Expr::from_rep_string(expr_str);
    }
    catch ( std::invalid_argument& err ) {
      PyErr_SetString(PyExc_ValueError, err.what());
      return nullptr;
    }
  }
  expr_obj->mPtr = new Expr{src_expr};
  return obj;
}

// 終了関数
void
Expr_dealloc(
  PyObject* self
)
{
  auto expr_obj = reinterpret_cast<ExprObject*>(self);
  delete expr_obj->mPtr;
  Py_TYPE(self)->tp_free(self);
}

// repr() 関数
PyObject*
Expr_repr(
  PyObject* self
)
{
  auto expr = PyExpr::_get(self);
  auto tmp_str = expr.rep_string();
  return Py_BuildValue("s", tmp_str.c_str());
}

// str() 関数
PyObject*
Expr_str(
  PyObject* self
)
{
  auto expr = PyExpr::_get(self);
  auto tmp_str = expr.to_string();
  return Py_BuildValue("s", tmp_str.c_str());
}

PyObject*
Expr_make_invalid(
  PyObject* Py_UNUSED(self),
  PyObject* Py_UNUSED(args)
)
{
  auto expr = Expr::make_invalid();
  return PyExpr::ToPyObject(expr);
}

PyObject*
Expr_make_zero(
  PyObject* Py_UNUSED(self),
  PyObject* Py_UNUSED(args)
)
{
  auto expr = Expr::make_zero();
  return PyExpr::ToPyObject(expr);
}

PyObject*
Expr_make_one(
  PyObject* Py_UNUSED(self),
  PyObject* Py_UNUSED(args)
)
{
  auto expr = Expr::make_one();
  return PyExpr::ToPyObject(expr);
}

PyObject*
Expr_make_literal(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "",
    "inv",
    nullptr
  };
  int id = -1;
  int inv = 0;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "i|$p",
				    const_cast<char**>(kwlist),
				    &id, &inv) ) {
    return nullptr;
  }
  auto expr = Expr::make_literal(static_cast<SizeType>(id), inv);
  return PyExpr::ToPyObject(expr);
}

PyObject*
Expr_make_posi_literal(
  PyObject* Py_UNUSED(self),
  PyObject* args
)
{
  int id = -1;
  if ( !PyArg_ParseTuple(args, "i", &id) ) {
    return nullptr;
  }
  auto expr = Expr::make_posi_literal(static_cast<SizeType>(id));
  return PyExpr::ToPyObject(expr);
}

PyObject*
Expr_make_nega_literal(
  PyObject* Py_UNUSED(self),
  PyObject* args
)
{
  int id = -1;
  if ( !PyArg_ParseTuple(args, "i", &id) ) {
    return nullptr;
  }
  auto expr = Expr::make_nega_literal(static_cast<SizeType>(id));
  return PyExpr::ToPyObject(expr);
}

PyObject*
Expr_make_and(
  PyObject* Py_UNUSED(self),
  PyObject* args
)
{
  PyObject* list_obj = nullptr;
  if ( !PyArg_ParseTuple(args, "O!", PyList_Type, &list_obj) ) {
    return nullptr;
  }
  SizeType n = PyList_Size(list_obj);
  vector<Expr> expr_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto obj1 = PyList_GetItem(list_obj, i);
    if ( !PyExpr::_check(obj1) ) {
      PyErr_SetString(PyExc_TypeError, "this function requires list of 'Expr's");
      return nullptr;
    }
    auto expr1 = PyExpr::_get(obj1);
    expr_list[i] = expr1;
  }
  auto expr = Expr::make_and(expr_list);
  return PyExpr::ToPyObject(expr);
}

PyObject*
Expr_make_or(
  PyObject* Py_UNUSED(self),
  PyObject* args
)
{
  PyObject* list_obj = nullptr;
  if ( !PyArg_ParseTuple(args, "O!", PyList_Type, &list_obj) ) {
    return nullptr;
  }
  SizeType n = PyList_Size(list_obj);
  vector<Expr> expr_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto obj1 = PyList_GetItem(list_obj, i);
    if ( !PyExpr::_check(obj1) ) {
      PyErr_SetString(PyExc_TypeError, "this function requires list of 'Expr's");
      return nullptr;
    }
    auto expr1 = PyExpr::_get(obj1);
    expr_list[i] = expr1;
  }
  auto expr = Expr::make_or(expr_list);
  return PyExpr::ToPyObject(expr);
}

PyObject*
Expr_make_xor(
  PyObject* Py_UNUSED(self),
  PyObject* args
)
{
  PyObject* list_obj = nullptr;
  if ( !PyArg_ParseTuple(args, "O!", PyList_Type, &list_obj) ) {
    return nullptr;
  }
  SizeType n = PyList_Size(list_obj);
  vector<Expr> expr_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto obj1 = PyList_GetItem(list_obj, i);
    if ( !PyExpr::_check(obj1) ) {
      PyErr_SetString(PyExc_TypeError, "this function requires list of 'Expr's");
      return nullptr;
    }
    auto expr1 = PyExpr::_get(obj1);
    expr_list[i] = expr1;
  }
  auto expr = Expr::make_xor(expr_list);
  return PyExpr::ToPyObject(expr);
}

PyObject*
Expr_compose(
  PyObject* self,
  PyObject* args
)
{
  PyObject* dict_obj = nullptr;
  if ( !PyArg_ParseTuple(args, "O!",
			 PyDict_Type, &dict_obj) ) {
    return nullptr;
  }
  unordered_map<SizeType, Expr> comp_map;
  PyObject* key;
  PyObject* value;
  Py_ssize_t pos = 0;
  while ( PyDict_Next(dict_obj, &pos, &key, &value) ) {
    auto id = PyLong_AsLong(key);
    if ( id == -1 && PyErr_Occurred() ) {
      return nullptr;
    }
    if ( !PyExpr::_check(value) ) {
      PyErr_SetString(PyExc_TypeError, "'Expr' expected in dictionary value");
      return nullptr;
    }
    auto expr1 = PyExpr::_get(value);
    comp_map.emplace(static_cast<SizeType>(id), expr1);
  }
  auto expr = PyExpr::_get(self);
  auto ans_expr = expr.compose(comp_map);
  return PyExpr::ToPyObject(ans_expr);
}

PyObject*
Expr_remap_var(
  PyObject* self,
  PyObject* args
)
{
  Py_RETURN_NONE;
}

PyObject*
Expr_simplify(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& expr = PyExpr::_get(self);
  expr.simplify();
  Py_RETURN_NONE;
}

PyObject*
Expr_eval(
  PyObject* self,
  PyObject* args
)
{
  Py_RETURN_NONE;
}

PyObject*
Expr_make_tv(
  PyObject* self,
  PyObject* args
)
{
  Py_RETURN_NONE;
}

PyObject*
Expr_is_valid(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto expr = PyExpr::_get(self);
  auto r = expr.is_valid();
  return PyBool_FromLong(r);
}

PyObject*
Expr_is_invalid(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto expr = PyExpr::_get(self);
  auto r = expr.is_invalid();
  return PyBool_FromLong(r);
}

PyObject*
Expr_is_zero(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto expr = PyExpr::_get(self);
  auto r = expr.is_zero();
  return PyBool_FromLong(r);
}

PyObject*
Expr_is_one(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto expr = PyExpr::_get(self);
  auto r = expr.is_one();
  return PyBool_FromLong(r);
}

PyObject*
Expr_is_constant(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto expr = PyExpr::_get(self);
  auto r = expr.is_constant();
  return PyBool_FromLong(r);
}

PyObject*
Expr_is_posi_literal(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto expr = PyExpr::_get(self);
  auto r = expr.is_posi_literal();
  return PyBool_FromLong(r);
}

PyObject*
Expr_is_nega_literal(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto expr = PyExpr::_get(self);
  auto r = expr.is_nega_literal();
  return PyBool_FromLong(r);
}

PyObject*
Expr_is_literal(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto expr = PyExpr::_get(self);
  auto r = expr.is_literal();
  return PyBool_FromLong(r);
}

PyObject*
Expr_is_and(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto expr = PyExpr::_get(self);
  auto r = expr.is_and();
  return PyBool_FromLong(r);
}

PyObject*
Expr_is_or(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto expr = PyExpr::_get(self);
  auto r = expr.is_or();
  return PyBool_FromLong(r);
}

PyObject*
Expr_is_xor(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto expr = PyExpr::_get(self);
  auto r = expr.is_xor();
  return PyBool_FromLong(r);
}

PyObject*
Expr_is_op(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto expr = PyExpr::_get(self);
  auto r = expr.is_op();
  return PyBool_FromLong(r);
}

PyObject*
Expr_is_simple(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto expr = PyExpr::_get(self);
  auto r = expr.is_simple();
  return PyBool_FromLong(r);
}

PyObject*
Expr_is_simple_and(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto expr = PyExpr::_get(self);
  auto r = expr.is_simple_and();
  return PyBool_FromLong(r);
}

PyObject*
Expr_is_simple_or(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto expr = PyExpr::_get(self);
  auto r = expr.is_simple_or();
  return PyBool_FromLong(r);
}

PyObject*
Expr_is_simple_xor(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto expr = PyExpr::_get(self);
  auto r = expr.is_simple_xor();
  return PyBool_FromLong(r);
}

PyObject*
Expr_is_sop(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto expr = PyExpr::_get(self);
  auto r = expr.is_sop();
  return PyBool_FromLong(r);
}

// メソッド定義
PyMethodDef Expr_methods[] = {
  {"make_invalid", Expr_make_invalid, METH_STATIC | METH_NOARGS,
   PyDoc_STR("make an invalid object")},
  {"make_zero", Expr_make_zero, METH_STATIC | METH_NOARGS,
   PyDoc_STR("make a ZERO object")},
  {"make_one", Expr_make_one, METH_STATIC | METH_NOARGS,
   PyDoc_STR("make a ONE object")},
  {"make_literal", reinterpret_cast<PyCFunction>(Expr_make_literal),
   METH_STATIC | METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("make a LITERAL object")},
  {"make_posi_literal", Expr_make_posi_literal, METH_STATIC | METH_NOARGS,
   PyDoc_STR("make a positive LITERAL object")},
  {"make_nega_literal", Expr_make_nega_literal, METH_STATIC | METH_NOARGS,
   PyDoc_STR("make a negative LITERAL object")},
  {"make_and", Expr_make_and, METH_STATIC | METH_VARARGS,
   PyDoc_STR("make an AND object")},
  {"make_or", Expr_make_or, METH_STATIC | METH_VARARGS,
   PyDoc_STR("make an OR object")},
  {"make_xor", Expr_make_xor, METH_STATIC | METH_VARARGS,
   PyDoc_STR("make an XOR object")},
  {"compose", Expr_compose, METH_VARARGS,
   PyDoc_STR("'compose' operation")},
  {"remap_var", Expr_remap_var, METH_VARARGS,
   PyDoc_STR("remap variable")},
  {"simplify", Expr_simplify, METH_NOARGS,
   PyDoc_STR("simplify")},
  {"eval", Expr_eval, METH_VARARGS,
   PyDoc_STR("evaluate")},
  {"make_tv", Expr_make_tv, METH_VARARGS,
   PyDoc_STR("convert to TvFunc")},
  {"is_valid", Expr_is_valid, METH_NOARGS,
   PyDoc_STR("True if valid")},
  {"is_invalid", Expr_is_invalid, METH_NOARGS,
   PyDoc_STR("True if invalid")},
  {"is_zero", Expr_is_zero, METH_NOARGS,
   PyDoc_STR("True if ZERO")},
  {"is_one", Expr_is_one, METH_NOARGS,
   PyDoc_STR("True if ONE")},
  {"is_constant", Expr_is_constant, METH_NOARGS,
   PyDoc_STR("True if constant(ONE/ZERO)")},
  {"is_posi_literal", Expr_is_posi_literal, METH_NOARGS,
   PyDoc_STR("True if positive literal")},
  {"is_nega_literal", Expr_is_nega_literal, METH_NOARGS,
   PyDoc_STR("True if negative literal")},
  {"is_literal", Expr_is_literal, METH_NOARGS,
   PyDoc_STR("True if literal")},
  {"is_and", Expr_is_and, METH_NOARGS,
   PyDoc_STR("True if the root operator is AND")},
  {"is_or", Expr_is_or, METH_NOARGS,
   PyDoc_STR("True if the root operator is OR")},
  {"is_xor", Expr_is_xor, METH_NOARGS,
   PyDoc_STR("True if the root operator is XOR")},
  {"is_op", Expr_is_op, METH_NOARGS,
   PyDoc_STR("True if operator type")},
  {"is_simple", Expr_is_simple, METH_NOARGS,
   PyDoc_STR("True if simple expression")},
  {"is_simple_and", Expr_is_simple_and, METH_NOARGS,
   PyDoc_STR("True if simple AND expression")},
  {"is_simple_or", Expr_is_simple_or, METH_NOARGS,
   PyDoc_STR("True if simple OR expression")},
  {"is_simple_xor", Expr_is_simple_xor, METH_NOARGS,
   PyDoc_STR("True if simple XOR expression")},
  {"is_sop", Expr_is_sop, METH_NOARGS,
   PyDoc_STR("True if SOP expression")},
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
Expr_varid(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto expr = PyExpr::_get(self);
  auto varid = expr.varid();
  return PyLong_FromLong(varid);
}

PyObject*
Expr_operand_list(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto expr = PyExpr::_get(self);
  SizeType n = expr.operand_num();
  auto ans_obj = PyList_New(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto expr1 = expr.operand(i);
    auto expr1_obj = PyExpr::ToPyObject(expr1);
    PyList_SetItem(ans_obj, i, expr1_obj);
  }
  return ans_obj;
}

PyObject*
Expr_input_size(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto expr = PyExpr::_get(self);
  auto ans = expr.input_size();
  return PyLong_FromLong(ans);
}

PyObject*
Expr_literal_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto expr = PyExpr::_get(self);
  auto ans = expr.literal_num();
  return PyLong_FromLong(ans);
}

PyObject*
Expr_sop_literal_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto expr = PyExpr::_get(self);
  auto ans = expr.sop_literal_num();
  return PyLong_FromLong(ans);
}

PyGetSetDef Expr_getsetters[] = {
  {"varid", Expr_varid, nullptr, PyDoc_STR("Variable ID"), nullptr},
  {"operand_list", Expr_operand_list, nullptr, PyDoc_STR("operand list"), nullptr},
  {"input_size", Expr_input_size, nullptr, PyDoc_STR("input size"), nullptr},
  {"literal_num", Expr_literal_num, nullptr, PyDoc_STR("literal count"), nullptr},
  {"sop_literal_num", Expr_sop_literal_num, nullptr, PyDoc_STR("literal count in SOP format"), nullptr},
  {nullptr, nullptr, nullptr, nullptr}
};

// 比較関数
PyObject*
Expr_richcmpfunc(
  PyObject* self,
  PyObject* other,
  int op
)
{
  if ( !PyExpr::_check(self) ||
       !PyExpr::_check(other) ) {
    goto not_implemented;
  }
  {
    auto expr1 = PyExpr::_get(self);
    auto expr2 = PyExpr::_get(other);
    if ( op == Py_EQ ) {
      return PyBool_FromLong(expr1 == expr2);
    }
    if ( op == Py_NE ) {
      return PyBool_FromLong(expr1 != expr2);
    }
  }
 not_implemented:
  Py_RETURN_NOTIMPLEMENTED;
}

// 否定演算(単項演算の例)
PyObject*
Expr_invert(
  PyObject* self
)
{
  if ( PyExpr::_check(self) ) {
    auto val = PyExpr::_get(self);
    return PyExpr::ToPyObject(~val);
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// AND演算
PyObject*
Expr_and(
  PyObject* self,
  PyObject* other
)
{
  if ( PyExpr::_check(self) && PyExpr::_check(other) ) {
    auto val1 = PyExpr::_get(self);
    auto val2 = PyExpr::_get(other);
    return PyExpr::ToPyObject(val1 & val2);
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// OR演算
PyObject*
Expr_or(
  PyObject* self,
  PyObject* other
)
{
  if ( PyExpr::_check(self) && PyExpr::_check(other) ) {
    auto val1 = PyExpr::_get(self);
    auto val2 = PyExpr::_get(other);
    return PyExpr::ToPyObject(val1 | val2);
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// XOR演算
PyObject*
Expr_xor(
  PyObject* self,
  PyObject* other
)
{
  if ( PyExpr::_check(self) && PyExpr::_check(other) ) {
    auto val1 = PyExpr::_get(self);
    auto val2 = PyExpr::_get(other);
    return PyExpr::ToPyObject(val1 ^ val2);
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// 数値演算メソッド定義
PyNumberMethods Expr_number = {
  .nb_invert = Expr_invert,
  .nb_and = Expr_and,
  .nb_xor = Expr_xor,
  .nb_or = Expr_or,
  .nb_inplace_and = Expr_and,
  .nb_inplace_xor = Expr_xor,
  .nb_inplace_or = Expr_or
};

END_NONAMESPACE


// @brief 'Expr' オブジェクトを使用可能にする．
bool
PyExpr::init(
  PyObject* m
)
{
  ExprType.tp_name = "Expr";
  ExprType.tp_basicsize = sizeof(ExprObject);
  ExprType.tp_itemsize = 0;
  ExprType.tp_dealloc = Expr_dealloc;
  ExprType.tp_flags = Py_TPFLAGS_DEFAULT;
  ExprType.tp_doc = PyDoc_STR("Expr object");
  ExprType.tp_richcompare = Expr_richcmpfunc;;
  ExprType.tp_methods = Expr_methods;
  ExprType.tp_getset = Expr_getsetters;
  ExprType.tp_new = Expr_new;
  ExprType.tp_repr = Expr_repr;
  ExprType.tp_str = Expr_str;
  ExprType.tp_as_number = &Expr_number;

  // 型オブジェクトの登録
  if ( !PyModule::reg_type(m, "Expr", &ExprType) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// @brief PyObject から Expr を取り出す．
bool
PyExpr::FromPyObject(
  PyObject* obj,
  Expr& val
)
{
  if ( !_check(obj) ) {
    PyErr_SetString(PyExc_TypeError, "object is not a Expr type");
    return false;
  }
  val = _get(obj);
  return true;
}

// @brief Expr を PyObject に変換する．
PyObject*
PyExpr::ToPyObject(
  const Expr& val
)
{
  auto obj = ExprType.tp_alloc(&ExprType, 0);
  auto expr_obj = reinterpret_cast<ExprObject*>(obj);
  expr_obj->mPtr = new Expr{val};
  return obj;
}

// @brief PyObject が Expr タイプか調べる．
bool
PyExpr::_check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, _typeobject());
}

// @brief Expr を表す PyObject から Expr を取り出す．
Expr&
PyExpr::_get(
  PyObject* obj
)
{
  auto expr_obj = reinterpret_cast<ExprObject*>(obj);
  return *expr_obj->mPtr;
}

// @brief Expr を表すオブジェクトの型定義を返す．
PyTypeObject*
PyExpr::_typeobject()
{
  return &ExprType;
}

END_NAMESPACE_YM