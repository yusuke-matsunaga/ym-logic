
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
) : mPtr{ptr}
{
  cout << "  BddMgrPtr::constr1" << endl;
  if ( is_valid() ) {
    mPtr->inc();
  }
}

// @brief コピーコンストラクタ
BddMgrPtr::BddMgrPtr(
  const BddMgrPtr& src
) : mPtr{src.mPtr}
{
  cout << "  BddMgrPtr::copy_constr" << endl;
  if ( is_valid() ) {
    mPtr->inc();
  }
}

// @brief コピー代入演算子
BddMgrPtr&
BddMgrPtr::operator=(
  const BddMgrPtr& src
)
{
  cout << "  BddMgrPtr::copy_assign" << endl;
  if ( src.is_valid() ) {
    src.mPtr->inc();
  }
  if ( is_valid() ) {
    mPtr->dec();
  }
  mPtr = src.mPtr;

  return *this;
}

// @brief ムーブコンストラクタ
BddMgrPtr::BddMgrPtr(
  BddMgrPtr&& src
) : mPtr{std::move(src.mPtr)}
{
  cout << "  BddMgrPtr::move_constr" << endl;
}

// @brief デストラクタ
BddMgrPtr::~BddMgrPtr()
{
  cout << "  BddMgrPtr::destr" << endl;
  if ( is_valid() ) {
    mPtr->dec();
  }
}

// @brief 親のマネージャを返す．
BddMgr
BddMgrPtr::mgr() const
{
  return BddMgr{*this};
}

END_NAMESPACE_YM_DD
