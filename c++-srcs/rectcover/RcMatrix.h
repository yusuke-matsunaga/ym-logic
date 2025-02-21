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


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


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
    const vector<SizeType> row_costs, ///< [in] 行のコストの配列
    const vector<SizeType> col_costs  ///< [in] 列のコストの配列
  )
  {
    auto row_size = row_costs.size();
    mRowHeadArray.reserve(row_size);
    mRowNumArray.reserve(row_size);
    mRowCostArray.reserve(row_size);
    for ( auto cost: row_costs ) {
      insert_row(cost);
    }
    auto col_size = col_costs.size();
    mColHeadArray.reserve(col_size);
    mColNumArray.reserve(col_size);
    mColCostArray.reserve(col_size);
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
    auto head = _new_elem(BAD_ID, row_size(), BAD_ID);
    mRowHeadArray.push_back(head);
    mRowNumArray.push_back(0);
    mRowCostArray.push_back(cost);
  }

  /// @brief 列を追加する．
  void
  insert_col(
    SizeType cost ///< [in] 列のコスト
  )
  {
    auto head = _new_elem(BAD_ID, BAD_ID, col_size());
    mColHeadArray.push_back(head);
    mColNumArray.push_back(0);
    mColCostArray.push_back(cost);
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
    auto row_head = mRowHeadArray[row];
    _row_insert(row_head, elem);
    ++ mRowNumArray[row];
    auto col_head = mColHeadArray[col];
    _col_insert(col_head, elem);
    ++ mColNumArray[col];
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

  /// @brief 行のヘッダを取得する．
  const RcElem*
  row_head(
    SizeType row ///< [in] 行番号
  ) const
  {
    _check_row(row);
    return mRowHeadArray[row];
  }

  /// @brief 行の要素数を取得する．
  SizeType
  row_num(
    SizeType row ///< [in] 行番号
  ) const
  {
    _check_row(row);
    return mRowNumArray[row];
  }

  /// @brief 行のコストを取得する．
  SizeType
  row_cost(
    SizeType row ///< [in] 行番号
  ) const
  {
    _check_row(row);
    return mRowCostArray[row];
  }

  /// @brief 列数を返す．
  SizeType
  col_size() const
  {
    return mColHeadArray.size();
  }

  /// @brief 列のヘッダを取得する．
  const RcElem*
  col_head(
    SizeType col ///< [in] 列番号
  ) const
  {
    _check_col(col);
    return mColHeadArray[col];
  }

  /// @brief 列の要素数を取得する．
  SizeType
  col_num(
    SizeType col ///< [in] 列番号
  ) const
  {
    _check_col(col);
    return mColNumArray[col];
  }

  /// @brief 列のコストを取得する．
  SizeType
  col_cost(
    SizeType col ///< [in] 列番号
  ) const
  {
    _check_col(col);
    return mColCostArray[col];
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

  /// @brief 行方向に要素を挿入する．
  void
  _row_insert(
    RcElem* row_head, ///< [in] 行のヘッダ
    RcElem* elem      ///< [in] 追加する要素
  );

  /// @brief 行方向の挿入を行う．
  void
  _row_connect(
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

  /// @brief 列方向に要素を挿入する．
  void
  _col_insert(
    RcElem* col_head, ///< [in] 列のヘッダ
    RcElem* elem      ///< [in] 追加する要素
  );

  /// @brief 列方向の接続を行う．
  void
  _col_connect(
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
  vector<RcElem*> mRowHeadArray;

  // 行の要素数の配列
  vector<SizeType> mRowNumArray;

  // 行のコストの配列
  vector<SizeType> mRowCostArray;

  // 列のヘッダを表す要素の配列
  vector<RcElem*> mColHeadArray;

  // 列の要素数の配列
  vector<SizeType> mColNumArray;

  // 列のコストの配列
  vector<SizeType> mColCostArray;

};

END_NAMESPACE_YM_RC

#endif // RCMATRIX_H
