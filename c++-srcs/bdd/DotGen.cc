
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
) : mS{s}
{
  // デフォルト値の設定
  mGraphAttrList.emplace("rankdir", "TB");
  mRootAttrList.emplace("shape", "box");
  mTerminalAttrList.emplace("shape", "box");
  mTerminal0AttrList.emplace("label", "\"0\"");
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
      mTerminalAttrList.emplace(attr_name, attr_val);
      mEdgeAttrList.emplace(attr_name, attr_val);
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
	mTerminalAttrList.emplace(attr_name1, attr_val);
      }
      else if ( type == "terminal0" ) {
	mTerminal0AttrList.emplace(attr_name1, attr_val);
      }
      else if ( type == "terminal1" ) {
	mTerminal1AttrList.emplace(attr_name1, attr_val);
      }
      else if ( type == "edge" ) {
	mEdgeAttrList.emplace(attr_name1, attr_val);
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
  mS << "digraph bdd {" << endl
     << "  graph";
  attr_begin();
  attr_add(mGraphAttrList);
  attr_end();

  // 根のノードの定義
  SizeType i = 1;
  for ( auto edge: root_list ) {
    mS << "  "
       << root_name(i);
    attr_begin();
    attr_add(mRootAttrList);
    ostringstream buf;
    buf << "\"BDD#" << i << "\"";
    attr_add("label", buf.str());
    attr_end();
    ++ i;
  }

  // ノードの定義
  for ( auto node: node_list() ) {
    mS << "  "
       << node_name(node);
    attr_begin();
    ostringstream buf;
    buf << "\"" << node->index() << "\"";
    attr_add("label", buf.str());
    attr_add(mNodeAttrList);
    attr_end();
  }

  // 終端の定義
  write_terminal(false);
  write_terminal(true);

  // 根の枝の定義
  i = 1;
  for ( auto edge: root_list ) {
    mS << "  " << root_name(i);
    write_edge(edge, false);
    ++ i;
  }

  // 枝の定義
  for ( auto node: node_list() ) {
    mS << "  " << node_name(node);
    write_edge(node->edge0(), true);
    mS << "  " << node_name(node);
    write_edge(node->edge1(), false);
  }

  // 根のランクの設定
  mS << "  { rank = same;";
  for ( SizeType i = 1; i <= root_list.size(); ++ i ) {
    mS << " " << root_name(i) << ";";
  }
  mS << "}" << endl;

  // ランクの設定
  for ( SizeType i = 0; i < max_index(); ++ i ) {
    mS << "  { rank = same;";
    for ( auto node: indexed_node_list(i) ) {
      mS << " " << node_name(node) << ";";
    }
    mS << "}" << endl;
  }

  // dot の終了
  mS << "}" << endl;
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

// @brief 終端ノードの内容を出力する．
void
DotGen::write_terminal(
  bool one
)
{
  mS << "  " << "const";
  if ( one ) {
    mS << "1";
  }
  else {
    mS << "0";
  }

  attr_begin();
  attr_add(mTerminalAttrList);
  if ( one ) {
    attr_add(mTerminal1AttrList);
  }
  else {
    attr_add(mTerminal0AttrList);
  }
  attr_end();
}

// @brief 枝の内容を出力する．
void
DotGen::write_edge(
  BddEdge edge,
  bool zero
)
{
  mS << " -> ";
  bool inv{false};
  if ( edge.is_zero() ) {
    mS << "const0";
  }
  else if ( edge.is_one() ) {
    mS << "const1";
  }
  else {
    inv = edge.inv();
    auto node = edge.node();
    mS << node_name(node);
  }

  attr_begin();
  attr_add(mEdgeAttrList);
  if ( zero ) {
    attr_add(mEdge0AttrList);
  }
  else {
    attr_add(mEdge1AttrList);
  }
  if ( inv ) {
    attr_add("dir", "both");
    attr_add("arrowtail", "odot");
  }
  attr_end();
}

// @breif 属性出力を開始する．
void
DotGen::attr_begin()
{
  mAttrStr = " [";
}

// @brief 属性リストの内容を追加する．
void
DotGen::attr_add(
  const unordered_map<string, string>& attr_list
)
{
  for ( auto& p: attr_list ) {
    auto attr_name = p.first;
    auto attr_val = p.second;
    attr_add(attr_name, attr_val);
  }
}

// @brief 属性を追加する．
void
DotGen::attr_add(
  const string& attr_name,
  const string& attr_val
)
{
  if ( attr_val == string{} ) {
    return;
  }

  mS << mAttrStr;
  mAttrStr = ", ";
  mS << attr_name
     << " = "
     << attr_val;
}

// @brief 属性出力を終了する．
void
DotGen::attr_end()
{
  if ( mAttrStr == ", " ) {
    mS << "]";
  }
  mS << endl;
}

END_NAMESPACE_YM_BDD
