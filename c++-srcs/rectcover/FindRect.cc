
/// @file FindRect.cc
/// @brief FindRect の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "FindRect.h"


BEGIN_NAMESPACE_YM_RC

BEGIN_NONAMESPACE

// 価値が最大の要素を保持するためのクラス
template<typename T, typename V>
class MaxHolder
{
public:

  /// @brief コンストラクタ
  MaxHolder() :
    mMaxValue{std::numeric_limits<V>::min()}
  {
  }

  /// @brief デストラクタ
  ~MaxHolder() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素を追加する．
  void
  add(
    const T& elem, ///< [in] 要素
    V value        ///< [in] 価値
  )
  {
    if ( mMaxValue <= value ) {
      if ( mMaxValue < value ) {
	mMaxValue = value;
	mMaxList.clear();
      }
      mMaxList.push_back(elem);
    }
  }

  /// @brief 最大価値の要素のリストを返す．
  const vector<T>&
  elem_list() const
  {
    return mMaxList;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 最大値
  V mMaxValue;

  // 最大価値の要素のリスト
  vector<T> mMaxList;

};

END_NONAMESPACE

RcRect
FindRect::operator()(
  const RcMatrix& matrix
)
{
  // 価値が最大となる矩形を保持するクラス
  MaxHolder<RcRect, int> max_rect;

  {
    // 価値が最大の行を最初の矩形とする．
    auto rect = find_row(matrix);
    // 価値が最大となるように行を追加する．
    do {
      auto value = matrix.rect_value(rect);
      max_rect.add(rect, value);
    }
    while ( row_expand(matrix, rect) );

    // rect の中から価値が最大となる列を選ぶ．
    rect = find_col(matrix, rect.col_list());
    // 価値が最大となるように列を追加する．
    do {
      auto value = matrix.rect_value(rect);
      max_rect.add(rect, value);
    }
    while ( col_expand(matrix, rect) );
  }
  {
    // 価値が最大となる列を選ぶ．
    auto rect = find_col(matrix);
    // 価値が最大となるように列を追加する．
    do {
      auto value = matrix.rect_value(rect);
      max_rect.add(rect, value);
    }
    while ( col_expand(matrix, rect) );

    // rect の中から価値が最大の行を最初の矩形とする．
    rect = find_row(matrix, rect.row_list());
    // 価値が最大となるように行を追加する．
    do {
      auto value = matrix.rect_value(rect);
      max_rect.add(rect, value);
    }
    while ( row_expand(matrix, rect) );
  }

  // 最大の価値を持つ矩形を選ぶ．
  return random_choice(max_rect.elem_list());
}

BEGIN_NONAMESPACE

int
row_value(
  const RcMatrix& matrix,
  SizeType row
)
{
  int val = 0;
  for ( auto elem: matrix.row_list(row) ) {
    auto vid = elem->val_id();
    val += matrix.value(vid);
    val -= matrix.col_cost(elem->col());
  }
  val -= matrix.row_cost(row);
  return val;
}

END_NONAMESPACE

// 価値が最大の行を選ぶ．
RcRect
FindRect::find_row(
  const RcMatrix& matrix,
  const vector<SizeType>& row_list
)
{
  MaxHolder<SizeType, SizeType> max_row;
  if ( row_list.empty() ) {
    for ( SizeType row = 0; row < matrix.row_size(); ++ row ) {
      auto val = row_value(matrix, row);
      max_row.add(row, val);
    }
  }
  else {
    for ( auto row: row_list ) {
      auto val = row_value(matrix, row);
      max_row.add(row, val);
    }
  }
  auto& max_row_list = max_row.elem_list();
  if ( max_row_list.empty() ) {
    return RcRect();
  }
  // 同じ価値ならランダムに選ぶ．{
  auto row = random_choice(max_row_list);
  vector<SizeType> col_list;
  col_list.reserve(matrix.row_num(row));
  for ( auto elem: matrix.row_list(row) ) {
    auto col = elem->col();
    col_list.push_back(col);
  }
  return RcRect({row}, col_list);
}

// @brief 価値が最大となるように行を追加する．
bool
FindRect::row_expand(
  const RcMatrix& matrix,
  RcRect& rect
)
{
  auto& rect_row_list = rect.row_list();
  auto& rect_col_list = rect.col_list();

  // 現在の矩形の列と共通部分を持つ行を調べる．
  vector<SizeType> row_cand_list;
  {
    // 処理済みの行のマーク
    vector<bool> row_mark(matrix.row_size(), false);
    // 現在の矩形の行を処理済みとマークする．
    for ( auto row: rect_row_list ) {
      row_mark[row] = true;
    }
    row_cand_list.reserve(matrix.row_size() - rect_row_list.size());
    for ( auto col: rect_col_list ) {
      for ( auto elem: matrix.col_list(col) ) {
	auto row = elem->row();
	if ( row_mark[row] ) {
	  continue;
	}
	row_mark[row] = true;
	row_cand_list.push_back(row);
      }
    }
  }

  // 今の矩形と新しい行の共通部分を求め，新しい
  // 価値を計算する．
  // その行を加えることで列が削除されない行は
  // 無条件で追加する．

  // 無条件で加える行のリスト
  vector<SizeType> new_row_list = rect_row_list;
  new_row_list.reserve(matrix.row_size());
  // 行番号と対応する列番号のリストのリスト
  vector<pair<SizeType, vector<SizeType>>> row_col_list;
  row_col_list.reserve(matrix.row_size());
  for ( auto row: row_cand_list ) {
    vector<SizeType> new_col_list;
    new_col_list.reserve(rect_col_list.size());
    auto iter = RcRowMergeIter{rect_col_list, matrix.row_list(row)};
    for ( ; !iter.is_end(); ++ iter ) {
      auto col = (*iter)->col();
      new_col_list.push_back(col);
    }
    if ( new_col_list.size() == rect_col_list.size() ) {
      new_row_list.push_back(row);
    }
    else if ( new_col_list.size() > 0 ) {
      row_col_list.push_back(make_pair(row, new_col_list));
    }
  }

  if ( row_col_list.empty() ) {
    // 列集合は変化しなかった．
    if ( new_row_list.size() == rect_row_list.size() ) {
      // 行集合も変化しなかった．
      return false;
    }
    rect = RcRect{new_row_list, rect_col_list};
    return true;
  }
  if ( row_col_list.size() == 1 ) {
    // 候補がただ一つ．
    auto& p = row_col_list.front();
    auto row = p.first;
    auto& col_list = p.second;
    new_row_list.push_back(row);
    rect = RcRect{new_row_list, col_list};
    return true;
  }
  // 最も価値の高いものを選ぶ．
  MaxHolder<RcRect, int> max_rect;
  auto row_list = new_row_list;
  row_list.push_back(0);
  for ( auto& p: row_col_list ) {
    auto row = p.first;
    auto& col_list = p.second;
    row_list.back() = row;
    auto rect = RcRect{row_list, col_list};
    int val = matrix.rect_value(rect);
    max_rect.add(rect, val);
  }
  rect = random_choice(max_rect.elem_list());
  return true;
}

BEGIN_NONAMESPACE

int
col_value(
  const RcMatrix& matrix,
  SizeType col
)
{
  int val = 0;
  for ( auto elem: matrix.col_list(col) ) {
    auto vid = elem->val_id();
    val += matrix.value(vid);
    val -= matrix.row_cost(elem->row());
  }
  val -= matrix.col_cost(col);
  return val;
}

END_NONAMESPACE

// @brief 価値が最大の列を選ぶ．
RcRect
FindRect::find_col(
  const RcMatrix& matrix,
  const vector<SizeType>& col_list
)
{
  MaxHolder<SizeType, SizeType> max_col;
  if ( col_list.empty() ) {
    for ( SizeType col = 0; col < matrix.col_size(); ++ col ) {
      auto val = col_value(matrix, col);
      max_col.add(col, val);
    }
  }
  else {
    for ( auto col: col_list ) {
      auto val = col_value(matrix, col);
      max_col.add(col, val);
    }
  }
  auto& max_col_list = max_col.elem_list();
  if ( max_col_list.empty() ) {
    return RcRect();
  }
  // 同じ価値ならランダムに選ぶ．
  auto col = random_choice(max_col_list);
  vector<SizeType> row_list;
  row_list.reserve(matrix.col_num(col));
  for ( auto elem: matrix.col_list(col) ) {
    auto row = elem->row();
    row_list.push_back(row);
  }
  return RcRect(row_list, {col});
}

// @brief 価値が最大となるように列を追加する．
bool
FindRect::col_expand(
  const RcMatrix& matrix,
  RcRect& rect
)
{
  auto& rect_row_list = rect.row_list();
  auto& rect_col_list = rect.col_list();

  // 現在の矩形の行と共通部分を持つ列を調べる．
  vector<SizeType> col_cand_list;
  {
    // 処理済みの列のマーク
    vector<bool> col_mark(matrix.col_size(), false);
    // 現在の矩形の列を処理済みとマークする．
    for ( auto col: rect_col_list ) {
      col_mark[col] = true;
    }
    col_cand_list.reserve(matrix.row_size() - rect_row_list.size());
    for ( auto row: rect_row_list ) {
      for ( auto elem: matrix.row_list(row) ) {
	auto col = elem->col();
	if ( col_mark[col] ) {
	  continue;
	}
	col_mark[col] = true;
	col_cand_list.push_back(col);
      }
    }
  }

  // 今の矩形と新しい列の共通部分を求め，新しい
  // 価値を計算する．
  // その列を加えることで行が削除されない列は
  // 無条件で追加する．

  // 無条件で加える列のリスト
  vector<SizeType> new_col_list = rect_col_list;
  new_col_list.reserve(matrix.col_size());
  // 列番号と対応する行番号のリストのリスト
  vector<pair<SizeType, vector<SizeType>>> col_row_list;
  col_row_list.reserve(matrix.col_size());
  for ( auto col: col_cand_list ) {
    vector<SizeType> new_row_list;
    new_row_list.reserve(rect_row_list.size());
    auto iter = RcColMergeIter{rect_row_list, matrix.col_list(col)};
    for ( ; !iter.is_end(); ++ iter ) {
      auto row = (*iter)->row();
      new_row_list.push_back(row);
    }
    if ( new_row_list.size() == rect_row_list.size() ) {
      new_col_list.push_back(col);
    }
    else if ( new_row_list.size() > 0 ) {
      col_row_list.push_back(make_pair(col, new_row_list));
    }
  }

  if ( col_row_list.empty() ) {
    // 行集合は変化しなかった．
    if ( new_col_list.size() == rect_col_list.size() ) {
      // 列集合も変化しなかった．
      return false;
    }
    rect = RcRect{rect_row_list, new_col_list};
    return true;
  }
  if ( col_row_list.size() == 1 ) {
    // 候補がただ一つ．
    auto& p = col_row_list.front();
    auto col = p.first;
    auto& row_list = p.second;
    new_col_list.push_back(col);
    rect = RcRect{row_list, new_col_list};
    return true;
  }
  // 最も価値の高いものを選ぶ．
  MaxHolder<RcRect, int> max_rect;
  auto col_list = new_col_list;
  col_list.push_back(0);
  for ( auto& p: col_row_list ) {
    auto col = p.first;
    auto& row_list = p.second;
    col_list.back() = col;
    auto rect = RcRect{row_list, col_list};
    int val = matrix.rect_value(rect);
    max_rect.add(rect, val);
  }
  rect = random_choice(max_rect.elem_list());
  return true;
}

END_NAMESPACE_YM_RC
