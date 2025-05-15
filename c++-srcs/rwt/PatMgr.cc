
/// @file PatMgr.cc
/// @brief PatMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "PatMgr.h"
#include "PatNode.h"


BEGIN_NAMESPACE_YM_LOGIC

BEGIN_NONAMESPACE

// ノード数を数える．
SizeType
count_size(
  const PatNode* node,
  vector<bool>& mark_array
)
{
  if ( mark_array[node->id()] ) {
    return 0;
  }
  mark_array[node->id()] = true;
  auto child0 = node->child0();
  SizeType size = 1;
  if ( child0 != nullptr ) {
    size += count_size(child0, mark_array);
  }
  auto child1 = node->child1();
  if ( child1 != nullptr ) {
    size += count_size(child1, mark_array);
  }
  return size;
}

END_NONAMESPACE

// @brief コンストラクタ
PatMgr::PatMgr(
) : mPatListArray(1 << 16)
{
  static
    std::uint16_t pat_data[] = {
#include "pat_data.h"
  };
  // テーブルには EOD が入っているので -1
  // それ以外に定数０，入力を4つで + 5
  SizeType node_size = sizeof(pat_data) / (sizeof(std::uint8_t) * 2) - 1 + 5;

  mNodeArray.reserve(node_size);
  // 定数0を登録する．
  add_node(false, 0x0000, 0, 0, nullptr, false, nullptr, false);
  // 入力0〜3を登録する．
  add_node(false, 0xAAAA, 0, 0, nullptr, false, nullptr, false);
  add_node(false, 0xCCCC, 0, 0, nullptr, false, nullptr, false);
  add_node(false, 0xF0F0, 0, 0, nullptr, false, nullptr, false);
  add_node(false, 0xFF00, 0, 0, nullptr, false, nullptr, false);
  if ( mNodeArray.size() != 5 ) {
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
    std::uint16_t tv;
    if ( xor_flag ) {
      // XOR の場合は入力の反転はない．
      tv = node0->tv() ^ node1->tv();
    }
    else {
      auto tv0 = node0->tv();
      if ( inv0 ) {
	tv0 = ~tv0;
      }
      auto tv1 = node1->tv();
      if ( inv1 ) {
	tv1 = ~tv1;
      }
      tv = tv0 & tv1;
    }
    vector<bool> mark_array(mNodeArray.size(), false);
    auto size0 = count_size(node0, mark_array);
    auto size1 = count_size(node1, mark_array);
    auto size = size0 + size1 + 1;
    auto level = std::max(node0->level(), node1->level()) + 1;
    add_node(xor_flag, tv, size, level, node0, inv0, node1, inv1);
  }
}

// @brief ノードを追加する．
void
PatMgr::add_node(
  bool xor_flag,
  std::uint16_t tv,
  std::uint8_t size,
  std::uint8_t level,
  const PatNode* child0,
  bool inv0,
  const PatNode* child1,
  bool inv1
)
{
  auto id = mNodeArray.size();
  mNodeArray.push_back(PatNode(id, xor_flag,
			       tv, size, level,
			       child0, inv0,
			       child1, inv1));
  auto node = &mNodeArray[id];
  mPatListArray[tv].push_back(node);
}

END_NAMESPACE_YM_LOGIC
