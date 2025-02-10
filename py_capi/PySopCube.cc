
/// @file PySopCube.cc
/// @brief Python SopCube の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PySopCube.h"
#include "pym/PyLiteral.h"
#include "pym/PyExpr.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
struct SopCubeObject
{
  PyObject_HEAD
  SopCube mVal;
};

// Python 用のタイプ定義
PyTypeObject SopCubeType = {
  PyVarObject_HEAD_INIT(nullptr, 0)
};

// 生成関数
PyObject*
SopCube_new(
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
  auto sopcube_obj = reinterpret_cast<SopCubeObject*>(self);
  new (&sopcube_obj->mVal) SopCube{ni, lit_list};
  return self;
}

// 終了関数
void
SopCube_dealloc(
  PyObject* self
)
{
  auto sopcube_obj = reinterpret_cast<SopCubeObject*>(self);
  sopcube_obj->mVal.~SopCube();
  Py_TYPE(self)->tp_free(self);
}

PyObject*
SopCube_copy(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& cube = PySopCube::Get(self);
  return PySopCube::ToPyObject(cube);
}

PyObject*
SopCube_is_valid(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& cube = PySopCube::Get(self);
  auto ans = cube.is_valid();
  return PyBool_FromLong(ans);
}

PyObject*
SopCube_is_invalid(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& cube = PySopCube::Get(self);
  auto ans = cube.is_invalid();
  return PyBool_FromLong(ans);
}

PyObject*
SopCube_is_tautology(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& cube = PySopCube::Get(self);
  auto ans = cube.is_tautology();
  return PyBool_FromLong(ans);
}

PyObject*
SopCube_get_pat(
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
  auto& cube = PySopCube::Get(self);
  auto pat = cube.get_pat(vpos);
  const char* ans_str = nullptr;
  if ( pat == SopPat::_X ) {
    ans_str = "-";
  }
  else if ( pat == SopPat::_0 ) {
    ans_str = "0";
  }
  else if ( pat == SopPat::_1 ) {
    ans_str = "1";
  }
  return Py_BuildValue("s", ans_str);
}

PyObject*
SopCube_check_literal(
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
  auto& cube = PySopCube::Get(self);
  auto lit = PyLiteral::Get(lit_obj);
  auto ans = cube.check_literal(lit);
  return PyBool_FromLong(ans);
}

PyObject*
SopCube_literal_list(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& cube = PySopCube::Get(self);
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
SopCube_check_containment(
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
				    PySopCube::_typeobject(), &cube_obj) ) {
    return nullptr;
  }
  auto& cube = PySopCube::Get(self);
  auto& cube1 = PySopCube::Get(cube_obj);
  if ( cube.variable_num() != cube1.variable_num() ) {
    PyErr_SetString(PyExc_ValueError,
		    "variable_num() is differenct from each other");
    return nullptr;
  }
  auto ans = cube.check_containment(cube1);
  return PyBool_FromLong(ans);
}

PyObject*
SopCube_expr(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& cube = PySopCube::Get(self);
  auto expr = cube.expr();
  return PyExpr::ToPyObject(expr);
}

