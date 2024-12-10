#ifndef ZDDMGRPTR_H
#define ZDDMGRPTR_H

/// @file ZddMgrPtr.h
/// @brief ZddMgrPtr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"


BEGIN_NAMESPACE_YM_DD

class ZddMgrImpl;

//////////////////////////////////////////////////////////////////////
/// @class ZddMgrPtr ZddMgrPtr.h "ym/ZddMgrPtr.h"
/// @brief ZddMgrImpl のスマートポインタ
///
/// 中身は std::shared_ptr<ZddMgrImpl> だが
/// インターフェイス定義のために別クラスにしている．
//////////////////////////////////////////////////////////////////////
class ZddMgrPtr
{
  friend class ZddMgrImpl;

public:

  /// @brief 空のコンストラクタ
  ///
  /// 不正な値となる．
  ZddMgrPtr() = default;

  /// @brief 生のポインタを指定したコンストラクタ
  explicit
  ZddMgrPtr(
    ZddMgrImpl* ptr  ///< [in] マネージャのポインタ
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

  /// @brief 親のマネージャを返す．
  ZddMgr
  mgr() const;

  /// マネージャの実体を返す．
  ZddMgrImpl*
  get() const
  {
    return mMgr;
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
    const ZddMgrPtr& right
  ) const
  {
    return mMgr == right.mMgr;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const ZddMgrPtr& right
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
    ZddMgrImpl* mgr
  );

  /// @brief 同じマネージャの要素かチェックする．
  ///
  /// 異なる場合には std::invalid_argument 例外を送出する．
  void
  _check_mgr(
    const ZddMgrPtr& other
  ) const
  {
    if ( mMgr != other.mMgr ) {
      throw std::invalid_argument{"ZddMgr mismatch"};
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マネージャ
  ZddMgrImpl* mMgr{nullptr};

};

END_NAMESPACE_YM_DD

#endif // ZDDMGRPTR_H
