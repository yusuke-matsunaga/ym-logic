#ifndef CKMATRIX_H
#define CKMATRIX_H

/// @file CkMatrix.h
/// @brief CkMatrix のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/KIX.h"
#include "RcMatrix.h"


BEGIN_NAMESPACE_YM_RC

//////////////////////////////////////////////////////////////////////
/// @class CkMatrix CkMatrix.h "CkMatrix.h"
/// @brief コカーネル・カーネルキューブ行列を表すクラス
///
/// メインの処理はコンストラクタで行われる．
/// 残りのインターフェースは行，列，要素に対応した情報の取得
//////////////////////////////////////////////////////////////////////
class CkMatrix :
  public RcMatrix
{
public:

  /// @brief 価値に関する情報
  struct ValueInfo {
    SizeType cover_id; ///< カバー番号
    SizeType cube_id;  ///< キューブ番号
  };

  /// @brief 行に関する情報
  struct RowInfo {
    SizeType cover_id; ///< カバー番号
    SopCube ccube;     ///< コカーネルキューブ
  };

  /// @brief 列に関する情報
  struct ColInfo {
    vector<Literal> cube; ///< カーネルキューブを表すリテラルのリスト
  };


public:

  /// @brief コンストラクタ
  CkMatrix(
    const vector<KIX::CoverInfo>& cover_list ///< [in] カバー情報のリスト
  );

  /// @brief デストラクタ
  ~CkMatrix() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 価値番号に対応したカバー番号とキューブ番号を返す．
  const ValueInfo&
  value_info(
    SizeType val_id ///< [in] 価値番号
  ) const
  {
    if ( val_id >= mValueArray.size() ) {
      throw std::out_of_range{"val_id is out of range"};
    }
    return mValueArray[val_id];
  }

  /// @brief 行番号に対応したカバー番号とキューブを返す．
  const RowInfo&
  row_info(
    SizeType row_id ///< [in] 行番号
  ) const
  {
    if ( row_id >= mRowArray.size() ) {
      throw std::out_of_range{"row_id is out of range"};
    }
    return mRowArray[row_id];
  }

  /// @brief 列番号に対応したキューブの情報を返す．
  const ColInfo&
  col_info(
    SizeType col_id ///< [in] 列番号
  ) const
  {
    if ( col_id >= mColArray.size() ) {
      throw std::out_of_range{"col_id is out of range"};
    }
    return mColArray[col_id];
  }

  /// @brief 内容を出力する．
  void
  print(
    ostream& s ///< [in] 出力先のストリーム
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // もとのカバー情報のリスト
  vector<KIX::CoverInfo> mCoverList;

  // 行列の価値番号をキーにしてカバー番号とキューブ番号を保持する配列
  vector<ValueInfo> mValueArray;

  // 列番号をキーにして対応するキューブ（を表すリテラルのリスト）を保持する配列
  vector<ColInfo> mColArray;

  // 行番号をキーにして対応するカバー番号とコカーネルキューブを保持する配列
  vector<RowInfo> mRowArray;

};

// CkMatrix::ValueInfo の等価比較演算
inline
bool
operator==(
  const CkMatrix::ValueInfo& a,
  const CkMatrix::ValueInfo& b
)
{
  return a.cover_id == b.cover_id && a.cube_id == b.cube_id;
}

// CkMatrix::ValueInfo の非等価比較演算
inline
bool
operator!=(
  const CkMatrix::ValueInfo& a,
  const CkMatrix::ValueInfo& b
)
{
  return !operator==(a, b);
}

END_NAMESPACE_YM_RC

BEGIN_NAMESPACE_STD

// CkMatrix::ValueInfo をキーにしたハッシュ関数クラスの定義
template <>
struct hash<YM_NAMESPACE::nsRectCover::CkMatrix::ValueInfo>
{
  SizeType
  operator()(
    const YM_NAMESPACE::nsRectCover::CkMatrix::ValueInfo& v
  ) const
  {
    return v.cover_id * 1048573 + v.cube_id;
  }

};

END_NAMESPACE_STD

#endif // CKMATRIX_H
