
/// @file PyExpr.cc
/// @brief Python Expr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyExpr.h"
#include "pym/PyLiteral.h"
#include "pym/PyTvFunc.h"
#include "pym/PyPrimType.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
struct ExprObject
{
  PyObject_HEAD
  Expr mExpr;
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
  static const char* kw_list[] = {
    "",
    nullptr
  };
  const char* expr_str = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "|s",
				    const_cast<char**>(kw_list),
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
  new (&expr_obj->mExpr) Expr{src_expr};
  return obj;
}

// 終了関数
void
Expr_dealloc(
  PyObject* self
)
{
  auto expr_obj = reinterpret_cast<ExprObject*>(self);
  expr_obj->mExpr.~Expr();
  Py_TYPE(self)->tp_free(self);
}

// repr() 関数
PyObject*
Expr_repr(
  PyObject* self
)
{
  auto expr = PyExpr::Get(self);
  auto tmp_str = expr.rep_string();
  return Py_BuildValue("s", tmp_str.c_str());
}

// str() 関数
PyObject*
Expr_str(
  PyObject* self
)
{
  auto expr = PyExpr::Get(self);
  auto tmp_str = expr.to_string();
  return Py_BuildValue("s", tmp_str.c_str());
}

PyObject*
Expr_invalid(
  PyObject* Py_UNUSED(self),
  PyObject* Py_UNUSED(args)
)
{
  auto expr = Expr::invalid();
  return PyExpr::ToPyObject(expr);
}

PyObject*
Expr_zero(
  PyObject* Py_UNUSED(self),
  PyObject* Py_UNUSED(args)
)
{
  auto expr = Expr::zero();
  return PyExpr::ToPyObject(expr);
}

PyObject*
Expr_one(
  PyObject* Py_UNUSED(self),
  PyObject* Py_UNUSED(args)
)
{
  auto expr = Expr::one();
  return PyExpr::ToPyObject(expr);
}

PyObject*
Expr_literal(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "var",
    "inv",
    nullptr
  };
  PyObject* var_obj = nullptr;
  int inv_int = 0;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O|$p",
				    const_cast<char**>(kw_list),
				    &var_obj, &inv_int) ) {
    return nullptr;
  }

  Expr expr;
  bool inv = static_cast<bool>(inv_int);
  if ( PyLiteral::Check(var_obj) ) {
    auto lit = PyLiteral::Get(var_obj);
    if ( inv ) {
      lit = ~lit;
    }
    expr = Expr::literal(lit);
  }
  else {
    PyErr_SetString(PyExc_TypeError,
		    "'var' must be an int or a Literal");
    return nullptr;
  }
  return PyExpr::ToPyObject(expr);
}

PyObject*
Expr_posi_literal(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "var",
    nullptr
  };
  SizeType id = -1;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
				    const_cast<char**>(kw_list),
				    &id) ) {
    return nullptr;
  }
  auto expr = Expr::posi_literal(static_cast<SizeType>(id));
  return PyExpr::ToPyObject(expr);
}

PyObject*
Expr_nega_literal(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "var",
    nullptr
  };
  SizeType id = -1;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
				    const_cast<char**>(kw_list),
				    &id) ) {
    return nullptr;
  }
  auto expr = Expr::nega_literal(static_cast<SizeType>(id));
  return PyExpr::ToPyObject(expr);
}

bool
get_expr_list(
  PyObject* arg_obj,
  vector<Expr>& expr_list
)
{
  if ( !PySequence_Check(arg_obj) ) {
    return false;
  }
  SizeType n = PySequence_Size(arg_obj);
  expr_list.clear();
  expr_list.reserve(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto obj1 = PySequence_GetItem(arg_obj, i);
    if ( !PyExpr::Check(obj1) ) {
      Py_XDECREF(obj1);
      return false;
    }
    expr_list.push_back(PyExpr::Get(obj1));
    Py_XDECREF(obj1);
  }
  return true;
}

PyObject*
Expr_and_op(
  PyObject* Py_UNUSED(self),
  PyObject* args
)
{
  PyObject* list_obj = nullptr;
  if ( !PyArg_ParseTuple(args, "O", &list_obj) ) {
    return nullptr;
  }
  vector<Expr> expr_list;
  if ( !get_expr_list(list_obj, expr_list) ) {
    PyErr_SetString(PyExc_TypeError,
		    "argument 1 must be a list of 'Expr's");
    return nullptr;
  }
  auto expr = Expr::and_op(expr_list);
  return PyExpr::ToPyObject(expr);
}

