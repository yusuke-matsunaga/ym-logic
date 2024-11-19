
/// @file DdInfoMgr.cc
/// @brief DdInfoMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "dd/DdInfoMgr.h"
#include "dd/DdEdge.h"
#include "dd/DdNode.h"
#include "dd/DdNodeMgr.h"
#include "NodeCollector.h"


BEGIN_NAMESPACE_YM_DD

// @brief コンストラクタ
DdInfoMgr::DdInfoMgr(
  const vector<DdEdge>& root_list,
  DdNodeMgr* node_mgr
)
{
  NodeCollector nc{root_list};

  // mRootList を作る．
  mRootList.reserve(root_list.size());
  for ( auto root: root_list ) {
    auto root_id = nc.edge2int(root);
    mRootList.push_back(root_id);
  }

  // mNodeList を作る．
  auto& node_list = nc.node_list();
  mNodeList.reserve(node_list.size());
  SizeType max_level = 0;
  for ( auto node: node_list ) {
    auto level = node->level();
    auto edge0 = nc.edge2int(node->edge0());
    auto edge1 = nc.edge2int(node->edge1());
    mNodeList.push_back(DdInfo{level, edge0, edge1});
    max_level = std::max(max_level, level + 1);
  }

  // レベルごとのノードリストを作る．
  mVarIdArray = vector<SizeType>(max_level);
  for ( SizeType i = 0; i < max_level; ++ i ) {
    mVarIdArray[i] = node_mgr->level_to_varid(i);
  }
  mIdListArray = vector<vector<SizeType>>(max_level);
  for ( SizeType i = 0; i < mNodeList.size(); ++ i ) {
    auto& node = mNodeList[i];
    auto level = node.level();
    auto id = i + 1;
    mIdListArray[level].push_back(id);
  }
}

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
DdInfoMgr::display(
  ostream& s
) const
{
  for ( auto root: mRootList ) {
    display_edge(s, root);
  }
  s << endl;
  SizeType n = mNodeList.size();
  for ( SizeType i = 0; i < n; ++ i ) {
    SizeType id = n - i;
    auto& node = mNodeList[id - 1];
    auto varid = level_to_varid(node.level());
    s << setw(6) << id << ": "
      << setw(4) << varid;
    display_edge(s, node.edge0());
    s << ": ";
    display_edge(s, node.edge1());
    s << endl;
    -- id;
  }
}

// @brief 構造を表す整数配列を作る．
vector<SizeType>
DdInfoMgr::rep_data() const
{
  vector<SizeType> ans;
  ans.push_back(mRootList.size());
  for ( auto root: mRootList ) {
    ans.push_back(root);
  }
  for ( auto& node: mNodeList ) {
    ans.push_back(node.level());
    ans.push_back(node.edge0());
    ans.push_back(node.edge1());
  }
  return ans;
}

END_NAMESPACE_YM_DD
