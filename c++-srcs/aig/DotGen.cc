
/// @file DotGen.cc
/// @brief DotGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/aig.h"
#include "AigMgrImpl.h"
#include "AigEdge.h"
#include "DotGen.h"
#include "common/DotWriter.h"


BEGIN_NAMESPACE_YM_DD

// @brief 複数のAIGを dot 形式で出力する．
void
AigMgrImpl::gen_dot(
  ostream& s,
  const vector<AigEdge>& root_list,
  const JsonValue& option
)
{
  DotGen dg{option};
  auto info_mgr = node_info(bdd_list);
  dg.write(s, info_mgr);
}

// @brief 複数のZDDを dot 形式で出力する．
void
ZddMgrImpl::gen_dot(
  ostream& s,
  const vector<Zdd>& zdd_list,
  const JsonValue& option
)
{
  DotGen dg{option};
  auto info_mgr = node_info(zdd_list);
  dg.write(s, info_mgr);
}


//////////////////////////////////////////////////////////////////////
// クラス DotGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DotGen::DotGen(
  const JsonValue& option
)
{
  // デフォルト値の設定
  mGraphAttrList.emplace("rankdir", "TB");
  mGraphAttrList.emplace("bgcolor", "beige");
  mRootAttrList.emplace("shape", "box");
  mNodeAttrList.emplace("shape", "circle");
  mTerminalAttrList.emplace("shape", "box");
  mTerminalAttrList.emplace("style", "filled");
  mTerminal0AttrList.emplace("color", "mediumpurple");
  mNormalEdgeAttrList.emplace("style", "dashed");
  mNoamrlEdgeAttrList.emplace("color", "blue");
  mInvEdgeAttrList.emplace("style", "solid");
  mInvEdgeAttrList.emplace("color", "red");
  // オプションの解析
  if ( option.is_object() ) {
    if ( option.has_key("attr") ) {
      auto attr_obj = option.at("attr");
      // 属性値の設定
      set_attr(attr_obj);
    }
    if ( option.has_key("var_label") ) {
      auto obj = option.at("var_label");
      set_label("var_label", obj, mLabelDict);
    }
    if ( option.has_key("var_texlbl") ) {
      auto obj = option.at("var_texlbl");
      set_label("var_texlbl", obj, mTexLblDict);
    }
  }
}

void
DotGen::set_attr(
  const JsonValue& attr_json
)
{
  if ( attr_json.is_null() ) {
    return;
  }
  if ( !attr_json.is_object() ) {
    throw std::invalid_argument{"attr should be a JSON object"};
  }
  for ( auto& p: attr_json.item_list() ) {
    auto attr_name = p.first;
    auto val_json = p.second;
    if ( !val_json.is_string() ) {
      throw std::invalid_argument{"value should be a string"};
    }
    auto attr_val = val_json.get_string();
    auto pos = attr_name.find_first_of(":");
    if ( pos == string::npos ) {
      // 全てのタイプに指定する．
      mRootAttrList.emplace(attr_name, attr_val);
      mNodeAttrList.emplace(attr_name, attr_val);
      mTerminalAttrList.emplace(attr_name, attr_val);
      mNormalEdgeAttrList.emplace(attr_name, attr_val);
      mNormalEdgeAttrList.emplace(attr_name, attr_val);
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
      else if ( type == "edge" ) {
	mEdgeAttrList.emplace(attr_name1, attr_val);
      }
      else {
	ostringstream buf;
	buf << type << ": Unknown group name";
	throw std::invalid_argument{buf.str()};
      }
    }
  }
}

// @brief 変数ラベルをセットする．
void
DotGen::set_label(
  const string& name,
  const JsonValue& label_array,
  std::unordered_map<SizeType, string>& label_dict
)
{
  if ( !label_array.is_array() ) {
    ostringstream buf;
    buf << "'" << name << "' should be a JSON array";
    throw std::invalid_argument{buf.str()};
  }
  auto n = label_array.size();
  for ( SizeType i = 0; i < n; ++ i ) {
    auto obj = label_array.at(i);
    if ( !obj.is_string() ) {
      ostringstream buf;
      buf << "value of '" << name << "' should be a string";
      throw std::invalid_argument{buf.str()};
    }
    auto label = obj.get_string();
    label_dict.emplace(i, label);
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
  writer.graph_begin("aig", mGraphAttrList);

  // 根のノードの定義
  SizeType i = 1;
  for ( auto edge: info_mgr.root_list() ) {
    auto attr_list = mRootAttrList;
    ostringstream buf;
    buf << "\"AIG#" << i << "\"";
    attr_list.emplace("label", buf.str());
    writer.write_node(root_name(i), attr_list);
    ++ i;
  }

  // ノードの定義
  {
    SizeType id = 1;
    for ( auto node: info_mgr.node_list() ) {
      auto attr_list = mNodeAttrList;
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
