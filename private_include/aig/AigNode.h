#ifndef AIGNODE_H
#define AIGNODE_H

/// @file AigNode.h
/// @brief AigNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/aig.h"
#include "ym/AigHandle.h"
#include "AigEdge.h"

#define DEBUG_AIGNODE 0
#define DOUT std::cout


BEGIN_NAMESPACE_YM_AIG

class AigHandle;

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
  friend class AigHandle;  // _inc_ref()/_dec_ref()
  friend class AigMgrImpl; // _inc_ref()/_dec_ref()

public:

  /// @brief 入力ノード用のコンストラクタ
  AigNode(
    SizeType id,      ///< [in] ノード番号
    SizeType input_id ///< [in] 入力番号
  ) : mId{id},
      mInput{true},
      mFanins{reinterpret_cast<PtrIntType>(input_id), 0}
  {
  }

  /// @brief ANDノード用のコンストラクタ
  AigNode(
    SizeType id,    ///< [in] ノード番号
    AigEdge fanin0, ///< [in] ファンイン0の枝
    AigEdge fanin1  ///< [in] ファンイン1の枝
  ) : mId{id},
      mInput{false},
      mFanins{fanin0.mPackedData, fanin1.mPackedData}
  {
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

  /// @brief 内容を出力する．
  void
  print(
    std::ostream& s ///< [in] 出力ストリーム
  ) const
  {
    s << "Node#" << id() << "["
      << ref_count() << "]: ";
    if ( is_input() ) {
      s << "Input#" << input_id();
    }
    else { // is_and()
      s << "And("
	<< fanin0()
	<< ", "
	<< fanin1()
	<< ")";
    }
    s << endl;
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
    return reinterpret_cast<SizeType>(mFanins[0]);
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
    return fanin(pos).node();
  }

  /// @brief ファンイン0のノードを返す．
  AigNode*
  fanin0_node() const
  {
    _check_and("fanin0_node");
    return fanin0().node();
  }

  /// @brief ファンイン1のノードを返す．
  AigNode*
  fanin1_node() const
  {
    _check_and("fanin1_node");
    return fanin1().node();
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
    return fanin(pos).inv();
  }

  /// @brief ファンイン0の反転属性を返す．
  bool
  fanin0_inv() const
  {
    _check_and("fanin0_inv");
    return fanin0().inv();
  }

  /// @brief ファンイン1の反転属性を返す．
  bool
  fanin1_inv() const
  {
    _check_and("fanin1_inv");
    return fanin1().inv();
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
    return AigEdge{mFanins[pos]};
  }

  /// @brief ファンイン0の枝を返す．
  AigEdge
  fanin0() const
  {
    _check_and("fanin0");
    return AigEdge{mFanins[0]};
  }

  /// @brief ファンイン0のハンドルを返す．
  AigEdge
  fanin1() const
  {
    _check_and("fanin1");
    return AigEdge{mFanins[1]};
  }

  /// @brief ANDグループのファンインのリストを返す．
  ///
  /// ANDグループとは反転していない枝で繋がったANDノードの集合
  vector<AigEdge>
  ex_fanin_list() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 参照回数を増やす
  bool
  _inc_ref()
  {
    ++ mRefCount;
    return mRefCount == 1;
  }

  /// @brief 参照回数を減らす
  bool
  _dec_ref()
  {
    -- mRefCount;
    return mRefCount == 0;
  }

  /// @brief 入力専用の関数の例外発生
  void
  _check_input(
    const char* name ///< [in] 関数名
  ) const
  {
    if ( !is_input() ) {
      abort();
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
      abort();
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
  SizeType mId : 63;

  // 入力/ANDの区別
  bool mInput : 1;

  // 参照回数
  SizeType mRefCount{0};

  // ファンインの枝
  PtrIntType mFanins[2];

};

END_NAMESPACE_YM_AIG

#endif // AIGNODE_H
