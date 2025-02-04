
/// @file PyAlgCover.cc
/// @brief Python AlgCover の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyAlgCover.h"
#include "pym/PyAlgCube.h"
#include "pym/PyLiteral.h"
#include "pym/PyExpr.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
struct AlgCoverObject
{
  PyObject_HEAD
  AlgCover mVal;
};

// Python 用のタイプ定義
PyTypeObject AlgCoverType = {
  PyVarObject_HEAD_INIT(nullptr, 0)
};

// 生成関数
PyObject*
AlgCover_new(
  PyTypeObject* type,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "input_num",
    "cube_list",
    nullptr
  };
  SizeType ni = 0;
  PyObject* obj1 = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k|$O!",
				    const_cast<char**>(kw_list),
				    &ni,
				    &PyList_Type, &obj1) ) {
    return nullptr;
  }
  vector<AlgCube> cube_list;
  if ( obj1 != nullptr ) {
    SizeType n = PyList_Size(obj1);
    cube_list.reserve(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      auto obj2 = PyList_GetItem(obj1, i);
      if ( PyAlgCube::Check(obj2) ) {
	auto cube = PyAlgCube::Get(obj2);
	if ( cube.variable_num() != ni ) {
	  PyErr_SetString(PyExc_ValueError,
			  "variable_num of cube mismatch");
	  return nullptr;
	}
	cube_list.push_back(cube);
      }
      else if ( PyList_Check(obj2) ) {
	SizeType m = PyList_Size(obj2);
	vector<Literal> lit_list;
	if ( m > 0 ) {
	  lit_list.reserve(m);
	  for ( SizeType j = 0; j < m; ++ j ) {
	    auto obj3 = PyList_GetItem(obj2, j);
	    if ( !PyLiteral::Check(obj3) ) {
	      goto error;
	    }
	    lit_list.push_back(PyLiteral::Get(obj3));
	  }
	}
	cube_list.push_back(AlgCube{ni, lit_list});
      }
      else {
	goto error;
      }
    }
  }
  {
    auto self = type->tp_alloc(type, 0);
    auto sopcover_obj = reinterpret_cast<AlgCoverObject*>(self);
    new (&sopcover_obj->mVal) AlgCover{ni, cube_list};
    return self;
  }
 error:
  PyErr_SetString(PyExc_TypeError,
		  "argument 2 must a list of 'AlgCube' or list of list of 'Literal'");
  return nullptr;
}

// 終了関数
void
AlgCover_dealloc(
  PyObject* self
)
{
  auto sopcover_obj = reinterpret_cast<AlgCoverObject*>(self);
  sopcover_obj->mVal.~AlgCover();
  Py_TYPE(self)->tp_free(self);
}

PyObject*
AlgCover_copy(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& cov = PyAlgCover::Get(self);
  return PyAlgCover::ToPyObject(cov);
}

PyObject*
AlgCover_literal_num(
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
  auto& cov = PyAlgCover::Get(self);
  bool inv = static_cast<bool>(inv_int);
  int ans = 0;
  if ( var_obj == nullptr ) {
    if ( inv ) {
      PyErr_SetString(PyExc_TypeError,
		      "'inv' should be specified with 'var'");
      return nullptr;
    }
    ans = cov.literal_num();
  }
  else if ( PyLiteral::Check(var_obj) ) {
    auto lit = PyLiteral::Get(var_obj);
    if ( inv ) {
      lit = ~lit;
    }
    ans = cov.literal_num(lit);
  }
  else {
    PyErr_SetString(PyExc_TypeError,
		    "'var' must be an int or a Literal");
    return nullptr;
  }
  return PyLong_FromLong(ans);
}

