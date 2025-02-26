#ifndef FINDRECT_H
#define FINDRECT_H

/// @file FindRect.h
/// @brief FindRect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "RcMatrix.h"
#include <random>


BEGIN_NAMESPACE_YM_RC

//////////////////////////////////////////////////////////////////////
/// @class FindRect FindRect.h "FindRect.h"
/// @brief 矩形を求めるためのクラス
//////////////////////////////////////////////////////////////////////
class FindRect
{
public:

  /// @brief コンストラクタ
  FindRect() = default;

  /// @brief デストラクタ
  ~FindRect() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 極大矩形を求める．
  RcRect
  operator()(
    const RcMatrix& matrix ///< [in] 対象の行列
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 価値が最大の行を選ぶ．
  RcRect
  find_row(
    const RcMatrix& matrix,          ///< [in] 対象の行列
    const vector<SizeType>& row_list ///< [in] 行の候補のリスト
    = {}
  );

  /// @brief 価値が最大となるように行を追加する．
  /// @return 変化がなくなったら false を返す．
  ///
  /// 新しい行を追加することで価値が減少する場合でも
  /// その中で価値が最大のものを選ぶ．
  bool
  row_expand(
    const RcMatrix& matrix, ///< [in] 対象の行列
    RcRect& rect            ///< [inout] 対象の矩形
  );

  /// @brief 価値が最大の列を選ぶ．
  RcRect
  find_col(
    const RcMatrix& matrix,          ///< [in] 対象の行列
    const vector<SizeType>& col_list ///< [in] 列の候補のリスト
    = {}
  );

  /// @brief 価値が最大となるように列を追加する．
  /// @return 変化がなくなったら false を返す．
  ///
  /// 新しい列を追加することで価値が減少する場合でも
  /// その中で価値が最大のものを選ぶ．
  bool
  col_expand(
    const RcMatrix& matrix, ///< [in] 対象の行列
    RcRect& rect            ///< [inout] 対象の矩形
  );

  /// @brief ランダムに選ぶ．
  template<typename T>
  T
  random_choice(
    const vector<T>& cand_list ///< [in] 候補のリスト
  )
  {
    auto n = cand_list.size();
    auto rd = std::uniform_int_distribution<SizeType>(0, n - 1);
    return cand_list[rd(mRandGen)];
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 乱数生成器
  std::mt19937 mRandGen;

};

END_NAMESPACE_YM_RC

#endif // FINDRECT_H
