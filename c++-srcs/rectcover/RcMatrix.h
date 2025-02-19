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

  /// @brief コンストラクタ
  RcMatrix(
    SizeType row_size, ///< [in] 行数
    SizeType col_size  ///< [in] 列数
  );

  /// @brief デストラクタ
  ~RcMatrix() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用のインターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素を追加する．
  /// @return 要素番号を返す．
  SizeType
  add_elem(
    SizeType row, ///< [in] 行番号
    SizeType col  ///< [in] 列番号
  );

  /// @brief 行方向に要素のリストを追加する．
  /// @return 追加された要素番号のリストを返す．
  vector<SizeType>
  add_row_elem(
    SizeType row,                    ///< [in] 行番号
    const vector<SizeType>& col_list ///< [in] 列番号のリスト
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
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


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素を確保する．
  RcElem*
  _new_elem(
    SizeType row, ///< [in] 行番号
    SizeType col  ///< [in] 列番号
  );

  /// @brief 行方向に要素を挿入する．
  void
  _row_insert(
    RcElem* row_head, ///< [in] 行のヘッダ
    RcElem* elem      ///< [in] 追加する要素
  );

  /// @brief 行方向の接続を行う．
  void
  _row_connect(
    RcElem* left, ///< [in] 左の要素
    RcElem* right ///< [in] 右の要素
  )
  {
    left->mRight = right;
    right->mLeft = left;
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
    SizeType down ///< [in] 下の要素
  )
  {
    up->mDown = down;
    down->mUp = up;
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

  // 列のヘッダを表す要素の配列
  vector<RcElem*> mColHeadArray;

};

END_NAMESPACE_YM_RC

#endif // RCMATRIX_H