// メソッド定義
PyMethodDef SopCube_methods[] = {
  {"copy", SopCube_copy,
   METH_NOARGS,
   PyDoc_STR("return copy of this object")},
  {"is_valid", SopCube_is_valid,
   METH_NOARGS,
   PyDoc_STR("return True if valid")},
  {"is_invalid", SopCube_is_invalid,
   METH_NOARGS,
   PyDoc_STR("return True if invalid")},
  {"is_tautology", SopCube_is_tautology,
   METH_NOARGS,
   PyDoc_STR("return True if tautology")},
  {"get_pat", reinterpret_cast<PyCFunction>(SopCube_get_pat),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("get the pattern('-', '0', or '1') of the specified position")},
  {"check_literal", reinterpret_cast<PyCFunction>(SopCube_check_literal),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return True if containing the specified literal")},
  {"literal_list", SopCube_literal_list,
   METH_NOARGS,
   PyDoc_STR("convert to the list of 'Literal'")},
  {"check_containment", reinterpret_cast<PyCFunction>(SopCube_check_containment),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return True if being contained in the argument cube")},
  {"expr", SopCube_expr,
   METH_NOARGS,
   PyDoc_STR("convert to 'Expr'")},
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
SopCube_variable_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& cube = PySopCube::Get(self);
  auto ans = cube.variable_num();
  return PyLong_FromLong(ans);
}

PyObject*
SopCube_literal_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& cube = PySopCube::Get(self);
  auto ans = cube.literal_num();
  return PyLong_FromLong(ans);
}

// get/set 関数定義
PyGetSetDef SopCube_getset[] = {
  {"variable_num", SopCube_variable_num, nullptr,
   PyDoc_STR("number of variables"), nullptr},
  {"literal_num", SopCube_literal_num, nullptr,
   PyDoc_STR("number of literals"), nullptr},
  {nullptr, nullptr, nullptr, nullptr, nullptr},
};

// 比較関数
PyObject*
SopCube_richcmpfunc(
  PyObject* self,
  PyObject* other,
  int op
)
{
  if ( PySopCube::Check(self) &&
       PySopCube::Check(other) ) {
    auto val1 = PySopCube::Get(self);
    auto val2 = PySopCube::Get(other);
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
SopCube_mul(
  PyObject* self,
  PyObject* other
)
{
  if ( PySopCube::Check(self) ) {
    auto& val1 = PySopCube::Get(self);
    if ( PySopCube::Check(other) ) {
      auto& val2 = PySopCube::Get(other);
      auto val3 = val1 & val2;
      return PySopCube::ToPyObject(val3);
    }
    if ( PyLiteral::Check(other) ) {
      auto val2 = PyLiteral::Get(other);
      auto val3 = val1 & val2;
      return PySopCube::ToPyObject(val3);
    }
  }
  else if ( PyLiteral::Check(self) && PySopCube::Check(other) ) {
    auto val1 = PyLiteral::Get(self);
    auto& val2 = PySopCube::Get(other);
    auto val3 = val1 & val2;
    return PySopCube::ToPyObject(val3);
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// 乗算つき代入
PyObject*
SopCube_imul(
  PyObject* self,
  PyObject* other
)
{
  if ( PySopCube::Check(self) ) {
    auto& val1 = PySopCube::Get(self);
    if ( PySopCube::Check(other) ) {
      auto& val2 = PySopCube::Get(other);
      val1 &= val2;
      Py_IncRef(self);
      return self;
    }
    if ( PyLiteral::Check(other) ) {
      auto val2 = PyLiteral::Get(other);
      val1 &= val2;
      Py_IncRef(self);
      return self;
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// 数値演算メソッド定義
PyNumberMethods SopCube_number = {
  .nb_and = SopCube_mul,
  .nb_inplace_and = SopCube_imul,
};

// ハッシュ関数
Py_hash_t
SopCube_hash(
  PyObject* self
)
{
  auto val = PySopCube::Get(self);
  return val.hash();
}

END_NONAMESPACE


// @brief 'SopCube' オブジェクトを使用可能にする．
bool
PySopCube::init(
  PyObject* m
)
{
  SopCubeType.tp_name = "SopCube";
  SopCubeType.tp_basicsize = sizeof(SopCubeObject);
  SopCubeType.tp_itemsize = 0;
  SopCubeType.tp_dealloc = SopCube_dealloc;
  SopCubeType.tp_flags = Py_TPFLAGS_DEFAULT;
  SopCubeType.tp_doc = PyDoc_STR("SopCube object");
  SopCubeType.tp_richcompare = SopCube_richcmpfunc;
  SopCubeType.tp_methods = SopCube_methods;
  SopCubeType.tp_getset = SopCube_getset;
  SopCubeType.tp_new = SopCube_new;
  SopCubeType.tp_as_number = &SopCube_number;
  SopCubeType.tp_hash = SopCube_hash;

  // 型オブジェクトの登録
  if ( !PyModule::reg_type(m, "SopCube", &SopCubeType) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// @brief SopCube を PyObject に変換する．
PyObject*
PySopCube::ToPyObject(
  const SopCube& val
)
{
  auto obj = SopCubeType.tp_alloc(&SopCubeType, 0);
  auto sopcube_obj = reinterpret_cast<SopCubeObject*>(obj);
  new (&sopcube_obj->mVal) SopCube{val};
  return obj;
}

// @brief SopCube を PyObject に変換する．
PyObject*
PySopCube::ToPyObject(
  const SopCube&& val
)
{
  auto obj = SopCubeType.tp_alloc(&SopCubeType, 0);
  auto sopcube_obj = reinterpret_cast<SopCubeObject*>(obj);
  new (&sopcube_obj->mVal) SopCube{std::move(val)};
  return obj;
}

// @brief PyObject が SopCube タイプか調べる．
bool
PySopCube::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, _typeobject());
}

// @brief SopCube を表す PyObject から SopCube を取り出す．
SopCube&
PySopCube::Get(
  PyObject* obj
)
{
  auto sopcube_obj = reinterpret_cast<SopCubeObject*>(obj);
  return sopcube_obj->mVal;
}

// @brief SopCube を表すオブジェクトの型定義を返す．
PyTypeObject*
PySopCube::_typeobject()
{
  return &SopCubeType;
}

END_NAMESPACE_YM
