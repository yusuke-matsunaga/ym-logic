
/// @file Zdd_cofactor.cc
/// @brief Zdd::cofactor の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Zdd.h"
#include "ZddEdge.h"
#include "ZddMgrImpl.h"
#include "CofactorOp.h"


BEGIN_NAMESPACE_YM_ZDD

// @brief onset を計算する．
Zdd
Zdd::onset(
  SizeType var
) const
{
  _check_valid();

  CofactorOp op{mMgr, var, true};
  auto e = op.op_step(ZddEdge{mRoot});
  return Zdd{mMgr, e};
}

// @brief offset を計算する．
Zdd
Zdd::offset(
  SizeType var
) const
{
  _check_valid();

  CofactorOp op{mMgr, var, false};
  auto e = op.op_step(ZddEdge{mRoot});
  return Zdd{mMgr, e};
}

// @brief onset を計算して代入する．
Zdd&
Zdd::onset_int(
  SizeType var
)
{
  _check_valid();

  CofactorOp op{mMgr, var, true};
  auto e = op.op_step(ZddEdge{mRoot});
  change_root(e);
  return *this;
}

// @brief offset を計算して代入する．
Zdd&
Zdd::offset_int(
  SizeType var
)
{
  _check_valid();

  CofactorOp op{mMgr, var, false};
  auto e = op.op_step(ZddEdge{mRoot});
  change_root(e);
  return *this;
}


//////////////////////////////////////////////////////////////////////
// クラス CofactorOp
//////////////////////////////////////////////////////////////////////

// @brief コファクターを計算する．
ZddEdge
CofactorOp::op_step(
  ZddEdge edge
)
{
  // 終端ならそのまま返す．
  if ( edge.is_const() ) {
    return edge;
  }

  if ( mTable.count(edge) > 0 ) {
    return mTable.at(edge);
  }

  auto node = edge.node();
  auto index = node->index();
  auto edge0 = node->edge0();
  auto edge1 = node->edge1();

  ZddEdge result;
  if ( index == mIndex ) {
    if ( mPhase ) {
      result = op_step(edge1);
    }
    else {
      result = op_step(edge0);
    }
  }
  else if ( index < mIndex ) {
    auto ans0 = op_step(edge0);
    auto ans1 = op_step(edge1);
    result = new_node(index, ans0, ans1);;
  }
  else { // index > mIndex
    result = edge;
  }
  mTable.emplace(edge, result);
  return result;
}

END_NAMESPACE_YM_ZDD
