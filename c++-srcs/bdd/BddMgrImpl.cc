
/// @file BddMgrImpl.cc
/// @brief BddMgrImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "BddMgrImpl.h"


BEGIN_NAMESPACE_YM_BDD

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
BddMgrImpl::BddMgrImpl()
{
  extend(1024);
}

// @brief デストラクタ
BddMgrImpl::~BddMgrImpl()
{
  delete [] mTable;
}

// @brief ノードを作る．
BddEdge
BddMgrImpl::new_node(
  SizeType index,
  BddEdge edge0,
  BddEdge edge1
)
{
  // 2つの枝が等しい時は新しいノードを作らない．
  if ( edge0 == edge1 ) {
    return edge0;
  }

  // 極性の正規化を行う．
  bool oinv{edge0.inv()};
  edge0 *= oinv;
  edge1 *= oinv;

  // ノードテーブルを探す．
  auto node = find(index, edge0, edge1);
  if ( node == nullptr ) {
    // なかったので新規に作る．
    ++ mGarbageNum;
    node = new BddNode{index, edge0, edge1};
    // テーブルに登録する．
    put(node);
  }
  return BddEdge{node, oinv};
}

// @brief ノード(枝)の参照回数を増やす．
void
BddMgrImpl::activate(
  BddEdge edge
)
{
  auto node = edge.node();
  if ( node != nullptr ) {
    ++ node->mRefCount;
    if ( node->mRefCount == 1 ) {
      -- mGarbageNum;
      activate(node->edge0());
      activate(node->edge1());
    }
  }
}

// @brief ノード(枝)の参照回数を減らす．
void
BddMgrImpl::deactivate(
  BddEdge edge
)
{
  auto node = edge.node();
  if ( node != nullptr ) {
    -- node->mRefCount;
    if ( node->mRefCount == 0 ) {
      ++ mGarbageNum;
      deactivate(node->edge0());
      deactivate(node->edge1());
    }
  }
}

// @brief ガーベージコレクションを行う．
void
BddMgrImpl::garbage_collection()
{
  if ( mGcEnable && mGarbageNum >= mGcLimit ) {
    for ( SizeType i = 0; i < mHashSize; ++ i ) {
      BddNode** pprev = &mTable[i];
      BddNode* node;
      while ( (node = *pprev) != nullptr ) {
	if ( node->mRefCount == 0 ) {
	  *pprev = node->mLink;
	  delete node;
	}
	else {
	  pprev = &node->mLink;
	}
      }
    }
    mNodeNum -= mGarbageNum;
    mGarbageNum = 0;
  }
}

// @brief 該当するノードを探す．
BddNode*
BddMgrImpl::find(
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
BddMgrImpl::put(
  BddNode* node
)
{
  if ( mNodeNum >= mNextLimit ) {
    // テーブルを拡張する．
    extend(mSize * 2);
  }

  SizeType pos = hash_func(node->index(), node->edge0(), node->edge1()) % mHashSize;
  auto prev = mTable[pos];
  mTable[pos] = node;
  node->mLink = prev;
  ++ mNodeNum;
}

// @brief 表を拡張する．
void
BddMgrImpl::extend(
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

END_NAMESPACE_YM_BDD
