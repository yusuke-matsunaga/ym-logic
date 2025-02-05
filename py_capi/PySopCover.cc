
/// @file PySopCover.cc
/// @brief Python SopCover の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PySopCover.h"
#include "pym/PySopCube.h"
#include "pym/PyLiteral.h"
#include "pym/PyExpr.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
struct SopCoverObject
{
  PyObject_HEAD
  SopCover mVal;
};

// Python 用のタイプ定義
PyTypeObject SopCoverType = {
  PyVarObject_HEAD_INIT(nullptr, 0)
};

// 生成関数
PyObject*
SopCover_new(
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
  vector<SopCube> cube_list;
  if ( obj1 != nullptr ) {
    SizeType n = PyList_Size(obj1);
    cube_list.reserve(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      auto obj2 = PyList_GetItem(obj1, i);
      if ( PySopCube::Check(obj2) ) {
	auto cube = PySopCube::Get(obj2);
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
	cube_list.push_back(SopCube{ni, lit_list});
      }
      else {
	goto error;
      }
    }
  }
  {
    auto self = type->tp_alloc(type, 0);
    auto sopcover_obj = reinterpret_cast<SopCoverObject*>(self);
    new (&sopcover_obj->mVal) SopCover{ni, cube_list};
    return self;
  }
 error:
  PyErr_SetString(PyExc_TypeError,
		  "argument 2 must a list of 'SopCube' or list of list of 'Literal'");
  return nullptr;
}

// 終了関数
void
SopCover_dealloc(
  PyObject* self
)
{
  auto sopcover_obj = reinterpret_cast<SopCoverObject*>(self);
  sopcover_obj->mVal.~SopCover();
  Py_TYPE(self)->tp_free(self);
}

PyObject*
SopCover_copy(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& cov = PySopCover::Get(self);
  return PySopCover::ToPyObject(cov);
}

