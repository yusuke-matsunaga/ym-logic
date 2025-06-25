
/// @file PatMgr.cc
/// @brief PatMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "PatMgr.h"
#include "PatGraph.h"
#include "PatNode.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
// クラス PatMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PatMgr::PatMgr()
{
  static std::uint16_t pat_data[] = {
#include "pat_data.h"
  };
  // テーブルには EOD が入っているので -1
  // それ以外に定数０，入力を4つで + 5
  SizeType node_size = sizeof(pat_data) / (sizeof(std::uint8_t) * 2) - 1 + 5;
  mNodeArray.reserve(node_size);

  // ノード番号をキーにして論理関数を保持する辞書
  std::unordered_map<SizeType, Tv4> tv_dict;

  // 定数0を登録する．
  mConst0 = add_node(0x0000, nullptr, false, nullptr, false,
		     tv_dict);
  if ( mConst0->id() != 0 ) {
    throw std::logic_error{"something wrong"};
  }

  // 入力0〜3を登録する．
  mInputList[0] = add_node(0xAAAA, nullptr, false, nullptr, false,
			   tv_dict);
  if ( mInputList[0]->id() != 1 ) {
    throw std::logic_error{"something wrong"};
  }

  mInputList[1] = add_node(0xCCCC, nullptr, false, nullptr, false,
			   tv_dict);
  if ( mInputList[1]->id() != 2 ) {
    throw std::logic_error{"something wrong"};
  }

  mInputList[2] = add_node(0xF0F0, nullptr, false, nullptr, false,
			   tv_dict);
  if ( mInputList[2]->id() != 3 ) {
    throw std::logic_error{"something wrong"};
  }

  mInputList[3] = add_node(0xFF00, nullptr, false, nullptr, false,
			   tv_dict);
  if ( mInputList[3]->id() != 4 ) {
    throw std::logic_error{"something wrong"};
  }

  // 残りはデータから読み込む．
  for ( SizeType i = 0; ; ++ i ) {
    auto base = i * 2;
    auto data0 = pat_data[base + 0];
    auto data1 = pat_data[base + 1];
    if ( data0 == 0 && data1 == 0 ) {
      break;
    }
    auto xor_flag = static_cast<bool>(data0 & 1);
    data0 >>= 1;
    auto inv0 = static_cast<bool>(data0 & 1);
    auto idx0 = data0 >> 1;
    auto inv1 = static_cast<bool>(data1 & 1);
    auto idx1 = data1 >> 1;
    auto node0 = &mNodeArray[idx0];
    auto node1 = &mNodeArray[idx1];
    auto tv0 = tv_dict.at(node0->id());
    auto tv1 = tv_dict.at(node1->id());
    if ( xor_flag ) {
      // XOR の場合は入力の反転はない．
      auto tv00 = ~tv0 & ~tv1;
      auto node00 = add_node(tv00, node0, true, node1, true, tv_dict);
      auto tv11 = tv0 & tv1;
      auto node11 = add_node(tv11, node0, false, node1, false, tv_dict);
      auto tv = tv0 ^ tv1;
      add_node(tv, node00, true, node11, true, tv_dict);
    }
    else {
      if ( inv0 ) {
	tv0 = ~tv0;
      }
      if ( inv1 ) {
	tv1 = ~tv1;
      }
      auto tv = tv0 & tv1;
      add_node(tv, node0, inv0, node1, inv1,
	       tv_dict);
    }
  }
}

// @brief ノードを追加する．
PatNode*
PatMgr::add_node(
  Tv4 tv,
  const PatNode* child0,
  bool inv0,
  const PatNode* child1,
  bool inv1,
  std::unordered_map<SizeType, Tv4>& tv_dict
)
{
  auto id = mNodeArray.size();
  tv_dict.emplace(id, tv);
  mNodeArray.push_back(PatNode(id, child0, inv0, child1, inv1));
  auto node = &mNodeArray[id];
  Npn4 npn;
  auto rep_tv = Npn4::normalize(tv, npn);
  if ( mPatGraphDict.count(rep_tv) == 0 ) {
    mPatGraphDict.emplace(rep_tv, std::vector<PatGraph>{});
  }

  auto& pat_list = mPatGraphDict.at(rep_tv);
  pat_list.push_back(PatGraph(node, npn));
  return node;
}


//////////////////////////////////////////////////////////////////////
// クラス PatGraph
//////////////////////////////////////////////////////////////////////

BEGIN_NONAMESPACE

void
print_dfs(
  std::ostream& s,
  const PatNode* node,
  std::unordered_set<SizeType>& mark
)
{
  if ( node->id() <= 4 ) {
    // node は定数か入力のはず．
    return;
  }
  if ( mark.count(node->id()) > 0 ) {
    return;
  }
  mark.emplace(node->id());

  // node は AND ノードのはず．
  auto node0 = node->child0();
  auto inv0 = node->inv0();
  auto node1 = node->child1();
  auto inv1 = node->inv1();
  s << "Node#" << node->id() << ": ";
  if ( inv0 ) {
    s << "~";
  }
  else {
    s << " ";
  }
  s << "Node#" << node0->id();
  s << " & ";
  if ( inv1 ) {
    s << "~";
  }
  else {
    s << " ";
  }
  s << "Node#" << node1->id();
  s << endl;
  print_dfs(s, node0, mark);
  print_dfs(s, node1, mark);
}

END_NONAMESPACE

// @brief 内容を出力する(主にデバッグ用)．
void
PatGraph::print(
  std::ostream& s
) const
{
  std::unordered_set<SizeType> mark;
  print_dfs(s, root(), mark);
  s << npn() << endl;
}

END_NAMESPACE_YM_AIG
