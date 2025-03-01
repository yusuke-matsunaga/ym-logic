#ifndef ZDDMGR_H
#define ZDDMGR_H

/// @file ZddMgr.h
/// @brief ZddMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/ZddMgrHolder.h"
#include "ym/JsonValue.h"
#include "ym/BinEnc.h"
#include "ym/BinDec.h"


BEGIN_NAMESPACE_YM_DD

class Zdd;

//////////////////////////////////////////////////////////////////////
/// @class ZddMgr ZddMgr.h "ym/ZddMgr.h"
/// @brief ZDD を管理するためのクラス
///
/// 実体は ZddMgrImpl でこのクラスはただの shared pointer となっている．
/// コピーしても同一の ZddMgrImpl を持つインスタンスが生成される．
///
/// ZddMgrImpl は内部で参照回数を持っており，参照回数がゼロになると
/// 自動的に開放される．
//////////////////////////////////////////////////////////////////////
class ZddMgr :
  public ZddMgrHolder
{
public:

  /// @brief コンストラクタ
  ZddMgr();

  /// @brief ZddMgrHolder を指定したコンストラクタ
  explicit
  ZddMgr(
    const ZddMgrHolder& ptr
  );

  /// @brief コピーコンストラクタ
  ZddMgr(
    const ZddMgr& src
  );

  /// @brief デストラクタ
  ~ZddMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 要素に関する関数
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


public:
  //////////////////////////////////////////////////////////////////////
  // ZDD を生成する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ZDD をコピーする．
  ///
  /// 通常は同じものを返すが，src のマネージャが異なる場合には
  /// 同じ構造のコピーを作る．
  Zdd
  copy(
    const Zdd& src
  );

  /// @brief 空集合を作る．
  Zdd
  zero();

  /// @brief ユニバースを作る．
  Zdd
  one();

  /// @brief 部分集合を作る．
  Zdd
  make_set(
    const vector<ZddItem> item_list ///< [in] 要素のリスト
  );


public:
  //////////////////////////////////////////////////////////////////////
  // ZDD の内容を出力する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief バイナリダンプから復元する．
  /// @return 生成されたZDDのリストを返す．
  ///
  /// 不正な形式の場合は std::invalid_argument 例外を送出する．
  vector<Zdd>
  restore(
    BinDec& s ///< [in] 入力ストリーム
  );


public:
  //////////////////////////////////////////////////////////////////////
  // マネージャの諸元に関する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ガーベージコレクションを行う．
  void
  garbage_collection();

  /// @brief ノード数を返す．
  SizeType
  node_num() const;

  /// @brief GC を起動するしきい値を返す．
  SizeType
  gc_limit() const;

  /// @brief GC を起動するしきい値を設定する．
  void
  set_gc_limit(
    SizeType limit
  );

  /// @brief GC を許可する．
  ///
  /// ただちに GC が起動するわけではない．
  void
  enable_gc();

  /// @brief GC を禁止する．
  void
  disable_gc();

  /// @brief 等価比較演算子
  bool
  operator==(
    const ZddMgr& right
  ) const
  {
    return has_same_mgr(right);
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const ZddMgr& right
  ) const
  {
    return !operator==(right);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 枝のリストをZddのリストに変換する．
  vector<Zdd>
  conv_to_zddlist(
    const vector<DdEdge>& edge_list ///< [in] 枝のリスト
  ) const;

  /// @brief 枝のリストを要素のリストに変換する．
  vector<ZddItem>
  conv_to_itemlist(
    const vector<DdEdge>& edge_list ///< [in] 枝のリスト
  ) const;

};

END_NAMESPACE_YM_DD

#endif // ZDDMGR_H
