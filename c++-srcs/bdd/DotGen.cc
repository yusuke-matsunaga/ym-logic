
/// @file DotGen.cc
/// @brief DotGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "DotGen.h"
#include "BddEdge.h"
#include "BddNode.h"


BEGIN_NAMESPACE_YM_BDD

// @brief dot 形式で出力する．
void
DotGen::write(
  const vector<BddEdge>& root_list
)
{
  // ノードリストを作る．
  for ( auto edge: root_list ) {
    get_node(edge);
  }

  // dot の開始
  mS << "digraph bdd {" << endl
     << "  graph [rankdir = TB];" << endl;

  // 根のノードの定義
  SizeType i = 1;
  for ( auto edge: root_list ) {
    mS << "  root" << i
      << " [shape=box,label=\"BDD#"
      << i << "\"];" << endl;
    ++ i;
  }

  // ノードの定義
  SizeType max_index = 0;
  for ( auto node: node_list() ) {
    auto id = node_id(node);
    mS << "  node" << id
      << " [label=\""
      << node->index()
      << "\"];" << endl;
    if ( max_index < node->index() ) {
      max_index = node->index();
    }
  }

  // 終端の定義
  mS << "  const0 [shape=box,label=\"0\"];" << endl;
  mS << "  const1 [shape=box,label=\"1\"];" << endl;

  // 根の枝の定義
  i = 1;
  for ( auto edge: root_list ) {
    mS << "  root" << i << " -> ";
    write_edge(edge, false);
    ++ i;
  }

  // 枝の定義
  vector<vector<SizeType>> rank_node_list(max_index + 1);
  for ( auto node: node_list() ) {
    auto id = node_id(node);
    mS << "  node" << id << " -> ";
    write_edge(node->edge0(), true);
    mS << "  node" << id << " -> ";
    write_edge(node->edge1(), false);
    rank_node_list[node->index()].push_back(id);
  }

  // ランクの設定
  for ( auto& node_list: rank_node_list ) {
    mS << "  { rank = same;";
    for ( auto id: node_list ) {
      mS << " node" << id << ";";
    }
    mS << "}" << endl;
  }

  // dot の終了
  mS << "}" << endl;
}

// @brief 枝の内容を出力する．
void
DotGen::write_edge(
  BddEdge edge,
  bool zero
)
{
  if ( edge.is_zero() ) {
    mS << "const0";
  }
  else if ( edge.is_one() ) {
    mS << "const1";
  }
  else {
    auto node = edge.node();
    auto inv = edge.inv();
    auto id = node_id(node);
    mS << "node" << id
      << "[";
    const char* comma = "";
    if ( zero ) {
      mS << "style=dotted";
      comma = ",";
    }
    if ( inv ) {
      mS << comma << "dir=both,arrowtail=odot";
      comma = ",";
    }
    mS << "]";
  }
  mS << ";" << endl;
}

END_NAMESPACE_YM_BDD
