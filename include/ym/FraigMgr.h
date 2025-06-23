#ifndef FRAIGMGR_H
#define FRAIGMGR_H

/// @file FraigMgr.h
/// @brief FraigMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/fraig.h"
#include "ym/FraigHandle.h"
#include "ym/SatInitParam.h"
#include "ym/SatBool3.h"


BEGIN_NAMESPACE_YM_FRAIG

class MgrImpl;

//////////////////////////////////////////////////////////////////////
/// @class FraigMgr FraigMgr.h "FraigMgr.h"
/// @brief FRAIG マネージャ
///
/// 実装は MgrImpl が行う．
/// このクラスは MgrImpl へのユニークポインタを持つだけ．
//////////////////////////////////////////////////////////////////////
class FraigMgr
{
public:

  /// @brief コンストラクタ
  FraigMgr(
    SizeType sig_size,                  ///< [in] シグネチャのサイズ
    const SatInitParam& init_param = {} ///< [in] SAT-solver の初期化パラメータ
  );

  /// @brief デストラクタ
  ~FraigMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 構造(FraigNode)を作成するメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数0関数をつくる．
  FraigHandle
  make_zero();

  /// @brief 定数1関数をつくる．
  FraigHandle
  make_one();

  /// @brief 外部入力を作る．
  FraigHandle
  make_input();

  /// @brief 2つのノードの AND を作る．
  FraigHandle
  make_and(
    FraigHandle edge1, ///< [in] 入力1のハンドル
    FraigHandle edge2  ///< [in] 入力2のハンドル
  );

  /// @brief 複数ノードの AND を作る．
  FraigHandle
  make_and(
    const vector<FraigHandle>& edge_list ///< [in] 入力のハンドルのリスト
  );

  /// @brief ２つのノードの OR を作る．
  FraigHandle
  make_or(
    FraigHandle edge1, ///< [in] 入力1のハンドル
    FraigHandle edge2  ///< [in] 入力2のハンドル
  );

  /// @brief 複数ノードの OR を作る．
  FraigHandle
  make_or(
    const vector<FraigHandle>& edge_list ///< [in] 入力のハンドルのリスト
  );

  /// @brief ２つのノードの XOR を作る．
  FraigHandle
  make_xor(
    FraigHandle edge1, ///< [in] 入力1のハンドル
    FraigHandle edge2  ///< [in] 入力2のハンドル
  );

  /// @brief 複数ノードの XOR を作る．
  FraigHandle
  make_xor(
    const vector<FraigHandle>& edge_list ///< [in] 入力のハンドルのリスト
  );

  /// @brief MUX を作る
  FraigHandle
  make_mux(
    FraigHandle sel_edge, ///< [in] 選択信号線
    FraigHandle edge0,    ///< [in] sel_edge == 0 の時選ばれる入力
    FraigHandle edge1     ///< [in] sel_edge == 0 の時選ばれる入力
  );

  /// @brief コファクターを計算する．
  FraigHandle
  make_cofactor(
    FraigHandle edge,  ///< [in] 対象のハンドル
    SizeType input_id, ///< [in] 入力番号
    bool inv           ///< [in] 反転フラグ
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 検証用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 2つのハンドルが等価かどうか調べる．
  SatBool3
  check_equiv(
    FraigHandle aig1, ///< [in] 入力1のハンドル
    FraigHandle aig2  ///< [in] 入力2のハンドル
  );

  /// @brief ログレベルを設定する．
  void
  set_loglevel(
    SizeType level ///< [in] ログレベル
  );

  /// @brief ログ出力用ストリームを設定する．
  void
  set_logstream(
    ostream* out ///< [in] 出力ストリーム
  );

  /// @brief 内部の統計情報を出力する．
  void
  dump_stats(
    ostream& s ///< [in] 出力ストリーム
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実装クラスへのポインタ
  std::unique_ptr<MgrImpl> mPtr;

};

END_NAMESPACE_YM_FRAIG

#endif // FRAIGMGR_H
