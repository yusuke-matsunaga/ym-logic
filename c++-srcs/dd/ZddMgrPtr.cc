
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
) : mPtr{ptr}
{
#if 0
  if ( is_valid() ) {
    mPtr->inc();
  }
#endif
}

// @brief コピーコンストラクタ
ZddMgrPtr::ZddMgrPtr(
  const ZddMgrPtr& src
) : mPtr{src.mPtr}
{
#if 0
  if ( is_valid() ) {
    mPtr->inc();
  }
#endif
}

// @brief コピー代入演算子
ZddMgrPtr&
ZddMgrPtr::operator=(
  const ZddMgrPtr& src
)
{
#if 0
  if ( src.is_valid() ) {
    src.mPtr->inc();
  }
  if ( is_valid() ) {
    mPtr->dec();
  }
#endif
  mPtr = src.mPtr;
  return *this;
}

// @brief デストラクタ
ZddMgrPtr::~ZddMgrPtr()
{
#if 0
  if ( is_valid() ) {
    mPtr->dec();
  }
#endif
}

// @brief 親のマネージャを返す．
ZddMgr
ZddMgrPtr::mgr() const
{
  return ZddMgr{get()};
}

END_NAMESPACE_YM_DD