PyObject*
SopCover_literal_num(
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
  auto& cov = PySopCover::Get(self);
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
SopCover_literal_list(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& cov = PySopCover::Get(self);
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
SopCover_get_pat(
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
  auto& cov = PySopCover::Get(self);
  auto pat = cov.get_pat(cpos, vpos);
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

#if 0
PyObject*
SopCover_common_cube(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& cov = PySopCover::Get(self);
  auto cc = cov.common_cube();
  return PySopCube::ToPyObject(cc);
}
#endif

PyObject*
SopCover_expr(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& cov = PySopCover::Get(self);
  auto expr = cov.expr();
  return PyExpr::ToPyObject(expr);
}

// メソッド定義
PyMethodDef SopCover_methods[] = {
  {"copy", SopCover_copy,
   METH_NOARGS,
   PyDoc_STR("return copy of this object.")},
  {"literal_num", reinterpret_cast<PyCFunction>(SopCover_literal_num),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("count the number of literals")},
  {"literal_list", SopCover_literal_list,
   METH_NOARGS,
   PyDoc_STR("convert to the list of list of literals")},
  {"get_pat", reinterpret_cast<PyCFunction>(SopCover_get_pat),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("get the pattern('-', '0', or '1') of the specified position")},
#if 0
  {"common_cube", SopCover_common_cube,
   METH_NOARGS,
   PyDoc_STR("return the common cube")},
#endif
  {"expr", SopCover_expr,
   METH_NOARGS,
   PyDoc_STR("convert to 'Expr'")},
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
SopCover_variable_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& cov = PySopCover::Get(self);
  auto ans = cov.variable_num();
  return PyLong_FromLong(ans);
}

PyObject*
SopCover_cube_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& cov = PySopCover::Get(self);
  auto ans = cov.cube_num();
  return PyLong_FromLong(ans);
}

// get/set 関数定義
PyGetSetDef SopCover_getset[] = {
  {"variable_num", SopCover_variable_num, nullptr,
   PyDoc_STR("number of variables"), nullptr},
  {"cube_num", SopCover_cube_num, nullptr,
   PyDoc_STR("number of cubes"), nullptr},
  {nullptr, nullptr, nullptr, nullptr, nullptr},
};

// 比較関数
PyObject*
SopCover_richcmpfunc(
  PyObject* self,
  PyObject* other,
  int op
)
{
  if ( PySopCover::Check(self) &&
       PySopCover::Check(other) ) {
    auto val1 = PySopCover::Get(self);
    auto val2 = PySopCover::Get(other);
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
SopCover_add(
  PyObject* self,
  PyObject* other
)
{
  if ( PySopCover::Check(self) ) {
    auto& val1 = PySopCover::Get(self);
    if ( PySopCover::Check(other) ) {
      auto& val2 = PySopCover::Get(other);
      if ( val1.variable_num() != val2.variable_num() ) {
	PyErr_SetString(PyExc_ValueError,
			"variable_num() is different from each other");
	return nullptr;
      }
      auto val3 = val1 | val2;
      return PySopCover::ToPyObject(val3);
    }
    if ( PySopCube::Check(other) ) {
      auto& val2 = PySopCube::Get(other);
      if ( val1.variable_num() != val2.variable_num() ) {
	PyErr_SetString(PyExc_ValueError,
			"variable_num() is different from each other");
	return nullptr;
      }
      auto val3 = val1 | val2;
      return PySopCover::ToPyObject(val3);
    }
  }
  else if ( PySopCube::Check(self) && PySopCover::Check(other) ) {
    auto& val1 = PySopCube::Get(self);
    auto& val2 = PySopCover::Get(other);
    if ( val1.variable_num() != val2.variable_num() ) {
      PyErr_SetString(PyExc_ValueError,
		      "variable_num() is different from each other");
      return nullptr;
    }
    auto val3 = val1 | val2;
    return PySopCover::ToPyObject(val3);
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// 加算(連結)つき代入
PyObject*
SopCover_iadd(
  PyObject* self,
  PyObject* other
)
{
  if ( PySopCover::Check(self) ) {
    auto& val1 = PySopCover::Get(self);
    if ( PySopCover::Check(other) ) {
      auto& val2 = PySopCover::Get(other);
      if ( val1.variable_num() != val2.variable_num() ) {
	PyErr_SetString(PyExc_ValueError,
			"variable_num() is different from each other");
	return nullptr;
      }
      val1 |= val2;
      Py_IncRef(self);
      return self;
    }
    if ( PySopCube::Check(other) ) {
      auto& val2 = PySopCube::Get(other);
      if ( val1.variable_num() != val2.variable_num() ) {
	PyErr_SetString(PyExc_ValueError,
			"variable_num() is different from each other");
	return nullptr;
      }
      val1 |= val2;
      Py_IncRef(self);
      return self;
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

#if 0
// 減算
PyObject*
SopCover_sub(
  PyObject* self,
  PyObject* other
)
{
  if ( PySopCover::Check(self) ) {
    auto& val1 = PySopCover::Get(self);
    if ( PySopCover::Check(other) ) {
      auto& val2 = PySopCover::Get(other);
      if ( val1.variable_num() != val2.variable_num() ) {
	PyErr_SetString(PyExc_ValueError,
			"variable_num() is different from each other");
	return nullptr;
      }
      auto val3 = val1 - val2;
      return PySopCover::ToPyObject(val3);
    }
    if ( PySopCube::Check(other) ) {
      auto& val2 = PySopCube::Get(other);
      if ( val1.variable_num() != val2.variable_num() ) {
	PyErr_SetString(PyExc_ValueError,
			"variable_num() is different from each other");
	return nullptr;
      }
      auto val3 = val1 - val2;
      return PySopCover::ToPyObject(val3);
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// 減算つき代入
PyObject*
SopCover_isub(
  PyObject* self,
  PyObject* other
)
{
  if ( PySopCover::Check(self) ) {
    auto& val1 = PySopCover::Get(self);
    if ( PySopCover::Check(other) ) {
      auto& val2 = PySopCover::Get(other);
      if ( val1.variable_num() != val2.variable_num() ) {
	PyErr_SetString(PyExc_ValueError,
			"variable_num() is different from each other");
	return nullptr;
      }
      val1 -= val2;
      Py_IncRef(self);
      return self;
    }
    if ( PySopCube::Check(other) ) {
      auto& val2 = PySopCube::Get(other);
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
#endif

#if 0
// 乗算
PyObject*
SopCover_mult(
  PyObject* self,
  PyObject* other
)
{
  if ( PySopCover::Check(self) ) {
    auto& val1 = PySopCover::Get(self);
    if ( PySopCover::Check(other) ) {
      auto& val2 = PySopCover::Get(other);
      if ( val1.variable_num() != val2.variable_num() ) {
	PyErr_SetString(PyExc_ValueError,
			"variable_num() is different from each other");
	return nullptr;
      }
      auto val3 = val1 * val2;
      return PySopCover::ToPyObject(val3);
    }
    if ( PySopCube::Check(other) ) {
      auto& val2 = PySopCube::Get(other);
      if ( val1.variable_num() != val2.variable_num() ) {
	PyErr_SetString(PyExc_ValueError,
			"variable_num() is different from each other");
	return nullptr;
      }
      auto val3 = val1 * val2;
      return PySopCover::ToPyObject(val3);
    }
    if ( PyLiteral::Check(other) ) {
      auto val2 = PyLiteral::Get(other);
      auto val3 = val1 * val2;
      return PySopCover::ToPyObject(val3);
    }
  }
  else if ( PySopCover::Check(other) ) {
    auto& val2 = PySopCover::Get(other);
    if ( PySopCube::Check(self) ) {
      auto& val1 = PySopCube::Get(self);
      if ( val1.variable_num() != val2.variable_num() ) {
	PyErr_SetString(PyExc_ValueError,
			"variable_num() is different from each other");
	return nullptr;
      }
      auto val3 = val1 * val2;
      return PySopCover::ToPyObject(val3);
    }
    if ( PyLiteral::Check(self) ) {
      auto val1 = PyLiteral::Get(self);
      auto val3 = val1 * val2;
      return PySopCover::ToPyObject(val3);
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// 乗算つき代入
PyObject*
SopCover_imult(
  PyObject* self,
  PyObject* other
)
{
  if ( PySopCover::Check(self) ) {
    auto& val1 = PySopCover::Get(self);
    if ( PySopCover::Check(other) ) {
      auto& val2 = PySopCover::Get(other);
      if ( val1.variable_num() != val2.variable_num() ) {
	PyErr_SetString(PyExc_ValueError,
			"variable_num() is different from each other");
	return nullptr;
      }
      val1 *= val2;
      Py_IncRef(self);
      return self;
    }
    if ( PySopCube::Check(other) ) {
      auto& val2 = PySopCube::Get(other);
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
SopCover_div(
  PyObject* self,
  PyObject* other
)
{
  if ( PySopCover::Check(self) ) {
    auto& val1 = PySopCover::Get(self);
    if ( PySopCover::Check(other) ) {
      auto& val2 = PySopCover::Get(other);
      if ( val1.variable_num() != val2.variable_num() ) {
	PyErr_SetString(PyExc_ValueError,
			"variable_num() is different from each other");
	return nullptr;
      }
      auto val3 = val1 / val2;
      return PySopCover::ToPyObject(val3);
    }
    if ( PySopCube::Check(other) ) {
      auto& val2 = PySopCube::Get(other);
      if ( val1.variable_num() != val2.variable_num() ) {
	PyErr_SetString(PyExc_ValueError,
			"variable_num() is different from each other");
	return nullptr;
      }
      auto val3 = val1 / val2;
      return PySopCover::ToPyObject(val3);
    }
    if ( PyLiteral::Check(other) ) {
      auto val2 = PyLiteral::Get(other);
      auto val3 = val1 / val2;
      return PySopCover::ToPyObject(val3);
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// 除算つき代入
PyObject*
SopCover_idiv(
  PyObject* self,
  PyObject* other
)
{
  if ( PySopCover::Check(self) ) {
    auto& val1 = PySopCover::Get(self);
    if ( PySopCover::Check(other) ) {
      auto& val2 = PySopCover::Get(other);
      if ( val1.variable_num() != val2.variable_num() ) {
	PyErr_SetString(PyExc_ValueError,
			"variable_num() is different from each other");
	return nullptr;
      }
      val1 /= val2;
      Py_IncRef(self);
      return self;
    }
    if ( PySopCube::Check(other) ) {
      auto& val2 = PySopCube::Get(other);
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
#endif

// 数値演算メソッド定義
PyNumberMethods SopCover_number = {
  .nb_add = SopCover_add,
  //.nb_subtract = SopCover_sub,
  //.nb_multiply = SopCover_mult,
  .nb_inplace_add = SopCover_iadd,
  //.nb_inplace_subtract = SopCover_isub,
  //.nb_inplace_multiply = SopCover_mult,
  //.nb_true_divide = SopCover_div,
  //.nb_inplace_true_divide = SopCover_idiv
};

// ハッシュ関数
Py_hash_t
SopCover_hash(
  PyObject* self
)
{
  auto& val = PySopCover::Get(self);
  return val.hash();
}

END_NONAMESPACE


// @brief 'SopCover' オブジェクトを使用可能にする．
bool
PySopCover::init(
  PyObject* m
)
{
  SopCoverType.tp_name = "SopCover";
  SopCoverType.tp_basicsize = sizeof(SopCoverObject);
  SopCoverType.tp_itemsize = 0;
  SopCoverType.tp_dealloc = SopCover_dealloc;
  SopCoverType.tp_flags = Py_TPFLAGS_DEFAULT;
  SopCoverType.tp_doc = PyDoc_STR("SopCover object");
  SopCoverType.tp_richcompare = SopCover_richcmpfunc;
  SopCoverType.tp_methods = SopCover_methods;
  SopCoverType.tp_getset = SopCover_getset;
  SopCoverType.tp_new = SopCover_new;
  SopCoverType.tp_as_number = &SopCover_number;
  SopCoverType.tp_hash = SopCover_hash;

  // 型オブジェクトの登録
  if ( !PyModule::reg_type(m, "SopCover", &SopCoverType) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// @brief SopCover を PyObject に変換する．
PyObject*
PySopCover::ToPyObject(
  const SopCover& val
)
{
  auto obj = SopCoverType.tp_alloc(&SopCoverType, 0);
  auto sopcover_obj = reinterpret_cast<SopCoverObject*>(obj);
  new (&sopcover_obj->mVal) SopCover{val};
  return obj;
}

// @brief SopCover を PyObject に変換する．
PyObject*
PySopCover::ToPyObject(
  const SopCover&& val
)
{
  auto obj = SopCoverType.tp_alloc(&SopCoverType, 0);
  auto sopcover_obj = reinterpret_cast<SopCoverObject*>(obj);
  new (&sopcover_obj->mVal) SopCover{std::move(val)};
  return obj;
}

// @brief PyObject が SopCover タイプか調べる．
bool
PySopCover::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, _typeobject());
}

// @brief SopCover を表す PyObject から SopCover を取り出す．
SopCover&
PySopCover::Get(
  PyObject* obj
)
{
  auto sopcover_obj = reinterpret_cast<SopCoverObject*>(obj);
  return sopcover_obj->mVal;
}

// @brief SopCover を表すオブジェクトの型定義を返す．
PyTypeObject*
PySopCover::_typeobject()
{
  return &SopCoverType;
}

END_NAMESPACE_YM
