
/// @file FraigMgr.cc
/// @brief FraigMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/FraigMgr.h"
#include "MgrImpl.h"


BEGIN_NAMESPACE_YM_FRAIG

//////////////////////////////////////////////////////////////////////
// クラス FraigMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FraigMgr::FraigMgr(
  SizeType sig_size,
  const SatInitParam& init_param
) : mPtr{new MgrImpl(sig_size, init_param)}
{
}

// @brief デストラクタ
FraigMgr::~FraigMgr()
{
}

// @brief 定数0関数をつくる．
FraigHandle
FraigMgr::make_zero()
{
  return mPtr->make_zero();
}

// @brief 定数1関数をつくる．
FraigHandle
FraigMgr::make_one()
{
  return mPtr->make_one();
}

// @brief 外部入力を作る．
FraigHandle
FraigMgr::make_input()
{
  return mPtr->make_input();
}

// @brief 2つのノードの AND を作る．
FraigHandle
FraigMgr::make_and(
  FraigHandle edge1,
  FraigHandle edge2
)
{
  return mPtr->make_and(edge1, edge2);
}

// @brief 複数ノードの AND を作る．
FraigHandle
FraigMgr::make_and(
  const vector<FraigHandle>& edge_list ///< [in] 入力のハンドルのリスト
)
{
  return mPtr->make_and(edge_list);
}

// @brief ２つのノードの OR を作る．
FraigHandle
FraigMgr::make_or(
  FraigHandle edge1,
  FraigHandle edge2
)
{
  return mPtr->make_or(edge1, edge2);
}

// @brief 複数ノードの OR を作る．
FraigHandle
FraigMgr::make_or(
  const vector<FraigHandle>& edge_list
)
{
  return mPtr->make_or(edge_list);
}

// @brief ２つのノードの XOR を作る．
FraigHandle
FraigMgr::make_xor(
  FraigHandle edge1,
  FraigHandle edge2
)
{
  return mPtr->make_xor(edge1, edge2);
}

// @brief 複数ノードの XOR を作る．
FraigHandle
FraigMgr::make_xor(
  const vector<FraigHandle>& edge_list
)
{
  return mPtr->make_xor(edge_list);
}

// @brief MUX を作る
FraigHandle
FraigMgr::make_mux(
  FraigHandle sel_edge,
  FraigHandle edge0,
  FraigHandle edge1
)
{
  return mPtr->make_mux(sel_edge, edge0, edge1);
}

// @brief コファクターを計算する．
FraigHandle
FraigMgr::make_cofactor(
  FraigHandle edge,
  SizeType input_id,
  bool inv
)
{
  return mPtr->make_cofactor(edge, input_id, inv);
}

// @brief 2つのハンドルが等価かどうか調べる．
SatBool3
FraigMgr::check_equiv(
  FraigHandle aig1,
  FraigHandle aig2
)
{
  return mPtr->check_equiv(aig1, aig2);
}

// @brief ログレベルを設定する．
void
FraigMgr::set_loglevel(
  SizeType level
)
{
  mPtr->set_loglevel(level);
}

// @brief ログ出力用ストリームを設定する．
void
FraigMgr::set_logstream(
  ostream* out
)
{
  mPtr->set_logstream(out);
}

// @brief 内部の統計情報を出力する．
void
FraigMgr::dump_stats(
  ostream& s
)
{
  mPtr->dump_stats(s);
}

END_NAMESPACE_YM_FRAIG
