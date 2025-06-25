
/// @file Cut.cc
/// @brief Cut の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CutMgr.h"
#include "../AigNode.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
/// @class CalcTv CalcTv.h "CalcTv.h"
/// @brief Cut の論理関数を計算するためのクラス
///
/// ここでは４入力関数のみを扱うので16ビットの符号なし整数(std::uint16_t)
/// で論理関数を表す．
//////////////////////////////////////////////////////////////////////
class CalcTv
{
public:

  using Tv4Type = Cut::Tv4Type;

public:

  /// @brief コンストラクタ
  CalcTv() = default;

  /// @brief デストラクタ
  ~CalcTv() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 関数をセットする．
  void
  set(
    AigNode* node, ///< [in] 対象のノード
    Tv4Type tv     ///< [in] 論理関数
  )
  {
    mTvDict.emplace(node->id(), tv);
  }

  /// @brief 関数を返す．
  Tv4Type
  tv(
    AigNode* node
  ) const
  {
    if ( mTvDict.count(node->id()) == 0 ) {
      std::ostringstream buf;
      buf << "Node#" << node->id() << " is not registered";
      throw std::logic_error{buf.str()};
    }
    return mTvDict.at(node->id());
  }

  /// @brief 関数を返す．
  Tv4Type
  tv(
    AigEdge edge ///< [in] 対象の枝
  )
  {
    auto node = edge.node();
    auto tv = this->tv(node);
    if ( edge.inv() ) {
      tv = ~tv;
    }
    return tv;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード番号をキーにして論理関数を保持する辞書
  std::unordered_map<SizeType, SizeType> mTvDict;

};


//////////////////////////////////////////////////////////////////////
// クラス Cut
//////////////////////////////////////////////////////////////////////

// @brief 論理関数を計算する．
Cut::Tv4Type
Cut::calc_tv() const
{
  // 入力の論理関数を登録する．
  // このパタンは PatMgr と一致させる必要がある．
  static Tv4Type pat[] = {
    0xAAAA,
    0xCCCC,
    0xF0F0,
    0xFF00
  };
  // ノード番号をキーにして論理関数を保持する辞書
  CalcTv calc_tv;
  for ( SizeType i = 0; i < leaf_size(); ++ i ) {
    auto node = leaf(i);
    calc_tv.set(node, pat[i]);
  }
  try {
    for ( auto node: mNodeList ) {
      auto tv0 = calc_tv.tv(node->fanin0());
      auto tv1 = calc_tv.tv(node->fanin1());
      auto tv = tv0 & tv1;
      calc_tv.set(node, tv);
    }
    return calc_tv.tv(root());
  }
  catch ( std::logic_error err ) {
    cerr << err.what() << endl;
    print(cerr);
    throw err;
  }
}

BEGIN_NONAMESPACE

bool
check_dfs(
  AigNode* node,
  std::unordered_set<SizeType>& node_mark,
  const std::unordered_set<SizeType>& leaf_mark,
  std::unordered_set<SizeType>& mark
)
{
  auto id = node->id();
  if ( mark.count(id) > 0 ) {
    return true;
  }
  mark.emplace(id);
  if ( leaf_mark.count(id) > 0 ) {
    return true;
  }
  node_mark.emplace(id);
  if ( !node->is_and() ) {
    std::ostringstream buf;
    buf << "Node#" << node->id()
	<< " is not AND";
    return false;
  }
  auto node0 = node->fanin0_node();
  if ( !check_dfs(node0, node_mark, leaf_mark, mark) ) {
    return false;
  }
  auto node1 = node->fanin1_node();
  if ( !check_dfs(node1, node_mark, leaf_mark, mark) ) {
    return false;
  }
  return true;
}

END_NONAMESPACE

// @brief 内容が正しいかチェックする．
bool
Cut::check() const
{
  {
    std::unordered_set<SizeType> node_mark;
    for ( auto node: mLeafList ) {
      node_mark.emplace(node->id());
    }
    for ( auto node: mNodeList ) {
      node_mark.emplace(node->id());
    }
    for ( auto node: mNodeList ) {
      auto node0 = node->fanin0_node();
      if ( node_mark.count(node0->id()) == 0 ) {
	std::ostringstream buf;
	buf << "Node#" << node0->id() << " is not contained in mNodeList";
	cout << buf.str() << endl;
	//throw std::logic_error{buf.str()};
	return false;
      }
      auto node1 = node->fanin1_node();
      if ( node_mark.count(node1->id()) == 0 ) {
	std::ostringstream buf;
	buf << "Node#" << node1->id() << " is not contained in mNodeList";
	cout << buf.str() << endl;
	//throw std::logic_error{buf.str()};
	return false;
      }
    }
  }
  {
    std::unordered_set<SizeType> leaf_mark;
    for ( auto node: mLeafList ) {
      leaf_mark.emplace(node->id());
    }
    std::unordered_set<SizeType> node_mark;
    std::unordered_set<SizeType> mark;
    if ( !check_dfs(root(), node_mark, leaf_mark, mark) ) {
      print(cout);
      throw std::logic_error{""};
    }
  }
  return true;
}

BEGIN_NONAMESPACE

void
count_dfs(
  AigNode* node,
  std::unordered_map<SizeType, SizeType>& count_dict
)
{
  auto& count = count_dict.at(node->id());
  ++ count;
  if ( count == node->ref_count() ) {
    if ( node->is_and() ) {
      count_dfs(node->fanin0_node(), count_dict);
      count_dfs(node->fanin1_node(), count_dict);
    }
  }
}

END_NONAMESPACE

// @brief 内容を出力する．
void
Cut::print(
  std::ostream& s
) const
{
  std::unordered_map<SizeType, SizeType> count_dict;
  for ( auto node: mNodeList ) {
    count_dict.emplace(node->id(), 0);
  }
  for ( auto node: mNodeList ) {
    if ( node->is_and() ) {
      auto node0 = node->fanin0_node();
      if ( count_dict.count(node0->id()) == 0 ) {
	count_dict.emplace(node0->id(), 0);
      }
      ++ count_dict.at(node0->id());
      auto node1 = node->fanin1_node();
      if ( count_dict.count(node1->id()) == 0 ) {
	count_dict.emplace(node1->id(), 0);
      }
      ++ count_dict.at(node1->id());
    }
  }
  s << "Root: Node#" << root()->id() << endl;
  s << "Leaves:";
  for ( auto node: mLeafList ) {
    s << " Node#" << node->id();
  }
  s << endl;
  for ( auto node: mNodeList ) {
    s << "  Node#" << node->id() << ": ";
    if ( node->is_input() ) {
      s << "Input#" << node->input_id();
    }
    else {
      s << "And(" << node->fanin0()
	<< ", " << node->fanin1()
	<< ")";
      s << " " << count_dict.at(node->id()) << "/" << node->ref_count();
    }
    s << endl;
  }
}

END_NAMESPACE_YM_AIG
