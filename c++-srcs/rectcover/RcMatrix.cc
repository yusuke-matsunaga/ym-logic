
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

// @brief コンストラクタ
RcMatrix::RcMatrix(
  SizeType row_size,
  SizeType col_size
) : mRowHeadArray(row_size, nullptr),
    mColHeadArray(col_size, nullptr)
{
  for ( SizeType i = 0; i < row_size; ++ i ) {
    auto head = _new_elem(i, -1);
    mRowHeadArray[i] = head;
  }
  for ( SizeType i = 0; i < col_size; ++ i ) {
    auto head = _new_elem(-1, i);
    mColHeadArray[i] = head;
  }
}

// @brief 要素を追加する．
SizeType
RcMatrix::add_elem(
  SizeType row,
  SizeType col
)
{
  _check_row(row);
  _check_col(col);
  auto elem = _new_elem(row, col);
  auto row_head = mRowHeadArray[row];
  _row_insert(row_head, elem);
  auto col_head = mColHeadArray[col];
  _col_insert(col_head, elem);
  return elem->id();
}

// @brief 行方向に要素のリストを追加する．
vector<SizeType>
RcMatrix::add_row_elem(
  SizeType row,
  const vector<SizeType>& col_list
)
{
  _check_row(row);
  auto row_head = mRowHeadArray[row];
  vector<SizeType> id_list;
  id_list.reserve(col_list.size());
  for ( auto col: col_list ) {
    _check_col(col);
    auto col_head = mColHeadArray[col];
    auto elem = _new_elem(row, col);
    _row_insert(row_head, elem);
    _col_insert(col_head, elem);
    id_list.push_back(elem->id());
  }
  return id_list;
}

// @brief 要素を確保する．
RcElem*
RcMatrix::_new_elem(
  SizeType row,
  SizeType col
)
{
  auto id = mElemList.size();
  auto elem = new RcElem(id, row, col);
  mElemList.push_back(std::unique_ptr<RcElem>{elem});
  return elem;
}

// @brief 行方向に要素を挿入する．
void
RcMatrix::_row_insert(
  RcElem* row_head,
  RcElem* elem
)
{
  // 最初に末尾を確かめる．
  auto last = row_head->mLeft;
  if ( last == row_head ) {
    // 空だった．
    _row_connect(row_head, elem);
    _row_connect(elem, row_head);
  }
  else if ( last->col() < elem->col() ) {
    // 末尾に追加する．
    _row_connect(last, elem);
    _row_connect(elem, row_head);
  }
  else {
    auto top = row_head->mRight;
    if ( top->col() > elem->col() ) {
      // 先頭に追加する．
      _row_connect(row_head, elem);
      _row_connect(elem, top);
    }
    else {
      // 挿入位置を探す．
      for ( auto left = top; left != row_head; left = left->mRight ) {
	if (
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
}

END_NAMESPACE_YM_RC