PyObject*
Expr_or_op(
  PyObject* Py_UNUSED(self),
  PyObject* args
)
{
  PyObject* list_obj = nullptr;
  if ( !PyArg_ParseTuple(args, "O", &list_obj) ) {
    return nullptr;
  }
  vector<Expr> expr_list;
  if ( !get_expr_list(list_obj, expr_list) ) {
    PyErr_SetString(PyExc_TypeError,
		    "argument 1 must be a list of 'Expr's");
    return nullptr;
  }
  auto expr = Expr::or_op(expr_list);
  return PyExpr::ToPyObject(expr);
}

PyObject*
Expr_xor_op(
  PyObject* Py_UNUSED(self),
  PyObject* args
)
{
  PyObject* list_obj = nullptr;
  if ( !PyArg_ParseTuple(args, "O", &list_obj) ) {
    return nullptr;
  }
  vector<Expr> expr_list;
  if ( !get_expr_list(list_obj, expr_list) ) {
    PyErr_SetString(PyExc_TypeError,
		    "argument 1 must be a list of 'Expr's");
    return nullptr;
  }
  auto expr = Expr::xor_op(expr_list);
  return PyExpr::ToPyObject(expr);
}

PyObject*
Expr_from_string(
  PyObject* Py_UNUSED(self),
  PyObject* args
)
{
  const char* expr_str = nullptr;
  if ( !PyArg_ParseTuple(args, "s", &expr_str) ) {
    return nullptr;
  }
  try {
    auto expr = Expr::from_string(expr_str);
    return PyExpr::ToPyObject(expr);
  }
  catch ( std::invalid_argument ) {
    PyErr_SetString(PyExc_ValueError,
		    "invalid argument");
    return nullptr;
  }
}

PyObject*
Expr_compose(
  PyObject* self,
  PyObject* args
)
{
  PyObject* dict_obj = nullptr;
  if ( !PyArg_ParseTuple(args, "O!",
			 &PyDict_Type, &dict_obj) ) {
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
    if ( !PyExpr::Check(value) ) {
      PyErr_SetString(PyExc_TypeError,
		      "'Expr' expected in dictionary value");
      return nullptr;
    }
    auto expr1 = PyExpr::Get(value);
    comp_map.emplace(static_cast<SizeType>(id), expr1);
  }
  auto expr = PyExpr::Get(self);
  auto ans_expr = expr.compose(comp_map);
  return PyExpr::ToPyObject(ans_expr);
}

PyObject*
Expr_remap_var(
  PyObject* self,
  PyObject* args
)
{
  PyObject* dict_obj = nullptr;
  if ( !PyArg_ParseTuple(args, "O!",
			 &PyDict_Type, &dict_obj) ) {
    return nullptr;
  }
  unordered_map<SizeType, SizeType> comp_map;
  PyObject* key;
  PyObject* value;
  Py_ssize_t pos = 0;
  while ( PyDict_Next(dict_obj, &pos, &key, &value) ) {
    auto id = PyLong_AsLong(key);
    if ( id == -1 && PyErr_Occurred() ) {
      return nullptr;
    }
    auto val = PyLong_AsLong(value);
    if ( val == -1 && PyErr_Occurred() ) {
      return nullptr;
    }
    comp_map.emplace(static_cast<SizeType>(id), static_cast<SizeType>(val));
  }
  auto expr = PyExpr::Get(self);
  auto ans_expr = expr.remap_var(comp_map);
  return PyExpr::ToPyObject(ans_expr);
}

PyObject*
Expr_simplify(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& expr = PyExpr::Get(self);
  expr.simplify();
  Py_RETURN_NONE;
}

PyObject*
Expr_eval(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "inputs",
    "mask",
    nullptr
  };
  PyObject* vect_obj = nullptr;
  Expr::BitVectType mask = ~0UL;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O|$k",
				    const_cast<char**>(kw_list),
				    &vect_obj, &mask) ) {
    return nullptr;
  }
  if ( !PySequence_Check(vect_obj) ) {
    PyErr_SetString(PyExc_TypeError,
		    "argument 1 must be a vector of int");
    return nullptr;
  }
  SizeType n = PySequence_Size(vect_obj);
  vector<Expr::BitVectType> vals(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto obj1 = PySequence_GetItem(vect_obj, i);
    auto val = PyLong_AsLong(obj1);
    Py_XDECREF(obj1);
    if ( val == -1 && PyErr_Occurred() ) {
      PyErr_SetString(PyExc_TypeError,
		      "argument 1 must be a vector of int");
      return nullptr;
    }
    vals[i] = val;
  }
  auto& expr = PyExpr::Get(self);
  auto ans = expr.eval(vals, mask);
  return PyLong_FromLong(ans);
}

