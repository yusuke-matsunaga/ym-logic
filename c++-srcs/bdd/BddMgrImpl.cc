
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
  v += edge0.hash() * 13;
  v += edge1.hash() * 17;
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

BddEdge
BddMgrImpl::and_step(
  BddEdge left,
  BddEdge right
)
{
  // trivial case のチェック

  // case 1: 片方が 0 なら 0 を返す．
  if ( left.is_zero() || right.is_zero() ) {
    return BddEdge::zero();
  }

  // case 2: 片方が 1 なら他方を返す．
  if ( left.is_one() ) {
    return right;
  }
  if ( right.is_one() ) {
    return left;
  }

  // case 3: 等しかったらそのまま返す．
  if ( left == right ) {
    return left;
  }

  // case 4: 極性違いで等しかったら 0 を返す．
  if ( left.node() == right.node() ) {
    return BddEdge::zero();
  }

  // 正規化を行う．
  if ( left.body() > right.body() ) {
    std::swap(left, right);
  }

  // 演算結果テーブルを調べる．
  Apply3Key key{left, right, BddEdge::zero()};
  if ( mIteTable.count(key) > 0 ) {
    return mIteTable.at(key);
  }

  // 見つからなかったので実際に apply 演算を行う．
  // 先頭のインデックスで分解する．
  BddEdge left0, left1;
  BddEdge right0, right1;
  auto top = decomp(left, right,
		    left0, left1,
		    right0, right1);
  auto ans0 = and_step(left0, right0);
  auto ans1 = and_step(left1, right1);
  auto result = new_node(top, ans0, ans1);
  mIteTable.emplace(key, result);
  return result;
}

// @brief XOR 演算を行う．
BddEdge
BddMgrImpl::xor_step(
  BddEdge left,
  BddEdge right
)
{
  // trivial case のチェック

  // case 1: 片方が 0 なら他方を返す．
  if ( left.is_zero() ) {
    return right;
  }
  if ( right.is_zero() ) {
    return left;
  }

  // case 2: 片方が 1 なら他方の否定を返す．
  if ( left.is_one() ) {
    return ~right;
  }
  if ( right.is_one() ) {
    return ~left;
  }

  // case 3: 等しかったら 0 を返す．
  if ( left == right ) {
    return BddEdge::zero();
  }

  // case 4: 極性違いで等しかったら 1 を返す．
  if ( left.node() == right.node() ) {
    return BddEdge::one();
  }

  // 正規化を行う．
  bool oinv = left.inv() * right.inv();
  left.make_positive();
  right.make_positive();
  if ( left.body() > right.body() ) {
    std::swap(left, right);
  }

  // 演算結果テーブルを調べる．
  Apply3Key key{left, ~right, right};
  if ( mIteTable.count(key) > 0 ) {
    return mIteTable.at(key);
  }

  // 見つからなかったので実際に apply 演算を行う．
  // 先頭のインデックスで分解する．
  BddEdge left0, left1;
  BddEdge right0, right1;
  auto top = decomp(left, right,
		    left0, left1,
		    right0, right1);
  auto ans0 = xor_step(left0, right0);
  auto ans1 = xor_step(left1, right1);
  auto result = new_node(top, ans0, ans1);
  mIteTable.emplace(key, result);
  return result * oinv;
}

// @brief ITE 演算を行う．
BddEdge
BddMgrImpl::ite_step(
  BddEdge e0,
  BddEdge e1,
  BddEdge e2
)
{
  // trivial cases

  // e0 が定数なら答は決まっている．
  if ( e0.is_zero() ) {
    return e2;
  }
  if ( e0.is_one() ) {
    return e1;
  }
  // e1 が 0 の時は ~e0 & e2 となる．
  if ( e1.is_zero() ) {
    return and_step(~e0, e2);
  }
  // e1 が 1 の時は e0 | e2 となる．
  if ( e1.is_one() ) {
    return or_step(e0, e2);
  }
  // e2 が 0 の時は e0 & e1 となる．
  if ( e2.is_zero() ) {
    return and_step(e0, e1);
  }
  // e2 が 1 の時は ~e0 | e1 となる．
  if ( e2.is_one() ) {
    return or_step(~e0, e1);
  }

  // e1 と e2 が等しければ e0 と無関係に e1 を返す．
  if ( e1 == e2 ) {
    return e1;
  }
  // e1 と e2 が相補的であれば e0 と e1 の XNOR となる．
  if ( e1 == ~e2 ) {
    return ~xor_step(e0, e1);
  }
  // e0 と e1 が等しければ e0 | e2 となる．
  if ( e0 == e1 ) {
    return or_step(e0, e2);
  }
  // e0 と e1 が相補的であれば e1 & e2 となる．
  if ( e0 == ~e1 ) {
    return and_step(e1, e2);
  }
  // e0 と e2 が等しければ e0 & e1 となる．
  if ( e0 == e2 ) {
    return and_step(e0, e1);
  }
  // e0 と e2 が相補的であれば ~e0 | e1 となる．
  if ( e0 == ~e2 ) {
    return or_step(~e0, e1);
  }

  Apply3Key key{e0, e1, e2};
  if ( mIteTable.count(key) > 0 ) {
    return mIteTable.at(key);
  }

  auto node0 = e0.node();
  auto inv0 = e0.inv();
  auto index0 = node0->index();

  auto node1 = e1.node();
  auto inv1 = e1.inv();
  auto index1 = node1->index();

  auto node2 = e2.node();
  auto inv2 = e2.inv();
  auto index2 = node2->index();

  auto top = std::min(std::min(index0, index1), index2);

  BddEdge e00, e01;
  if ( index0 == top ) {
    e00 = node0->edge0() * inv0;
    e01 = node0->edge1() * inv0;
  }
  else {
    e00 = e01 = e0;
  }

  BddEdge e10, e11;
  if ( index1 == top ) {
    e10 = node1->edge0() * inv1;
    e11 = node1->edge1() * inv1;
  }
  else {
    e10 = e11 = e1;
  }

  BddEdge e20, e21;
  if ( index2 == top ) {
    e20 = node2->edge0() * inv2;
    e21 = node2->edge1() * inv2;
  }
  else {
    e20 = e21 = e2;
  }

  auto r0 = ite_step(e00, e10, e20);
  auto r1 = ite_step(e01, e11, e21);
  auto result = new_node(top, r0, r1);
  mIteTable.emplace(key, result);
  return result;
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
  // ハッシュ値を求める．
  SizeType pos0 = hash_func(index, edge0, edge1);
  SizeType pos = pos0 % mHashSize;
  BddNode* node = nullptr;
  for ( node = mTable[pos]; node != nullptr; node = node->mLink ) {
    if ( node->index() == index &&
	 node->edge0() == edge0 &&
	 node->edge1() == edge1 ) {
      // 見つけた．
      break;
    }
  }
  if ( node == nullptr ) {
    // なかったので新規に作る．
    node = new BddNode{index, edge0, edge1};
    // テーブルに登録する．
    if ( mNodeNum >= mNextLimit ) {
      // テーブルを拡張する．
      extend(mSize * 2);
      // 位置は再計算する．
      pos = pos0 % mHashSize;
    }
    auto prev = mTable[pos];
    mTable[pos] = node;
    node->mLink = prev;
    ++ mNodeNum;
    ++ mGarbageNum;
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

    mIteTable.clear();
  }
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
