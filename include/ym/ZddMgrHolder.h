#ifndef ZDDMGRMGRHOLDER_H
#define ZDDMGRMGRHOLDER_H

/// @file ZddMgrHolder.h
/// @brief ZddMgrHolder のヘッダファイル
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
/// @class ZddMgrHolder ZddMgrHolder.h "ym/ZddMgrHolder.h"
/// @brief ZddMgrImpl のスマートポインタ
///
/// 中身は std::shared_ptr<ZddMgrImpl> だが
/// インターフェイス定義のために別クラスにしている．
//////////////////////////////////////////////////////////////////////
class ZddMgrHolder
{
public:

  /// @brief 生のポインタを指定したコンストラクタ
  explicit
  ZddMgrHolder(
    ZddMgrImpl* ptr = nullptr ///< [in] マネージャのポインタ
  );

  /// @brief コピーコンストラクタ
  ZddMgrHolder(
    const ZddMgrHolder& src ///< [in] コピー元のオブジェクト
  );

  /// @brief コピー代入演算子
  ZddMgrHolder&
  operator=(
    const ZddMgrHolder& src ///< [in] コピー元のオブジェクト
  );

  /// @brief デストラクタ
  ~ZddMgrHolder();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のマネージャを返す．
  ZddMgr
  mgr() const;

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

  /// @brief 同じマネージャのオブジェクトなら true を返す．
  bool
  has_same_mgr(
    const ZddMgrHolder& other
  ) const
  {
    return mPtr == other.mPtr;
  }


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスで用いられる関数
  //////////////////////////////////////////////////////////////////////

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

  /// @brief 適正なポインタを持っているかチェックする．
  void
  _check_valid() const
  {
    if ( is_invalid() ) {
      throw std::logic_error{"mPtr is nullptr"};
    }
  }

  /// @brief 同じマネージャの要素かチェックする．
  ///
  /// 異なる場合には std::invalid_argument 例外を送出する．
  void
  _check_mgr(
    const ZddMgrHolder& other
  ) const
  {
    if ( !has_same_mgr(other) ) {
      throw std::invalid_argument{"ZddMgr mismatch"};
    }
  }

  /// @brief 同じ適正なマネージャの要素かチェックする．
  ///
  /// 異なる場合には std::invalid_argument 例外を送出する．
  void
  _check_valid_mgr(
    const ZddMgrHolder& other
  ) const
  {
    _check_valid();
    _check_mgr(other);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 枝のリストを Zdd のリストに変換する．
  std::vector<Zdd>
  conv_to_zddlist(
    const std::vector<DdEdge>& edge_list ///< [in] 枝のリスト
  ) const;

  /// @brief 枝のリストを要素のリストに変換する．
  std::vector<ZddItem>
  conv_to_itemlist(
    const std::vector<DdEdge>& edge_list ///< [in] 枝のリスト
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マネージャの本体
  std::shared_ptr<ZddMgrImpl> mPtr;

};

END_NAMESPACE_YM_DD

#endif // ZDDMGRMGRHOLDER_H
