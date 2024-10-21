
/// @file DotGen.cc
/// @brief DotGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "ym/Zdd.h"
#include "dd/BddMgrImpl.h"
#include "dd/ZddMgrImpl.h"
#include "DotGen.h"
#include "dd/DdEdge.h"
#include "dd/DdInfo.h"


BEGIN_NAMESPACE_YM_DD

// @brief 複数のBDDを dot 形式で出力する．
void
BddMgrImpl::gen_dot(
  ostream& s,
  const vector<Bdd>& bdd_list,
  const unordered_map<string, string>& attr_dict
)
{
  DotGen dg{s, attr_dict};
  vector<SizeType> redge_list;
  auto node_list = node_info(bdd_list, redge_list);
  dg.write(node_list, redge_list);
}

// @brief 複数のZDDを dot 形式で出力する．
void
ZddMgrImpl::gen_dot(
  ostream& s,
  const vector<Zdd>& zdd_list,
  const unordered_map<string, string>& attr_dict
)
{
  DotGen dg{s, attr_dict};
  vector<SizeType> redge_list;
  auto node_list = node_info(zdd_list, redge_list);
  dg.write(node_list, redge_list);
}


//////////////////////////////////////////////////////////////////////
// クラス DotGen
//////////////////////////////////////////////////////////////////////

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
  const vector<DdInfo>& node_list,
  const vector<SizeType>& redge_list
)
{
  // dot の開始
  mWriter.graph_begin("bdd", mGraphAttrList);

  // 根のノードの定義
  SizeType i = 1;
  for ( auto edge: redge_list ) {
    auto attr_list{mRootAttrList};
    ostringstream buf;
    buf << "\"BDD#" << i << "\"";
    attr_list.emplace("label", buf.str());
    mWriter.write_node(root_name(i), attr_list);
    ++ i;
  }

  // ノードの定義
  {
    SizeType id = 1;
    for ( auto node: node_list ) {
      auto attr_list{mNodeAttrList};
      ostringstream buf;
      buf << "\"" << node.index() << "\"";
      attr_list.emplace("label", buf.str());
      mWriter.write_node(node_name(id), attr_list);
      ++ id;
    }
  }

  // 終端の定義
  mWriter.write_node("const0", mTerminal0AttrList);
  mWriter.write_node("const1", mTerminal1AttrList);

  // 根の枝の定義
  i = 1;
  for ( auto edge: redge_list ) {
    write_edge(root_name(i), edge, false);
    ++ i;
  }

  // 枝の定義
  {
    SizeType id = 1;
    for ( auto node: node_list ) {
      write_edge(node_name(id), node.edge0(), true);
      write_edge(node_name(id), node.edge1(), false);
      ++ id;
    }
  }

  // 根のランクの設定
  {
    SizeType n = redge_list.size();
    vector<string> node_list(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      node_list[i] = root_name(i + 1);
    }
    mWriter.write_rank_group(node_list);
  }

  // ノードのランクの設定
  SizeType max_index = 0;
  for ( auto node: node_list ) {
    SizeType index = node.index();
    if ( max_index < index ) {
      max_index = index;
    }
  }
  ++ max_index;
  vector<vector<SizeType>> indexed_node_list(max_index);
  {
    SizeType id = 1;
    for ( auto node: node_list ) {
      SizeType index = node.index();
      indexed_node_list[index].push_back(id);
      ++ id;
    }
  }
  for ( SizeType i = 0; i < max_index; ++ i ) {
    auto& node_list = indexed_node_list[i];
    SizeType n = node_list.size();
    vector<string> node_name_list;
    node_name_list.reserve(n);
    for ( auto id: node_list ) {
      node_name_list.push_back(node_name(id));
    }
    mWriter.write_rank_group(node_name_list);
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
  SizeType id
)
{
  ostringstream buf;
  buf << "node" << id;
  return buf.str();
}

// @brief 枝の内容を出力する．
void
DotGen::write_edge(
  const string& from_node,
  SizeType edge,
  bool zero
)
{
  string to_node;
  bool inv{false};
  if ( edge == 0 ) {
    to_node = "const0";
  }
  else if ( edge == 1 ) {
    to_node = "const1";
  }
  else {
    inv = static_cast<bool>(edge & 1);
    to_node = node_name(edge >> 1);
  }

  auto attr_list{zero ? mEdge0AttrList : mEdge1AttrList};
  if ( inv ) {
    attr_list.emplace("dir", "both");
    attr_list.emplace("arrowtail", "odot");
  }

  mWriter.write_edge(from_node, to_node, attr_list);
}

END_NAMESPACE_YM_DD
