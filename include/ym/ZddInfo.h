#ifndef ZDDINFO_H
#define ZDDINFO_H

/// @file ZddInfo.h
/// @brief ZddInfo のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
/// @class ZddInfo ZddInfo.h "ZddInfo.h"
/// @grief ノードの情報を表す構造体
///
/// 内容の出力のために用いられる．
//////////////////////////////////////////////////////////////////////
class ZddInfo
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  ZddInfo(
    SizeType index, ///< [in] インデックス
    SizeType edge0, ///< [in] 0枝の情報
    SizeType edge1  ///< [in] 1枝の情報
  ) : mIndex{index},
      mEdge0{edge0},
      mEdge1{edge1}
  {
  }

  /// @brief デストラクタ
  ~ZddInfo() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief インデックスを返す．
  SizeType
  index() const
  {
    return mIndex;
  }

  /// @brief 0枝の情報を返す．
  SizeType
  edge0() const
  {
    return mEdge0;
  }

  /// @brief 0枝のノード番号を返す．
  SizeType
  edge0_node() const
  {
    return edge2node(mEdge0);
  }

  /// @brief 0枝の極性(反転フラグ)を返す．
  bool
  edge0_inv() const
  {
    return edge2inv(mEdge0);
  }

  /// @brief 1枝の情報を返す．
  SizeType
  edge1() const
  {
    return mEdge1;
  }

  /// @brief 1枝のノード番号を返す．
  SizeType
  edge1_node() const
  {
    return edge2node(mEdge1);
  }

  /// @brief 1枝の極性(反転フラグ)を返す．
  bool
  edge1_inv() const
  {
    return edge2inv(mEdge1);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 枝の情報の変換用
  //////////////////////////////////////////////////////////////////////

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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // インデックス
  SizeType mIndex;

  // 0枝の情報
  SizeType mEdge0;

  // 1枝の情報
  SizeType mEdge1;

};

END_NAMESPACE_YM_ZDD

#endif // ZDDINFO_H
