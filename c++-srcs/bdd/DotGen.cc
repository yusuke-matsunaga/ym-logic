
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

// @brief コンストラクタ
DotGen::DotGen(
  ostream& s,
  const unordered_map<string, string>& attr_dict
) : mWriter{s}
{
  // デフォルト値の設定
  mGraphAttrList.emplace("rankdir", "TB");
  mRootAttrList.emplace("shape", "box");
  mTerminal0AttrList.emplace("shape", "box");
  mTerminal0AttrList.emplace("label", "\"0\"");
  mTerminal1AttrList.emplace("shape", "box");
  mTerminal1AttrList.emplace("label", "\"1\"");
  mEdge0AttrList.emplace("style", "dotted");
  for ( auto& p: attr_dict ) {
    auto attr_name = p.first;
    auto attr_val = p.second;
    auto pos = attr_name.find_first_of(":");
    if ( pos == string::npos ) {
      // 全てのタイプに指定する．
      mRootAttrList.emplace(attr_name, attr_val);
      mNodeAttrList.emplace(attr_name, attr_val);
      mTerminal0AttrList.emplace(attr_name, attr_val);
      mTerminal1AttrList.emplace(attr_name, attr_val);
      mEdge0AttrList.emplace(attr_name, attr_val);
      mEdge1AttrList.emplace(attr_name, attr_val);
    }
    else {
      auto type = attr_name.substr(0, pos);
      auto attr_name1 = attr_name.substr(pos + 1);
      if ( type == "graph" ) {
	mGraphAttrList.emplace(attr_name1, attr_val);
      }
      else if ( type == "root" ) {
	mRootAttrList.emplace(attr_name1, attr_val);
      }
      else if ( type == "node" ) {
	mNodeAttrList.emplace(attr_name1, attr_val);
      }
      else if ( type == "terminal" ) {
	mTerminal0AttrList.emplace(attr_name1, attr_val);
	mTerminal1AttrList.emplace(attr_name1, attr_val);
      }
      else if ( type == "terminal0" ) {
	mTerminal0AttrList.emplace(attr_name1, attr_val);
      }
      else if ( type == "terminal1" ) {
	mTerminal1AttrList.emplace(attr_name1, attr_val);
      }
      else if ( type == "edge" ) {
	mEdge0AttrList.emplace(attr_name1, attr_val);
	mEdge1AttrList.emplace(attr_name1, attr_val);
      }
      else if ( type == "edge0" ) {
	mEdge0AttrList.emplace(attr_name1, attr_val);
      }
      else if ( type == "edge1" ) {
	mEdge1AttrList.emplace(attr_name1, attr_val);
      }
      else {
	// type error
      }
    }
  }
}

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
  mWriter.graph_begin("bdd", mGraphAttrList);

  // 根のノードの定義
  SizeType i = 1;
  for ( auto edge: root_list ) {
    auto attr_list{mRootAttrList};
    ostringstream buf;
    buf << "\"BDD#" << i << "\"";
    attr_list.emplace("label", buf.str());
    mWriter.write_node(root_name(i), attr_list);
    ++ i;
  }

  // ノードの定義
  for ( auto node: node_list() ) {
    auto attr_list{mNodeAttrList};
    ostringstream buf;
    buf << "\"" << node->index() << "\"";
    attr_list.emplace("label", buf.str());
    mWriter.write_node(node_name(node), attr_list);
  }

  // 終端の定義
  mWriter.write_node("const0", mTerminal0AttrList);
  mWriter.write_node("const1", mTerminal1AttrList);

  // 根の枝の定義
  i = 1;
  for ( auto edge: root_list ) {
    write_edge(root_name(i), edge, false);
    ++ i;
  }

  // 枝の定義
  for ( auto node: node_list() ) {
    write_edge(node_name(node), node->edge0(), true);
    write_edge(node_name(node), node->edge1(), false);
  }

  // 根のランクの設定
  {
    SizeType n = root_list.size();
    vector<string> node_list(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      node_list[i] = root_name(i + 1);
    }
    mWriter.write_rank_group(node_list);
  }

  // ノードのランクの設定
  for ( SizeType i = 0; i < max_index(); ++ i ) {
    SizeType n = indexed_node_list(i).size();
    vector<string> node_list;
    node_list.reserve(n);
    for ( auto node: indexed_node_list(i) ) {
      node_list.push_back(node_name(node));
    }
    mWriter.write_rank_group(node_list);
  }

  // 終端ノードのランクの設定
  vector<string> terminal_nodes{"const0", "const1"};
  mWriter.write_rank_group(terminal_nodes);

  mWriter.graph_end();
}

// @brief ルート名を返す．
string
DotGen::root_name(
  SizeType i
)
{
  ostringstream buf;
  buf << "root" << i;
  return buf.str();
}

// @brief ノード名を返す．
string
DotGen::node_name(
  BddNode* node
)
{
  ostringstream buf;
  buf << "node" << node_id(node);
  return buf.str();
}

// @brief 枝の内容を出力する．
void
DotGen::write_edge(
  const string& from_node,
  BddEdge edge,
  bool zero
)
{
  string to_node;
  bool inv{false};
  if ( edge.is_zero() ) {
    to_node = "const0";
  }
  else if ( edge.is_one() ) {
    to_node = "const1";
  }
  else {
    inv = edge.inv();
    auto node = edge.node();
    to_node = node_name(node);
  }

  auto attr_list{zero ? mEdge0AttrList : mEdge1AttrList};
  if ( inv ) {
    attr_list.emplace("dir", "both");
    attr_list.emplace("arrowtail", "odot");
  }

  mWriter.write_edge(from_node, to_node, attr_list);
}

END_NAMESPACE_YM_BDD
