#ifndef AIGNODE_H
#define AIGNODE_H

/// @file AigNode.h
/// @brief AigNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/aig.h"
#include "AigEdge.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
/// @class AigNode AigNode.h "AigNode.h"
/// @brief AIG のノードを表すクラス
///
/// 以下の情報を持つ．
/// - ID番号
/// - 参照回数
/// - 入力ノードかANDノードかのフラグ
/// - 入力ノード: 入力番号
/// - ANDノード: 2つのファンインの枝
//////////////////////////////////////////////////////////////////////
class AigNode
{
  friend class AigMgrImpl; // 参照回数の更新のため

public:

  /// @brief 入力ノード用のコンストラクタ
  AigNode(
    SizeType id,      ///< [in] ノード番号
    SizeType input_id ///< [in] 入力番号
  ) : mId{id},
      mRefCount{0}
  {
    _set_input(input_id);
  }

  /// @brief ANDノード用のコンストラクタ
  AigNode(
    SizeType id,    ///< [in] ノード番号
    AigEdge fanin0, ///< [in] ファンイン0の枝
    AigEdge fanin1  ///< [in] ファンイン1の枝
  ) : mId{id},
      mRefCount{0}
  {
    _set_and(fanin0, fanin1);
  }

  /// @brief デストラクタ
  ~AigNode() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 基本情報
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  SizeType
  id() const
  {
    return mId;
  }

  /// @brief 入力ノードの時，true を返す．
  bool
  is_input() const
  {
    return mInput;
  }

  /// @brief ANDノードの時，true を返す．
  bool
  is_and() const
  {
    return !is_input();
  }

  /// @brief 参照回数を返す．
  SizeType
  ref_count() const
  {
    return mRefCount;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 入力ノード用の情報取得関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力番号を返す．
  SizeType
  input_id() const
  {
    _check_input("input_id");
    return reinterpret_cast<PtrIntType>(mFanins[0]);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // ANDノード用の情報取得関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファンインのノードを返す．
  AigNode*
  fanin_node(
    SizeType pos ///< [in] 位置 ( 0 or 1 )
  ) const
  {
    _check_and("fanin_node");
    // 安全のため pos の範囲を補正しておく．
    pos &= 1;
    return mFanins[pos];
  }

  /// @brief ファンイン0のノードを返す．
  AigNode*
  fanin0_node() const
  {
    _check_and("fanin0_node");
    return mFanins[0];
  }

  /// @brief ファンイン1のノードを返す．
  AigNode*
  fanin1_node() const
  {
    _check_and("fanin1_node");
    return mFanins[1];
  }

  /// @brief ファンインの反転属性を返す．
  bool
  fanin_inv(
    SizeType pos ///< [in] 位置 ( 0 or 1 )
  ) const
  {
    _check_and("fanin_inv");
    // 安全のため pos の範囲を補正しておく．
    pos &= 1;
    return ( pos == 0 )? mInv0 : mInv1;
  }

  /// @brief ファンイン0の反転属性を返す．
  bool
  fanin0_inv() const
  {
    _check_and("fanin0_inv");
    return mInv0;
  }

  /// @brief ファンイン1の反転属性を返す．
  bool
  fanin1_inv() const
  {
    _check_and("fanin1_inv");
    return mInv1;
  }

  /// @brief ファンインの枝を返す．
  AigEdge
  fanin(
    SizeType pos ///< [in] 位置 ( 0 or 1 )
  ) const
  {
    _check_and("fanin");
    // 安全のため pos の範囲を補正しておく．
    pos &= 1;
    return (pos == 0)? fanin0() : fanin1();
  }

  /// @brief ファンイン0の枝を返す．
  AigEdge
  fanin0() const
  {
    _check_and("fanin0");
    return AigEdge{mFanins[0], mInv0};
  }

  /// @brief ファンイン0のハンドルを返す．
  AigEdge
  fanin1() const
  {
    _check_and("fanin1");
    return AigEdge{mFanins[1], mInv1};
  }

  /// @brief ANDグループのファンインのリストを返す．
  ///
  /// ANDグループとは反転していない枝で繋がったANDノードの集合
  vector<AigEdge>
  ex_fanin_list() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 参照回数の更新を行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 参照回数を増やす．
  /// @return 結果の参照回数を返す．
  SizeType
  inc_ref()
  {
    ++ mRefCount;
    return mRefCount;
  }

  /// @brief 参照回数を減らす．
  /// @return 結果の参照回数を返す．
  SizeType
  dec_ref()
  {
    -- mRefCount;
    return mRefCount;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ノードの内容をセットする．
  void
  _set_input(
    SizeType input_id ///< [in] 入力番号
  )
  {
    mFanins[0] = reinterpret_cast<AigNode*>(input_id);
    mFanins[1] = nullptr;
    mInput = true;
    mInv0 = false;
    mInv1 = false;
  }

  /// @brief ANDノードの内容をセットする．
  void
  _set_and(
    AigEdge fanin0, ///< [in] ファンイン0の枝
    AigEdge fanin1  ///< [in] ファンイン1の枝
  )
  {
    mFanins[0] = fanin0.node();
    mFanins[1] = fanin1.node();
    mInput = false;
    mInv0 = fanin0.inv();
    mInv1 = fanin1.inv();
  }

  /// @brief 入力専用の関数の例外発生
  void
  _check_input(
    const char* name ///< [in] 関数名
  ) const
  {
    if ( !is_input() ) {
      ostringstream buf;
      buf << "AigNode::" << name << "() is valid if is_input() == true";
      throw std::invalid_argument{buf.str()};
    }
  }

  /// @brief AND 専用の関数の例外発生
  void
  _check_and(
    const char* name ///< [in] 関数名
  ) const
  {
    if ( !is_and() ) {
      ostringstream buf;
      buf << "AigNode::" << name << "() is valid if is_and() == true";
      throw std::invalid_argument{buf.str()};
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  SizeType mId;

  // ファンインのノード
  AigNode* mFanins[2]{nullptr, nullptr};

  // 入力/ANDの区別
  bool mInput : 1;

  // ファンイン0の極性
  bool mInv0 : 1;

  // ファンイン1の極性
  bool mInv1 : 1;

  // 参照回数(ファンアウト数)
  SizeType mRefCount : 61;

};

END_NAMESPACE_YM_AIG

#endif // AIGNODE_H
