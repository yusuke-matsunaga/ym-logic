#ifndef ENUMRECT_H
#define ENUMRECT_H

/// @file EnumRect.h
/// @brief EnumRect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "RcMatrix.h"


BEGIN_NAMESPACE_YM_RC

//////////////////////////////////////////////////////////////////////
/// @class EnumRect EnumRect.h "EnumRect.h"
/// @brief 矩形を列挙するためのクラス
//////////////////////////////////////////////////////////////////////
class EnumRect
{
public:

  /// @brief コンストラクタ
  EnumRect(
    const RcMatrix& matrix
  ) : mMatrix{matrix}
  {
  }

  /// @brief デストラクタ
  ~EnumRect() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 極大矩形を求める．
  std::vector<RcRect>
  operator()();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 矩形を列挙する下請け関数
  void
  enum_sub(
    const RcRect& src_rect, ///< [in] 元の矩形
    SizeType row0           ///< [in] 探索を開始する行番号
  );

  /// @brief 与えられた列集合に含まれる行集合を求める．
  /// @return row0 よりも小さい行が含まれていたら true を返す．
  bool
  expand(
    SizeType row0,                         ///< [in] 基準となる行番号
    const std::vector<SizeType>& col_list, ///< [in] 列集合を表すリスト
    std::vector<SizeType>& row_list        ///< [out] 結果の行集合を格納するリスト
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の行列
  const RcMatrix& mMatrix;

  // 現在の最大値
  int mMaxValue;

  // 最大の価値を持つ矩形のリスト
  std::vector<RcRect> mMaxList;

};

END_NAMESPACE_YM_RC

#endif // ENUMRECT_H
