
/// @file BddMgrHolder.cc
/// @brief BddMgrHolder の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BddMgrHolder.h"
#include "ym/BddLit.h"
#include "BddMgrImpl.h"
#include "DdInfoMgr.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
// クラス BddMgrHolder
//////////////////////////////////////////////////////////////////////

// @brief 生のポインタを指定したコンストラクタ
BddMgrHolder::BddMgrHolder(
  BddMgrImpl* ptr
) : mPtr{ptr}
{
}

// @brief コピーコンストラクタ
BddMgrHolder::BddMgrHolder(
  const BddMgrHolder& src
) : mPtr{src.mPtr}
{
}

// @brief コピー代入演算子
BddMgrHolder&
BddMgrHolder::operator=(
  const BddMgrHolder& src
)
{
  mPtr = src.mPtr;

  return *this;
}

// @brief デストラクタ
BddMgrHolder::~BddMgrHolder()
{
}

// @brief DdEdge を Bdd に変換する．
Bdd
BddMgrHolder::_bdd(
  DdEdge edge
) const
{
  return Bdd{*this, edge};
}

// @brief DdEdge を BddVar に変換する．
BddVar
BddMgrHolder::_var(
  DdEdge edge
) const
{
  return BddVar{*this, edge};
}

END_NAMESPACE_YM_DD
