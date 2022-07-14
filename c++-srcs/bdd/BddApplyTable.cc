
/// @file BddApplyTable.cc
/// @brief BddApplyTable の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "BddApplyTable.h"
#include "BddEdge.h"


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
  SizeType op,
  BddEdge left,
  BddEdge right
)
{
  SizeType v{0};
  v += op;
  v += left.hash() * 3;
  v += right.hash() * 7;
  return v;
}

END_NONAMESPACE

BddApplyTable::BddApplyTable()
{
  resize(2048);
}

// @brief デストラクタ
BddApplyTable::~BddApplyTable()
{
  delete [] mTable;
}

// @brief テーブルサイズを変更する．
void
BddApplyTable::resize(
  SizeType req_size
)
{
  auto old_size = mSize;
  auto old_hash_size = mHashSize;
  auto old_table = mTable;
  mSize = req_size;
  mTable = new Cell[mSize];
  for ( SizeType i = 0; i < mSize; ++ i ) {
    mTable[i].mResult = BddEdge::make_invalid();
  }

  mHashSize = 0;
  for ( auto size: tablesize ) {
    if ( mSize / 2 < size && size < mSize ) {
      mHashSize = size;
      break;
    }
  }
  ASSERT_COND( mHashSize != 0 );

  if ( old_hash_size > 0 ) {
    for ( SizeType i = 0; i < old_hash_size; ++ i ) {
      const auto& cell = old_table[i];
      SizeType pos = hash_func(cell.mOp, cell.mLeft, cell.mRight) % mHashSize;
      mTable[pos] = cell;
    }
    delete [] old_table;
  }
}

// @brief 演算結果が登録されているか調べる．
bool
BddApplyTable::find(
  SizeType op,
  BddEdge e1,
  BddEdge e2,
  BddEdge& result
)
{
  SizeType pos = hash_func(op, e1, e2) % mHashSize;
  const auto& cell = mTable[pos];
  if ( cell.mOp == op && cell.mLeft == e1 && cell.mRight == e2 ) {
    result = cell.mResult;
    return true;
  }
  else {
    return false;
  }
}

// @brief 演算結果を登録する．
void
BddApplyTable::put(
  SizeType op,
  BddEdge e1,
  BddEdge e2,
  BddEdge& result
)
{
  SizeType pos = hash_func(op, e1, e2) % mHashSize;
  auto& cell = mTable[pos];
  cell.mOp = op;
  cell.mRight = e1;
  cell.mLeft = e2;
  cell.mResult = result;
}

END_NAMESPACE_YM
