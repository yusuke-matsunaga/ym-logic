#ifndef SOP2TV_H
#define SOP2TV_H

/// @file Sop2Tv.h
/// @brief Sop2Tv のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/SopCover.h"
#include "ym/SopCube.h"
#include "ym/TvFunc.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class Sop2Tv Sop2Tv.h "Sop2Tv.h"
/// @brief SopCover/SopCube を TvFunc に変換するクラス
///
/// 実はただの関数群
//////////////////////////////////////////////////////////////////////
class Sop2Tv
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief SopCover を TvFunc に変換する．
  static
  TvFunc
  to_tvfunc(
    const SopCover& cover ///< [in] 対象のカバー
  );

  /// @brief SopCube のリストを TvFunc に変換する．
  static
  TvFunc
  to_tvfunc(
    const vector<SopCube>& cube_list ///< [in] 対象のキューブのリスト
  );

  /// @brief SopCube を TvFunc に変換する．
  static
  TvFunc
  to_tvfunc(
    const SopCube& cube ///< [in] 対象のキューブ
  );

};

END_NAMESPACE_YM

#endif // SOP2TV_H
