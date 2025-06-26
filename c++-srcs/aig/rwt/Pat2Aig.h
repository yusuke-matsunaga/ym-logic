#ifndef PAT2AIG_H
#define PAT2AIG_H

/// @file Pat2Aig.h
/// @brief Pat2Aig のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/aig.h"


BEGIN_NAMESPACE_YM_AIG

class Cut;
class AigMgrImpl;
class AigEdge;
class PatGraph;
class PatNode;
class CalcMerit;

//////////////////////////////////////////////////////////////////////
/// @class Pat2Aig Pat2Aig.h "Pat2Aig.h"
/// @brief PatGraph を AIG に変換するクラス
//////////////////////////////////////////////////////////////////////
class Pat2Aig
{
public:

  static bool debug;

public:

  /// @brief コンストラクタ
  Pat2Aig(
    AigMgrImpl* aig_mgr
  ) : mAigMgr{aig_mgr}
  {
  }

  /// @brief デストラクタ
  ~Pat2Aig() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief PatGraph を AIG に変換する．
  AigEdge
  new_aig(
    const Cut* cut,
    const PatGraph& pat
  );

  /// @brief 新規に追加されるノード数を数える．
  int
  calc_cost(
    const Cut* cut,
    const PatGraph& pat,
    const CalcMerit& calc_merit
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  AigEdge
  aig_sub(
    const PatNode* node
  );

  AigEdge
  calc_sub(
    const PatNode* node,
    const CalcMerit& calc_merit
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  AigMgrImpl* mAigMgr;

  // PatNode のノード番号と AIG の枝の対応付けを行う辞書
  std::unordered_map<SizeType, AigEdge> mNodeDict;

  // calc_cost() 用のカウンタ
  int mCount;

};

END_NAMESPACE_YM_AIG

#endif // PAT2AIG_H