PyObject*
Expr_tvfunc(
  PyObject* self,
  PyObject* args
)
{
  SizeType ni = 0;
  if ( !PyArg_ParseTuple(args, "|k", &ni) ) {
    return nullptr;
  }
  auto& expr = PyExpr::Get(self);
  auto func = expr.tvfunc(ni);
  return PyTvFunc::ToPyObject(std::move(func));
}

PyObject*
Expr_is_valid(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto expr = PyExpr::Get(self);
  auto r = expr.is_valid();
  return PyBool_FromLong(r);
}

PyObject*
Expr_is_invalid(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto expr = PyExpr::Get(self);
  auto r = expr.is_invalid();
  return PyBool_FromLong(r);
}

PyObject*
Expr_is_zero(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto expr = PyExpr::Get(self);
  auto r = expr.is_zero();
  return PyBool_FromLong(r);
}

PyObject*
Expr_is_one(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto expr = PyExpr::Get(self);
  auto r = expr.is_one();
  return PyBool_FromLong(r);
}

PyObject*
Expr_is_constant(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto expr = PyExpr::Get(self);
  auto r = expr.is_constant();
  return PyBool_FromLong(r);
}

PyObject*
Expr_is_posi_literal(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto expr = PyExpr::Get(self);
  auto r = expr.is_posi_literal();
  return PyBool_FromLong(r);
}

PyObject*
Expr_is_nega_literal(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto expr = PyExpr::Get(self);
  auto r = expr.is_nega_literal();
  return PyBool_FromLong(r);
}

PyObject*
Expr_is_literal(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto expr = PyExpr::Get(self);
  auto r = expr.is_literal();
  return PyBool_FromLong(r);
}

PyObject*
Expr_is_and(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto expr = PyExpr::Get(self);
  auto r = expr.is_and();
  return PyBool_FromLong(r);
}

PyObject*
Expr_is_or(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto expr = PyExpr::Get(self);
  auto r = expr.is_or();
  return PyBool_FromLong(r);
}

PyObject*
Expr_is_xor(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto expr = PyExpr::Get(self);
  auto r = expr.is_xor();
  return PyBool_FromLong(r);
}

PyObject*
Expr_is_op(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto expr = PyExpr::Get(self);
  auto r = expr.is_op();
  return PyBool_FromLong(r);
}

PyObject*
Expr_is_simple(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto expr = PyExpr::Get(self);
  auto r = expr.is_simple();
  return PyBool_FromLong(r);
}

PyObject*
Expr_is_simple_and(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto expr = PyExpr::Get(self);
  auto r = expr.is_simple_and();
  return PyBool_FromLong(r);
}

PyObject*
Expr_is_simple_or(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto expr = PyExpr::Get(self);
  auto r = expr.is_simple_or();
  return PyBool_FromLong(r);
}

PyObject*
Expr_is_simple_xor(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto expr = PyExpr::Get(self);
  auto r = expr.is_simple_xor();
  return PyBool_FromLong(r);
}

PyObject*
Expr_is_sop(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto expr = PyExpr::Get(self);
  auto r = expr.is_sop();
  return PyBool_FromLong(r);
}

PyObject*
Expr_analyze(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto expr = PyExpr::Get(self);
  auto ptype = expr.analyze();
  return PyPrimType::ToPyObject(ptype);
}

PyObject*
Expr_get_varid(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto expr = PyExpr::Get(self);
  auto varid = expr.varid();
  return PyLong_FromLong(varid);
}

PyObject*
Expr_get_literal(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto expr = PyExpr::Get(self);
  auto lit = expr.literal();
  return PyLiteral::ToPyObject(lit);
}

PyObject*
Expr_get_operand_list(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto expr = PyExpr::Get(self);
  SizeType n = expr.operand_num();
  auto ans_obj = PyList_New(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto expr1 = expr.operand(i);
    auto expr1_obj = PyExpr::ToPyObject(expr1);
    PyList_SET_ITEM(ans_obj, i, expr1_obj);
  }
  return ans_obj;
}

