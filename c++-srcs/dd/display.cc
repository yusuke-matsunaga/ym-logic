
/// @file display.cc
/// @brief Bdd::display(), Zdd::display() の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "NodeCollector.h"
#include "DdInfo.h"


BEGIN_NAMESPACE_YM_DD

BEGIN_NONAMESPACE

// @brief 枝の内容を出力する．
void
display_edge(
  ostream& s,
  SizeType edge
)
{
  if ( edge == 0 ) {
    s << std::setw(7) << std::right << "ZERO";
  }
  else if ( edge == 1 ) {
    s << std::setw(7) << std::right << "ONE";
  }
  else {
    SizeType node = edge >> 1;
    bool inv = static_cast<bool>(edge & 1);
    s << setw(6) << node;
    if ( inv ) {
      s << "~";
    }
    else {
      s << " ";
    }
  }
}

END_NONAMESPACE

// @brief 内容を見やすい形式で出力する．
void
DdInfo::display(
  ostream& s,
  const vector<SizeType>& root_edge_list,
  const vector<DdInfo>& node_list
)
{
  for ( auto root: root_edge_list ) {
    display_edge(s, root);
  }
  s << endl;
  SizeType n = node_list.size();
  for ( SizeType i = 0; i < n; ++ i ) {
    SizeType id = n - i;
    auto& node = node_list[id - 1];
    s << setw(6) << id << ": "
      << setw(4) << node.index();
    display_edge(s, node.edge0());
    s << ": ";
    display_edge(s, node.edge1());
    s << endl;
    -- id;
  }
}

// @brief 構造を表す整数配列を作る．
vector<SizeType>
DdInfo::rep_data(
  const vector<SizeType>& root_edge_list,
  const vector<DdInfo>& node_list
)
{
  vector<SizeType> ans;
  ans.push_back(root_edge_list.size());
  for ( auto root: root_edge_list ) {
    ans.push_back(root);
  }
  for ( auto& node: node_list ) {
    ans.push_back(node.index());
    ans.push_back(node.edge0());
    ans.push_back(node.edge1());
  }
  return ans;
}

END_NAMESPACE_YM_DD
