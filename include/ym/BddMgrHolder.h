#ifndef BDDMGRHOLDER_H
#define BDDMGRHOLDER_H

/// @file BddMgrHolder.h
/// @brief BddMgrHolder のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/BinDec.h"
#include "ym/BinEnc.h"
#include "ym/JsonValue.h"


BEGIN_NAMESPACE_YM_DD

class BddMgrImpl;
class DdEdge;
class DdInfoMgr;

//////////////////////////////////////////////////////////////////////
/// @class BddMgrHolder BddMgrHolder.h "ym/BddMgrHolder.h"
/// @brief BddMgrImpl のスマートポインタ
///
/// 中身は std::shared_ptr<BddMgrImpl> だが意味的には BddMgrImpl
/// の delegate クラスとなっている．
//////////////////////////////////////////////////////////////////////
class BddMgrHolder
{
public:

  /// @brief 生のポインタを指定したコンストラクタ
  explicit
  BddMgrHolder(
    BddMgrImpl* ptr = nullptr ///< [in] マネージャのポインタ
  );

  /// @brief コピーコンストラクタ
  BddMgrHolder(
    const BddMgrHolder& src ///< [in] コピー元のオブジェクト
  );

  /// @brief コピー代入演算子
  BddMgrHolder&
  operator=(
    const BddMgrHolder& src ///< [in] コピー元のオブジェクト
  );

  /// @brief デストラクタ
  ~BddMgrHolder();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のマネージャを返す．
  BddMgr
  mgr() const;

  /// @brief 適正な値を持っている時に true を返す．
  bool
  is_valid() const
  {
    return mPtr.get() != nullptr;
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
    const BddMgrHolder& other
  ) const
  {
    return mPtr == other.mPtr;
  }


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスで用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief BddMgrImpl のポインタを取り出す．
  BddMgrImpl*
  get() const
  {
    _check_valid();
    return mPtr.get();
  }

  /// @brief get() の別名
  BddMgrImpl*
  operator->() const
  {
    return get();
  }

  /// @brief DdEdge を Bdd に変換する．
  Bdd
  _bdd(
    DdEdge edge
  ) const;

  /// @brief DdEdge を BddVar に変換する．
  BddVar
  _var(
    DdEdge edge
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
    const BddMgrHolder& other
  ) const
  {
    if ( !has_same_mgr(other) ) {
      throw std::invalid_argument{"BddMgr mismatch"};
    }
  }

  /// @brief 同じ適正なマネージャの要素かチェックする．
  ///
  /// 異なる場合には std::invalid_argument 例外を送出する．
  void
  _check_valid_mgr(
    const BddMgrHolder& other
  ) const
  {
    _check_valid();
    _check_mgr(other);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マネージャの本体
  std::shared_ptr<BddMgrImpl> mPtr;

};

END_NAMESPACE_YM_DD

#endif // BDDMGRHOLDER_H
