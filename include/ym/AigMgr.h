#ifndef AIGMGR_H
#define AIGMGR_H

/// @file AigMgr.h
/// @brief AigMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/aig.h"
#include "ym/AigMgrPtr.h"
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
class AigMgr
{
public:

  /// @brief コンストラクタ
  AigMgr();

  /// @brief 内容を指定したコンストラクタ
  AigMgr(
    const AigMgrPtr& ptr
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

  /// @brief dot 形式で出力する．
  ///
  /// - option は以下のようなキーを持った JSON オブジェクト
  ///   * attr: dot の各種属性値を持った辞書
  ///     属性値は <グループ名> ':' <属性名> で表す．
  ///     グループ名は以下の通り
  ///     - graph:    グラフ全体
  ///     - root:     根のノード
  ///     - node:     通常のノード
  ///     - terminal: 入力ノード
  ///     - edge:     枝
  ///     グループ名と ':' がない場合には全てのグループに対して同一の属性値
  ///     を適用する．
  ///     具体的な属性名と属性値については graphviz の仕様を参照すること．
  ///   * var_label: 変数ラベルを表す配列．配列のキーは変数番号
  ///   * var_texlbl: TeX用の変数ラベルを表す配列．配列のキーは変数番号
  ///   * var_label と var_texlbl は排他的となる．var_texlbl がある時，
  ///     var_label は無視される．
  static
  void
  gen_dot(
    ostream& s,                         ///< [in] 出力ストリーム
    const vector<AigHandle>& root_list, ///< [in] 出力のリスト
    const JsonValue& option             ///< [in] オプションを表す JSON オブジェクト
    = JsonValue{}
  );


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
    return mMgr == right.mMgr;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const AigMgr& right
  ) const
  {
    return !operator==(right);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実装クラスのスマートポインタ
  AigMgrPtr mMgr;

};

END_NAMESPACE_YM_AIG

#endif // AIGMGR_H
