#ifndef MGRIMPL_H
#define MGRIMPL_H

/// @file MgrImpl.h
/// @brief MgrImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/fraig.h"
#include "ym/FraigHandle.h"
#include "StructTable.h"
#include "PatTable.h"
#include "FraigSat.h"
#include <random>

#include "ym/SatBool3.h"
#include "ym/SatInitParam.h"


BEGIN_NAMESPACE_YM_FRAIG

//////////////////////////////////////////////////////////////////////
/// @class MgrImpl MgrImpl.h "MgrImpl.h"
/// @brief FraigMgr の実装クラス
//////////////////////////////////////////////////////////////////////
class MgrImpl
{
public:

  /// @brief コンストラクタ
  MgrImpl(
    SizeType sig_size,                  ///< [in] シグネチャのサイズ
    const SatInitParam& init_param = {} ///< [in] SAT-solver の初期化パラメータ
  );

  /// @brief デストラクタ
  ~MgrImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得するメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ノード数を得る．
  SizeType
  input_num() const
  {
    return mInputNodes.size();
  }

  /// @brief 入力ノードを取り出す．
  FraigNode*
  input_node(
    SizeType pos ///< [in] 入力番号 ( 0 <= pos < input_num() )
  ) const
  {
    ASSERT_COND( pos >= 0 && pos < input_num() );

    return mInputNodes[pos];
  }

  /// @brief ノード数を得る．
  SizeType
  node_num() const
  {
    return mAllNodes.size();
  }

  /// @brief ノードを取り出す．
  ///
  /// ANDノードの他に入力ノードも含まれる．
  FraigNode*
  node(
    SizeType pos ///< [in] ノード番号 ( 0 <= pos < node_num() )
  ) const
  {
    if ( pos >= node_num() ) {
      throw std::out_of_range{"pos is out of range"};
    }
    return mAllNodes[pos].get();
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 構造(FraigNode)を作成するメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数0関数をつくる．
  FraigHandle
  make_zero()
  {
    return FraigHandle::zero();
  }

  /// @brief 定数1関数をつくる．
  FraigHandle
  make_one()
  {
    return FraigHandle::one();
  }

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
  )
  {
    return make_and_sub(edge_list, 0, edge_list.size(), false);
  }

  /// @brief ２つのノードの OR を作る．
  FraigHandle
  make_or(
    FraigHandle edge1, ///< [in] 入力1のハンドル
    FraigHandle edge2  ///< [in] 入力2のハンドル
  )
  {
    return ~make_and(~edge1, ~edge2);
  }

  /// @brief 複数ノードの OR を作る．
  FraigHandle
  make_or(
    const vector<FraigHandle>& edge_list ///< [in] 入力のハンドルのリスト
  )
  {
    return make_and_sub(edge_list, 0, edge_list.size(), true);
  }

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
  )
  {
    return make_xor_sub(edge_list, 0, edge_list.size());
  }

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
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief make_and() の下請け関数
  FraigHandle
  make_and_sub(
    const vector<FraigHandle>& edge_list, ///< [in] 入力ハンドルのリスト
    SizeType start_pos,                   ///< [in] 開始位置
    SizeType end_pos,                     ///< [in] 終了位置
    bool iinv                             ///< [in] 入力の反転フラグ
  );

  /// @brief make_xor() の下請け関数
  FraigHandle
  make_xor_sub(
    const vector<FraigHandle>& edge_list, ///< [in] 入力ハンドルのリスト
    SizeType start_pos,                   ///< [in] 開始位置
    SizeType end_pos                      ///< [in] 終了位置
  );

  /// @brief make_cofactor() の下請け関数
  FraigHandle
  cofactor_sub(
    FraigNode* node,   ///< [in] 対象のNode
    SizeType input_id, ///< [in] 置き換える変数番号
    bool inv           ///< [in] 置き換える極性
  );

  /// @brief 縮退検査を行う．
  /// @retval SatBool3::True 定数に縮退していた．
  /// @retval SatBool3::False 定数ではなかった．
  /// @retval SatBool3::X 不明(アボート)
  SatBool3
  verify_const(
    FraigNode* node, ///< [in] 対象のノード
    FraigHandle& ans ///< [out] 答
  );

  /// @brief パタンハッシュを調べて論理的に等価なノードを探す．
  /// @return 等価なハンドル
  ///
  /// 等価なノードが見つからなかった場合はパタンハッシュに登録する．
  FraigHandle
  check_pat(
    FraigNode* node ///< [in] 対象のノード
  );

  /// @brief 全ノードのシミュレーションパタン用配列を拡大する．
  void
  resize_pat(
    SizeType size ///< [in] サイズ
  );

  /// @breif 直前の SAT の反例を加えて再ハッシュする．
  void
  add_pat(
    FraigNode* node ///< [in] 対象のノード
  );

  /// @brief ノードを登録する．
  void
  reg_node(
    FraigNode* node
  );

  /// @brief ハンドルの内容を表す文字列を作る．
  std::string
  print_handle(
    FraigHandle handle
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID 番号をキーにしたノードの配列
  // ノードの所有権を持つ．
  std::vector<std::unique_ptr<FraigNode>> mAllNodes;

  // 入力ノードの配列
  std::vector<FraigNode*> mInputNodes;

  // 構造ハッシュ
  StructTable mStructTable;

  // パタンハッシュ
  PatTable mPatTable;

  // 乱数発生器
  std::mt19937 mRandGen;

  // SATソルバ
  FraigSat mSolver;

  // recsolver 用のストリーム
  std::ostream* mOutP;

  // ログレベル
  SizeType mLogLevel;

  // ログ出力用のストリーム
  std::ostream* mLogStream;

};

END_NAMESPACE_YM_FRAIG

#endif // MGRIMPL_H
