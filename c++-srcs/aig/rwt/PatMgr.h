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
#include "PatGraph.h"


BEGIN_NAMESPACE_YM_AIG

class PatNode;

//////////////////////////////////////////////////////////////////////
/// @class PatMgr PatMgr.h "PatMgr.h"
/// @brief Rewrite 用のパタンマネージャ
///
/// 関数の真理値表を表す数値(16ビット)をキーとして対応するパタンの
/// リストを返す．
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

  /// @brief 定数０のグラフを返す．
  PatGraph
  zero()
  {
    return PatGraph(mConst0, Npn4());
  }

  /// @brief 定数1のグラフを返す．
  PatGraph
  one()
  {
    return PatGraph(mConst0, Npn4(true));
  }

  /// @brief 関数をキーにして対応するパタンのリストを返す．
  std::vector<PatGraph>
  get_pat(
    Tv4 tv ///< [in] 関数の真理値表を表す数値
  ) const
  {
    // 結果のリスト
    std::vector<PatGraph> pat_list;
    Npn4 xnpn;
    auto rep_tv = Npn4::normalize(tv, xnpn);
    auto rep_npn = ~xnpn;
    if ( mPatGraphDict.count(rep_tv) > 0 ) {
      auto& src_list = mPatGraphDict.at(rep_tv);
      pat_list.reserve(src_list.size());
      for ( auto src_pat: src_list ) {
	pat_list.push_back(src_pat * rep_npn);
      }
    }
    return pat_list;
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
  PatNode*
  add_node(
    Tv4 tv,                 ///< [in] 真理値表を表すパタン
    const PatNode* child0,  ///< [in] 左の子供
    bool inv0,              ///< [in] 左の子供の反転フラグ
    const PatNode* child1,  ///< [in] 右の子供
    bool inv1,              ///< [in] 右の子供の反転フラグ
    std::unordered_map<SizeType, Tv4>& tv_dict
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
