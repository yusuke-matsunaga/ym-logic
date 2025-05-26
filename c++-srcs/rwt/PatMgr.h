#ifndef PATMGR_H
#define PATMGR_H

/// @file PatMgr.h
/// @brief PatMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/aig.h"
#include "Npn4.h"


BEGIN_NAMESPACE_YM_AIG

class PatNode;

//////////////////////////////////////////////////////////////////////
/// @brief 置き換え用のパタンを表す構造体
///
/// - 葉のノードは 1 〜 4 番のノード
/// - npn の変換を施すことで代表関数になる．
//////////////////////////////////////////////////////////////////////
struct PatGraph
{
  const PatNode* root; ///< 根のノード
  Npn4 npn;            ///< 正規化用の変換
};


//////////////////////////////////////////////////////////////////////
/// @class PatMgr PatMgr.h "PatMgr.h"
/// @brief Rewrite 用のパタンマネージャ
///
/// 関数の真理値表を表す数値(16ビット)をキーとして対応するパタンのリストを返す．
/// ただし，内部では Npn4 に基づく正規化を行っておく．
//////////////////////////////////////////////////////////////////////
class PatMgr
{
public:

  // 4入力関数の真理値表を表す型
  using Tv4 = Npn4::Tv4;

public:

  /// @brief コンストラクタ
  PatMgr();

  /// @brief デストラクタ
  ~PatMgr() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 関数をキーにして対応するパタンのリストを返す．
  const std::vector<PatGraph>&
  get_pat(
    Tv4 tv,   ///< [in] 関数の真理値表を表す数値
    Npn4& npn ///< [out] 代表関数に対するNPN変換
  ) const
  {
    auto rep_tv = Npn4::normalize(tv, npn);
    if ( mPatGraphDict.count(rep_tv) > 0 ) {
      return mPatGraphDict.at(rep_tv);
    }
    // 空のリスト
    static std::vector<PatGraph> dummy{};
    return dummy;
  }

  /// @brief 定数0ノードを返す．
  const PatNode*
  const0() const
  {
    return mConst0;
  }

  /// @brief 入力ノードを返す．
  const PatNode*
  input(
    SizeType pos ///< [in] 入力番号 ( 0 <= pos < 4 )
  ) const
  {
    if ( pos < 0 || 4 <= pos ) {
      throw std::out_of_range{"pos is out of range"};
    }
    return mInputList[pos];
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
  std::vector<PatNode> mNodeArray;

  // 定数ノード
  const PatNode* mConst0;

  // 入力ノードのリスト
  const PatNode* mInputList[4];

  // 代表関数をキーにしてパタングラフのリストを保持する配列
  std::unordered_map<Tv4, std::vector<PatGraph>> mPatGraphDict;

};

END_NAMESPACE_YM_AIG

#endif // PATMGR_H
