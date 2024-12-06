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
#include "AigNode.h"
#include "AigTable.h"


BEGIN_NAMESPACE_YM_AIG

class ValArray;
class EdgeDict;

//////////////////////////////////////////////////////////////////////
/// @class AigMgrImpl AigMgrImpl.h "AigMgrImpl.h"
/// @brief AigMgr の実装クラス
//////////////////////////////////////////////////////////////////////
class AigMgrImpl
{
  friend class AigMgrPtr;

private:

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
    return mNodeArray.size();
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

  /// @brief 外部入力ノードを作る．
  AigEdge
  make_input()
  {
    auto id = mNodeArray.size();
    auto input_id = mInputArray.size();
    auto node = new AigNode{id, input_id};
    mNodeArray.push_back(node);
    mInputArray.push_back(node);
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
    auto id = mNodeArray.size();
    auto node = new AigNode{id, fanin0, fanin1};
    mNodeArray.push_back(node);
    mAndTable.insert(node);
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


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 参照回数を増やす
  void
  inc();

  /// @brief 参照回数を減らす
  void
  dec();

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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 参照回数
  SizeType mRefCount{0};

  // ID番号をキーにして AigNode を収めた配列
  vector<AigNode*> mNodeArray;

  // 入力番号をキーにして入力ノードを収めた配列
  vector<AigNode*> mInputArray;

  // ANDノードの構造ハッシュ
  AigTable mAndTable;

};

END_NAMESPACE_YM_AIG

#endif // AIGMGRIMPL_H
