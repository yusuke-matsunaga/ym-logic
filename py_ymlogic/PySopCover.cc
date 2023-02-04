
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
  SopCover* mVal;
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
    "",
    "",
    nullptr
  };
  SizeType ni = 0;
  PyObject* obj1 = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k|O!",
				    const_cast<char**>(kw_list),
				    &ni,
				    &PyList_Type, &obj1) ) {
    return nullptr;
  }
  SizeType n = PyList_Size(obj1);
  vector<SopCube> cube_list;
  if ( n > 0 ) {
    cube_list.reserve(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      auto obj2 = PyList_GetItem(obj1, i);
      if ( PySopCube::_check(obj2) ) {
	cube_list.push_back(PySopCube::_get(obj2));
      }
      else if ( PyList_Check(obj2) ) {
	SizeType m = PyList_Size(obj2);
	vector<Literal> lit_list;
	if ( m > 0 ) {
	  lit_list.reserve(m);
	  for ( SizeType j = 0; j < m; ++ j ) {
	    auto obj3 = PyList_GetItem(obj2, j);
	    if ( !PyLiteral::_check(obj3) ) {
	      goto error;
	    }
	    lit_list.push_back(PyLiteral::_get(obj3));
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
    sopcover_obj->mVal = new SopCover{ni, cube_list};
    return self;
  }
 error:
  PyErr_SetString(PyExc_TypeError, "argument 2 must a list of 'SopCube' or list of list of 'Literal'");
  return nullptr;
}

// 終了関数
void
SopCover_dealloc(
  PyObject* self
)
{
  auto sopcover_obj = reinterpret_cast<SopCoverObject*>(self);
  delete sopcover_obj->mVal;
  Py_TYPE(self)->tp_free(self);
}

PyObject*
SopCover_literal_num(
  PyObject* self,
  PyObject* args
)
{
  PyObject* lit_obj = nullptr;
  if ( !PyArg_ParseTuple(args, "|O!",
			 PyLiteral::_typeobject(), &lit_obj) ) {
    return nullptr;
  }
  auto& cov = PySopCover::_get(self);
  if ( lit_obj == nullptr ) {
    return PyLong_FromLong(cov.literal_num());
  }
  else {
    auto lit = PyLiteral::_get(lit_obj);
    return PyLong_FromLong(cov.literal_num(lit));
  }
}

PyObject*
SopCover_literal_list(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& cov = PySopCover::_get(self);
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
      PyList_SetItem(obj1, j, lit_obj);
    }
    PyList_SetItem(ans_obj, i, obj1);
  }
  return ans_obj;
}

PyObject*
SopCover_get_pat(
  PyObject* self,
  PyObject* args
)
{
  SizeType cpos = -1;
  SizeType vpos = -1;
  if ( !PyArg_ParseTuple(args, "kk", &cpos, &vpos) ) {
    return nullptr;
  }
  auto& cov = PySopCover::_get(self);
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

PyObject*
SopCover_common_cube(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& cov = PySopCover::_get(self);
  auto cc = cov.common_cube();
  return PySopCube::ToPyObject(cc);
}

PyObject*
SopCover_expr(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& cov = PySopCover::_get(self);
  auto expr = cov.expr();
  return PyExpr::ToPyObject(expr);
}

// メソッド定義
PyMethodDef SopCover_methods[] = {
  {"literal_num", SopCover_literal_num, METH_VARARGS,
   PyDoc_STR("count the number of literals")},
  {"literal_list", SopCover_literal_list, METH_NOARGS,
   PyDoc_STR("convert to the list of list of literals")},
  {"get_pat", SopCover_get_pat, METH_VARARGS,
   PyDoc_STR("get the pattern('-', '0', or '1') of the specified position")},
  {"common_cube", SopCover_common_cube, METH_NOARGS,
   PyDoc_STR("return the common cube")},
  {"expr", SopCover_expr, METH_NOARGS,
   PyDoc_STR("convert to 'Expr'")},
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
SopCover_variable_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& cov = PySopCover::_get(self);
  auto ans = cov.variable_num();
  return PyLong_FromLong(ans);
}

PyObject*
SopCover_cube_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& cov = PySopCover::_get(self);
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
  if ( PySopCover::_check(self) &&
       PySopCover::_check(other) ) {
    auto val1 = PySopCover::_get(self);
    auto val2 = PySopCover::_get(other);
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
  if ( PySopCover::_check(self) ) {
    auto val1 = PySopCover::_get(self);
    if ( PySopCover::_check(other) ) {
      auto val2 = PySopCover::_get(other);
      return PySopCover::ToPyObject(val1 + val2);
    }
    if ( PySopCube::_check(other) ) {
      auto val2 = PySopCube::_get(other);
      return PySopCover::ToPyObject(val1 + val2);
    }
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
  if ( PySopCover::_check(self) ) {
    auto val1 = PySopCover::_get(self);
    if ( PySopCover::_check(other) ) {
      auto val2 = PySopCover::_get(other);
      val1 += val2;
      Py_IncRef(self);
      return self;
    }
    if ( PySopCube::_check(other) ) {
      auto val2 = PySopCube::_get(other);
      val1 += val2;
      Py_IncRef(self);
      return self;
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// 減算
PyObject*
SopCover_sub(
  PyObject* self,
  PyObject* other
)
{
  if ( PySopCover::_check(self) ) {
    auto val1 = PySopCover::_get(self);
    if ( PySopCover::_check(other) ) {
      auto val2 = PySopCover::_get(other);
      return PySopCover::ToPyObject(val1 + val2);
    }
    if ( PySopCube::_check(other) ) {
      auto val2 = PySopCube::_get(other);
      return PySopCover::ToPyObject(val1 + val2);
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
  if ( PySopCover::_check(self) ) {
    auto val1 = PySopCover::_get(self);
    if ( PySopCover::_check(other) ) {
      auto val2 = PySopCover::_get(other);
      val1 -= val2;
      Py_IncRef(self);
      return self;
    }
    if ( PySopCube::_check(other) ) {
      auto val2 = PySopCube::_get(other);
      val1 -= val2;
      Py_IncRef(self);
      return self;
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// 乗算
PyObject*
SopCover_mult(
  PyObject* self,
  PyObject* other
)
{
  if ( PySopCover::_check(self) ) {
    auto val1 = PySopCover::_get(self);
    if ( PySopCover::_check(other) ) {
      auto val2 = PySopCover::_get(other);
      return PySopCover::ToPyObject(val1 * val2);
    }
    if ( PySopCube::_check(other) ) {
      auto val2 = PySopCube::_get(other);
      return PySopCover::ToPyObject(val1 * val2);
    }
    if ( PyLiteral::_check(other) ) {
      auto val2 = PyLiteral::_get(other);
      return PySopCover::ToPyObject(val1 * val2);
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
  if ( PySopCover::_check(self) ) {
    auto val1 = PySopCover::_get(self);
    if ( PySopCover::_check(other) ) {
      auto val2 = PySopCover::_get(other);
      val1 *= val2;
      Py_IncRef(self);
      return self;
    }
    if ( PySopCube::_check(other) ) {
      auto val2 = PySopCube::_get(other);
      val1 *= val2;
      Py_IncRef(self);
      return self;
    }
    if ( PyLiteral::_check(other) ) {
      auto val2 = PyLiteral::_get(other);
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
  if ( PySopCover::_check(self) ) {
    auto val1 = PySopCover::_get(self);
    if ( PySopCover::_check(other) ) {
      auto val2 = PySopCover::_get(other);
      return PySopCover::ToPyObject(val1 / val2);
    }
    if ( PySopCube::_check(other) ) {
      auto val2 = PySopCube::_get(other);
      return PySopCover::ToPyObject(val1 / val2);
    }
    if ( PyLiteral::_check(other) ) {
      auto val2 = PyLiteral::_get(other);
      return PySopCover::ToPyObject(val1 / val2);
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
  if ( PySopCover::_check(self) ) {
    auto val1 = PySopCover::_get(self);
    if ( PySopCover::_check(other) ) {
      auto val2 = PySopCover::_get(other);
      val1 /= val2;
      Py_IncRef(self);
      return self;
    }
    if ( PySopCube::_check(other) ) {
      auto val2 = PySopCube::_get(other);
      val1 /= val2;
      Py_IncRef(self);
      return self;
    }
    if ( PyLiteral::_check(other) ) {
      auto val2 = PyLiteral::_get(other);
      val1 /= val2;
      Py_IncRef(self);
      return self;
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// 数値演算メソッド定義
PyNumberMethods SopCover_number = {
  .nb_add = SopCover_add,
  .nb_subtract = SopCover_sub,
  .nb_multiply = SopCover_mult,
  .nb_inplace_add = SopCover_iadd,
  .nb_inplace_subtract = SopCover_isub,
  .nb_inplace_multiply = SopCover_mult,
  .nb_true_divide = SopCover_div,
  .nb_inplace_true_divide = SopCover_idiv
};

// ハッシュ関数
Py_hash_t
SopCover_hash(
  PyObject* self
)
{
  auto& val = PySopCover::_get(self);
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
  sopcover_obj->mVal = new SopCover{val};
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
  sopcover_obj->mVal = new SopCover{std::move(val)};
  return obj;
}

// @brief PyObject が SopCover タイプか調べる．
bool
PySopCover::_check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, _typeobject());
}

// @brief SopCover を表す PyObject から SopCover を取り出す．
SopCover&
PySopCover::_get(
  PyObject* obj
)
{
  auto sopcover_obj = reinterpret_cast<SopCoverObject*>(obj);
  return *sopcover_obj->mVal;
}

// @brief SopCover を表すオブジェクトの型定義を返す．
PyTypeObject*
PySopCover::_typeobject()
{
  return &SopCoverType;
}

END_NAMESPACE_YM
