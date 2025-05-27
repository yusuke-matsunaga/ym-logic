#ifndef CALCCOST_H
#define CALCCOST_H

/// @file CalcCost.h
/// @brief CalcCost のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/aig.h"


BEGIN_NAMESPACE_YM_AIG

class AigMgrImpl;
class Npn4;
class PatGraph;

//////////////////////////////////////////////////////////////////////
/// @class CalcCost CalcCost.h "CalcCost.h"
/// @brief 新たに追加されるノード数を見積もるクラス
//////////////////////////////////////////////////////////////////////
class CalcCost
{
public:

  /// @brief コンストラクタ
  CalcCost(
    AigMgrImpl* aig_mgr
  ) : mAigMgr{aig_mgr}
  {
  }

  /// @brief デストラクタ
  ~CalcCost() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 新規に追加されるノード数を数える．
  int
  calc(
    const Cut* cut,
    const Npn4& rep_npn,
    const PatGraph& pat
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  int
  calc_sub(
    const PatNode* node,
    const Cut* cut,
    const Npn4& npn
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  AigMgrImpl* mAigMgr;

  // PatNode のノード番号と AIG の枝の対応付けを行う辞書
  std::unordered_map<SizeType, AigEdge> mNodeDict;

};

END_NAMESPACE_YM_AIG

#endif // CALCCOST_H
