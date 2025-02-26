
/// @file RcMatrix.cc
/// @brief RcMatrix の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "RcMatrix.h"


BEGIN_NAMESPACE_YM_RC

//////////////////////////////////////////////////////////////////////
// クラス RcRowHead
//////////////////////////////////////////////////////////////////////

// @brief 要素を挿入する．
void
RcRowHead::insert(
  RcElem* elem
)
{
  auto top = this->right();
  auto last = this->left();
  if ( last == this ) {
    // 空だった．
    RcElem::row_connect(this, elem);
  }
  else if ( last->col() < elem->col() ) {
    // 末尾に追加する．
    RcElem::row_connect(last, elem);
  }
  else if ( last->col() == elem->col() ) {
    throw std::invalid_argument{"duplicate row insertion"};
  }
  else if ( top->col() > elem->col() ) {
    // 先頭に追加する．
    RcElem::row_connect(this, elem);
  }
  else if ( top->col() == elem->col() ) {
    throw std::invalid_argument{"duplicate row insertion"};
  }
  else {
    // ここに来たということは top->col() < elem->col() < last->col()
    // left->col() < elem->col() <= left->right()->col() となるような挿入位置を探す．
    // ただし重複した要素の追加はエラーとする．
    for ( auto left = top; left != this; left = left->right() ) {
      auto right = left->right();
      if ( elem->col() < right->col() ) {
	// 見つけた
	RcElem::row_connect(left,  elem);
	break;
      }
      if ( elem->col() == right->col() ) {
	throw std::invalid_argument{"duplicate row insertion"};
      }
    }
  }
  ++ mNum;
}


//////////////////////////////////////////////////////////////////////
// クラス RcColHead
//////////////////////////////////////////////////////////////////////

// @brief 要素を挿入する．
void
RcColHead::insert(
  RcElem* elem
)
{
  auto top = this->down();
  auto last = this->up();
  if ( last == this ) {
    // 空だった．
    RcElem::col_connect(this, elem);
  }
  else if ( last->row() < elem->row() ) {
    // 末尾に追加する．
    RcElem::col_connect(last, elem);
  }
  else if ( last->row() == elem->row() ) {
    throw std::invalid_argument{"duplicate col insertion"};
  }
  else if ( top->row() > elem->row() ) {
    // 先頭に追加する．
    RcElem::col_connect(this, elem);
  }
  else if ( top->row() == elem->row() ) {
    throw std::invalid_argument{"duplicate col insertion"};
  }
  else {
    // ここに来たということは top->row() < elem->row() < last->row()
    // left->row() < elem->row() <= left->right()->row() となるような挿入位置を探す．
    // ただし重複した要素の追加はエラーとする．
    for ( auto left = top; left != this; left = left->right() ) {
      auto right = left->right();
      if ( elem->row() < right->row() ) {
	// 見つけた
	RcElem::col_connect(left,  elem);
	break;
      }
      if ( elem->row() == right->row() ) {
	throw std::invalid_argument{"duplicate col insertion"};
      }
    }
  }
  ++ mNum;
}

END_NAMESPACE_YM_RC