PyObject*
Expr_literal_num(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "var",
    "inv",
    nullptr
  };
  PyObject* var_obj = nullptr;
  int inv_int = 0;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "|O$p",
				    const_cast<char**>(kw_list),
				    &var_obj, &inv_int) ) {
    return nullptr;
  }
  auto expr = PyExpr::Get(self);
  bool inv = static_cast<bool>(inv_int);
  int ans = 0;
  if ( var_obj == nullptr ) {
    if ( inv ) {
      PyErr_SetString(PyExc_TypeError,
		      "'inv' should be specified with 'var'");
      return nullptr;
    }
    ans = expr.literal_num();
  }
  else if ( PyLiteral::Check(var_obj) ) {
    auto lit = PyLiteral::Get(var_obj);
    if ( inv ) {
      lit = ~lit;
    }
    ans = expr.literal_num(lit);
  }
  else {
    PyErr_SetString(PyExc_TypeError,
		    "'var' must be an int or a Literal");
    return nullptr;
  }
  return PyLong_FromLong(ans);
}

PyObject*
Expr_sop_literal_num(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "var",
    "inv",
    nullptr
  };
  PyObject* var_obj = nullptr;
  int inv_int = false;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "|O$p",
				    const_cast<char**>(kw_list),
				    &var_obj, &inv_int) ) {
    return nullptr;
  }
  auto expr = PyExpr::Get(self);
  bool inv = static_cast<bool>(inv_int);
  int ans = 0;
  if ( var_obj == nullptr ) {
    if ( inv ) {
      PyErr_SetString(PyExc_TypeError,
		      "'inv' should be specified with 'var'");
      return nullptr;
    }
    ans = expr.sop_literal_num();
  }
  else if ( PyLiteral::Check(var_obj) ) {
    auto lit = PyLiteral::Get(var_obj);
    if ( inv_int ) {
      lit = ~lit;
    }
    ans = expr.sop_literal_num(lit);
  }
  else {
    PyErr_SetString(PyExc_TypeError,
		    "'var' must be an int or a Literal");
    return nullptr;
  }
  return PyLong_FromLong(ans);
}

