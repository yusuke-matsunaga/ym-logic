
/// @file BddNodeTable.cc
/// @brief BddNodeTable の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "BddNodeTable.h"


BEGIN_NAMESPACE_YM

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
  0
};

inline
SizeType
hash_func(
  SizeType index,
  BddEdge edge0,
  BddEdge edge1
)
{
  SizeType v{0};
  v += index;
  v += edge0.hash() * 3;
  v += edge1.hash() * 7;
  return v;
}

END_NONAMESPACE

// @brief コンストラクタ
BddNodeTable::BddNodeTable()
{
  extend(1024);
}

// @brief デストラクタ
BddNodeTable::~BddNodeTable()
{
  delete [] mTable;
}

// @brief 該当するノードを探す．
BddNode*
BddNodeTable::find(
  SizeType index,
  BddEdge edge0,
  BddEdge edge1
) const
{
  // ハッシュ値を求める．
  SizeType pos = hash_func(index, edge0, edge1) % mHashSize;
  for ( auto node = mTable[pos]; node != nullptr; node = node->mLink ) {
    if ( node->index() == index &&
	 node->edge0() == edge0 &&
	 node->edge1() == edge1 ) {
      // 見つけた．
      return node;
    }
  }
  // 見つからなかった．
  return nullptr;
}

// @brief ノードを登録する．
void
BddNodeTable::put(
  BddNode* node
)
{
  if ( mCount >= mNextLimit ) {
    // テーブルを拡張する．
    extend(mSize * 2);
  }

  SizeType pos = hash_func(node->index(), node->edge0(), node->edge1()) % mHashSize;
  auto prev = mTable[pos];
  mTable[pos] = node;
  node->mLink = prev;
  ++ mCount;
}

// @brief 表を拡張する．
void
BddNodeTable::extend(
  SizeType req_size
)
{
  SizeType old_size = mSize;
  BddNode** old_table = mTable;

  mSize = req_size;
  mTable = new BddNode*[mSize];
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
      BddNode* next = nullptr;
      for ( auto node = old_table[i]; node != nullptr; node = next ) {
	next = node->mLink;
	SizeType pos = hash_func(node->index(), node->edge0(), node->edge1()) % mHashSize;
	auto prev = mTable[pos];
	mTable[pos] = node;
	node->mLink = prev;
      }
    }
    delete [] old_table;
  }

  mNextLimit = static_cast<SizeType>(mSize * 1.8);
}

END_NAMESPACE_YM
