#ifndef AIGMGRIMPL_H
#define AIGMGRIMPL_H

/// @file AigMgrImpl.h
/// @brief AigMgrImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/aig.h"
#include "ym/Expr.h"
#include "aig/AigNode.h"
#include "AigTable.h"
#include "ym/JsonValue.h"


BEGIN_NAMESPACE_YM_AIG

class ValArray;
class EdgeDict;

//////////////////////////////////////////////////////////////////////
/// @class AigMgrImpl AigMgrImpl.h "AigMgrImpl.h"
/// @brief AigMgr の実装クラス
//////////////////////////////////////////////////////////////////////
class AigMgrImpl
{
public:

  /// @brief コンストラクタ
  AigMgrImpl();

  /// @brief デストラクタ
  ~AigMgrImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード数を返す．
  SizeType
  node_num() const
  {
    return mNodeNum;
  }

  /// @brief 入力ノード数を返す．
  SizeType
  input_num() const
  {
    return mInputArray.size();
  }

  /// @brief 入力の枝を返す．
  ///
  /// この枝は常に反転なし
  AigEdge
  input(
    SizeType input_id ///< [in] 入力番号 ( 0 <= input_id < input_num() )
  ) const
  {
    if ( input_id >= input_num() ) {
      throw std::out_of_range{"'input_id' is out of range"};
    }
    return AigEdge{mInputArray[input_id], false};
  }

  /// @brief ANDノード数を返す．
  SizeType
  and_num() const
  {
    return mAndTable.size();
  }

  /// @brief ANDノードの入力側からのトポロジカル順のリストを得る．
  std::vector<AigNode*>
  and_list() const;

  /// @brief 論理シミュレーションを行う．
  /// @return output_list に対応する出力値のリストを返す．
  AigBitVect
  eval(
    const vector<AigBitVect>& input_vals, ///< [in] 入力値のリスト
    AigEdge output                        ///< [in] 出力
  ) const;

  /// @brief 論理シミュレーションを行う．
  vector<AigBitVect>
  eval(
    const vector<AigBitVect>& input_vals, ///< [in] 入力値のリスト
    const vector<AigEdge>& output_list    ///< [in] 出力のリスト
  ) const;

  /// @brief ノード数を返す．
  SizeType
  size(
    const vector<AigEdge>& edge_list ///< [in] 根の枝のリスト
  ) const;

  /// @brief dot 形式で出力する．
  void
  gen_dot(
    ostream& s,                       ///< [in] 出力ストリーム
    const vector<AigEdge>& edge_list, ///< [in] 根の枝のリスト
    const JsonValue& option           ///< [in] オプションを表す JSON オブジェクト
  ) const;

  /// @brief 外部入力ノードを作る．
  AigEdge
  make_input()
  {
    auto node = _new_input();
    return AigEdge{node, false};
  }

  /// @brief ANDノードを作る．
  AigEdge
  and_op(
    const AigEdge& fanin0, ///< [in] ファンイン0のハンドル
    const AigEdge& fanin1  ///< [in] ファンイン1のハンドル
  )
  {
    // step1: 境界条件
    if ( fanin0.is_zero() ) {
      return AigEdge::zero();
    }
    if ( fanin1.is_zero() ) {
      return AigEdge::zero();
    }
    if ( fanin0.is_one() ) {
      return fanin1;
    }
    if ( fanin1.is_one() ) {
      return fanin0;
    }

    // step2: 構造テーブルを探す．
    AigNode key{0, fanin0, fanin1};
    auto p = mAndTable.find(&key);
    if ( p != mAndTable.end() ) {
      auto node = *p;
      return AigEdge{node, false};
    }

    // step3: 新しいノードを作る．
    auto node = _new_and(fanin0, fanin1);
    return AigEdge{node, false};
  }

  /// @brief ORノードを作る．
  AigEdge
  or_op(
    const AigEdge& fanin0, ///< [in] ファンイン0のハンドル
    const AigEdge& fanin1  ///< [in] ファンイン1のハンドル
  )
  {
    // DeMorgan の法則
    return ~and_op(~fanin0, ~fanin1);
  }

  /// @brief xORノードを作る．
  AigEdge
  xor_op(
    const AigEdge& fanin0, ///< [in] ファンイン0のハンドル
    const AigEdge& fanin1  ///< [in] ファンイン1のハンドル
  )
  {
    // ab' + a'b を作る．
    auto h1 = and_op(fanin0, ~fanin1);
    auto h2 = and_op(~fanin0, fanin1);
    return or_op(h1, h2);
  }

  /// @brief 複数の入力の AND を表すハンドルを返す．
  AigEdge
  and_op(
    const vector<AigEdge>& fanin_list ///< [in] ファンインのハンドルのリスト
  )
  {
    auto n = fanin_list.size();
    return and_sub(fanin_list, 0, n);
  }

  /// @brief 複数の入力の OR を表すハンドルを返す．
  AigEdge
  or_op(
    const vector<AigEdge>& fanin_list ///< [in] ファンインのハンドルのリスト
  )
  {
    auto n = fanin_list.size();
    return or_sub(fanin_list, 0, n);
  }