PyObject*
AlgCover_literal_list(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& cov = PyAlgCover::Get(self);
  auto cube_list = cov.literal_list();
  SizeType n = cube_list.size();
  auto ans_obj = PyList_New(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto& lit_list = cube_list[i];
    SizeType m = lit_list.size();
    auto obj1 = PyList_New(m);
    for ( SizeType j = 0; j < m; ++ j ) {
      auto lit = lit_list[j];
      auto lit_obj = PyLiteral::ToPyObject(lit);
      PyList_SET_ITEM(obj1, j, lit_obj);
    }
    PyList_SET_ITEM(ans_obj, i, obj1);
  }
  return ans_obj;
}

PyObject*
AlgCover_get_pat(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kw_list[] = {
    "cube_pos",
    "var_pos",
    nullptr
  };
  SizeType cpos = -1;
  SizeType vpos = -1;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "kk",
				    const_cast<char**>(kw_list),
				    &cpos, &vpos) ) {
    return nullptr;
  }
  auto& cov = PyAlgCover::Get(self);
  auto pat = cov.get_pat(cpos, vpos);
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
AlgCover_common_cube(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& cov = PyAlgCover::Get(self);
  auto cc = cov.common_cube();
  return PyAlgCube::ToPyObject(cc);
}

PyObject*
AlgCover_expr(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& cov = PyAlgCover::Get(self);
  auto expr = cov.expr();
  return PyExpr::ToPyObject(expr);
}

// メソッド定義
PyMethodDef AlgCover_methods[] = {
  {"copy", AlgCover_copy,
   METH_NOARGS,
   PyDoc_STR("return copy of this object.")},
  {"literal_num", reinterpret_cast<PyCFunction>(AlgCover_literal_num),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("count the number of literals")},
  {"literal_list", AlgCover_literal_list,
   METH_NOARGS,
   PyDoc_STR("convert to the list of list of literals")},
  {"get_pat", reinterpret_cast<PyCFunction>(AlgCover_get_pat),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("get the pattern('-', '0', or '1') of the specified position")},
  {"common_cube", AlgCover_common_cube,
   METH_NOARGS,
   PyDoc_STR("return the common cube")},
  {"expr", AlgCover_expr,
   METH_NOARGS,
   PyDoc_STR("convert to 'Expr'")},
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
AlgCover_variable_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& cov = PyAlgCover::Get(self);
  auto ans = cov.variable_num();
  return PyLong_FromLong(ans);
}

PyObject*
AlgCover_cube_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& cov = PyAlgCover::Get(self);
  auto ans = cov.cube_num();
  return PyLong_FromLong(ans);
}

// get/set 関数定義
PyGetSetDef AlgCover_getset[] = {
  {"variable_num", AlgCover_variable_num, nullptr,
   PyDoc_STR("number of variables"), nullptr},
  {"cube_num", AlgCover_cube_num, nullptr,
   PyDoc_STR("number of cubes"), nullptr},
  {nullptr, nullptr, nullptr, nullptr, nullptr},
};

