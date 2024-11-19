
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
#include "dd/DdEdge.h"
#include "dd/DdInfo.h"
#include "dd/DdInfoMgr.h"
#include "DotGen.h"
#include "DotWriter.h"


BEGIN_NAMESPACE_YM_DD

// @brief 複数のBDDを dot 形式で出力する．
void
BddMgrImpl::gen_dot(
  ostream& s,
  const vector<Bdd>& bdd_list,
  const unordered_map<string, string>& attr_dict
)
{
  DotGen dg{attr_dict};
  auto info_mgr = node_info(bdd_list);
  dg.write(s, info_mgr);
}

// @brief 複数のBDDを dot 形式で出力する．
void
BddMgrImpl::gen_dot(
  ostream& s,
  const vector<Bdd>& bdd_list,
  const JsonValue& attr
)
{
  DotGen dg{attr};
  auto info_mgr = node_info(bdd_list);
  dg.write(s, info_mgr);
}

// @brief 複数のZDDを dot 形式で出力する．
void
ZddMgrImpl::gen_dot(
  ostream& s,
  const vector<Zdd>& zdd_list,
  const unordered_map<string, string>& attr_dict
)
{
  DotGen dg{attr_dict};
  auto info_mgr = node_info(zdd_list);
  dg.write(s, info_mgr);
}

// @brief 複数のZDDを dot 形式で出力する．
void
ZddMgrImpl::gen_dot(
  ostream& s,
  const vector<Zdd>& zdd_list,
  const JsonValue& attr
)
{
  DotGen dg{attr};
  auto info_mgr = node_info(zdd_list);
  dg.write(s, info_mgr);
}


//////////////////////////////////////////////////////////////////////
// クラス DotGen
//////////////////////////////////////////////////////////////////////

BEGIN_NONAMESPACE

std::unordered_map<string, string>
json_to_dict(
  const JsonValue& attr
)
{
  std::unordered_map<string, string> attr_dict;
  if ( attr.is_object() ) {
    for ( auto& p: attr.item_list() ) {
      auto key = p.first;
      auto val = p.second;
      if ( val.is_string() ) {
	attr_dict.emplace(key, val.get_string());
      }
      else {
	throw std::invalid_argument{"value should be a string"};
      }
    }
  }
  else if ( !attr.is_null() ) {
    throw std::invalid_argument{"attr should be a JSON object"};
  }
  return attr_dict;
}

END_NONAMESPACE

// @brief コンストラクタ
DotGen::DotGen(
  const JsonValue& attr
) : DotGen{json_to_dict(attr)}
{
}

// @brief コンストラクタ
DotGen::DotGen(
  const unordered_map<string, string>& attr_dict
)
{
  // デフォルト値の設定
  mGraphAttrList.emplace("rankdir", "TB");
  mGraphAttrList.emplace("bgcolor", "beige");
  mRootAttrList.emplace("shape", "box");
  mTerminal0AttrList.emplace("shape", "box");
  mTerminal0AttrList.emplace("style", "filled");
  mTerminal0AttrList.emplace("label", "\"0\"");
  mTerminal0AttrList.emplace("color", "mediumpurple");
  mTerminal1AttrList.emplace("shape", "box");
  mTerminal1AttrList.emplace("style", "filled");
  mTerminal1AttrList.emplace("label", "\"1\"");
  mTerminal1AttrList.emplace("color", "lightsalmon");
  mEdge0AttrList.emplace("style", "dotted");
  mEdge0AttrList.emplace("color", "blue");
  mEdge1AttrList.emplace("style", "solid");
  mEdge1AttrList.emplace("color", "red");
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
  ostream& s,
  const DdInfoMgr& info_mgr
)
{
  DotWriter writer{s};

  // dot の開始
  writer.graph_begin("bdd", mGraphAttrList);

  // 根のノードの定義
  SizeType i = 1;
  for ( auto edge: info_mgr.root_list() ) {
    auto attr_list{mRootAttrList};
    ostringstream buf;
    buf << "\"BDD#" << i << "\"";
    attr_list.emplace("label", buf.str());
    writer.write_node(root_name(i), attr_list);
    ++ i;
  }

  // ノードの定義
  {
    SizeType id = 1;
    for ( auto node: info_mgr.node_list() ) {
      auto attr_list{mNodeAttrList};
      ostringstream buf;
      buf << "\"" << node.level() << "\"";
      attr_list.emplace("label", buf.str());
      writer.write_node(node_name(id), attr_list);
      ++ id;
    }
  }

  // 終端の定義
  writer.write_node("const0", mTerminal0AttrList);
  writer.write_node("const1", mTerminal1AttrList);

  // 根の枝の定義
  i = 1;
  for ( auto edge: info_mgr.root_list() ) {
    write_edge(writer, root_name(i), edge, false);
    ++ i;
  }

  // 枝の定義
  {
    SizeType id = 1;
    for ( auto node: info_mgr.node_list() ) {
      write_edge(writer, node_name(id), node.edge0(), true);
      write_edge(writer, node_name(id), node.edge1(), false);
      ++ id;
    }
  }

  // 根のランクの設定
  {
    SizeType n = info_mgr.root_list().size();
    vector<string> node_list(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      node_list[i] = root_name(i + 1);
    }
    writer.write_rank_group(node_list, "min");
  }

  // ノードのランクの設定
  for ( SizeType level = 0; level < info_mgr.max_level(); ++ level ) {
    auto& node_list = info_mgr.id_list(level);
    SizeType n = node_list.size();
    vector<string> node_name_list;
    node_name_list.reserve(n);
    for ( auto id: node_list ) {
      node_name_list.push_back(node_name(id));
    }
    writer.write_rank_group(node_name_list);
  }

  // 終端ノードのランクの設定
  vector<string> terminal_nodes{"const0", "const1"};
  writer.write_rank_group(terminal_nodes, "max");

  writer.graph_end();
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
  DotWriter& writer,
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

  writer.write_edge(from_node, to_node, attr_list);
}

END_NAMESPACE_YM_DD
