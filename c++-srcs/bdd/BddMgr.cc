
/// @file BddMgr.cc
/// @brief BddMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BddMgr.h"
#include "ym/Bdd.h"
#include "ym/BddError.h"
#include "BddMgrImpl.h"
#include "TruthOp.h"


BEGIN_NAMESPACE_YM_BDD

// @brief コンストラクタ
BddMgr::BddMgr(
) : mImpl{new BddMgrImpl}
{
}

// @brief デストラクタ
BddMgr::~BddMgr()
{
  delete mImpl;
}

// @brief 恒儀関数を作る．
Bdd
BddMgr::zero()
{
  return Bdd{mImpl, BddEdge::zero()};
}

// @brief 恒新関数を作る．
Bdd
BddMgr::one()
{
  return Bdd{mImpl, BddEdge::one()};
}

// @brief リテラル関数を作る．
Bdd
BddMgr::literal(
  VarId var,
  bool inv
)
{
  auto e = mImpl->make_literal(var.val()) *  inv;
  return Bdd{mImpl, e};
}

// @brief 肯定のリテラル関数を作る．
Bdd
BddMgr::posi_literal(
  VarId var
)
{
  return literal(var, false);
}

// @brief 否定のリテラル関数を作る．
Bdd
BddMgr::nega_literal(
  VarId var
)
{
  return literal(var, true);
}

// @brief 真理値表形式の文字列からBDDを作る．
Bdd
BddMgr::from_truth(
  const string& str
)
{
  TruthOp op{mImpl};
  auto e = op.op_step(str, 0);
  return Bdd{mImpl, e};
}

// @brief ノード数を返す．
SizeType
BddMgr::node_num() const
{
  return mImpl->node_num();
}

// @brief GC を起動するしきい値を返す．
SizeType
BddMgr::gc_limit() const
{
  return mImpl->gc_limit();
}

// @brief GC を起動するしきい値を設定する．
void
BddMgr::set_gc_limit(
  SizeType limit
)
{
  mImpl->set_gc_limit(limit);
}

// @brief GC を許可する．
void
BddMgr::enable_gc()
{
  mImpl->enable_gc();
}

// @brief GC を禁止する．
void
BddMgr::disable_gc()
{
  mImpl->disable_gc();
}

END_NAMESPACE_YM_BDD
