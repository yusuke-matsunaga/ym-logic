
/// @file BddMgrPtr.cc
/// @brief BddMgrPtr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BddMgrPtr.h"
#include "ym/BddMgr.h"
#include "BddMgrImpl.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
// クラス BddMgrPtr
//////////////////////////////////////////////////////////////////////

// @brief 生のポインタを指定したコンストラクタ
BddMgrPtr::BddMgrPtr(
  BddMgrImpl* ptr
) : mMgr{ptr}
{
  if ( is_valid() ) {
    mMgr->inc();
  }
}

// @brief コピーコンストラクタ
BddMgrPtr::BddMgrPtr(
  const BddMgrPtr& src
) : mMgr{src.mMgr}
{
  if ( is_valid() ) {
    mMgr->inc();
  }
}

// @brief コピー代入演算子
BddMgrPtr&
BddMgrPtr::operator=(
  const BddMgrPtr& src
)
{
  if ( src.is_valid() ) {
    src.mMgr->inc();
  }
  if ( is_valid() ) {
    mMgr->dec();
  }
  mMgr = src.mMgr;
  return *this;
}

// @brief デストラクタ
BddMgrPtr::~BddMgrPtr()
{
  if ( is_valid() ) {
    mMgr->dec();
  }
}

// @brief 親のマネージャを返す．
BddMgr
BddMgrPtr::mgr() const
{
  return BddMgr{mMgr};
}

END_NAMESPACE_YM_DD
