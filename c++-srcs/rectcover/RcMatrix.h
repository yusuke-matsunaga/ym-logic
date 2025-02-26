#ifndef RCMATRIX_H
#define RCMATRIX_H

/// @file RcMatrix.h
/// @brief RcMatrix のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"


BEGIN_NAMESPACE_YM_RC

const SizeType BAD_ID = static_cast<SizeType>(-1);

//////////////////////////////////////////////////////////////////////
/// @class RcRect RcMatrix.h "RcMatrix.h"
/// @brief 矩形を表すクラス
///
/// 具体的には 行番号の集合と列番号の集合を持つ．
//////////////////////////////////////////////////////////////////////
class RcRect
{
public:

  /// @brief 空のコンストラクタ
  RcRect() = default;

  /// @brief コンストラクタ
  RcRect(
    const vector<SizeType>& row_list, ///< [in] 行番号のリスト
    const vector<SizeType>& col_list  ///< [in] 列番号のリスト
  ) : mRowList{row_list},
      mColList{col_list}
  {
    std::sort(mRowList.begin(), mRowList.end());
    std::sort(mColList.begin(), mColList.end());
  }

  /// @brief デストラクタ
  ~RcRect() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 行数を返す．
  SizeType
  row_size() const
  {
    return mRowList.size();
  }

  /// @brief 行番号のリストを返す．
  const vector<SizeType>&
  row_list() const
  {
    return mRowList;
  }

  /// @brief 列数を返す．
  SizeType
  col_size() const
  {
    return mColList.size();
  }

  /// @brief 列番号のリストを返す．
  const vector<SizeType>&
  col_list() const
  {
    return mColList;
  }

  /// @brief 等価比較演算子
  bool
  operator==(
    const RcRect& right
  ) const
  {
    return mRowList == right.mRowList && mColList == right.mColList;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const RcRect& right
  ) const
  {
    return !operator==(right);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 行番号のリスト
  vector<SizeType> mRowList;

  // 列番号のリスト
  vector<SizeType> mColList;

};

/// @brief RcRect のストリーム出力
inline
ostream&
operator<<(
  ostream& s,
  const RcRect& rect
)
{
  s << "({";
  const char* comma = "";
  for ( auto row: rect.row_list() ) {
    s << comma << row;
    comma = ", ";
  }
  s << "}, {";
  comma = "";
  for ( auto col: rect.col_list() ) {
    s << comma << col;
    comma = ", ";
  }
  s << "})";
  return s;
}


//////////////////////////////////////////////////////////////////////
/// @class RcElem RcMatrix.h "RcMatrix.h"
/// @brief RcMatrix の要素を表すクラス
///
/// 各々の要素は行番号，列番号の他に価値番号を持つ
//////////////////////////////////////////////////////////////////////
class RcElem
{
  friend class RcMatrix;

public:

  /// @brief コンストラクタ
  RcElem(
    SizeType val_id, ///< [in] 価値番号
    SizeType row,    ///< [in] 行番号
    SizeType col     ///< [in] 列番号
  ) : mValId{val_id},
      mRow{row},
      mCol{col}
  {
    mLeft = mRight = mUp = mDown = this;
  }

  /// @brief デストラクタ
  ~RcElem() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 価値番号を返す．
  SizeType
  val_id() const
  {
    return mValId;
  }

  /// @brief 行番号を反す．
  SizeType
  row() const
  {
    return mRow;
  }

  /// @brief 列番号を返す．
  SizeType
  col() const
  {
    return mCol;
  }

  /// @brief 行方向の前の要素を返す．
  RcElem*
  left() const
  {
    return mLeft;
  }

  /// @brief 行方向の次の要素を返す．
  RcElem*
  right() const
  {
    return mRight;
  }

  /// @brief 列方向の前の要素を返す．
  RcElem*
  up() const
  {
    return mUp;
  }

  /// @brief 列方向の次の要素を返す．
  RcElem*
  down() const
  {
    return mDown;
  }

