#ifndef BDDOBJ_H
#define BDDOBJ_H

/// @file BddObj.h
/// @brief BddObj のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"


BEGIN_NAMESPACE_YM_DD

class BddMgrImpl;

//////////////////////////////////////////////////////////////////////
/// @class BddObj BddObj.h "BddObj.h"
/// @brief BddMgr に属するオブジェクトの基底クラス
///
/// BddMgr の実体である BddMgrImpl へのポインタを持つ．
/// BddMgrImpl は参照回数のカウンタを持っており，
/// 参照回数が０になった時点で開放される．
//////////////////////////////////////////////////////////////////////
class BddObj
{
  friend class BddMgrImpl;

protected:

  /// @brief 空のコンストラクタ
  ///
  /// 不正な値となる．
  BddObj();

  /// @brief マネージャを指定したコンストラクタ
  explicit
  BddObj(
    BddMgrImpl* mgr  ///< [in] マネージャ
  );

  /// @brief コピーコンストラクタ
  BddObj(
    const BddObj& src ///< [in] コピー元のオブジェクト
  );

  /// @brief デストラクタ
  ~BddObj();


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
    return mMgr != nullptr;
  }

  /// @brief 不正値の時に true を返す．
  bool
  is_invalid() const
  {
    return !is_valid();
  }


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// マネージャの実体を返す．
  BddMgrImpl*
  _mgr() const
  {
    return mMgr;
  }

  /// @brief 同じマネージャの要素かチェックする．
  ///
  /// 異なる場合には std::invalid_argument 例外を送出する．
  void
  _check_mgr(
    const BddObj& other
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

#endif // BDDOBJ_H
