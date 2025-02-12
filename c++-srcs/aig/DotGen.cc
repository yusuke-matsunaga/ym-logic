
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


BEGIN_NAMESPACE_YM_AIG

// @brief 複数のAIGを dot 形式で出力する．
void
AigMgrImpl::gen_dot(
  ostream& s,
  const vector<AigEdge>& root_list,
  const JsonValue& option
) const
{
  DotGen dg{option};
  dg.write(s, root_list, mNodeArray);
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
  mNodeAttrList.emplace("color", "aquamarine");
  mNodeAttrList.emplace("style", "filled");
  mTerminalAttrList.emplace("shape", "box");
  mTerminalAttrList.emplace("style", "filled");
  mTerminalAttrList.emplace("color", "mediumpurple");
  mEdgeAttrList.emplace("style", "solid");
  mEdgeAttrList.emplace("color", "blue");
  mConstAttrList.emplace("shape", "box");
  mConstAttrList.emplace("style", "filled");
  mConstAttrList.emplace("color", "coral");
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
      mEdgeAttrList.emplace(attr_name, attr_val);
      mConstAttrList.emplace(attr_name, attr_val);
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
      else if ( type == "const" ) {
	mConstAttrList.emplace(attr_name1, attr_val);
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
  const vector<AigEdge>& root_list,
  const vector<std::unique_ptr<AigNode>>& node_list
)
{
  DotWriter writer{s};

  // dot の開始
  writer.graph_begin("digraph", "aig", mGraphAttrList);

  // 根のノードの定義
  {
    SizeType id = 1;
    for ( auto _: root_list ) {
      auto attr_list = mRootAttrList;
      ostringstream buf;
      buf << "\"AIG#" << id << "\"";
      attr_list.emplace("label", buf.str());
      writer.write_node(root_name(id), attr_list);
      ++ id;
    }
  }

  // ノードの定義
  SizeType input_num = 0;
  for ( auto& node: node_list ) {
    auto name = node_name(node);
    if ( node->is_input() ) {
      auto attr_list = mTerminalAttrList;
      add_label_attr(attr_list, node->input_id());
      writer.write_node(name, attr_list);
      ++ input_num;
    }
    else {
      auto attr_list = mNodeAttrList;
      attr_list.emplace("label", "\"\"");
      writer.write_node(name, attr_list);
    }
  }

  // 根の枝の定義
  {
    SizeType id = 1;
    for ( auto edge: root_list ) {
      write_edge(writer, root_name(id), edge);
      ++ id;
    }
  }

  // 枝の定義
  for ( auto& node: node_list ) {
    if ( node->is_and() ) {
      auto name = node_name(node);
      write_edge(writer, name, node->fanin0());
      write_edge(writer, name, node->fanin1());
    }
  }

  // 根のランクの設定
  {
    SizeType n = root_list.size();
    vector<string> name_list(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      name_list[i] = root_name(i + 1);
    }
    writer.write_rank_group(name_list, "min");
  }

  // 入力ノードのランクの設定
  {
    vector<string> name_list;
    name_list.reserve(input_num);
    for ( auto& node: node_list ) {
      if ( node->is_input() ) {
	auto name = node_name(node);
	name_list.push_back(name);
      }
    }
    writer.write_rank_group(name_list, "max");
  }

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
  const AigNode* node
)
{
  ostringstream buf;
  buf << "node" << node->id();
  return buf.str();
}

// @brief 変数ラベルを設定する．
void
DotGen::add_label_attr(
  std::unordered_map<string, string>& attr_list,
  SizeType input_id
)
{
  {
    ostringstream buf;
    buf << "\"";
    if ( mLabelDict.count(input_id) > 0 ) {
      buf << mLabelDict.at(input_id);
    }
    else {
      buf << "Input#" << input_id;
    }
    buf << "\"";
    attr_list.emplace("label", buf.str());
  }
  if ( mTexLblDict.count(input_id) > 0 ) {
    ostringstream buf;
    buf << "\""
	<< mTexLblDict.at(input_id)
	<< "\"";
    attr_list.emplace("texlbl", buf.str());
  }
}

// @brief 枝の内容を出力する．
void
DotGen::write_edge(
  DotWriter& writer,
  const string& from_node,
  AigEdge edge
)
{
  string to_node;
  if ( edge.is_zero() ) {
    // その都度 const0 ノードを作る．
    ostringstream buf;
    buf << from_node << "_const0";
    to_node = buf.str();
    auto attr_list = mConstAttrList;
    attr_list.emplace("label", "const_0");
    writer.write_node(to_node, attr_list);
  }
  else if ( edge.is_one() ) {
    // その都度 const1 ノードを作る．
    ostringstream buf;
    buf << from_node << "_const1";
    to_node = buf.str();
    auto attr_list = mConstAttrList;
    attr_list.emplace("label", "const_1");
    writer.write_node(to_node, attr_list);
  }
  else {
    auto node = edge.node();
    to_node = node_name(node);
  }

  auto attr_list = mEdgeAttrList;
  if ( edge.inv() && !edge.is_one() ) {
    attr_list.emplace("dir", "both");
    attr_list.emplace("arrowtail", "odot");
  }

  writer.write_edge(from_node, to_node, attr_list);
}

END_NAMESPACE_YM_AIG
