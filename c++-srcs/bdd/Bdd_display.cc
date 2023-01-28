
/// @file Bdd_display.cc
/// @brief Bdd::display の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "ym/BddInfo.h"


BEGIN_NAMESPACE_YM_BDD

BEGIN_NONAMESPACE

// @brief 枝の内容を出力する．
void
write_edge(
  ostream& s,
  SizeType edge
)
{
  if ( edge == 0 ) {
    s << "   ZERO";
  }
  else if ( edge == 1 ) {
    s << "    ONE";
  }
  else {
    SizeType node = edge >> 1;
    bool inv = static_cast<bool>(edge & 1);
    s << setw(6) << node;
    if ( inv ) {
      s << "~";
    }
    else {
      s << " ";
    }
  }
}

// @brief ノードの内容を出力する．
void
write_node_list(
  ostream& s,
  const vector<BddInfo>& node_list
)
{
  SizeType id = 1;
  for ( auto& node: node_list ) {
    s << setw(6) << id << ": "
      << setw(4) << node.index;
    write_edge(s, node.edge0);
    s << ": ";
    write_edge(s, node.edge1);
    s << endl;
    ++ id;
  }
}

END_NONAMESPACE

// @brief 内容を出力する．
void
Bdd::display(
  ostream& s
) const
{
  if ( mMgr == nullptr ) {
    s << "Invalid BDD" << endl;
  }
  else {
    SizeType root_edge;
    auto node_list = node_info(root_edge);
    s << "Root: ";
    write_edge(s, root_edge);
    s << endl;
    write_node_list(s, node_list);
  }
}

// @brief 複数のBDDの内容を出力する．
void
Bdd::display(
  ostream& s,
  const vector<Bdd>& bdd_list
)
{
  vector<SizeType> root_edge_list;
  auto node_list = node_info(bdd_list, root_edge_list);
  for ( auto root: root_edge_list ) {
    write_edge(s, root);
  }
  s << endl;
  write_node_list(s, node_list);
}

END_NAMESPACE_YM_BDD