  /// @brief 複数の入力の XOR を表すハンドルを返す．
  AigEdge
  xor_op(
    const vector<AigEdge>& fanin_list ///< [in] ファンインのハンドルのリスト
  )
  {
    auto n = fanin_list.size();
    return xor_sub(fanin_list, 0, n);
  }

  /// @brief Expr から変換する．
  AigEdge
  from_expr(
    const Expr& expr ///< [in] 論理式
  );

  /// @brief 複数の Expr から変換する．
  vector<AigEdge>
  from_expr_list(
    const vector<Expr>& expr_list ///< [in] 論理式のリスト
  );

  /// @brief コファクター演算
  ///
  /// - cube に含まれるノードの値を固定した AIG を作る．
  AigEdge
  cofactor(
    const vector<AigEdge>& cedge_list, ///< [in] コファクターキューブ
    AigEdge edge                       ///< [in] 対象の枝
  );

  /// @brief コファクター演算
  ///
  /// - cube に含まれるノードの値を固定した AIG を作る．
  vector<AigEdge>
  cofactor(
    const vector<AigEdge>& cedge_list, ///< [in] コファクターキューブ
    const vector<AigEdge>& edge_list   ///< [in] 対象の枝のリスト
  );

  /// @brief 参照回数が0のノードを取り除く
  ///
  /// ノードのID番号が変わる可能性がある．
  void
  sweep();

  /// @brief 枝の参照回数を増やす．
  void
  inc_ref(
    AigEdge edge ///< [in] 対象の枝
  )
  {
    auto node = edge.node();
    if ( node != nullptr ) {
      node->inc_ref();
    }
  }

  /// @brief 枝の参照回数を減らす．
  void
  dec_ref(
    AigEdge edge ///< [in] 対象の枝
  )
  {
    auto node = edge.node();
    if ( node != nullptr ) {
      if ( node->dec_ref() == 0 ) {
	_free_node(node);
      }
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief and_op() の下請け関数
  AigEdge
  and_sub(
    const vector<AigEdge>& fanin_list, ///< [in] ファンインのリスト
    SizeType begin,                    ///< [in] 開始位置
    SizeType end                       ///< [in] 終了位置
  );

  /// @brief or_op() の下請け関数
  AigEdge
  or_sub(
    const vector<AigEdge>& fanin_list, ///< [in] ファンインのリスト
    SizeType begin,                    ///< [in] 開始位置
    SizeType end                       ///< [in] 終了位置
  );

  /// @brief xor_op() の下請け関数
  AigEdge
  xor_sub(
    const vector<AigEdge>& fanin_list, ///< [in] ファンインのリスト
    SizeType begin,                    ///< [in] 開始位置
    SizeType end                       ///< [in] 終了位置
  );

  /// @brief eval() の下請け関数
  void
  eval_sub(
    const vector<AigBitVect>& input_vals, ///< [in] 入力値のリスト
    ValArray& val_array ///< [in] シミュレーション結果を格納するオブジェクト
  ) const;

  /// @brief cofactor() の下請け関数
  AigEdge
  cofactor_sub(
    AigEdge edge,      ///< [in] 対象の枝
    EdgeDict& res_dict ///< [in] 結果を格納する辞書
  );

  /// @brief 入力ノードを作る．
  AigNode*
  _new_input()
  {
    auto input_id = mInputArray.size();
    auto id = mNodeArray.size();
    auto node = new AigNode(id, input_id);
    mNodeArray.push_back(std::unique_ptr<AigNode>{node});
    mInputArray.push_back(node);
    ++ mNodeNum;
    return node;
  }

  /// @brief ANDノードを作る．
  AigNode*
  _new_and(
    AigEdge fanin0,
    AigEdge fanin1
  )
  {
    auto id = mNodeArray.size();
    auto node = new AigNode{id, fanin0, fanin1};
    mNodeArray.push_back(std::unique_ptr<AigNode>{node});
    inc_ref(fanin0);
    inc_ref(fanin1);
    mAndTable.insert(node);
    ++ mNodeNum;
    return node;
  }

  /// @brief ノードを削除する．
  ///
  /// 実際には sweep() を呼ぶまでは削除されない．
  void
  _free_node(
    AigNode* node
  )
  {
    if ( node->is_input() ) {
      // 入力ノードは削除しない．
      return;
    }
    -- mNodeNum;
    mAndTable.erase(node);
    dec_ref(node->fanin0());
    dec_ref(node->fanin1());
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 参照回数
  SizeType mRefCount{0};

  // アクティブなノード数
  SizeType mNodeNum{0};

  // ID番号をキーにして AigNode を収めた配列
  // AigNode の所有権を持つ．
  std::vector<std::unique_ptr<AigNode>> mNodeArray;

  // 入力番号をキーにして入力ノードを収めた配列
  std::vector<AigNode*> mInputArray;

  // ANDノードの構造ハッシュ
  AigTable mAndTable;

};

END_NAMESPACE_YM_AIG

#endif // AIGMGRIMPL_H
