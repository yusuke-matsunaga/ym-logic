
/// @file RcMatrix.cc
/// @brief RcMatrix の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "RcMatrix.h"


BEGIN_NAMESPACE_YM_RC

//////////////////////////////////////////////////////////////////////
// クラス RcMatrix
//////////////////////////////////////////////////////////////////////

// @brief 行方向に要素を挿入する．
void
RcMatrix::_row_insert(
  RcElem* row_head,
  RcElem* elem
)
{
  auto top = row_head->mRight;
  auto last = row_head->mLeft;
  if ( last == row_head ) {
    // 空だった．
    _row_connect(row_head, elem);
  }
  else if ( last->col() < elem->col() ) {
    // 末尾に追加する．
    _row_connect(last, elem);
  }
  else if ( last->col() == elem->col() ) {
    throw std::invalid_argument{"duplicate row insertion"};
  }
  else if ( top->col() > elem->col() ) {
    // 先頭に追加する．
    _row_connect(row_head, elem);
  }
  else if ( top->col() == elem->col() ) {
    throw std::invalid_argument{"duplicate row insertion"};
  }
  else {
    // ここに来たということは top->col() < elem->col() < last->col()
    // left->col() < elem->col() <= left->right()->col() となるような挿入位置を探す．
    // ただし重複した要素の追加はエラーとする．
    for ( auto left = top; left != row_head; left = left->mRight ) {
      auto right = left->mRight;
      if ( elem->col() < right->col() ) {
	// 見つけた
	_row_connect(left,  elem);
	break;
      }
      if ( elem->col() == right->col() ) {
	throw std::invalid_argument{"duplicate row insertion"};
      }
    }
  }
}

// @brief 列方向に要素を挿入する．
void
RcMatrix::_col_insert(
  RcElem* col_head,
  RcElem* elem
)
{
  auto top = col_head->mDown;
  auto last = col_head->mUp;
  if ( last == col_head ) {
    // 空だった．
    _col_connect(col_head, elem);
  }
  else if ( last->row() < elem->row() ) {
    // 末尾に追加する．
    _col_connect(last, elem);
  }
  else if ( last->row() == elem->row() ) {
    throw std::invalid_argument{"duplicate col insertion"};
  }
  else if ( top->row() > elem->row() ) {
    // 先頭に追加する．
    _col_connect(col_head, elem);
  }
  else if ( top->row() == elem->row() ) {
    throw std::invalid_argument{"duplicate col insertion"};
  }
  else {
    // ここに来たということは top->row() < elem->row() < last->row()
    // left->row() < elem->row() <= left->right()->row() となるような挿入位置を探す．
    // ただし重複した要素の追加はエラーとする．
    for ( auto left = top; left != row_head; left = left->mRight ) {
      auto right = left->mRight;
      if ( elem->row() < right->row() ) {
	// 見つけた
	_col_connect(left,  elem);
	break;
      }
      if ( elem->row() == right->row() ) {
	throw std::invalid_argument{"duplicate col insertion"};
      }
    }
  }
}

END_NAMESPACE_YM_RC
