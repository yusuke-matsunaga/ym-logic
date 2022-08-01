#ifndef BDDINFO_H
#define BDDINFO_H

/// @file BddInfo.h
/// @brief BddInfo のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bdd_nsdef.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @class BddInfo BddInfo.h "BddInfo.h"
/// @grief ノードの情報を表す構造体
///
/// 枝は最下位ビットが反転属性を表し，
/// 残りがノード番号を表す．
//////////////////////////////////////////////////////////////////////
struct BddInfo
{
  SizeType id;    ///< ノード番号
  SizeType index; ///< インデックス
  SizeType edge0; ///< 0枝
  SizeType edge1; ///< 1枝

  /// @brief 枝の情報からノード番号を取り出す．
  static
  SizeType
  edge2node(
    SizeType edge ///< [in] 枝
  )
  {
    return edge >> 1;
  }

  /// @brief 枝の情報から反転フラグを取り出す．
  static
  bool
  edge2inv(
    SizeType edge ///< [in] 枝
  )
  {
    return static_cast<bool>(edge & 1U);
  }

};


END_NAMESPACE_YM_BDD

#endif // BDDINFO_H
