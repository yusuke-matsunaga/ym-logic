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
#include "dd/DdNode.h"
#include "dd/DdEdge.h"
#include "dd/DdNodeMgr.h"


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

  /// @brief 要素を返す．
  ZddItem
  item(
    SizeType elem_id ///< [in] 要素番号
  );

  /// @brief 要素のリストを返す．
  vector<ZddItem>
  item_list() const;

  /// @brief 空集合を作る．
  Zdd
  zero();

  /// @brief ユニバースを作る．
  Zdd
  one();

  /// @brief ZDDを作る．
  Zdd
  zdd(
    SizeType level,   ///< [in] レベル
    const Zdd& edge0, ///< [in] 0枝
    const Zdd& edge1  ///< [in] 1枝
  );

  /// @brief ZDD をコピーする．
  ///
  /// 通常は同じものを返すが，src のマネージャが異なる場合には
  /// 同じ構造のコピーを作る．
  Zdd
  copy(
    const Zdd& src
  );

  /// @brief 部分集合を作る．
  Zdd
  make_set(
    const vector<ZddItem>& item_list
  );

  /// @brief ZDD を反転する．
  Zdd
  invert(
    const Zdd& src
  );

  /// @brief CAP 演算を行う．
  Zdd
  cap(
    const Zdd& left,
    const Zdd& right
  );

  /// @brief CUP 演算を行う．
  Zdd
  cup(
    const Zdd& left,
    const Zdd& right
  );

  /// @brief DIFF 演算を行う．
  Zdd
  diff(
    const Zdd& left,
    const Zdd& right
  );

  /// @brief 変数を含む集合を求める．
  /// @return 結果を返す．
  Zdd
  onset(
    const Zdd& zdd,
    const ZddItem& item
  );

  /// @brief 変数を含まない集合を求める．
  /// @return 結果を返す．
  Zdd
  offset(
    const Zdd& zdd,
    const ZddItem& item
  );

  /// @brief PRODUCT 演算を行う．
  Zdd
  product(
    const Zdd& left,
    const Zdd& right
  );

  /// @brief 複数のZDDの内容を出力する．
  void
  display(
    ostream& s,                 ///< [in] 出力ストリーム
    const vector<Zdd>& zdd_list ///< [in] ZDDのリスト
  );

  /// @brief 複数のZDDを dot 形式で出力する．
  void
  gen_dot(
    ostream& s,                                    ///< [in] 出力ストリーム
    const vector<Zdd>& zdd_list,                   ///< [in] ZDDのリスト
    const unordered_map<string, string>& attr_dict ///< [in] 属性値の辞書
  );

  /// @brief 複数のBDDを dot 形式で出力する．
  void
  gen_dot(
    ostream& s,                  ///< [in] 出力ストリーム
    const vector<Zdd>& bdd_list, ///< [in] ZDDのリスト
    const JsonValue& attr        ///< [in] 属性値を表す JSON オブジェクト
  );

  /// @brief 構造を表す整数配列を作る．
  vector<SizeType>
  rep_data(
    const vector<Zdd>& zdd_list ///< [in] ZDDのリスト
  );

  /// @brief 複数のZDDを独自形式でバイナリダンプする．
  ///
  /// 復元には ZddMgr::restore() を用いる．
  void
  dump(
    BinEnc& s,                  ///< [in] 出力ストリーム
    const vector<Zdd>& zdd_list ///< [in] ZDDのリスト
  );

  /// @brief バイナリダンプから復元する．
  /// @return 生成されたZDDのリストを返す．
  ///
  /// 不正な形式の場合は std::invalid_argument 例外を送出する．
  vector<Zdd>
  restore(
    BinDec& s ///< [in] 入力ストリーム
  );

  /// @brief 複数のZDDのノード数を数える．
  SizeType
  zdd_size(
    const vector<Zdd>& zdd_list ///< [in] ZDDのリスト
  );

  /// @brief 複数のBDDのノードの情報を取り出す．
  DdInfoMgr
  node_info(
    const vector<Zdd>& zdd_list ///< [in] ZDDのリスト
  );


public:
  //////////////////////////////////////////////////////////////////////
  // DdEdge を扱う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief DdEdge を Zdd に変換する．
  Zdd
  _zdd(
    DdEdge edge ///< [in] 根の枝
  );

  /// @brief Zdd から DdEdge を取り出す．
  DdEdge
  _edge(
    const Zdd& zdd
  );

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

  /// @brief ZDDの根の枝のリストを作る．
  vector<DdEdge>
  root_list(
    const vector<Zdd>& zdd_list ///< [in] ZDDのリスト
  );

  /// @brief レベルを要素に変換する．
  ZddItem
  level_to_item(
    SizeType level ///< [in] レベル
  );


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

  /// @brief このマネージャに属しているオブジェクトかチェックする．
  void
  _check_mgr(
    const Zdd& zdd ///< [in] 対象のオブジェクト
  ) const
  {
    if ( zdd.mMgr != this ) {
      throw std::invalid_argument{"ZddMgr mismatch"};
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素のリスト
  vector<ZddItem> mItemList;

};

END_NAMESPACE_YM_DD

#endif // ZDDMGRIMPL_H