  /// @brief 行方向の接続を行う．
  static
  void
  row_connect(
    RcElem* left,  ///< [in] 左の要素
    RcElem* elem   ///< [in] 挿入する要素
  )
  {
    auto right = left->mRight;
    left->mRight = elem;
    elem->mLeft = left;
    elem->mRight = right;
    right->mLeft = elem;
  }

  /// @brief 列方向の接続を行う．
  static
  void
  col_connect(
    RcElem* up,  ///< [in] 上の要素
    RcElem* elem ///< [in] 挿入する要素
  )
  {
    auto down = up->mDown;
    up->mDown = elem;
    elem->mUp = up;
    elem->mDown = down;
    down->mUp = elem;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 価値番号
  SizeType mValId;

  // 行番号
  SizeType mRow;

  // 列番号
  SizeType mCol;

  // 行方向の前の要素
  RcElem* mLeft;

  // 行方向の次の要素
  RcElem* mRight;

  // 列方向の前の要素
  RcElem* mUp;

  // 列方向の次の要素
  RcElem* mDown;

};


//////////////////////////////////////////////////////////////////////
/// @class RcRowHead RcMatrix.h "RcMatrix.h"
/// @brief 行の先頭を表すクラス
//////////////////////////////////////////////////////////////////////
class RcRowHead :
  public RcElem
{
public:

  /// @brief コンストラクタ
  RcRowHead(
    SizeType row, ///< [in] 行番号
    SizeType cost ///< [in] コスト
  ) : RcElem{BAD_ID, row, BAD_ID},
      mCost{cost}
  {
  }

  /// @brief デストラクタ
  ~RcRowHead() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数を返す．
  SizeType
  num() const
  {
    return mNum;
  }

  /// @brief コストを返す．
  SizeType
  cost() const
  {
    return mCost;
  }

  /// @brief 要素を挿入する．
  void
  insert(
    RcElem* elem ///< [in] 挿入する要素
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素数
  SizeType mNum{0};

  // コスト
  SizeType mCost;

};


//////////////////////////////////////////////////////////////////////
/// @class RcRowIter RcMatrix.h "RcMatrix.h"
/// @brief 行方向の反復子を表すクラス
//////////////////////////////////////////////////////////////////////
class RcRowIter
{
public:

  /// @brief コンストラクタ
  RcRowIter(
    const RcElem* elem ///< [in] 要素
  ) : mElem(elem)
  {
  }

  /// @brief デストラクタ
  ~RcRowIter() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素を取り出す．
  const RcElem*
  operator*() const
  {
    return mElem;
  }

  /// @brief 次の要素に移動する．
  RcRowIter&
  operator++()
  {
    mElem = mElem->right();
    return *this;
  }

  /// @brief 等価比較演算子
  bool
  operator==(
    const RcRowIter& right
  ) const
  {
    return mElem == right.mElem;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const RcRowIter& right
  ) const
  {
    return !operator==(right);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素
  const RcElem* mElem;

};


//////////////////////////////////////////////////////////////////////
/// @class RcRowList RcMatrix.h "RcMatrix.h"
/// @brief 行方向のリストを表すクラス
//////////////////////////////////////////////////////////////////////
class RcRowList
{
public:

  using iterator = RcRowIter;

public:

  /// @brief コンストラクタ
  RcRowList(
    const RcRowHead* head ///< [in] 先頭
  ) : mHead{head}
  {
  }

  /// @brief デストラクタ
  ~RcRowList() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数を返す．
  SizeType
  size() const
  {
    return mHead->num();
  }

  /// @brief 先頭の反復子を返す．
  iterator
  begin() const
  {
    return iterator{mHead->right()};
  }

  /// @brief 末尾の反復子を返す．
  iterator
  end() const
  {
    return iterator{mHead};
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭
  const RcRowHead* mHead;

};


//////////////////////////////////////////////////////////////////////
/// @class RcRowMergeIter RcRowMergeIter.h "RcRowMergeIter.h"
/// @brief 矩形と行のマージを行うための反復子
///
/// 矩形は行番号のリスト(vector<SizeType>)
/// 行は RcRowList を対象として，共通な列番号を返す．
///
/// ただし，終了状態がどちらかの反復子が終了しているということなので
/// c++ タイプの反復子では実装が難しい．
/// そこで java タイプの反復子にしている．
//////////////////////////////////////////////////////////////////////
class RcRowMergeIter
{
public:

  /// @brief コンストラクタ
  RcRowMergeIter(
    const vector<SizeType>& rect_row_list, ///< [in] 矩形の行のリスト
    const RcRowList& row_list              ///< [in] 行のリスト
  ) : mRectIter{rect_row_list.begin()},
      mRectEnd{rect_row_list.end()},
      mRowIter{row_list.begin()},
      mRowEnd{row_list.end()}
  {
    _sync();
  }

  /// @brief デストラクタ
  ~RcRowMergeIter() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素を返す．
  const RcElem*
  operator*() const
  {
    return *mRowIter;
  }

  /// @brief 次の要素に移動する．
  RcRowMergeIter&
  operator++()
  {
    if ( !is_end() ) {
      ++ mRectIter;
      ++ mRowIter;
      _sync();
    }
    return *this;
  }

  /// @brief 終了状態の時 true を返す．
  bool
  is_end() const
  {
    return mRectIter == mRectEnd || mRowIter == mRowEnd;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の共通点を探す．
  void
  _sync()
  {
    while ( !is_end() ) {
      auto col0 = *mRectIter;
      auto col1 = (*mRowIter)->col();
      if ( col0 < col1 ) {
	++ mRectIter;
      }
      else if ( col0 > col1 ) {
	++ mRowIter;
      }
      else { // col == col1
	break;
      }
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 矩形の反復子
  vector<SizeType>::const_iterator mRectIter;

  // 矩形の反復子の末尾
  vector<SizeType>::const_iterator mRectEnd;

  // 行の反復子
  RcRowIter mRowIter;

  // 行の反復子の末尾
  RcRowIter mRowEnd;

};


//////////////////////////////////////////////////////////////////////
/// @class RcColHead RcMatrix.h "RcMatrix.h"
/// @brief 列の先頭を表すクラス
//////////////////////////////////////////////////////////////////////
class RcColHead :
  public RcElem
{
public:

  /// @brief コンストラクタ
  RcColHead(
    SizeType col, ///< [in] 列番号
    SizeType cost ///< [in] コスト
  ) : RcElem{BAD_ID, BAD_ID, col},
      mCost{cost}
  {
  }

  /// @brief デストラクタ
  ~RcColHead() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数を返す．
  SizeType
  num() const
  {
    return mNum;
  }

  /// @brief コストを返す．
  SizeType
  cost() const
  {
    return mCost;
  }

  /// @brief 要素を挿入する．
  void
  insert(
    RcElem* elem ///< [in] 挿入する要素
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素数
  SizeType mNum{0};

  // コスト
  SizeType mCost;

};


//////////////////////////////////////////////////////////////////////
/// @class RcColIter RcColIter.h "RcMatrix.h"
/// @brief 列方向の反復子を表すクラス
//////////////////////////////////////////////////////////////////////
class RcColIter
{
public:

  /// @brief コンストラクタ
  RcColIter(
    const RcElem* elem ///< [in] 要素
  ) : mElem{elem}
  {
  }

  /// @brief デストラクタ
  ~RcColIter() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素を取り出す．
  const RcElem*
  operator*() const
  {
    return mElem;
  }

  /// @brief 次の要素に移動する．
  RcColIter&
  operator++()
  {
    mElem = mElem->down();
    return *this;
  }

  /// @brief 等価比較演算子
  bool
  operator==(
    const RcColIter& right
  ) const
  {
    return mElem == right.mElem;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const RcColIter& right
  ) const
  {
    return !operator==(right);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素
  const RcElem* mElem;

};


//////////////////////////////////////////////////////////////////////
/// @class RcColList RcMatrix.h "RcMatrix.h"
/// @brief 列方向のリストを表すクラス
//////////////////////////////////////////////////////////////////////
class RcColList
{
public:

  using iterator = RcColIter;

public:

  /// @brief コンストラクタ
  RcColList(
    const RcColHead* head ///< [in] 先頭
  ) : mHead{head}
  {
  }

  /// @brief デストラクタ
  ~RcColList() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数を返す．
  SizeType
  size() const
  {
    return mHead->num();
  }

  /// @brief 先頭の反復子を返す．
  iterator
  begin() const
  {
    return iterator{mHead->down()};
  }

  /// @brief 末尾の反復子を返す．
  iterator
  end() const
  {
    return iterator{mHead};
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭
  const RcColHead* mHead;

};


//////////////////////////////////////////////////////////////////////
/// @class RcColMergeIter RcMatrix.h "RcMatrix.h"
/// @brief 矩形と列のマージを行うための反復子
///
/// 矩形は列番号のリスト(vector<SizeType>)
/// 列は RcColList を対象として，共通な行番号を返す．
///
/// ただし，終了状態がどちらかの反復子が終了しているということなので
/// c++ タイプの反復子では実装が難しい．
/// そこで java タイプの反復子にしている．
//////////////////////////////////////////////////////////////////////
class RcColMergeIter
{
public:

  /// @brief コンストラクタ
  RcColMergeIter(
    const vector<SizeType>& rect_col_list, ///< [in] 矩形の列のリスト
    const RcColList& col_list              ///< [in] 列のリスト
  ) : mRectIter{rect_col_list.begin()},
      mRectEnd{rect_col_list.end()},
      mColIter{col_list.begin()},
      mColEnd{col_list.end()}
  {
    _sync();
  }

  /// @brief デストラクタ
  ~RcColMergeIter() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素を返す．
  const RcElem*
  operator*() const
  {
    return *mColIter;
  }

  /// @brief 次の要素に移動する．
  RcColMergeIter&
  operator++()
  {
    if ( !is_end() ) {
      ++ mRectIter;
      ++ mColIter;
      _sync();
    }
    return *this;
  }

  /// @brief 終了状態の時 true を返す．
  bool
  is_end() const
  {
    return mRectIter == mRectEnd || mColIter == mColEnd;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の共通点を探す．
  void
  _sync()
  {
    while ( !is_end() ) {
      auto row0 = *mRectIter;
      auto row1 = (*mColIter)->col();
      if ( row0 < row1 ) {
	++ mRectIter;
      }
      else if ( row0 > row1 ) {
	++ mColIter;
      }
      else { // row0 == row1
	break;
      }
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 矩形の反復子
  vector<SizeType>::const_iterator mRectIter;

  // 矩形の反復子の末尾
  vector<SizeType>::const_iterator mRectEnd;

  // 列の反復子
  RcColIter mColIter;

  // 列の反復子の末尾
  RcColIter mColEnd;

};


//////////////////////////////////////////////////////////////////////
/// @class RcMatrix RcMatrix.h "RcMatrix.h"
/// @brief 矩形被覆問題の行列を表すクラス
///
/// 行列と言っても行列演算は行わない．ただ単に二次元に配置された
/// 要素の集合を表す．
/// 通常，意味のある要素が疎らなのでスパース行列の手法を用いている．
///
/// 各要素は隣接する非ゼロの要素とリンクポインタでつながっているため，
/// 非ゼロ要素をたどる処理は効率良く行える．
/// 一方，行番号と列番号を指定して要素を取り出す処理は行や列上の要素数に
/// 比例した時間を必要とする．
/// 要素の追加も同様であるが，行方向は上から下に，列方向は左から右の順
/// に追加を行う限り O(1) で追加が行える．
///
/// 矩形被覆問題独特の特徴として，同じ価値を共有する複数の行列要素
/// があるという点がある．
/// そのため，行列要素に価値番号をもたせて，価値番号から価値を取得
/// するようにしている．
/// 被覆された要素の価値は0となるが，同じ価値番号を共有する要素の
/// 価値も同時に0となる．
//////////////////////////////////////////////////////////////////////
class RcMatrix
{
public:

  /// @brief 空のコンストラクタ
  RcMatrix()
  {
  }

  /// @brief コンストラクタ
  RcMatrix(
    const vector<SizeType>& row_costs, ///< [in] 行のコストの配列
    const vector<SizeType>& col_costs  ///< [in] 列のコストの配列
  )
  {
    auto row_size = row_costs.size();
    mRowHeadArray.reserve(row_size);
    for ( auto cost: row_costs ) {
      insert_row(cost);
    }
    auto col_size = col_costs.size();
    mColHeadArray.reserve(col_size);
    for ( auto cost: col_costs ) {
      insert_col(cost);
    }
  }

  /// @brief デストラクタ
  ~RcMatrix() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用のインターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 行を追加する．
  void
  insert_row(
    SizeType cost ///< [in] 行のコスト
  )
  {
    auto head = new RcRowHead(row_size(), cost);
    mRowHeadArray.push_back(std::unique_ptr<RcRowHead>{head});
  }

  /// @brief 列を追加する．
  void
  insert_col(
    SizeType cost ///< [in] 列のコスト
  )
  {
    auto head = new RcColHead(col_size(), cost);
    mColHeadArray.push_back(std::unique_ptr<RcColHead>{head});
  }

  /// @brief 価値を登録する．
  /// @return 価値番号を返す．
  SizeType
  add_value(
    SizeType value ///< [in] 価値
  )
  {
    auto id = mValueArray.size();
    mValueArray.push_back(value);
    return id;
  }

  /// @brief 価値を変更する．
  void
  set_value(
    SizeType val_id,   ///< [in] 価値番号
    SizeType new_value ///< [in] 新しい価値
  )
  {
    _check_val(val_id);
    mValueArray[val_id] = new_value;
  }

  /// @brief 要素を追加する．
  void
  add_elem(
    SizeType val_id, ///< [in] 価値番号
    SizeType row,    ///< [in] 行番号
    SizeType col     ///< [in] 列番号
  )
  {
    _check_val(val_id);
    _check_row(row);
    _check_col(col);
    auto elem = _new_elem(val_id, row, col);
    mRowHeadArray[row]->insert(elem);
    mColHeadArray[col]->insert(elem);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 情報取得用の外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 価値配列のサイズを得る．
  SizeType
  val_size() const
  {
    return mValueArray.size();
  }

  /// @brief 価値を返す．
  SizeType
  value(
    SizeType val_id ///< [in] 価値番号 ( 0 <= val_id < val_size() )
  ) const
  {
    _check_val(val_id);
    if ( val_id == BAD_ID ) {
      return 0;
    }
    return mValueArray[val_id];
  }

  /// @brief 行数を返す．
  SizeType
  row_size() const
  {
    return mRowHeadArray.size();
  }

  /// @brief 行のリストを取得する．
  RcRowList
  row_list(
    SizeType row ///< [in] 行番号
  ) const
  {
    return RcRowList{_row_head(row)};
  }

  /// @brief 行の要素数を取得する．
  SizeType
  row_num(
    SizeType row ///< [in] 行番号
  ) const
  {
    return _row_head(row)->num();
  }

  /// @brief 行のコストを取得する．
  SizeType
  row_cost(
    SizeType row ///< [in] 行番号
  ) const
  {
    return _row_head(row)->cost();
  }

#if 0
  /// @brief 1行の矩形を作る．
  RcRect
  row_rect(
    SizeType row ///< [in] 行番号
  ) const
  {
    vector<SizeType> col_list;
    col_list.reserve(row_num(row));
    for ( auto elem: row_list(row) ) {
      auto col = elem->col();
      col_list.push_back(col);
    }
    return RcRect{{row}, col_list};
  }
#endif

  /// @brief 列数を返す．
  SizeType
  col_size() const
  {
    return mColHeadArray.size();
  }

  /// @brief 列のリストを取得する．
  RcColList
  col_list(
    SizeType col ///< [in] 列番号
  ) const
  {
    return RcColList{_col_head(col)};
  }

  /// @brief 列の要素数を取得する．
  SizeType
  col_num(
    SizeType col ///< [in] 列番号
  ) const
  {
    return _col_head(col)->num();
  }

  /// @brief 列のコストを取得する．
  SizeType
  col_cost(
    SizeType col ///< [in] 列番号
  ) const
  {
    return _col_head(col)->cost();
  }

#if 0
  /// @brief 1列の矩形を作る．
  RcRect
  col_rect(
    SizeType col ///< [in] 列番号
  ) const
  {
    vector<SizeType> row_list;
    row_list.reserve(col_num(col));
    for ( auto elem: col_list(col) ) {
      auto row = elem->row();
      row_list.push_back(row);
    }
    return RcRect{row_list, {col}};
  }

  /// @brief 行の要素の価値を求める．
  SizeType
  row_value(
    SizeType row ///< [in] 行番号
  ) const
  {
    SizeType ans = 0;
    for ( auto elem: row_list(row) ) {
      auto vid = elem->val_id();
      ans += value(vid);
    }
    return ans;
  }

  /// @brief 列の要素の価値を求める．
  SizeType
  col_value(
    SizeType col ///< [in] 列番号
  ) const
  {
    SizeType ans = 0;
    for ( auto elem: col_list(col) ) {
      auto vid = elem->val_id();
      ans += value(vid);
    }
    return ans;
  }
#endif

  /// @brief 矩形がカバーしている価値番号のリストを求める．
  vector<SizeType>
  rect_vid_list(
    const RcRect& rect ///< [in] 矩形
  ) const
  {
    auto& rect_row_list = rect.row_list();
    auto& rect_col_list = rect.col_list();
    vector<SizeType> vid_list;
    vid_list.reserve(rect.row_size() * rect.col_size());
    for ( auto row: rect.row_list() ) {
      auto iter = RcRowMergeIter{rect_col_list, row_list(row)};
      for ( ; !iter.is_end(); ++ iter ) {
	auto elem = *iter;
	auto vid = elem->val_id();
	vid_list.push_back(vid);
      }
    }
    return vid_list;
  }

  /// @brief 最大価値を持つ矩形を列挙する．
  vector<RcRect>
  enum_max_rects() const;

  /// @brief 矩形の価値を求める．
  int
  rect_value(
    const RcRect& rect ///< [in] 矩形
  ) const
  {
    int ans = 0;
    for ( auto vid: rect_vid_list(rect) ) {
      ans += value(vid);
    }
    for ( auto row: rect.row_list() ) {
      ans -= row_cost(row);
    }
    for ( auto col: rect.col_list() ) {
      ans -= col_cost(col);
    }
    return ans;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素を確保する．
  RcElem*
  _new_elem(
    SizeType val_id, ///< [in] 価値番号
    SizeType row,    ///< [in] 行番号
    SizeType col     ///< [in] 列番号
  )
  {
    auto elem = new RcElem(val_id, row, col);
    mElemList.push_back(std::unique_ptr<RcElem>{elem});
    return elem;
  }

  /// @brief 行のヘッダを取得する．
  const RcRowHead*
  _row_head(
    SizeType row ///< [in] 行番号
  ) const
  {
    _check_row(row);
    return mRowHeadArray[row].get();
  }

  /// @brief 列のヘッダを取得する．
  const RcColHead*
  _col_head(
    SizeType col ///< [in] 列番号
  ) const
  {
    _check_col(col);
    return mColHeadArray[col].get();
  }

  /// @brief 価値番号が適正かチェックする．
  void
  _check_val(
    SizeType val_id
  ) const
  {
    if ( val_id != BAD_ID && val_id >= val_size() ) {
      throw std::out_of_range{"val_id is out of range"};
    }
  }

  /// @brief 行番号が適正かチェックする．
  void
  _check_row(
    SizeType row
  ) const
  {
    if ( row >= mRowHeadArray.size() ) {
      throw std::out_of_range{"row is out of range"};
    }
  }

  /// @brief 列番号が適正かチェックする．
  void
  _check_col(
    SizeType col
  ) const
  {
    if ( col >= mColHeadArray.size() ) {
      throw std::out_of_range{"col is out of range"};
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 価値の配列
  vector<SizeType> mValueArray;

  // 要素の所有権管理用のリスト
  vector<std::unique_ptr<RcElem>> mElemList;

  // 行のヘッダを表す要素の配列
  vector<std::unique_ptr<RcRowHead>> mRowHeadArray;

  // 列のヘッダを表す要素の配列
  vector<std::unique_ptr<RcColHead>> mColHeadArray;

};

END_NAMESPACE_YM_RC

#endif // RCMATRIX_H
