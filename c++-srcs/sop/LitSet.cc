
/// @file LitSet.cc
/// @brief LitSet の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017, 2018, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "LitSet.h"
#include "SopMgr.h"
#include "ym/SopCube.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
// クラス LitSet
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LitSet::LitSet(
  SizeType variable_num
) : mVariableNum{variable_num},
    mBody{nullptr}
{
  SopMgr mgr{mVariableNum};
  mBody = mgr.new_bv();
  mgr.cube_clear(mBody);
}

// @brief コンストラクタ
LitSet::LitSet(
  SizeType variable_num,
  Literal lit
) : LitSet{variable_num}
{
  SopMgr mgr(mVariableNum);
  mgr.cube_set_literal(mBody, lit);
}

// @brief コンストラクタ
LitSet::LitSet(
  SizeType variable_num,
  const vector<Literal>& lit_list
) : LitSet{variable_num}
{
  SopMgr mgr(mVariableNum);
  mgr.cube_set_literals(mBody, lit_list);
}

// @brief コンストラクタ
LitSet::LitSet(
  SizeType variable_num,
  std::initializer_list<Literal>& lit_list
) : LitSet{variable_num}
{
  SopMgr mgr(mVariableNum);
  mgr.cube_set_literals(mBody, lit_list);
}

// @brief コピーコンストラクタ
LitSet::LitSet(
  const LitSet& src
) : LitSet{src.mVariableNum}
{
  SopMgr mgr(mVariableNum);
  mgr.cube_copy(mBody, src.mBody);
}

// @brief 代入演算子
LitSet&
LitSet::operator=(
  const LitSet& src
)
{
  if ( &src != this ) {
    SopMgr mgr(src.mVariableNum);
    if ( mVariableNum != src.mVariableNum ) {
      delete_body();
      mVariableNum = src.mVariableNum;
      mBody = mgr.new_bv();
    }
    mgr.cube_copy(mBody, src.mBody);
  }

  return *this;
}

// @brief ムーブコンストラクタ
LitSet::LitSet(
  LitSet&& src
) : mVariableNum{src.mVariableNum},
    mBody{src.mBody}
{
  src.mBody = nullptr;
}

// @brief ムーブ代入演算子
LitSet&
LitSet::operator=(
  LitSet&& src
)
{
  delete_body();

  mVariableNum = src.mVariableNum;
  mBody = src.mBody;

  src.mBody = nullptr;

  return *this;
}

// @brief デストラクタ
LitSet::~LitSet()
{
  delete_body();
}

// @brief 要素を足す．
LitSet&
LitSet::operator+=(
  Literal lit
)
{
  SopMgr mgr(mVariableNum);
  mgr.cube_set_literal(mBody, lit);

  return *this;
}

// @brief mBody の領域を削除する．
void
LitSet::delete_body()
{
  SopMgr::delete_bv(mBody);
}

END_NAMESPACE_YM_SOP
