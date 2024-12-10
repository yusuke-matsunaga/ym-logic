#ifndef ZDDMGRPTR_H
#define ZDDMGRPTR_H

/// @file ZddMgrPtr.h
/// @brief ZddMgrPtr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/BinDec.h"
#include "ym/BinEnc.h"
#include "ym/JsonValue.h"


BEGIN_NAMESPACE_YM_DD

class ZddMgrImpl;
class DdEdge;
class DdEdgeMgr;
class DdInfoMgr;

//////////////////////////////////////////////////////////////////////
/// @class ZddMgrPtr ZddMgrPtr.h "ym/ZddMgrPtr.h"
/// @brief ZddMgrImpl のスマートポインタ
///
/// 中身は std::shared_ptr<ZddMgrImpl> だが
/// インターフェイス定義のために別クラスにしている．
//////////////////////////////////////////////////////////////////////
class ZddMgrPtr
{
public:

  /// @brief 生のポインタを指定したコンストラクタ
  explicit
  ZddMgrPtr(
    ZddMgrImpl* ptr = nullptr ///< [in] マネージャのポインタ
  );

  /// @brief コピーコンストラクタ
  ZddMgrPtr(
    const ZddMgrPtr& src ///< [in] コピー元のオブジェクト
  );

  /// @brief コピー代入演算子
  ZddMgrPtr&
  operator=(
    const ZddMgrPtr& src ///< [in] コピー元のオブジェクト
  );

  /// @brief デストラクタ
  ~ZddMgrPtr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素を返す．
  ZddItem
  item(
    SizeType elem_id ///< [in] 要素番号
  ) const;

  /// @brief 要素のリストを返す．
  vector<ZddItem>
  item_list() const;

  /// @brief 空集合を作る．
  Zdd
  zero() const;

  /// @brief ユニバースを作る．
  Zdd
  one() const;

  /// @brief ZDDを作る．
  Zdd
  zdd(
    SizeType level,   ///< [in] レベル
    const Zdd& edge0, ///< [in] 0枝
    const Zdd& edge1  ///< [in] 1枝
  ) const;

  /// @brief ZDD をコピーする．
  ///
  /// 通常は同じものを返すが，src のマネージャが異なる場合には
  /// 同じ構造のコピーを作る．
  Zdd
  copy(
    const Zdd& src
  ) const;

  /// @brief 部分集合を作る．
  Zdd
  make_set(
    const vector<ZddItem>& item_list
  ) const;

  /// @brief ZDD を反転する．
  Zdd
  invert(
    const Zdd& src
  ) const;

  /// @brief CAP 演算を行う．
  Zdd
  cap(
    const Zdd& left,
    const Zdd& right
  ) const;

  /// @brief CUP 演算を行う．
  Zdd
  cup(
    const Zdd& left,
    const Zdd& right
  ) const;

  /// @brief DIFF 演算を行う．
  Zdd
  diff(
    const Zdd& left,
    const Zdd& right
  ) const;

  /// @brief 変数を含む集合を求める．
  /// @return 結果を返す．
  Zdd
  onset(
    const Zdd& zdd,
    const ZddItem& item
  ) const;

  /// @brief 変数を含まない集合を求める．
  /// @return 結果を返す．
  Zdd
  offset(
    const Zdd& zdd,
    const ZddItem& item
  ) const;

  /// @brief PRODUCT 演算を行う．
  Zdd
  product(
    const Zdd& left,
    const Zdd& right
  ) const;

  /// @brief レベルを要素に変換する．
  ZddItem
  level_to_item(
    SizeType level ///< [in] レベル
  ) const;

  /// @brief 複数のZDDの内容を出力する．
  void
  display(
    ostream& s,                 ///< [in] 出力ストリーム
    const vector<Zdd>& zdd_list ///< [in] ZDDのリスト
  ) const;

  /// @brief 複数のBDDを dot 形式で出力する．
  void
  gen_dot(
    ostream& s,                  ///< [in] 出力ストリーム
    const vector<Zdd>& bdd_list, ///< [in] ZDDのリスト
    const JsonValue& option      ///< [in] オプションを表す JSON オブジェクト
  ) const;

  /// @brief 構造を表す整数配列を作る．
  vector<SizeType>
  rep_data(
    const vector<Zdd>& zdd_list ///< [in] ZDDのリスト
  ) const;

  /// @brief 複数のZDDを独自形式でバイナリダンプする．
  ///
  /// 復元には ZddMgr::restore() を用いる．
  void
  dump(
    BinEnc& s,                  ///< [in] 出力ストリーム
    const vector<Zdd>& zdd_list ///< [in] ZDDのリスト
  ) const;

  /// @brief バイナリダンプから復元する．
  /// @return 生成されたZDDのリストを返す．
  ///
  /// 不正な形式の場合は std::invalid_argument 例外を送出する．
  vector<Zdd>
  restore(
    BinDec& s ///< [in] 入力ストリーム
  ) const;

  /// @brief 複数のZDDのノード数を数える．
  SizeType
  zdd_size(
    const vector<Zdd>& zdd_list ///< [in] ZDDのリスト
  ) const;

  /// @brief 複数のBDDのノードの情報を取り出す．
  DdInfoMgr
  node_info(
    const vector<Zdd>& zdd_list ///< [in] ZDDのリスト
  ) const;

  /// @brief 親のマネージャを返す．
  ZddMgr
  mgr() const;

  /// @brief DdEdge を Zdd に変換する．
  Zdd
  _zdd(
    DdEdge edge ///< [in] 根の枝
  ) const;

  /// @brief DdEdge を ZddItem に変換する．
  ZddItem
  _item(
    DdEdge edge ///< [in] 根の枝
  ) const;

  /// マネージャの実体を返す．
  ZddMgrImpl*
  get() const
  {
    return mPtr.get();
  }

  /// @brief get() の別名
  ZddMgrImpl*
  operator->() const
  {
    return get();
  }

  /// @brief 適正な値を持っている時に true を返す．
  bool
  is_valid() const
  {
    return get() != nullptr;
  }

  /// @brief 不正値の時に true を返す．
  bool
  is_invalid() const
  {
    return !is_valid();
  }

  /// @brief 等価比較演算子
  bool
  operator==(
    const ZddMgrPtr& right
  ) const
  {
    return mPtr == right.mPtr;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const ZddMgrPtr& right
  ) const
  {
    return !operator==(right);
  }

  /// @brief 同じマネージャの要素かチェックする．
  ///
  /// 異なる場合には std::invalid_argument 例外を送出する．
  void
  _check_mgr(
    const ZddMgrPtr& other
  ) const
  {
    if ( mPtr != other.mPtr ) {
      throw std::invalid_argument{"ZddMgr mismatch"};
    }
  }

  /// @brief 同じマネージャの要素かチェックする．
  ///
  /// 異なる場合には std::invalid_argument 例外を送出する．
  void
  _check_mgr(
    const Zdd& zdd
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 枝のリストを Zdd のリストに変換する．
  vector<Zdd>
  conv_to_zddlist(
    const vector<DdEdge>& edge_list ///< [in] 枝のリスト
  ) const;

  /// @brief 枝のリストを要素のリストに変換する．
  vector<ZddItem>
  conv_to_itemlist(
    const vector<DdEdge>& edge_list ///< [in] 枝のリスト
  ) const;



private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マネージャの本体
  std::shared_ptr<ZddMgrImpl> mPtr;

};

END_NAMESPACE_YM_DD

#endif // ZDDMGRPTR_H
