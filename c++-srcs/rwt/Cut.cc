
/// @file Cut.cc
/// @brief Cut の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CutMgr.h"
#include "aig/AigNode.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
/// @class CalcTv CalcTv.h "CalcTv.h"
/// @brief Cut の論理関数を計算するためのクラス
///
/// ここでは４入力関数のみを扱うので16ビットの符号なし整数(std::uint16_t)
/// で論理関数を表す．
//////////////////////////////////////////////////////////////////////
class CalcTv
{
public:

  using Tv4Type = Cut::Tv4Type;

public:

  /// @brief コンストラクタ
  CalcTv() = default;

  /// @brief デストラクタ
  ~CalcTv() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 関数をセットする．
  void
  set_tv(
    AigNode* node, ///< [in] 対象のノード
    Tv4Type tv     ///< [in] 論理関数
  )
  {
    mTvDict.emplace(node->id(), tv);
  }

  /// @brief 関数を計算する．
  Tv4Type
  calc(
    AigNode* node ///< [in] 対象のノード
  )
  {
    if ( mTvDict.count(node->id()) > 0 ) {
      return mTvDict.at(node->id());
    }
    ASSERT_COND( node->is_and() );
    auto node0 = node->fanin0_node();
    auto node1 = node->fanin1_node();
    auto tv0 = calc(node0);
    auto tv1 = calc(node1);
    if ( node->fanin0_inv() ) {
      tv0 = ~tv0;
    }
    if ( node->fanin1_inv() ) {
      tv1 = ~tv1;
    }
    auto tv = tv0 & tv1;
    set_tv(node, tv);
    return tv;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード番号をキーにして論理関数を保持する辞書
  std::unordered_map<SizeType, SizeType> mTvDict;

};


//////////////////////////////////////////////////////////////////////
// クラス Cut
//////////////////////////////////////////////////////////////////////

// @brief 論理関数を計算する．
Cut::Tv4Type
Cut::calc_tv() const
{
  if ( leaf_size() != 4 ) {
    throw std::invalid_argument{"leaf_size != 4"};
  }

  CalcTv calc_tv;
  // 入力の論理関数を登録する．
  // このパタンは PatMgr と一致させる必要がある．
  calc_tv.set_tv(mLeafList[0], 0xAAAA);
  calc_tv.set_tv(mLeafList[1], 0xCCCC);
  calc_tv.set_tv(mLeafList[2], 0xF0F0);
  calc_tv.set_tv(mLeafList[3], 0xFF00);
  return calc_tv.calc(mRoot);
}

END_NAMESPACE_YM_AIG
