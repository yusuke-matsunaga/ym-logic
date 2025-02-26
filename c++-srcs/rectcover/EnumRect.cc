
/// @file EnumRect.cc
/// @brief EnumRect の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EnumRect.h"


BEGIN_NAMESPACE_YM_RC

vector<RcRect>
EnumRect::operator()()
{
  mMaxValue = std::numeric_limits<int>::min();
  mMaxList.clear();
  for ( SizeType row = 0; row < mMatrix.row_size(); ++ row ) {
    vector<SizeType> new_col_list;
    new_col_list.reserve(mMatrix.row_num(row));
    for ( auto elem: mMatrix.row_list(row) ) {
      auto col = elem->col();
      new_col_list.push_back(col);
    }
    vector<SizeType> new_row_list;
    if ( expand(row, new_col_list, new_row_list) ) {
      continue;
    }
    auto rect = RcRect(new_row_list, new_col_list);
    enum_sub(rect, row + 1);
  }

  return mMaxList;
}

// @brief 矩形を列挙する下請け関数
void
EnumRect::enum_sub(
  const RcRect& src_rect,
  SizeType row0
)
{
  // src_rect を記録する．
  auto value = mMatrix.rect_value(src_rect);
  if ( mMaxValue <= value ) {
    if ( mMaxValue < value ) {
      mMaxValue = value;
      mMaxList.clear();
    }
    mMaxList.push_back(src_rect);
  }

  // 現在の矩形の列と共通部分を持つ row0 以上の行を列挙する．

  // 処理済みのマーク
  vector<bool> mark(mMatrix.row_size(), false);
  // 現在の矩形の行を処理済みとする．
  for ( auto row: src_rect.row_list() ) {
    mark[row] = true;
  }

  for ( auto col: src_rect.col_list() ) {
    for ( auto elem: mMatrix.col_list(col) ) {
      auto row = elem->row();
      if ( row < row0 ) {
	continue;
      }
      if ( mark[row] ) {
	continue;
      }
      mark[row] = true;

      // 現在の矩形と row の行の共通部分を求める．
      vector<SizeType> new_col_list;
      new_col_list.reserve(mMatrix.row_num(row));
      auto iter = RcRowMergeIter(src_rect.col_list(), mMatrix.row_list(row));
      for ( ; !iter.is_end(); ++ iter ) {
	auto col = (*iter)->col();
	new_col_list.push_back(col);
      }

      // さらにその共通部分に完全に含まれる行を求める．
      // その行に row 未満の行が含まれている場合は
      // 同じ矩形がすでに列挙されているということなので
      // スキップする．
      vector<SizeType> new_row_list;
      if ( expand(row, new_col_list, new_row_list) ) {
	continue;
      }
      auto rect = RcRect(new_row_list, new_col_list);
      enum_sub(rect, row + 1);
    }
  }
}

// @brief 与えられた列集合に含まれる行集合を求める．
bool
EnumRect::expand(
  SizeType row0,
  const vector<SizeType>& col_list,
  vector<SizeType>& row_list
)
{
  if ( col_list.empty() ) {
    throw std::logic_error{"col_list is empty"};
  }

  // 結果の行集合の要素数は col_list 中の任意の列に含まれる
  // 要素数を上回らない．
  auto n = mMatrix.col_num(col_list.front());
  row_list.clear();
  row_list.reserve(n);

  // col_list に含まれていることを示すビットベクタ
  vector<bool> col_mark(mMatrix.col_size(), false);
  SizeType col_num = col_list.size();
  for ( auto col: col_list ) {
    col_mark[col] = true;
  }

  // 処理済みを示すマーク
  vector<bool> row_mark(mMatrix.row_size(), false);
  row_mark[row0] = true;
  row_list.push_back(row0);
  // col_list に含まれる列に含まれる行を対象にする．
  for ( auto col: col_list ) {
    for ( auto elem: mMatrix.col_list(col) ) {
      auto row = elem->row();
      if ( row_mark[row] ) {
	continue;
      }
      row_mark[row] = true;

      SizeType count = 0;
      for ( auto elem: mMatrix.row_list(row) ) {
	auto col = elem->col();
	if ( col_mark[col] ) {
	  ++ count;
	}
      }
      if ( count < col_num ) {
	// col_list に含まれていない要素があった．
	continue;
      }
      if ( row < row0 ) {
	// row0 よりも小さな行が含まれていた．
	return true;
      }
      row_list.push_back(row);
    }
  }
  return false;
}


//////////////////////////////////////////////////////////////////////
// クラス RcMatrix
//////////////////////////////////////////////////////////////////////

// @brief 最大価値を持つ矩形を列挙する．
vector<RcRect>
RcMatrix::enum_max_rects() const
{
  EnumRect enum_rect(*this);

  return enum_rect();
}

END_NAMESPACE_YM_RC
