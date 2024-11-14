
/// @file DdNodeTable.cc
/// @brief DdNodeTable の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "dd/DdNodeTable.h"
#include "dd/DdNode.h"
#include "dd/DdEdge.h"


BEGIN_NAMESPACE_YM_DD

BEGIN_NONAMESPACE

// 2のべき乗数を超えない最大の素数
static
SizeType tablesize[] = {
  1021,
  2039,
  4093,
  8191,
  16381,
  32749,
  65521,
  131071,
  262139,
  524287,
  1048573,
  2097143,
  4194301,
  8388593,
  16777213,
  33554393,
  67108859,
  134217689,
  268435399,
  536870909,
  1073741789,
  0
};

inline
SizeType
hash_func(
  DdEdge edge0,
  DdEdge edge1
)
{
  SizeType v{0};
  v += edge0.hash();
  v += edge1.hash() * 17;
  return v;
}

END_NONAMESPACE

// @brief コンストラクタ
DdNodeTable::DdNodeTable(
  SizeType varid
) : mVarId{varid}
{
  extend(1024);
}

// @brief デストラクタ
DdNodeTable::~DdNodeTable()
{
  DdNode* next = nullptr;
  for ( SizeType i = 0; i < mSize; ++ i ) {
    for ( auto node = mTable[i]; node != nullptr; node = next ) {
      next = node->mLink;
      delete node;
    }
  }
  delete [] mTable;
}

// @brief ノードを作る．
bool
DdNodeTable::new_node(
  SizeType index,
  DdEdge edge0,
  DdEdge edge1,
  DdNode*& node
)
{
  // ノードテーブルを探す．
  // ハッシュ値を求める．
  auto pos0 = hash_func(edge0, edge1);
  auto pos = pos0 % mHashSize;
  for ( node = mTable[pos]; node != nullptr; node = node->mLink ) {
    if ( node->edge0() == edge0 &&
	 node->edge1() == edge1 ) {
      // 見つけた．
      return false;
    }
  }
  // なかったので新規に作る．
  node = new DdNode{index, edge0, edge1};
  reg_node(node);
  return true;
}

// @brief ノードを登録する．
void
DdNodeTable::reg_node(
  DdNode* node
)
{
  // テーブルに登録する．
  if ( mNodeNum >= mNextLimit ) {
    // テーブルを拡張する．
    extend(mSize * 2);
  }
  auto pos0 = hash_func(node->edge0(), node->edge1());
  auto pos = pos0 % mHashSize;
  auto prev = mTable[pos];
  mTable[pos] = node;
  node->mLink = prev;
  ++ mNodeNum;
}

// @brief 内容を取り出す．
vector<DdNode*>
DdNodeTable::move()
{
  vector<DdNode*> node_list;
  node_list.reserve(mNodeNum);
  for ( SizeType i = 0; i < mHashSize; ++ i ) {
    for ( auto node = mTable[i]; node != nullptr; node = node->mLink ) {
      node_list.push_back(node);
    }
    mTable[i] = nullptr;
  }
  mNodeNum = 0;
  return node_list;
}

// @brief ガーベージコレクションを行う．
SizeType
DdNodeTable::garbage_collection()
{
  SizeType dcount = 0;
  for ( SizeType i = 0; i < mHashSize; ++ i ) {
    DdNode** pprev = &mTable[i];
    DdNode* node;
    while ( (node = *pprev) != nullptr ) {
      if ( node->mRefCount == 0 ) {
	*pprev = node->mLink;
	delete node;
	++ dcount;
      }
      else {
	pprev = &node->mLink;
      }
    }
  }
  mNodeNum -= dcount;
  return dcount;
}

// @brief 表を拡張する．
void
DdNodeTable::extend(
  SizeType req_size
)
{
  SizeType old_size = mSize;
  DdNode** old_table = mTable;

  mSize = req_size;
  mTable = new DdNode*[mSize];
  for ( SizeType i = 0; i < mSize; ++ i ) {
    mTable[i] = nullptr;
  }

  mHashSize = 0;
  for ( auto size: tablesize ) {
    if ( mSize / 2 < size && size < mSize ) {
      mHashSize = size;
      break;
    }
  }
  ASSERT_COND( mHashSize != 0 );

  if ( old_size > 0 ) {
    // 昔のテーブルの内容をコピーする．
    for ( SizeType i = 0; i < old_size; ++ i ) {
      DdNode* next = nullptr;
      for ( auto node = old_table[i]; node != nullptr; node = next ) {
	next = node->mLink;
	auto pos = hash_func(node->edge0(), node->edge1()) % mHashSize;
	auto prev = mTable[pos];
	mTable[pos] = node;
	node->mLink = prev;
      }
    }
    delete [] old_table;
  }

  mNextLimit = static_cast<SizeType>(mSize * 1.8);
}

END_NAMESPACE_YM_DD
