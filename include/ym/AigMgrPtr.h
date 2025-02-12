#ifndef AIGMGRPTR_H
#define AIGMGRPTR_H

/// @file AigMgrPtr.h
/// @brief AigMgrPtr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/aig.h"


BEGIN_NAMESPACE_YM_AIG

class AigEdge;

//////////////////////////////////////////////////////////////////////
/// @class AigMgrPtr AigMgrPtr.h "AigMgrPtr.h"
/// @brief AigMgrImpl のスマートポインタ
///
/// friend class 以外から使われることはない．
//////////////////////////////////////////////////////////////////////
class AigMgrPtr
{
  friend class AigMgr;
  friend class AigHandle;

private:

  /// @brief 空のコンストラクタ
  ///
  /// 不正なポインタとなる．
  AigMgrPtr() = default;

  /// @brief 値を指定したコンストラクタ
  AigMgrPtr(
    AigMgrImpl* ptr
  );

  /// @brief コピーコンストラクタ
  AigMgrPtr(
    const AigMgrPtr& src
  );

  /// @brief デストラクタ
  ~AigMgrPtr();


private:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief dereference 演算子
  AigMgrImpl*
  get() const
  {
    return mPtr.get();
  }

  /// @brief get() の別名
  AigMgrImpl*
  operator->() const
  {
    return get();
  }

  /// @brief 等価比較演算子
  bool
  operator==(
    const AigMgrPtr& right
  ) const
  {
    return mPtr == right.mPtr;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const AigMgrPtr& right
  ) const
  {
    return !operator==(right);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // AigMgr/AigHandle 用の便利関数
  //////////////////////////////////////////////////////////////////////

  /// @brief AigHandle を AigEdge に変換する．
  ///
  /// 異なるマネージャのハンドルなら例外を送出する．
  AigEdge
  handle_to_edge(
    AigHandle handle ///< [in] ハンドル
  ) const;

  /// @brief AigHandle のリストを AigEdge のリストに変換する．
  ///
  /// 異なるマネージャのハンドルが含まれていたら例外を送出する．
  vector<AigEdge>
  hlist_to_elist(
    const vector<AigHandle>& handle_list ///< [in] ハンドルのリスト
  ) const;

  /// @brief AigHandle のリストを AigEdge のリストに変換する．
  /// @return マネージャを返す．
  ///
  /// 異なるマネージャのハンドルが含まれていたら例外を送出する．
  static
  AigMgrPtr
  hlist_to_elist(
    const vector<AigHandle>& handle_list, ///< [in] ハンドルのリスト
    vector<AigEdge>& edge_list            ///< [out] 変換した枝のリスト
  );

  /// @brief AigEdge を AigHandle に変換する．
  AigHandle
  edge_to_handle(
    AigEdge edge ///< [in] 枝
  ) const;

  /// @brief AigEdge のリストを AigHandle のリストに変換する．
  vector<AigHandle>
  elist_to_hlist(
    const vector<AigEdge>& edge_list ///< [in] 枝のリスト
  ) const;

  /// @brief ハンドルが同じマネージャに属しているかチェックする．
  void
  _check_mgr(
    const AigHandle& handle
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ポインタ本体
  std::shared_ptr<AigMgrImpl> mPtr;

};

END_NAMESPACE_YM_AIG

#endif // AIGMGRPTR_H
