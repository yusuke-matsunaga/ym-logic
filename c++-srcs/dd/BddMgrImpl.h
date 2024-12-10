#ifndef BDDMGRIMPL_H
#define BDDMGRIMPL_H

/// @file BddMgrImpl.h
/// @brief BddMgrImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/JsonValue.h"
#include "ym/BinDec.h"
#include "ym/BinEnc.h"
#include "DdNode.h"
#include "DdEdge.h"
#include "DdNodeMgr.h"


BEGIN_NAMESPACE_YM_DD

class DdInfoMgr;

//////////////////////////////////////////////////////////////////////
/// @class BddMgrImpl BddMgrImpl.h "BddMgrImpl.h"
/// @brief BddMgr の実際の処理を行うクラス
//////////////////////////////////////////////////////////////////////
class BddMgrImpl :
  public DdNodeMgr
{
public:

  /// @brief コンストラクタ
  BddMgrImpl();

  /// @brief デストラクタ
  ~BddMgrImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @breif 変数の数を返す．
  SizeType
  variable_num() const;

  /// @brief 変数(を表す枝)を返す．
  DdEdge
  variable(
    SizeType varid ///< [in] 変数番号
  );

  /// @brief 変数(を表す枝)のリストを返す．
  const vector<DdEdge>&
  variable_list()
  {
    return mVarList;
  }

  /// @brief 変数順を表す変数のリストを返す．
  ///
  /// レベルの昇順に並んでいる
  vector<DdEdge>
  variable_order();

  /// @brief 変数順を設定する．
  void
  set_variable_order(
    const vector<DdEdge>& order_list ///< [in] 変数順を表すリスト
  );

  /// @brief sifting を用いて変数順の最適化を行う．
  void
  dvo_sift();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容の出力を行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief BDD の内容をバイナリダンプする．
  void
  dump(
    BinEnc& s,                ///< [in] 出力ストリーム
    const DdInfoMgr& info_mgr ///< [in] ノード情報
  );

  /// @brief バイナリダンプから復元する．
  /// @return 生成されたBDDのリストを返す．
  ///
  /// 不正な形式の場合は std::invalid_argument 例外を送出する．
  vector<DdEdge>
  restore(
    BinDec& s ///< [in] 入力ストリーム
  );


public:
  //////////////////////////////////////////////////////////////////////
  // DdEdge を扱う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードを作る．
  DdEdge
  new_node(
    SizeType index, ///< [in] インデックス
    DdEdge edge0,  ///< [in] 0枝
    DdEdge edge1   ///< [in] 1枝
  );

  /// @brief トップの変数で分解する．
  static
  SizeType
  decomp(
    DdEdge left,
    DdEdge right,
    DdEdge& left0,
    DdEdge& left1,
    DdEdge& right0,
    DdEdge& right1
  )
  {
    auto l_node = left.node();
    auto l_inv = left.inv();
    auto l_level = l_node->level();
    auto r_node = right.node();
    auto r_inv = right.inv();
    auto r_level = r_node->level();
    auto top = std::min(l_level, r_level);
    if ( l_level == top ) {
      left0 = l_node->edge0() ^ l_inv;
      left1 = l_node->edge1() ^ l_inv;
    }
    else {
      left0 = left;
      left1 = left;
    }
    if ( r_level == top ) {
      right0 = r_node->edge0() ^ r_inv;
      right1 = r_node->edge1() ^ r_inv;
    }
    else {
      right0 = right;
      right1 = right;
    }
    return top;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // DdNodeMgr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief garbage_collection() が呼ばれた後に呼び出される関数
  void
  after_gc() override;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief dvo_sift() の下請け関数
  ///
  /// level の移動先として最適な位置を求める．
  void
  dvo_sub(
    SizeType level
  );

  /// @brief 隣り合うレベルを交換する．
  void
  swap_level(
    SizeType level ///< [in] 交換する上のレベル
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数(の枝)のリスト
  vector<DdEdge> mVarList;

};

END_NAMESPACE_YM_DD

#endif // BDDMGRIMPL_H