// メソッド定義
PyMethodDef Expr_methods[] = {
  {"invalid", Expr_invalid,
   METH_STATIC | METH_NOARGS,
   PyDoc_STR("make an invalid object")},
  {"zero", Expr_zero,
   METH_STATIC | METH_NOARGS,
   PyDoc_STR("make a ZERO object")},
  {"one", Expr_one,
   METH_STATIC | METH_NOARGS,
   PyDoc_STR("make a ONE object")},
  {"literal", reinterpret_cast<PyCFunction>(Expr_literal),
   METH_STATIC | METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("make a LITERAL object")},
  {"posi_literal", reinterpret_cast<PyCFunction>(Expr_posi_literal),
   METH_STATIC | METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("make a positive LITERAL object")},
  {"nega_literal", reinterpret_cast<PyCFunction>(Expr_nega_literal),
   METH_STATIC | METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("make a negative LITERAL object")},
  {"and_op", Expr_and_op,
   METH_STATIC | METH_VARARGS,
   PyDoc_STR("make an AND object")},
  {"or_op", Expr_or_op,
   METH_STATIC | METH_VARARGS,
   PyDoc_STR("make an OR object")},
  {"xor_op", Expr_xor_op,
   METH_STATIC | METH_VARARGS,
   PyDoc_STR("make an XOR object")},
  {"from_string", Expr_from_string,
   METH_STATIC | METH_VARARGS,
   PyDoc_STR("make Expr from string")},
  {"compose", Expr_compose,
   METH_VARARGS,
   PyDoc_STR("'compose' operation")},
  {"remap_var", Expr_remap_var,
   METH_VARARGS,
   PyDoc_STR("remap variable")},
  {"simplify", Expr_simplify,
   METH_NOARGS,
   PyDoc_STR("simplify")},
  {"eval", reinterpret_cast<PyCFunction>(Expr_eval),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("evaluate")},
  {"tvfunc", Expr_tvfunc,
   METH_VARARGS,
   PyDoc_STR("convert to TvFunc")},
  {"is_valid", Expr_is_valid,
   METH_NOARGS,
   PyDoc_STR("True if valid")},
  {"is_invalid", Expr_is_invalid,
   METH_NOARGS,
   PyDoc_STR("True if invalid")},
  {"is_zero", Expr_is_zero,
   METH_NOARGS,
   PyDoc_STR("True if ZERO")},
  {"is_one", Expr_is_one,
   METH_NOARGS,
   PyDoc_STR("True if ONE")},
  {"is_constant", Expr_is_constant,
   METH_NOARGS,
   PyDoc_STR("True if constant(ONE/ZERO)")},
  {"is_posi_literal", Expr_is_posi_literal,
   METH_NOARGS,
   PyDoc_STR("True if positive literal")},
  {"is_nega_literal", Expr_is_nega_literal,
   METH_NOARGS,
   PyDoc_STR("True if negative literal")},
  {"is_literal", Expr_is_literal,
   METH_NOARGS,
   PyDoc_STR("True if literal")},
  {"is_and", Expr_is_and,
   METH_NOARGS,
   PyDoc_STR("True if the root operator is AND")},
  {"is_or", Expr_is_or,
   METH_NOARGS,
   PyDoc_STR("True if the root operator is OR")},
  {"is_xor", Expr_is_xor,
   METH_NOARGS,
   PyDoc_STR("True if the root operator is XOR")},
  {"is_op", Expr_is_op,
   METH_NOARGS,
   PyDoc_STR("True if operator type")},
  {"is_simple", Expr_is_simple,
   METH_NOARGS,
   PyDoc_STR("True if simple expression")},
  {"is_simple_and", Expr_is_simple_and,
   METH_NOARGS,
   PyDoc_STR("True if simple AND expression")},
  {"is_simple_or", Expr_is_simple_or,
   METH_NOARGS,
   PyDoc_STR("True if simple OR expression")},
  {"is_simple_xor", Expr_is_simple_xor,
   METH_NOARGS,
   PyDoc_STR("True if simple XOR expression")},
  {"is_sop", Expr_is_sop,
   METH_NOARGS,
   PyDoc_STR("True if SOP expression")},
  {"analyze", Expr_analyze, METH_NOARGS,
   PyDoc_STR("check if this expression represents a primitive function")},
  {"literal_num", reinterpret_cast<PyCFunction>(Expr_literal_num),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("count the literal number")},
  {"get_varid", Expr_get_varid,
   METH_NOARGS,
   PyDoc_STR("get var-id (literal type only)")},
  {"get_literal", Expr_get_literal,
   METH_NOARGS,
   PyDoc_STR("get literal (literal type only)")},
  {"get_operand_list", Expr_get_operand_list,
   METH_NOARGS,
   PyDoc_STR("get operand list (OP type only)")},
  {"sop_literal_num", reinterpret_cast<PyCFunction>(Expr_sop_literal_num),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("count the literal number in SOP form")},
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
Expr_sop_cube_num(
  PyObject* self,
  void* Py_UNUSED(colsure)
)
{
  auto expr = PyExpr::Get(self);
  auto val = expr.sop_cube_num();
  return PyLong_FromLong(val);
}

PyObject*
Expr_input_size(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto expr = PyExpr::Get(self);
  auto ans = expr.input_size();
  return PyLong_FromLong(ans);
}

PyGetSetDef Expr_getsetters[] = {
  {"input_size", Expr_input_size, nullptr,
   PyDoc_STR("input size"), nullptr},
  {"sop_cube_num", Expr_sop_cube_num, nullptr,
   PyDoc_STR("number of cubes in SOP form"), nullptr},
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
  if ( !PyExpr::Check(self) ||
       !PyExpr::Check(other) ) {
    goto not_implemented;
  }
  {
    auto expr1 = PyExpr::Get(self);
    auto expr2 = PyExpr::Get(other);
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
  if ( PyExpr::Check(self) ) {
    auto val = PyExpr::Get(self);
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
  if ( PyExpr::Check(self) && PyExpr::Check(other) ) {
    auto val1 = PyExpr::Get(self);
    auto val2 = PyExpr::Get(other);
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
  if ( PyExpr::Check(self) && PyExpr::Check(other) ) {
    auto val1 = PyExpr::Get(self);
    auto val2 = PyExpr::Get(other);
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
  if ( PyExpr::Check(self) && PyExpr::Check(other) ) {
    auto val1 = PyExpr::Get(self);
    auto val2 = PyExpr::Get(other);
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

// @brief Expr を PyObject に変換する．
PyObject*
PyExpr::ToPyObject(
  const Expr& val
)
{
  auto obj = ExprType.tp_alloc(&ExprType, 0);
  auto expr_obj = reinterpret_cast<ExprObject*>(obj);
  new (&expr_obj->mExpr) Expr{val};
  return obj;
}

// @brief PyObject が Expr タイプか調べる．
bool
PyExpr::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, _typeobject());
}

// @brief Expr を表す PyObject から Expr を取り出す．
Expr&
PyExpr::Get(
  PyObject* obj
)
{
  auto expr_obj = reinterpret_cast<ExprObject*>(obj);
  return expr_obj->mExpr;
}

// @brief Expr を表すオブジェクトの型定義を返す．
PyTypeObject*
PyExpr::_typeobject()
{
  return &ExprType;
}

END_NAMESPACE_YM
