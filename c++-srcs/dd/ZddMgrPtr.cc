
/// @file ZddMgrPtr.cc
/// @brief ZddMgrPtr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ZddMgrPtr.h"
#include "ym/ZddMgr.h"
#include "ZddMgrImpl.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
// クラス ZddMgrPtr
//////////////////////////////////////////////////////////////////////

// @brief 生のポインタを指定したコンストラクタ
ZddMgrPtr::ZddMgrPtr(
  ZddMgrImpl* ptr
) : mMgr{ptr}
{
  if ( is_valid() ) {
    mMgr->inc();
  }
}

// @brief コピーコンストラクタ
ZddMgrPtr::ZddMgrPtr(
  const ZddMgrPtr& src
) : mMgr{src.mMgr}
{
  if ( is_valid() ) {
    mMgr->inc();
  }
}

// @brief コピー代入演算子
ZddMgrPtr&
ZddMgrPtr::operator=(
  const ZddMgrPtr& src
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
ZddMgrPtr::~ZddMgrPtr()
{
  if ( is_valid() ) {
    mMgr->dec();
  }
}

// @brief 親のマネージャを返す．
ZddMgr
ZddMgrPtr::mgr() const
{
  return ZddMgr{mMgr};
}

END_NAMESPACE_YM_DD
