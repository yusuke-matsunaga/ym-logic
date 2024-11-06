
/// @file BddObj.cc
/// @brief BddObj の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BddObj.h"
#include "ym/BddMgr.h"
#include "dd/BddMgrImpl.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
// クラス BddObj
//////////////////////////////////////////////////////////////////////

// @brief マネージャを指定したコンストラクタ
BddObj::BddObj(
  BddMgrImpl* mgr
) : mMgr{mgr}
{
  if ( mMgr != nullptr ) {
    mMgr->inc();
  }
}

// @brief コピーコンストラクタ
BddObj::BddObj(
  const BddObj& src
) : BddObj{src.mMgr}
{
}

// @brief デストラクタ
BddObj::~BddObj()
{
  if ( mMgr != nullptr ) {
    mMgr->dec();
  }
}

// @brief 親のマネージャを返す．
BddMgr
BddObj::mgr() const
{
  return BddMgr{mMgr};
}

// @brief マネージャをセットする．
void
BddObj::set_mgr(
  BddMgrImpl* mgr
)
{
  if ( mgr != nullptr ) {
    mgr->inc();
  }
  if ( is_valid() ) {
    mMgr->dec();
  }
  mMgr = mgr;
}


//////////////////////////////////////////////////////////////////////
// クラス BddVar
//////////////////////////////////////////////////////////////////////

// @brief 肯定のリテラルを返す．
BddLit
BddVar::to_posilit() const
{
  return BddLit{*this, false};
}

// @brief 否定のリテラルを返す．
BddLit
BddVar::to_negalit() const
{
  return BddLit{*this, true};
}


//////////////////////////////////////////////////////////////////////
// クラス BddLit
//////////////////////////////////////////////////////////////////////

// @brief BddVar と極性を引数にとるコンストラクタ
BddLit::BddLit(
  const BddVar& var,
  bool inv
) : BddLit{var._mgr(), (var.mVarId << 1) | static_cast<SizeType>(inv)}
{
}

// @brief 変数を取り出す．
BddVar
BddLit::var() const
{
  return BddVar{_mgr(), mVal >> 1};
}

END_NAMESPACE_YM_DD
