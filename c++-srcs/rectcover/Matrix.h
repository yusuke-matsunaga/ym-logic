#ifndef MATRIX_H
#define MATRIX_H

/// @file Matrix.h
/// @brief Matrix のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"


BEGIN_NAMESPACE_RECTCOVER

//////////////////////////////////////////////////////////////////////
/// @class MxRow Matrix.h "Matrix.h"
/// @brief 行を表すクラス
//////////////////////////////////////////////////////////////////////
class MxRow
{
public:

  /// @brief コンストラクタ
  MxRow();

  /// @brief デストラクタ
  ~MxRow() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 行番号を返す．
  SizeType
  row_id() const
  {
    return mRowId;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 行番号
  SizeType mRowId;

  // 前の要素へのリンク
  MxRow* mPrev;

  // 次の要素へのリンク
  MxRow* mNext;

  // 行の要素のダミーヘッダ
  MxElem mDummyHead;

};


//////////////////////////////////////////////////////////////////////
/// @class MxCol MxCol.h "MxCol.h"
/// @brief 列を表すクラス
//////////////////////////////////////////////////////////////////////
class MxCol
{
public:

  /// @brief コンストラクタ
  MxCol();

  /// @brief デストラクタ
  ~MxCol() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 列番号を返す．
  col_id() const
  {
    return mColId;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 列番号
  SizeType mColId;

  // 前の要素へのリンク
  MxCol* mPrev;

  // 次の要素へのリンク
  MxCol* mNext;

  // 列の要素のダミーヘッダ
  MxElem mDummyHead;

};


//////////////////////////////////////////////////////////////////////
/// @class MxElem Matrix.h "Matrix.h"
/// @brief 行列要素を表すクラス
//////////////////////////////////////////////////////////////////////
class MxElem
{
public:

  /// @brief コンストラクタ
  MxElem(
    SizeType row_pos, ///< [in] 行番号
    SizeType col_pos, ///< [in] 列番号
    SizeType id       ///< [in] ID番号
  ) : mRowPos{row_pos},
      mColPos{col_pos},
      mId{id}
  {
  }

  /// @brief デストラクタ
  ~MxElem() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 行番号を返す．
  SizeType
  row_pos() const
  {
    return mRowPos;
  }

  /// @brief 列番号を返す．
  SizeType
  col_pos() const
  {
    return mColPos;
  }

  /// @brief ID番号を返す．
  SizeType
  id() const
  {
    return mId;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  //  行番号
  SizeType mRowPos;

  // 列番号
  SizeType mColPos;

  // ID番号
  SizeType mId;

  // 行方向の前の要素へのリンク
  MxElem* mLeft{nullptr};

  // 行方向の次の要素へのリンク
  MxElem* mRight{nullptr};

  // 列方向の前の要素へのリンク
  MxElem* mUp{nullptr};

  // 列方向の次の要素へのリンク
  MxElem* mDown{nullptr};

};


//////////////////////////////////////////////////////////////////////
/// @class Matrix Matrix.h "Matrix.h"
/// @brief 行列を表すクラス
//////////////////////////////////////////////////////////////////////
class Matrix
{
public:

  using ElemType = std::tuple<SizeType, SizeType, SizeType>;

public:

  /// @brief コンストラクタ
  Matrix(
    SizeType row_size,                ///< [in] 行のサイズ
    SizeType col_size,                ///< [in] 列のサイズ
    const vector<ElemType>& elem_list ///< [in] 要素のリスト
    = {}
  );

  /// @brief デストラクタ
  ~Matrix() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素を追加する．
  void
  add_elem(
    SizeType row_pos, ///< [in] 行位置
    SizeType col_pos, ///< [in] 列位置
    SizeType id       ///< [in] ID番号
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 行の配列
  vector<std::unique_ptr<MxRow>> mRowArray;

  // 列の配列
  vector<std::unique_ptr<MxCol>> mColArray;

  // 要素の所有権のリスト
  vector<std::unique_ptr<MxElem>> mElemList;

};

END_NAMESPACE_RECTCOVER

#endif // MATRIX_H
