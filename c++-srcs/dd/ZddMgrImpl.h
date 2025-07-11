#ifndef ZDDMGRIMPL_H
#define ZDDMGRIMPL_H

/// @file ZddMgrImpl.h
/// @brief ZddMgrImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/Zdd.h"
#include "ym/ZddItem.h"
#include "ym/JsonValue.h"
#include "ym/BinEnc.h"
#include "ym/BinDec.h"
#include "DdNode.h"
#include "DdEdge.h"
#include "DdNodeMgr.h"


BEGIN_NAMESPACE_YM_DD

class DdInfoMgr;

//////////////////////////////////////////////////////////////////////
/// @class ZddMgrImpl ZddMgrImpl.h "ZddMgrImpl.h"
/// @brief ZddMgr の実際の処理を行うクラス
//////////////////////////////////////////////////////////////////////
class ZddMgrImpl :
  public DdNodeMgr
{
public:

  /// @brief コンストラクタ
  ZddMgrImpl();

  /// @brief デストラクタ
  ~ZddMgrImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数を返す．
  SizeType
  item_num() const;

  /// @brief 要素(を表す枝)を返す．
  DdEdge
  item(
    SizeType elem_id ///< [in] 要素番号
  );

  /// @brief 要素(を表す枝)のリストを返す．
  std::vector<DdEdge>
  item_list()
  {
    return mItemList;
  }

  /// @brief 部分集合を作る．
  DdEdge
  make_set(
    const std::vector<SizeType>& level_list
  );

  /// @brief 複数のZDDを独自形式でバイナリダンプする．
  ///
  /// 復元には ZddMgr::restore() を用いる．
  void
  dump(
    BinEnc& s,                ///< [in] 出力ストリーム
    const DdInfoMgr& info_mgr ///< [in] 構造の情報
  );

  /// @brief バイナリダンプから復元する．
  /// @return 生成されたZDDのリストを返す．
  ///
  /// 不正な形式の場合は std::invalid_argument 例外を送出する．
  std::vector<DdEdge>
  restore(
    BinDec& s ///< [in] 入力ストリーム
  );


public:
  //////////////////////////////////////////////////////////////////////
  // DdEdge を扱う関数
  //////////////////////////////////////////////////////////////////////

  // @brief ノードを作る．
  DdEdge
  new_node(
    SizeType level,
    DdEdge edge0,
    DdEdge edge1
  );

  /// @brief 先頭の変数で分解する．
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
    SizeType l_level;
    const DdNode* l_node;
    if ( left.is_const() ) {
      l_level = BAD_VARID;
    }
    else {
      l_node = left.node();
      l_level = l_node->level();
    }
    SizeType r_level;
    const DdNode* r_node;
    if ( right.is_const() ) {
      r_level = BAD_VARID;
    }
    else {
      r_node = right.node();
      r_level = r_node->level();
    }
    auto top = std::min(l_level, r_level);
    if ( l_level == top ) {
      left0 = l_node->edge0();
      left1 = l_node->edge1();
    }
    else {
      left0 = left;
      left1 = DdEdge::zero();
    }
    if ( r_level == top ) {
      right0 = r_node->edge0();
      right1 = r_node->edge1();
    }
    else {
      right0 = right;
      right1 = DdEdge::zero();
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
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素(の枝)のリスト
  std::vector<DdEdge> mItemList;

};

END_NAMESPACE_YM_DD

#endif // ZDDMGRIMPL_H
