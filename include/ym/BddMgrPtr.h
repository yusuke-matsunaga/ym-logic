#ifndef BDDMGRPTR_H
#define BDDMGRPTR_H

/// @file BddMgrPtr.h
/// @brief BddMgrPtr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"


BEGIN_NAMESPACE_YM_DD

class BddMgrImpl;

//////////////////////////////////////////////////////////////////////
/// @class BddMgrPtr BddMgrPtr.h "ym/BddMgrPtr.h"
/// @brief BddMgrImpl のスマートポインタ
///
/// 中身は std::shared_ptr<BddMgrImpl> だが
/// インターフェイス定義のために別クラスにしている．
//////////////////////////////////////////////////////////////////////
class BddMgrPtr
{
  friend class BddMgrImpl;

public:

  /// @brief 空のコンストラクタ
  ///
  /// 不正な値となる．
  BddMgrPtr() = default;

  /// @brief 生のポインタを指定したコンストラクタ
  explicit
  BddMgrPtr(
    BddMgrImpl* ptr  ///< [in] マネージャのポインタ
  );

  /// @brief コピーコンストラクタ
  BddMgrPtr(
    const BddMgrPtr& src ///< [in] コピー元のオブジェクト
  );

  /// @brief コピー代入演算子
  BddMgrPtr&
  operator=(
    const BddMgrPtr& src ///< [in] コピー元のオブジェクト
  );

  /// @brief デストラクタ
  ~BddMgrPtr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のマネージャを返す．
  BddMgr
  mgr() const;

  /// マネージャの実体を返す．
  BddMgrImpl*
  get() const
  {
    return mMgr;
  }

  /// @brief get() の別名
  BddMgrImpl*
  operator->() const
  {
    return get();
  }

  /// @brief 適正な値を持っている時に true を返す．
  bool
  is_valid() const
  {
    return mMgr != nullptr;
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
    const BddMgrPtr& right
  ) const
  {
    return mMgr == right.mMgr;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const BddMgrPtr& right
  ) const
  {
    return !operator==(right);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief マネージャをセットする．
  void
  set_mgr(
    BddMgrImpl* mgr
  );

  /// @brief 同じマネージャの要素かチェックする．
  ///
  /// 異なる場合には std::invalid_argument 例外を送出する．
  void
  _check_mgr(
    const BddMgrPtr& other
  ) const
  {
    if ( mMgr != other.mMgr ) {
      throw std::invalid_argument{"BddMgr mismatch"};
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マネージャ
  BddMgrImpl* mMgr{nullptr};

};

END_NAMESPACE_YM_DD

#endif // BDDMGRPTR_H
