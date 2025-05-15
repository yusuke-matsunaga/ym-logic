#ifndef PATMGR_H
#define PATMGR_H

/// @file PatMgr.h
/// @brief PatMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/aig.h"


BEGIN_NAMESPACE_YM_AIG

class PatNode;

//////////////////////////////////////////////////////////////////////
/// @class PatMgr PatMgr.h "PatMgr.h"
/// @brief Rewrite 用のパタンマネージャ
//////////////////////////////////////////////////////////////////////
class PatMgr
{
public:

  /// @brief コンストラクタ
  PatMgr();

  /// @brief デストラクタ
  ~PatMgr() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 関数をキーにして対応するパタンの根のノードをリストを返す．
  const vector<const PatNode*>&
  pat_list(
    SizeType tv ///< [in] 関数の真理値表を表す数値
  ) const
  {
    if ( tv >= mPatListArray.size() ) {
      throw std::invalid_argument{"tv is out of range"};
    }
    return mPatListArray[tv];
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードを追加する．
  void
  add_node(
    bool xor_flag,         ///< [in] XORフラグ
    std::uint16_t tv,      ///< [in] 真理値表を表すパタン
    std::uint8_t size,     ///< [in] サイズ
    std::uint8_t level,    ///< [in] レベル
    const PatNode* child0, ///< [in] 左の子供
    bool inv0,             ///< [in] 左の子供の反転フラグ
    const PatNode* child1, ///< [in] 右の子供
    bool inv1              ///< [in] 右の子供の反転フラグ
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードの本体の配列
  vector<PatNode> mNodeArray;

  // 真理値表をキーにして根のノードのリストを保持する配列
  vector<vector<const PatNode*>> mPatListArray;

};

END_NAMESPACE_YM_AIG

#endif // PATMGR_H
