
/// @file PyAlgCube.cc
/// @brief Python AlgCube の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyAlgCube.h"
#include "pym/PyLiteral.h"
#include "pym/PyExpr.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
struct AlgCubeObject
{
  PyObject_HEAD
  AlgCube mVal;
};

// Python 用のタイプ定義
PyTypeObject AlgCubeType = {
  PyVarObject_HEAD_INIT(nullptr, 0)
};

// 生成関数
PyObject*
AlgCube_new(
  PyTypeObject* type,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "input_num",
    "literal_list",
    nullptr
  };
  SizeType ni = -1;
  PyObject* obj1 = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k|$O",
				    const_cast<char**>(kw_list),
				    &ni, &obj1) ) {
    return nullptr;
  }
  vector<Literal> lit_list;
  if ( obj1 != nullptr ) {
    if ( PyLiteral::Check(obj1) ) {
      auto lit = PyLiteral::Get(obj1);
      lit_list.push_back(lit);
    }
    else if ( PyList_Check(obj1) ) {
      SizeType n = PyList_Size(obj1);
      lit_list.reserve(n);
      for ( SizeType i = 0; i < n; ++ i ) {
	auto obj2 = PyList_GetItem(obj1, i);
	if ( !PyLiteral::Check(obj2) ) {
	  PyErr_SetString(PyExc_TypeError,
			  "argument 2 must be a 'Literal' or list of 'Literal'");
	  return nullptr;
	}
	auto lit = PyLiteral::Get(obj2);
	lit_list.push_back(lit);
      }
    }
  }
  auto self = type->tp_alloc(type, 0);
  auto sopcube_obj = reinterpret_cast<AlgCubeObject*>(self);
  new (&sopcube_obj->mVal) AlgCube{ni, lit_list};
  return self;
}

// 終了関数
void
AlgCube_dealloc(
  PyObject* self
)
{
  auto sopcube_obj = reinterpret_cast<AlgCubeObject*>(self);
  sopcube_obj->mVal.~AlgCube();
  Py_TYPE(self)->tp_free(self);
}

PyObject*
AlgCube_copy(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& cube = PyAlgCube::Get(self);
  return PyAlgCube::ToPyObject(cube);
}

PyObject*
AlgCube_is_tautology(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& cube = PyAlgCube::Get(self);
  auto ans = cube.is_tautology();
  return PyBool_FromLong(ans);
}

PyObject*
AlgCube_get_pat(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "var_pos",
    nullptr
  };
  SizeType vpos = -1;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
				    const_cast<char**>(kw_list),
				    &vpos) ) {
    return nullptr;
  }
  auto& cube = PyAlgCube::Get(self);
  auto pat = cube.get_pat(vpos);
  const char* ans_str = nullptr;
  if ( pat == AlgPat::_X ) {
    ans_str = "-";
  }
  else if ( pat == AlgPat::_0 ) {
    ans_str = "0";
  }
  else if ( pat == AlgPat::_1 ) {
    ans_str = "1";
  }
  return Py_BuildValue("s", ans_str);
}

PyObject*
AlgCube_check_literal(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "literal",
    nullptr
  };
  PyObject* lit_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!",
				    const_cast<char**>(kw_list),
				    PyLiteral::_typeobject(), &lit_obj) ) {
    return nullptr;
  }
  auto& cube = PyAlgCube::Get(self);
  auto lit = PyLiteral::Get(lit_obj);
  auto ans = cube.check_literal(lit);
  return PyBool_FromLong(ans);
}

PyObject*
AlgCube_literal_list(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& cube = PyAlgCube::Get(self);
  auto lit_list = cube.literal_list();
  SizeType n = lit_list.size();
  auto ans_obj = PyList_New(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto lit = lit_list[i];
    auto lit_obj = PyLiteral::ToPyObject(lit);
    PyList_SET_ITEM(ans_obj, i, lit_obj);
  }
  return ans_obj;
}

