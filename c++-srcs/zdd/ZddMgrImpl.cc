
/// @file ZddMgrImpl.cc
/// @brief ZddMgrImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ZddMgrImpl.h"
#include "ym/Zdd.h"


BEGIN_NAMESPACE_YM_ZDD

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
  ZddEdge edge0,
  ZddEdge edge1
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
ZddMgrImpl::ZddMgrImpl()
{
  extend(1024);
}

// @brief デストラクタ
ZddMgrImpl::~ZddMgrImpl()
{
  delete [] mTable;
}

ZddEdge
ZddMgrImpl::cap_step(
  ZddEdge left,
  ZddEdge right
)
{
  // trivial case のチェック

  // case 1: 片方が 0 なら 0 を返す．
  if ( left.is_zero() || right.is_zero() ) {
    return ZddEdge::zero();
  }

  // case 3: 等しかったらそのまま返す．
  if ( left == right ) {
    return left;
  }

  // 正規化を行う．
  if ( left.body() > right.body() ) {
    std::swap(left, right);
  }

  // 演算結果テーブルを調べる．
  Apply2Key key{left, right};
  if ( mCapTable.count(key) > 0 ) {
    return mCapTable.at(key);
  }

  // 見つからなかったので実際に apply 演算を行う．
  // 先頭のインデックスで分解する．
  ZddEdge left0, left1;
  ZddEdge right0, right1;
  auto top = decomp(
    left, right,
    left0, left1,
    right0, right1
  );
  auto ans0 = cap_step(left0, right0);
  auto ans1 = cap_step(left1, right1);
  auto result = new_node(top, ans0, ans1);
  mCapTable.emplace(key, result);
  return result;
}

// @brief CUP 演算を行う．
ZddEdge
ZddMgrImpl::cup_step(
  ZddEdge left,
  ZddEdge right
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

  // case 3: 等しかったら自分自身を返す．
  if ( left == right ) {
    return left;
  }

  // 正規化を行う．
  if ( left.body() > right.body() ) {
    std::swap(left, right);
  }

  // 演算結果テーブルを調べる．
  Apply2Key key{left, right};
  if ( mCupTable.count(key) > 0 ) {
    return mCupTable.at(key);
  }
  // 見つからなかったので実際に apply 演算を行う．
  // 先頭のインデックスで分解する．
  ZddEdge left0, left1;
  ZddEdge right0, right1;
  auto top = decomp(
    left, right,
    left0, left1,
    right0, right1
  );
  auto ans0 = cup_step(left0, right0);
  auto ans1 = cup_step(left1, right1);
  auto result = new_node(top, ans0, ans1);
  mCupTable.emplace(key, result);
  return result;
}

// @brief DIFF 演算を行う．
ZddEdge
ZddMgrImpl::diff_step(
  ZddEdge left,
  ZddEdge right
)
{
  // trivial case のチェック

  // case 1: left が 0 なら 0 を返す．
  if ( left.is_zero() ) {
    return ZddEdge::zero();
  }

  // case 2: rightが 0 なら left を返す．
  if ( right.is_zero() ) {
    return left;
  }

  // case 5: 等しかったら 0 を返す．
  if ( left == right ) {
    return ZddEdge::zero();
  }

  // 演算結果テーブルを調べる．
  Apply2Key key{left, right};
  if ( mDiffTable.count(key) > 0 ) {
    return mDiffTable.at(key);
  }
  // 見つからなかったので実際に apply 演算を行う．
  // 先頭のインデックスで分解する．
  ZddEdge left0, left1;
  ZddEdge right0, right1;
  auto top = decomp(
    left, right,
    left0, left1,
    right0, right1
  );
  auto ans0 = diff_step(left0, right0);
  auto ans1 = diff_step(left1, right1);
  auto result = new_node(top, ans0, ans1);
  mDiffTable.emplace(key, result);
  return result;
}

// @brief 部分集合を作る．
ZddEdge
ZddMgrImpl::make_set(
  const vector<SizeType>& elem_list
)
{
  vector<SizeType> tmp_list{elem_list};
  sort(tmp_list.begin(), tmp_list.end(), [](int a, int b) {
    return a > b;
  });

  auto f0 = ZddEdge::zero();
  auto f1 = ZddEdge::one();
  for ( auto elem: tmp_list ) {
    f1 = new_node(elem, f0, f1);
  }
  return f1;
}

// @brief ノードを作る．
ZddEdge
ZddMgrImpl::new_node(
  SizeType index,
  ZddEdge edge0,
  ZddEdge edge1
)
{
  // edge1 が 0 の時は新しいノードを作らない．
  if ( edge1.is_zero() ) {
    return edge0;
  }

  // ノードテーブルを探す．
  // ハッシュ値を求める．
  SizeType pos0 = hash_func(index, edge0, edge1);
  SizeType pos = pos0 % mHashSize;
  ZddNode* node = nullptr;
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
    node = new ZddNode{index, edge0, edge1};
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
  return ZddEdge{node};
}

// @brief ノード(枝)の参照回数を増やす．
void
ZddMgrImpl::activate(
  ZddEdge edge
)
{
  inc_ref(edge);
}

// @brief ノード(枝)の参照回数を減らす．
void
ZddMgrImpl::deactivate(
  ZddEdge edge
)
{
  dec_ref(edge);
  garbage_collection();
}

// @brief ノード(枝)の参照回数を増やす．
void
ZddMgrImpl::inc_ref(
  ZddEdge edge
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
ZddMgrImpl::dec_ref(
  ZddEdge edge
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
ZddMgrImpl::garbage_collection()
{
  if ( mGcEnable && mGarbageNum >= mGcLimit ) {
    for ( SizeType i = 0; i < mHashSize; ++ i ) {
      ZddNode** pprev = &mTable[i];
      ZddNode* node;
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

    mCapTable.clear();
    mCupTable.clear();
    mDiffTable.clear();
    mProductTable.clear();
  }
}

// @brief 表を拡張する．
void
ZddMgrImpl::extend(
  SizeType req_size
)
{
  SizeType old_size = mSize;
  ZddNode** old_table = mTable;

  mSize = req_size;
  mTable = new ZddNode*[mSize];
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
      ZddNode* next = nullptr;
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

END_NAMESPACE_YM_ZDD
