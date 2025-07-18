#ifndef TV2SOP_H
#define TV2SOP_H

/// @file Tv2Sop.h
/// @brief Tv2Sop のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/TvFunc.h"
#include "ym/SopCube.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class Tv2Sop Tv2Sop.h "Tv2Sop.h"
/// @brief TvFunc に対して積和形論理式に関連した処理を行うクラス
///
/// 実はただの関数群の定義
//////////////////////////////////////////////////////////////////////
class Tv2Sop
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 主項を列挙する．
  static
  std::vector<SopCube>
  all_primes(
    const TvFunc& f ///< [in] 対象の関数
  );

  /// @brief 主項を列挙する．
  static
  std::vector<SopCube>
  all_primes(
    const TvFunc& f, ///< [in] 対象の関数
    const TvFunc& dc ///< [in] ドントケアの関数
  )
  {
    // 主項にとってはドントケアはオンセットと同じ
    return all_primes(f | dc);
  }

  /// @brief 単純なシャノン展開を行って非冗長積和形を求める．
  static
  std::vector<SopCube>
  isop(
    const TvFunc& f ///< [in] 対象の関数
  );

  /// @brief 単純なシャノン展開を行って非冗長積和形を求める．
  ///
  /// dc の入力数は f の入力数と等しくなければならない．
  static
  std::vector<SopCube>
  isop(
    const TvFunc& f, ///< [in] 対象の関数
    const TvFunc& dc ///< [in] ドントケアの関数
  );

};

END_NAMESPACE_YM

#endif // TV2SOP_H