// 比較関数
PyObject*
AlgCover_richcmpfunc(
  PyObject* self,
  PyObject* other,
  int op
)
{
  if ( PyAlgCover::Check(self) &&
       PyAlgCover::Check(other) ) {
    auto val1 = PyAlgCover::Get(self);
    auto val2 = PyAlgCover::Get(other);
    if ( val1.variable_num() != val2.variable_num() ) {
      PyErr_SetString(PyExc_ValueError,
		      "variable_num() is different from each other");
      return nullptr;
    }
    Py_RETURN_RICHCOMPARE(val1, val2, op);
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// 加算(連結)
PyObject*
AlgCover_add(
  PyObject* self,
  PyObject* other
)
{
  if ( PyAlgCover::Check(self) ) {
    auto& val1 = PyAlgCover::Get(self);
    if ( PyAlgCover::Check(other) ) {
      auto& val2 = PyAlgCover::Get(other);
      if ( val1.variable_num() != val2.variable_num() ) {
	PyErr_SetString(PyExc_ValueError,
			"variable_num() is different from each other");
	return nullptr;
      }
      auto val3 = val1 + val2;
      return PyAlgCover::ToPyObject(val3);
    }
    if ( PyAlgCube::Check(other) ) {
      auto& val2 = PyAlgCube::Get(other);
      if ( val1.variable_num() != val2.variable_num() ) {
	PyErr_SetString(PyExc_ValueError,
			"variable_num() is different from each other");
	return nullptr;
      }
      auto val3 = val1 + val2;
      return PyAlgCover::ToPyObject(val3);
    }
  }
  else if ( PyAlgCube::Check(self) && PyAlgCover::Check(other) ) {
    auto& val1 = PyAlgCube::Get(self);
    auto& val2 = PyAlgCover::Get(other);
    if ( val1.variable_num() != val2.variable_num() ) {
      PyErr_SetString(PyExc_ValueError,
		      "variable_num() is different from each other");
      return nullptr;
    }
    auto val3 = val1 + val2;
    return PyAlgCover::ToPyObject(val3);
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// 加算(連結)つき代入
PyObject*
AlgCover_iadd(
  PyObject* self,
  PyObject* other
)
{
  if ( PyAlgCover::Check(self) ) {
    auto& val1 = PyAlgCover::Get(self);
    if ( PyAlgCover::Check(other) ) {
      auto& val2 = PyAlgCover::Get(other);
      if ( val1.variable_num() != val2.variable_num() ) {
	PyErr_SetString(PyExc_ValueError,
			"variable_num() is different from each other");
	return nullptr;
      }
      val1 += val2;
      Py_IncRef(self);
      return self;
    }
    if ( PyAlgCube::Check(other) ) {
      auto& val2 = PyAlgCube::Get(other);
      if ( val1.variable_num() != val2.variable_num() ) {
	PyErr_SetString(PyExc_ValueError,
			"variable_num() is different from each other");
	return nullptr;
      }
      val1 += val2;
      Py_IncRef(self);
      return self;
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// 減算
PyObject*
AlgCover_sub(
  PyObject* self,
  PyObject* other
)
{
  if ( PyAlgCover::Check(self) ) {
    auto& val1 = PyAlgCover::Get(self);
    if ( PyAlgCover::Check(other) ) {
      auto& val2 = PyAlgCover::Get(other);
      if ( val1.variable_num() != val2.variable_num() ) {
	PyErr_SetString(PyExc_ValueError,
			"variable_num() is different from each other");
	return nullptr;
      }
      auto val3 = val1 - val2;
      return PyAlgCover::ToPyObject(val3);
    }
    if ( PyAlgCube::Check(other) ) {
      auto& val2 = PyAlgCube::Get(other);
      if ( val1.variable_num() != val2.variable_num() ) {
	PyErr_SetString(PyExc_ValueError,
			"variable_num() is different from each other");
	return nullptr;
      }
      auto val3 = val1 - val2;
      return PyAlgCover::ToPyObject(val3);
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// 減算つき代入
PyObject*
AlgCover_isub(
  PyObject* self,
  PyObject* other
)
{
  if ( PyAlgCover::Check(self) ) {
    auto& val1 = PyAlgCover::Get(self);
    if ( PyAlgCover::Check(other) ) {
      auto& val2 = PyAlgCover::Get(other);
      if ( val1.variable_num() != val2.variable_num() ) {
	PyErr_SetString(PyExc_ValueError,
			"variable_num() is different from each other");
	return nullptr;
      }
      val1 -= val2;
      Py_IncRef(self);
      return self;
    }
    if ( PyAlgCube::Check(other) ) {
      auto& val2 = PyAlgCube::Get(other);
      if ( val1.variable_num() != val2.variable_num() ) {
	PyErr_SetString(PyExc_ValueError,
			"variable_num() is different from each other");
	return nullptr;
      }
      val1 -= val2;
      Py_IncRef(self);
      return self;
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// 乗算
PyObject*
AlgCover_mult(
  PyObject* self,
  PyObject* other
)
{
  if ( PyAlgCover::Check(self) ) {
    auto& val1 = PyAlgCover::Get(self);
    if ( PyAlgCover::Check(other) ) {
      auto& val2 = PyAlgCover::Get(other);
      if ( val1.variable_num() != val2.variable_num() ) {
	PyErr_SetString(PyExc_ValueError,
			"variable_num() is different from each other");
	return nullptr;
      }
      auto val3 = val1 * val2;
      return PyAlgCover::ToPyObject(val3);
    }
    if ( PyAlgCube::Check(other) ) {
      auto& val2 = PyAlgCube::Get(other);
      if ( val1.variable_num() != val2.variable_num() ) {
	PyErr_SetString(PyExc_ValueError,
			"variable_num() is different from each other");
	return nullptr;
      }
      auto val3 = val1 * val2;
      return PyAlgCover::ToPyObject(val3);
    }
    if ( PyLiteral::Check(other) ) {
      auto val2 = PyLiteral::Get(other);
      auto val3 = val1 * val2;
      return PyAlgCover::ToPyObject(val3);
    }
  }
  else if ( PyAlgCover::Check(other) ) {
    auto& val2 = PyAlgCover::Get(other);
    if ( PyAlgCube::Check(self) ) {
      auto& val1 = PyAlgCube::Get(self);
      if ( val1.variable_num() != val2.variable_num() ) {
	PyErr_SetString(PyExc_ValueError,
			"variable_num() is different from each other");
	return nullptr;
      }
      auto val3 = val1 * val2;
      return PyAlgCover::ToPyObject(val3);
    }
    if ( PyLiteral::Check(self) ) {
      auto val1 = PyLiteral::Get(self);
      auto val3 = val1 * val2;
      return PyAlgCover::ToPyObject(val3);
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// 乗算つき代入
PyObject*
AlgCover_imult(
  PyObject* self,
  PyObject* other
)
{
  if ( PyAlgCover::Check(self) ) {
    auto& val1 = PyAlgCover::Get(self);
    if ( PyAlgCover::Check(other) ) {
      auto& val2 = PyAlgCover::Get(other);
      if ( val1.variable_num() != val2.variable_num() ) {
	PyErr_SetString(PyExc_ValueError,
			"variable_num() is different from each other");
	return nullptr;
      }
      val1 *= val2;
      Py_IncRef(self);
      return self;
    }
    if ( PyAlgCube::Check(other) ) {
      auto& val2 = PyAlgCube::Get(other);
      if ( val1.variable_num() != val2.variable_num() ) {
	PyErr_SetString(PyExc_ValueError,
			"variable_num() is different from each other");
	return nullptr;
      }
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
AlgCover_div(
  PyObject* self,
  PyObject* other
)
{
  if ( PyAlgCover::Check(self) ) {
    auto& val1 = PyAlgCover::Get(self);
    if ( PyAlgCover::Check(other) ) {
      auto& val2 = PyAlgCover::Get(other);
      if ( val1.variable_num() != val2.variable_num() ) {
	PyErr_SetString(PyExc_ValueError,
			"variable_num() is different from each other");
	return nullptr;
      }
      auto val3 = val1 / val2;
      return PyAlgCover::ToPyObject(val3);
    }
    if ( PyAlgCube::Check(other) ) {
      auto& val2 = PyAlgCube::Get(other);
      if ( val1.variable_num() != val2.variable_num() ) {
	PyErr_SetString(PyExc_ValueError,
			"variable_num() is different from each other");
	return nullptr;
      }
      auto val3 = val1 / val2;
      return PyAlgCover::ToPyObject(val3);
    }
    if ( PyLiteral::Check(other) ) {
      auto val2 = PyLiteral::Get(other);
      auto val3 = val1 / val2;
      return PyAlgCover::ToPyObject(val3);
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// 除算つき代入
PyObject*
AlgCover_idiv(
  PyObject* self,
  PyObject* other
)
{
  if ( PyAlgCover::Check(self) ) {
    auto& val1 = PyAlgCover::Get(self);
    if ( PyAlgCover::Check(other) ) {
      auto& val2 = PyAlgCover::Get(other);
      if ( val1.variable_num() != val2.variable_num() ) {
	PyErr_SetString(PyExc_ValueError,
			"variable_num() is different from each other");
	return nullptr;
      }
      val1 /= val2;
      Py_IncRef(self);
      return self;
    }
    if ( PyAlgCube::Check(other) ) {
      auto& val2 = PyAlgCube::Get(other);
      if ( val1.variable_num() != val2.variable_num() ) {
	PyErr_SetString(PyExc_ValueError,
			"variable_num() is different from each other");
	return nullptr;
      }
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
PyNumberMethods AlgCover_number = {
  .nb_add = AlgCover_add,
  .nb_subtract = AlgCover_sub,
  .nb_multiply = AlgCover_mult,
  .nb_inplace_add = AlgCover_iadd,
  .nb_inplace_subtract = AlgCover_isub,
  .nb_inplace_multiply = AlgCover_mult,
  .nb_true_divide = AlgCover_div,
  .nb_inplace_true_divide = AlgCover_idiv
};

// ハッシュ関数
Py_hash_t
AlgCover_hash(
  PyObject* self
)
{
  auto& val = PyAlgCover::Get(self);
  return val.hash();
}

END_NONAMESPACE


// @brief 'AlgCover' オブジェクトを使用可能にする．
bool
PyAlgCover::init(
  PyObject* m
)
{
  AlgCoverType.tp_name = "AlgCover";
  AlgCoverType.tp_basicsize = sizeof(AlgCoverObject);
  AlgCoverType.tp_itemsize = 0;
  AlgCoverType.tp_dealloc = AlgCover_dealloc;
  AlgCoverType.tp_flags = Py_TPFLAGS_DEFAULT;
  AlgCoverType.tp_doc = PyDoc_STR("AlgCover object");
  AlgCoverType.tp_richcompare = AlgCover_richcmpfunc;
  AlgCoverType.tp_methods = AlgCover_methods;
  AlgCoverType.tp_getset = AlgCover_getset;
  AlgCoverType.tp_new = AlgCover_new;
  AlgCoverType.tp_as_number = &AlgCover_number;
  AlgCoverType.tp_hash = AlgCover_hash;

  // 型オブジェクトの登録
  if ( !PyModule::reg_type(m, "AlgCover", &AlgCoverType) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// @brief AlgCover を PyObject に変換する．
PyObject*
PyAlgCover::ToPyObject(
  const AlgCover& val
)
{
  auto obj = AlgCoverType.tp_alloc(&AlgCoverType, 0);
  auto sopcover_obj = reinterpret_cast<AlgCoverObject*>(obj);
  new (&sopcover_obj->mVal) AlgCover{val};
  return obj;
}

// @brief AlgCover を PyObject に変換する．
PyObject*
PyAlgCover::ToPyObject(
  const AlgCover&& val
)
{
  auto obj = AlgCoverType.tp_alloc(&AlgCoverType, 0);
  auto sopcover_obj = reinterpret_cast<AlgCoverObject*>(obj);
  new (&sopcover_obj->mVal) AlgCover{std::move(val)};
  return obj;
}

// @brief PyObject が AlgCover タイプか調べる．
bool
PyAlgCover::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, _typeobject());
}

// @brief AlgCover を表す PyObject から AlgCover を取り出す．
AlgCover&
PyAlgCover::Get(
  PyObject* obj
)
{
  auto sopcover_obj = reinterpret_cast<AlgCoverObject*>(obj);
  return sopcover_obj->mVal;
}

// @brief AlgCover を表すオブジェクトの型定義を返す．
PyTypeObject*
PyAlgCover::_typeobject()
{
  return &AlgCoverType;
}

END_NAMESPACE_YM
