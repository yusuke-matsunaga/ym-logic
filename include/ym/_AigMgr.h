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

  /// @brief ANDノード数を返す．
  SizeType
  and_num() const;

  /// @brief 論理シミュレーションを行う．
  /// @return hanndle_list に対応する出力値のリストを返す．
  vector<AigBitVect>
  eval(
    const std::unordered_map<SizeType, AigBitVect>& input_vals, ///< [in] 入力値の辞書
    const vector<AigHandle>& handle_list                        ///< [in] 出力ハンドルのリスト
  ) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を操作する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief コピーを作る．
  ///
  /// handle が自身に属している場合には handle を返す．
  AigHandle
  copy(
    AigHandle handle
  );

  /// @brief 複数のハンドルのコピーを作る．
  std::vector<AigHandle>
  copy(
    const std::vector<AigHandle>& handle_list
  );

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

  /// @brief 外部入力ノードを返す．
  AigHandle
  input(
    SizeType input_id ///< [in] 入力番号
  );

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

  /// @brief PrimType の演算を行う．
  AigHandle
  primitive_op(
    PrimType type,                      ///< [in] 演算の種類
    const vector<AigHandle>& fanin_list ///< [in] ファンインのハンドルのリスト
  );

  /// @brief Expr から変換する．
  ///
  /// 論理式の i 番目の変数は i 番目の入力を用いる．
  AigHandle
  from_expr(
    const Expr& expr ///< [in] 論理式
  );

  /// @brief Expr から変換する．
  ///
  /// 論理式の i 番目の変数は input_list[i] を用いる．
  AigHandle
  from_expr(
    const Expr& expr,                        ///< [in] 論理式
    const std::vector<AigHandle>& input_list ///< [in] 変数に対応するハンドルのリスト
  );

  /// @brief 複数の Expr から変換する．
  ///
  /// 論理式の i 番目の変数は i 番目の入力を用いる．
  vector<AigHandle>
  from_expr_list(
    const vector<Expr>& expr_list ///< [in] 論理式のリスト
  );

  /// @brief 複数の Expr から変換する．
  ///
  /// 論理式の i 番目の変数は input_list[i] を用いる．
  vector<AigHandle>
  from_expr_list(
    const vector<Expr>& expr_list,           ///< [in] 論理式のリスト
    const std::vector<AigHandle>& input_list ///< [in] 変数に対応するハンドルのリスト
  );

  /// @brief SopCover から変換する．
  ///
  /// カバーの i 番目の変数は i 番目の入力を用いる．
  AigHandle
  from_cover(
    const SopCover& cover ///< [in] カバー
  );

  /// @brief SopCover から変換する．
  ///
  /// カバーの i 番目の変数は input_list[i] を用いる．
  AigHandle
  from_cover(
    const SopCover& cover,                   ///< [in] カバー
    const std::vector<AigHandle>& input_list ///< [in] 変数に対応するハンドルのリスト
  );

  /// @brief SopCube から変換する．
  ///
  /// キューブの i 番目の変数は i 番目の入力を用いる．
  AigHandle
  from_cube(
    const SopCube& cube ///< [in] キューブ
  );

  /// @brief SopCube から変換する．
  ///
  /// キューブの i 番目の変数は input_list[i] を用いる．
  AigHandle
  from_cube(
    const SopCube& cube,                     ///< [in] カバー
    const std::vector<AigHandle>& input_list ///< [in] 変数に対応するハンドルのリスト
  );

  /// @brief コファクター演算
  ///
  /// - cube に含まれるノードの値を固定した AIG を作る．
  vector<AigHandle>
  cofactor(
    const vector<AigHandle>& cube,       ///< [in] コファクターキューブ
    const vector<AigHandle>& handle_list ///< [in] 対象のハンドルのリスト
  );

  /// @brief local rewriting を行う．
  void
  rewrite();

  /// @brief 参照されていないノードを削除する．
  void
  sweep();


public:
  //////////////////////////////////////////////////////////////////////
  // 演算
  //////////////////////////////////////////////////////////////////////

  /// @brief 等価比較演算子
  ///
  /// 意味的な等価ではなく，同一のマネージャかどうか調べる．
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


public:
  //////////////////////////////////////////////////////////////////////
  // その他の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を出力する．
  void
  print(
    std::ostream& s ///< [in] 出力ストリーム
  ) const;

};

END_NAMESPACE_YM_AIG

#endif // AIGMGR_H
