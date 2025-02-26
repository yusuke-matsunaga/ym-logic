#ifndef KIX_H
#define KIX_H

/// @file KIX.h
/// @brief KIX のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/SopCover.h"


BEGIN_NAMESPACE_YM_RC

//////////////////////////////////////////////////////////////////////
/// @class KIX KIX.h "KIX.h"
/// @brief Kernel Intersection Extraction を行うクラス
///
/// 実際にはただの関数の集まり
//////////////////////////////////////////////////////////////////////
class KIX
{
public:

  /// @brief カバーの情報
  struct CoverInfo
  {
    SopCover cover;          ///< カバー
    vector<SizeType> inputs; ///< 入力番号のリスト
  };


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 共通な論理式を括りだす．
  /// @return 結果のカバーのリストを返す．
  ///
  /// 括りだした新しいカバーは結果のカバーの末尾に追加される．
  /// 新しいカバーの出力に対応する入力はもとの入力の後ろに追加される．
  static
  vector<CoverInfo>
  extract(
    const vector<CoverInfo>& cover_list ///< [in] カバー情報のリスト
  );

};

END_NAMESPACE_YM_RC

#endif // KIX_H
