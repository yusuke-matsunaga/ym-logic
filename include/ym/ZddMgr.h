#ifndef ZDDMGR_H
#define ZDDMGR_H

/// @file ZddMgr.h
/// @brief ZddMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/BinDec.h"


BEGIN_NAMESPACE_YM_ZDD

class Zdd;
class ZddMgrImpl;

//////////////////////////////////////////////////////////////////////
/// @class ZddMgr ZddMgr.h "ym/ZddMgr.h"
/// @brief ZDD を管理するためのクラス
//////////////////////////////////////////////////////////////////////
class ZddMgr
{
public:

  /// @brief コンストラクタ
  ZddMgr();

  /// @brief デストラクタ
  ~ZddMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ZDD をコピーする．
  ///
  /// 通常は同じものを返すが，src のマネージャが異なる場合には
  /// 同じ構造のコピーを作る．
  Zdd
  copy(
    const Zdd& src
  );

  /// @brief ZDD を反転する．
  Zdd
  invert(
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
    const vector<SizeType> elem_list ///< [in] 要素番号のリスト
  );

  /// @brief 新しいノードを作る．
  Zdd
  new_node(
    SizeType index,   ///< [in] インデックス
    const Zdd& edge0, ///< [in] 0枝の ZDD
    const Zdd& edge1  ///< [in] 1枝の ZDD
  );

  /// @brief バイナリダンプから復元する．
  /// @return 生成されたZDDのリストを返す．
  ///
  /// 不正な形式の場合は std::invalid_argument 例外を送出する．
  vector<Zdd>
  restore(
    BinDec& s ///< [in] 入力ストリーム
  );

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


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ZddMgrImpl を取り出す．
  ZddMgrImpl*
  impl() const
  {
    return mImpl.get();
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本体
  unique_ptr<ZddMgrImpl> mImpl;

};

END_NAMESPACE_YM_ZDD

#endif // ZDDMGR_H
