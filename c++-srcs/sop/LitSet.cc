
/// @file LitSet.cc
/// @brief LitSet の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
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
  SizeType var_num
) : SopBase{var_num},
    mBody(_cube_size(), 0ULL)
{
}

// @brief コンストラクタ
LitSet::LitSet(
  SizeType var_num,
  Literal lit
) : LitSet{var_num}
{
  SopMgr mgr(variable_num());
  mgr.cube_set_literal(mBody.begin(), lit);
}

// @brief コンストラクタ
LitSet::LitSet(
  SizeType var_num,
  const vector<Literal>& lit_list
) : LitSet{var_num}
{
  SopMgr mgr{variable_num()};
  mgr.cube_set_literals(mBody.begin(), lit_list);
}

// @brief コンストラクタ
LitSet::LitSet(
  SizeType var_num,
  std::initializer_list<Literal>& lit_list
) : LitSet{var_num}
{
  SopMgr mgr{variable_num()};
  mgr.cube_set_literals(mBody.begin(), lit_list);
}

// @brief コピーコンストラクタ
LitSet::LitSet(
  const LitSet& src
) : SopBase{src},
    mBody{src.mBody}
{
}

// @brief 代入演算子
LitSet&
LitSet::operator=(
  const LitSet& src
)
{
  if ( &src != this ) {
    SopBase::operator=(src);
    mBody = src.mBody;
  }

  return *this;
}

// @brief ムーブコンストラクタ
LitSet::LitSet(
  LitSet&& src
) : SopBase{src},
    mBody{std::move(src.mBody)}
{
}

// @brief ムーブ代入演算子
LitSet&
LitSet::operator=(
  LitSet&& src
)
{
  SopBase::operator=(src);
  std::swap(mBody, src.mBody);

  return *this;
}

// @brief 要素を足す．
LitSet&
LitSet::operator+=(
  Literal lit
)
{
  SopMgr mgr{variable_num()};
  mgr.cube_set_literal(mBody.begin(), lit);

  return *this;
}

END_NAMESPACE_YM_SOP
