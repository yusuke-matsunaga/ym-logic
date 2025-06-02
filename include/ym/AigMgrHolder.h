#ifndef AIGMGRHOLDER_H
#define AIGMGRHOLDER_H

/// @file AigMgrHolder.h
/// @brief AigMgrHolder のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/aig.h"


BEGIN_NAMESPACE_YM_AIG

class AigEdge;

//////////////////////////////////////////////////////////////////////
/// @class AigMgrHolder AigMgrHolder.h "AigMgrHolder.h"
/// @brief AigMgrImpl のスマートポインタ
///
/// AigMgr と AigHandle の基底クラス
//////////////////////////////////////////////////////////////////////
class AigMgrHolder
{
public:

  /// @brief 空のコンストラクタ
  ///
  /// 不正なポインタとなる．
  AigMgrHolder() = default;

  /// @brief コピーコンストラクタ
  AigMgrHolder(
    const AigMgrHolder& src ///< [in] コピー元のオブジェクト
  );

  /// @brief ムーブコンストラクタ
  AigMgrHolder(
    AigMgrHolder&& src ///< [in] ムーブ元のオブジェクト
  );

  /// @brief コピー代入演算子
  AigMgrHolder&
  operator=(
    const AigMgrHolder& src ///< [in] コピー元のオブジェクト
  );

  /// @brief ムーブ代入演算子
  AigMgrHolder&
  operator=(
    AigMgrHolder&& src ///< [in] ムーブ元のオブジェクト
  );

  /// @brief デストラクタ
  ~AigMgrHolder();


protected:

  /// @brief 値を指定したコンストラクタ
  explicit
  AigMgrHolder(
    AigMgrImpl* ptr ///< [in] マネージャのポインタ
  );


protected:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 適正なポインタを持っている時 true を返す．
  bool
  is_valid() const
  {
    return mPtr != nullptr;
  }

  /// @brief 適正なポインタを持っていない時 true を返す．
  bool
  is_invalid() const
  {
    return !is_valid();
  }

  /// @brief dereference 演算子
  AigMgrImpl*
  get() const
  {
    _check_valid();
    return mPtr.get();
  }

  /// @brief get() の別名
  AigMgrImpl*
  operator->() const
  {
    return get();
  }

  /// @brief 同じマネージャを持つかチェックする．
  bool
  check_mgr(
    const AigMgrHolder& right
  ) const
  {
    return mPtr == right.mPtr;
  }


protected:
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
  AigMgrImpl*
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

  /// @brief 適正なポインタを持っていない時，例外を送出する．
  void
  _check_valid() const
  {
    if ( is_invalid() ) {
      throw std::logic_error{"mPtr is null"};
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ポインタ本体
  std::shared_ptr<AigMgrImpl> mPtr;

};

END_NAMESPACE_YM_AIG

#endif // AIGMGRHOLDER_H