PyObject*
AlgCube_check_containment(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "cube",
    nullptr
  };
  PyObject* cube_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!",
				    const_cast<char**>(kw_list),
				    PyAlgCube::_typeobject(), &cube_obj) ) {
    return nullptr;
  }
  auto& cube = PyAlgCube::Get(self);
  auto& cube1 = PyAlgCube::Get(cube_obj);
  if ( cube.variable_num() != cube1.variable_num() ) {
    PyErr_SetString(PyExc_ValueError,
		    "variable_num() is differenct from each other");
    return nullptr;
  }
  auto ans = cube.check_containment(cube1);
  return PyBool_FromLong(ans);
}

PyObject*
AlgCube_expr(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& cube = PyAlgCube::Get(self);
  auto expr = cube.expr();
  return PyExpr::ToPyObject(expr);
}

// メソッド定義
PyMethodDef AlgCube_methods[] = {
  {"copy", AlgCube_copy,
   METH_NOARGS,
   PyDoc_STR("return copy of this object")},
  {"is_tautology", AlgCube_is_tautology,
   METH_NOARGS,
   PyDoc_STR("return True if tautology")},
  {"get_pat", reinterpret_cast<PyCFunction>(AlgCube_get_pat),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("get the pattern('-', '0', or '1') of the specified position")},
  {"check_literal", reinterpret_cast<PyCFunction>(AlgCube_check_literal),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return True if containing the specified literal")},
  {"literal_list", AlgCube_literal_list,
   METH_NOARGS,
   PyDoc_STR("convert to the list of 'Literal'")},
  {"check_containment", reinterpret_cast<PyCFunction>(AlgCube_check_containment),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return True if being contained in the argument cube")},
  {"expr", AlgCube_expr,
   METH_NOARGS,
   PyDoc_STR("convert to 'Expr'")},
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
AlgCube_variable_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& cube = PyAlgCube::Get(self);
  auto ans = cube.variable_num();
  return PyLong_FromLong(ans);
}

PyObject*
AlgCube_literal_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& cube = PyAlgCube::Get(self);
  auto ans = cube.literal_num();
  return PyLong_FromLong(ans);
}

// get/set 関数定義
PyGetSetDef AlgCube_getset[] = {
  {"variable_num", AlgCube_variable_num, nullptr,
   PyDoc_STR("number of variables"), nullptr},
  {"literal_num", AlgCube_literal_num, nullptr,
   PyDoc_STR("number of literals"), nullptr},
  {nullptr, nullptr, nullptr, nullptr, nullptr},
};

// 比較関数
PyObject*
AlgCube_richcmpfunc(
  PyObject* self,
  PyObject* other,
  int op
)
{
  if ( PyAlgCube::Check(self) &&
       PyAlgCube::Check(other) ) {
    auto val1 = PyAlgCube::Get(self);
    auto val2 = PyAlgCube::Get(other);
    if ( val1.variable_num() != val2.variable_num() ) {
      PyErr_SetString(PyExc_ValueError,
		      "variable_num() is differenct from each other");
      return nullptr;
    }
    Py_RETURN_RICHCOMPARE(val1, val2, op);
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// 乗算
PyObject*
AlgCube_mul(
  PyObject* self,
  PyObject* other
)
{
  if ( PyAlgCube::Check(self) ) {
    auto& val1 = PyAlgCube::Get(self);
    if ( PyAlgCube::Check(other) ) {
      auto& val2 = PyAlgCube::Get(other);
      auto val3 = val1 * val2;
      return PyAlgCube::ToPyObject(val3);
    }
    if ( PyLiteral::Check(other) ) {
      auto val2 = PyLiteral::Get(other);
      auto val3 = val1 * val2;
      return PyAlgCube::ToPyObject(val3);
    }
  }
  else if ( PyLiteral::Check(self) && PyAlgCube::Check(other) ) {
    auto val1 = PyLiteral::Get(self);
    auto& val2 = PyAlgCube::Get(other);
    auto val3 = val1 * val2;
    return PyAlgCube::ToPyObject(val3);
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// 乗算つき代入
PyObject*
AlgCube_imul(
  PyObject* self,
  PyObject* other
)
{
  if ( PyAlgCube::Check(self) ) {
    auto& val1 = PyAlgCube::Get(self);
    if ( PyAlgCube::Check(other) ) {
      auto& val2 = PyAlgCube::Get(other);
      val1 *= val2;
      Py_IncRef(self);
      return self;
    }
    if ( PyLiteral::Check(other) ) {
      auto val2 = PyLiteral::Get(other);
      val1 *= val2;
      Py_IncRef(self);
      return self;
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// 除算
PyObject*
AlgCube_div(
  PyObject* self,
  PyObject* other
)
{
  if ( PyAlgCube::Check(self) ) {
    auto& val1 = PyAlgCube::Get(self);
    if ( PyAlgCube::Check(other) ) {
      auto& val2 = PyAlgCube::Get(other);
      auto val3 = val1 / val2;
      return PyAlgCube::ToPyObject(val3);
    }
    if ( PyLiteral::Check(other) ) {
      auto val2 = PyLiteral::Get(other);
      auto val3 = val1 / val2;
      return PyAlgCube::ToPyObject(val3);
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// 除算つき代入
PyObject*
AlgCube_idiv(
  PyObject* self,
  PyObject* other
)
{
  if ( PyAlgCube::Check(self) ) {
    auto& val1 = PyAlgCube::Get(self);
    if ( PyAlgCube::Check(other) ) {
      auto& val2 = PyAlgCube::Get(other);
      val1 /= val2;
      Py_IncRef(self);
      return self;
    }
    if ( PyLiteral::Check(other) ) {
      auto val2 = PyLiteral::Get(other);
      val1 /= val2;
      Py_IncRef(self);
      return self;
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// 数値演算メソッド定義
PyNumberMethods AlgCube_number = {
  .nb_multiply = AlgCube_mul,
  .nb_inplace_multiply = AlgCube_imul,
  .nb_true_divide = AlgCube_div,
  .nb_inplace_true_divide = AlgCube_div
};

// ハッシュ関数
Py_hash_t
AlgCube_hash(
  PyObject* self
)
{
  auto val = PyAlgCube::Get(self);
  return val.hash();
}

END_NONAMESPACE


// @brief 'AlgCube' オブジェクトを使用可能にする．
bool
PyAlgCube::init(
  PyObject* m
)
{
  AlgCubeType.tp_name = "AlgCube";
  AlgCubeType.tp_basicsize = sizeof(AlgCubeObject);
  AlgCubeType.tp_itemsize = 0;
  AlgCubeType.tp_dealloc = AlgCube_dealloc;
  AlgCubeType.tp_flags = Py_TPFLAGS_DEFAULT;
  AlgCubeType.tp_doc = PyDoc_STR("AlgCube object");
  AlgCubeType.tp_richcompare = AlgCube_richcmpfunc;
  AlgCubeType.tp_methods = AlgCube_methods;
  AlgCubeType.tp_getset = AlgCube_getset;
  AlgCubeType.tp_new = AlgCube_new;
  AlgCubeType.tp_as_number = &AlgCube_number;
  AlgCubeType.tp_hash = AlgCube_hash;

  // 型オブジェクトの登録
  if ( !PyModule::reg_type(m, "AlgCube", &AlgCubeType) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// @brief AlgCube を PyObject に変換する．
PyObject*
PyAlgCube::ToPyObject(
  const AlgCube& val
)
{
  auto obj = AlgCubeType.tp_alloc(&AlgCubeType, 0);
  auto sopcube_obj = reinterpret_cast<AlgCubeObject*>(obj);
  new (&sopcube_obj->mVal) AlgCube{val};
  return obj;
}

// @brief AlgCube を PyObject に変換する．
PyObject*
PyAlgCube::ToPyObject(
  const AlgCube&& val
)
{
  auto obj = AlgCubeType.tp_alloc(&AlgCubeType, 0);
  auto sopcube_obj = reinterpret_cast<AlgCubeObject*>(obj);
  new (&sopcube_obj->mVal) AlgCube{std::move(val)};
  return obj;
}

// @brief PyObject が AlgCube タイプか調べる．
bool
PyAlgCube::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, _typeobject());
}

// @brief AlgCube を表す PyObject から AlgCube を取り出す．
AlgCube&
PyAlgCube::Get(
  PyObject* obj
)
{
  auto sopcube_obj = reinterpret_cast<AlgCubeObject*>(obj);
  return sopcube_obj->mVal;
}

// @brief AlgCube を表すオブジェクトの型定義を返す．
PyTypeObject*
PyAlgCube::_typeobject()
{
  return &AlgCubeType;
}

END_NAMESPACE_YM
