
/// @file DdNodeMgr.cc
/// @brief DdNodeMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "dd/DdNodeMgr.h"
#include "dd/DdNode.h"
#include "dd/DdEdge.h"
#include "dd/DdNodeTable.h"


BEGIN_NAMESPACE_YM_DD

// @brief コンストラクタ
DdNodeMgr::DdNodeMgr()
{
}

// @brief デストラクタ
DdNodeMgr::~DdNodeMgr()
{
}

// @brief 変数の数を返す．
SizeType
DdNodeMgr::variable_num() const
{
  return mIndexArray.size();
}

// @brief 新しい変数テーブルを追加する．
SizeType
DdNodeMgr::new_variable()
{
  auto varid = mTableArray.size();
  auto table = new DdNodeTable{varid};
  // 追加される変数のインデックスは最初は変数番号に等しい
  auto index = varid;
  mTableArray.push_back(table);
  mIndexArray.push_back(varid);
  return varid;
}

// @brief 変数番号からインデックスを返す．
SizeType
DdNodeMgr::varid_to_index(
  SizeType varid
) const
{
  if ( varid >= mIndexArray.size() ) {
    throw std::out_of_range{"varid is out of range"};
  }
  return mIndexArray[varid];
}

// @brief インデックスから変数番号を返す．
SizeType
DdNodeMgr::index_to_varid(
  SizeType index
) const
{
  if ( index >= mTableArray.size() ) {
    throw std::out_of_range{"index is out of range"};
  }
  auto table = mTableArray[index];
  return table->varid();
}

// @brief ノードを作る．
const DdNode*
DdNodeMgr::new_node(
  SizeType index,
  DdEdge edge0,
  DdEdge edge1
)
{
  if ( index >= mTableArray.size() ) {
    throw std::out_of_range("index is out of range");
  }
  auto table = mTableArray[index];
  DdNode* node = nullptr;
  if ( table->new_node(index, edge0, edge1, node) ) {
    ++ mNodeNum;
    ++ mGarbageNum;
  }
  return node;
}

// @brief ノードを登録する．
void
DdNodeMgr::reg_node(
  DdNode* node
)
{
  auto index = node->index();
  auto table = mTableArray[index];
  table->reg_node(node);
}

// @brief ノード(枝)の参照回数を増やす．
void
DdNodeMgr::activate(
  DdEdge edge
)
{
  inc_ref(edge);
}

// @brief ノード(枝)の参照回数を減らす．
void
DdNodeMgr::deactivate(
  DdEdge edge
)
{
  dec_ref(edge);
  garbage_collection();
}

// @brief ノード(枝)の参照回数を増やす．
void
DdNodeMgr::inc_ref(
  DdEdge edge
)
{
  auto node = edge.node();
  if ( node != nullptr ) {
    ++ node->mRefCount;
    if ( node->mRefCount == 1 ) {
      -- mGarbageNum;
      inc_ref(node->edge0());
      inc_ref(node->edge1());
    }
  }
}

// @brief ノード(枝)の参照回数を減らす．
void
DdNodeMgr::dec_ref(
  DdEdge edge
)
{
  auto node = edge.node();
  if ( node != nullptr ) {
    -- node->mRefCount;
    if ( node->mRefCount == 0 ) {
      ++ mGarbageNum;
      dec_ref(node->edge0());
      dec_ref(node->edge1());
    }
  }
}

// @brief ガーベージコレクションを行う．
void
DdNodeMgr::garbage_collection()
{
  if ( mGcEnable && mGarbageNum >= mGcLimit ) {
    for ( auto table: mTableArray ) {
      auto n = table->garbage_collection();
      mNodeNum -= n;
    }
    mGarbageNum = 0;
  }
  after_gc();
}

// @brief garbage_collection() が呼ばれた後に呼び出される関数
void
DdNodeMgr::after_gc()
{
}


//////////////////////////////////////////////////////////////////////
// クラス DdEdge
//////////////////////////////////////////////////////////////////////

BEGIN_NONAMESPACE

SizeType
dfs_sub(
  DdEdge edge,
  vector<SizeType>& data_list,
  unordered_map<DdEdge, SizeType>& id_map
)
{
  if ( edge.is_zero() ) {
    return 0;
  }
  if ( edge.is_one() ) {
    return 1;
  }
  if ( id_map.count(edge) > 0 ) {
    return id_map.at(edge);
  }
  auto node = edge.node();
  auto index = node->index();
  auto edge0 = node->edge0();
  auto edge1 = node->edge1();
  auto id0 = dfs_sub(edge0.posi_edge(), data_list, id_map);
  auto id1 = dfs_sub(edge1.posi_edge(), data_list, id_map);
  auto id = (data_list.size() / 3) + 1;
  id_map.emplace(edge, id);
  data_list.push_back(index);
  auto inv = edge.inv();
  auto inv0 = node->edge0().inv() ^ inv;
  SizeType tmp0 = id0 * 2;
  if ( inv0 ) {
    tmp0 += 1;
  }
  data_list.push_back(tmp0);
  auto inv1 = node->edge1().inv() ^ inv;
  SizeType tmp1 = id1 * 2;
  if ( inv1 ) {
    tmp1 += 1;
  }
  data_list.push_back(tmp1);
  return id;
}

void
print_edge(
  ostream& s,
  SizeType val
)
{
  if ( val == 0 ) {
    s << "   ZERO ";
  }
  else if ( val == 1 ) {
    s << "    ONE ";
  }
  else {
    SizeType id = val / 2;
    s << setw(7) << id;
    if ( val % 2 ) {
      s << "~";
    }
    else {
      s << " ";
    }
  }
}

END_NONAMESPACE

// @brief root を根とするDDの内容を出力する(デバッグ用)．
void
print(
  ostream& s,
  DdEdge root
)
{
  unordered_map<DdEdge, SizeType> id_map;
  vector<SizeType> data_list;
  auto id = dfs_sub(root.posi_edge(), data_list, id_map);
  auto tmp = id * 2;
  if ( root.inv() ) {
    tmp += 1;
  }
  s << "Root:  ";
  print_edge(s, tmp);
  s << endl;
  SizeType n = data_list.size() / 3;
  for ( SizeType i = 0; i < n; ++ i ) {
    SizeType base = (n - i - 1) * 3;
    auto index = data_list[base + 0];
    auto tmp0 = data_list[base + 1];
    auto tmp1 = data_list[base + 2];
    s << setw(7) << (n - i) << ":"
      << setw(6) << index;
    print_edge(s, tmp0);
    print_edge(s, tmp1);
    s << endl;
  }
}

END_NAMESPACE_YM_DD
