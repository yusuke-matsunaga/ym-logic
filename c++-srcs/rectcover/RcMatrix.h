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

//////////////////////////////////////////////////////////////////////
/// @class RcElem RcMatrix.h "RcMatrix.h"
/// @brief RcMatrix の要素を表すクラス
//////////////////////////////////////////////////////////////////////
class RcElem
{
  friend class RcMatrix;

public:

  /// @brief コンストラクタ
  RcElem(
    SizeType id,  ///< [in] ID番号
    SizeType row, ///< [in] 行番号
    SizeType col  ///< [in] 列番号
  ) : mId{id},
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

  /// @brief ID番号を返す．
  SizeType
  id() const
  {
    return mId;
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

  // ID番号
  SizeType mId;

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
    mRowCostArray.reserve(row_size);
    for ( auto cost: row_costs ) {
      insert_row(cost);
    }
    auto col_size = col_costs.size();
    mColHeadArray.reserve(col_size);
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
    auto head = _new_elem(row_size(), -1);
    mRowHeadArray.push_back(head);
    mRowCostArray.push_back(cost);
  }

  /// @brief 列を追加する．
  void
  insert_col(
    SizeType cost ///< [in] 列のコスト
  )
  {
    auto head = _new_elem(-1, col_size());
    mColHeadArray.push_back(head);
    mColCostArray.push_back(cost);
  }

  /// @brief 要素を追加する．
  void
  add_elem(
    SizeType id,  ///< [in] 要素番号
    SizeType row, ///< [in] 行番号
    SizeType col  ///< [in] 列番号
  )
  {
    _check_row(row);
    _check_col(col);
    auto elem = _new_elem(id, row, col);
    auto row_head = mRowHeadArray[row];
    _row_insert(row_head, elem);
    auto col_head = mColHeadArray[col];
    _col_insert(col_head, elem);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 情報取得用の外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 行数を返す．
  SizeType
  row_size() const
  {
    return mRowHeadArray;
  }

  /// @brief 列数を返す．
  SizeType
  col_size() const
  {
    return mColHeadArray;
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

  /// @brief 列のヘッダを取得する．
  const RcElem*
  col_head(
    SizeType col ///< [in] 列番号
  ) const
  {
    _check_col(col);
    return mColHeadArray[col];
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素を確保する．
  RcElem*
  _new_elem(
    SizeType row, ///< [in] 行番号
    SizeType col  ///< [in] 列番号
  )
  {
    auto elem = new RcElem(id, row, col);
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
    SizeType up,  ///< [in] 上の要素
    SizeType elem ///< [in] 挿入する要素
  )
  {
    auto down = up->mDown;
    up->mDown = elem;
    elem->mUp = up;
    elem->mDown = down;
    down->mUp = elem;
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

  // 要素の所有権管理用のリスト
  vector<std::unique_ptr<RcElem>> mElemList;

  // 行のヘッダを表す要素の配列
  vector<RcElem*> mRowHeadArray;

  // 行のコストの配列
  vector<SizeType> mRowCostArray;

  // 列のヘッダを表す要素の配列
  vector<RcElem*> mColHeadArray;

  // 列のコストの配列
  vector<SizeType> mColCostArray;

};

END_NAMESPACE_YM_RC

#endif // RCMATRIX_H
