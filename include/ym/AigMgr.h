#ifndef AIGMGR_H
#define AIGMGR_H

/// @file AigMgr.h
/// @brief AigMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/aig.h"
#include "ym/AigMgrHolder.h"
#include "ym/AigHandle.h"
#include "ym/Expr.h"
#include "ym/JsonValue.h"


BEGIN_NAMESPACE_YM_AIG

class AigEdge;

//////////////////////////////////////////////////////////////////////
/// @class AigMgr AigMgr.h "AigMgr.h"
/// @brief AIG 全体を管理するクラス
///
/// 実体は実装クラスへのスマートポインタなのでコピーしても
/// 実装クラスは共有される．
/// 実装クラスはすべての参照がなくなった時に開放される．
//////////////////////////////////////////////////////////////////////
class AigMgr :
  public AigMgrHolder
{
public:

  /// @brief コンストラクタ
  AigMgr();

  /// @brief 内容を指定したコンストラクタ
  explicit
  AigMgr(
    const AigMgrHolder& holder
  );

  /// @brief デストラクタ
  ~AigMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード数を返す．
  SizeType
  node_num() const;

  /// @brief 入力ノード数を返す．
  SizeType
  input_num() const;

  /// @brief 入力のハンドルを返す．
  ///
  /// このハンドルは常に反転なし
  AigHandle
  input(
    SizeType input_id ///< [in] 入力番号 ( 0 <= input_id < input_num() )
  );

  /// @brief ANDノード数を返す．
  SizeType
  and_num() const;

  /// @brief 論理シミュレーションを行う．
  /// @return hanndle_list に対応する出力値のリストを返す．
  vector<AigBitVect>
  eval(
    const vector<AigBitVect>& input_vals, ///< [in] 入力値のリスト
    const vector<AigHandle>& handle_list  ///< [in] 出力ハンドルのリスト
  ) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を操作する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数0 のハンドルを作る．
  AigHandle
  make_zero()
  {
    return AigHandle::zero();
  }

  /// @brief 定数1 のハンドルを作る．
  AigHandle
  make_one()
  {
    return AigHandle::one();
  }

  /// @brief 外部入力ノードを作る．
  AigHandle
  make_input();

  /// @brief 複数の入力の AND を表すハンドルを返す．
  AigHandle
  and_op(
    const vector<AigHandle>& fanin_list ///< [in] ファンインのハンドルのリスト
  );

  /// @brief 複数の入力の OR を表すハンドルを返す．
  AigHandle
  or_op(
    const vector<AigHandle>& fanin_list ///< [in] ファンインのハンドルのリスト
  );

  /// @brief 複数の入力の XOR を表すハンドルを返す．
  AigHandle
  xor_op(
    const vector<AigHandle>& fanin_list ///< [in] ファンインのハンドルのリスト
  );

  /// @brief Expr から変換する．
  AigHandle
  from_expr(
    const Expr& expr ///< [in] 論理式
  );

  /// @brief 複数の Expr から変換する．
  vector<AigHandle>
  from_expr_list(
    const vector<Expr>& expr_list ///< [in] 論理式のリスト
  );

  /// @brief コファクター演算
  ///
  /// - cube に含まれるノードの値を固定した AIG を作る．
  vector<AigHandle>
  cofactor(
    const vector<AigHandle>& cube,       ///< [in] コファクターキューブ
    const vector<AigHandle>& handle_list ///< [in] 対象のハンドルのリスト
  );

  /// @brief 等価比較演算子
  bool
  operator==(
    const AigMgr& right
  ) const
  {
    return check_mgr(right);
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const AigMgr& right
  ) const
  {
    return !operator==(right);
  }

};

END_NAMESPACE_YM_AIG

#endif // AIGMGR_H
