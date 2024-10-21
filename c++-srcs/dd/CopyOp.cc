
/// @file CopyOp.cc
/// @brief CopyOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "CopyOp.h"
#include "dd/DdEdge.h"
#include "dd/DdNode.h"
#include "dd/DdNodeMgr.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
// クラス CopyOp
//////////////////////////////////////////////////////////////////////

// @brief コピーする．
DdEdge
CopyOp::copy_step(
  DdEdge edge
)
{
  if ( edge.is_const() ) {
    return edge;
  }

  auto node = edge.node();
  auto inv = edge.inv();
  auto index = node->index();
  if ( mTable.count(node) > 0 ) {
    auto rnode = mTable.at(node);
    return DdEdge{rnode, inv};
  }
  auto edge0 = node->edge0();
  auto edge1 = node->edge1();
  auto redge0 = copy_step(edge0);
  auto redge1 = copy_step(edge1);
  auto rnode = mNodeMgr.new_node(index, redge0, redge1);
  mTable.emplace(node, rnode);
  return DdEdge{rnode, inv};
}

END_NAMESPACE_YM_DD
