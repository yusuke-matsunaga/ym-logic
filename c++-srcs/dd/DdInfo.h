#ifndef DDINFO_H
#define DDINFO_H

/// @file DdInfo.h
/// @brief DdInfo のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
/// @class DdInfo DdInfo.h "DdInfo.h"
/// @grief ノードの情報を表す構造体
///
/// 内容の出力のために用いられる．
//////////////////////////////////////////////////////////////////////
class DdInfo
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  DdInfo(
    SizeType level, ///< [in] レベル
    SizeType edge0, ///< [in] 0枝の情報
    SizeType edge1  ///< [in] 1枝の情報
  ) : mLevel{level},
      mEdge0{edge0},
      mEdge1{edge1}
  {
  }

  /// @brief デストラクタ
  ~DdInfo() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief レベルを返す．
  SizeType
  level() const
  {
    return mLevel;
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

  /// @brief 内容を見やすい形式で出力する．
  static
  void
  display(
    std::ostream& s,                             ///< [in] 出力ストリーム
    const std::vector<SizeType>& root_edge_list, ///< [in] 根の枝のリスト
    const std::vector<DdInfo>& node_list         ///< [in] ノード情報のリスト
  );

  // @brief 構造を表す整数配列を作る．
  static
  std::vector<SizeType>
  rep_data(
    const std::vector<SizeType>& root_edge_list, ///< [in] 根の枝のリスト
    const std::vector<DdInfo>& node_list	    ///< [in] ノード情報のリスト
  );


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

  // レベル
  SizeType mLevel;

  // 0枝の情報
  SizeType mEdge0;

  // 1枝の情報
  SizeType mEdge1;

};

END_NAMESPACE_YM_DD

#endif // DDINFO_H
