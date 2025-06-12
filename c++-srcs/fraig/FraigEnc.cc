
/// @file FraigEnc.cc
/// @brief FraigEnc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "FraigEnc.h"
#include "ym/BnModel.h"
#include "ym/BnNodeList.h"
#include "ym/BnFunc.h"
#include "ym/BddVar.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_EQUIV

//////////////////////////////////////////////////////////////////////
// クラス FraigEnc
//////////////////////////////////////////////////////////////////////

// @brief ネットワークの出力の論理関数 を AIG に変換する．
vector<FraigHandle>
FraigEnc::operator()(
  const BnModel& network,
  const vector<FraigHandle>& input_handles
)
{
  mNodeDict.clear();

  //////////////////////////////////////////////////////////////////////
  // 外部入力に対応するハンドルを登録する．
  //////////////////////////////////////////////////////////////////////
  auto ni = network.input_num();
  if ( input_handles.size() != ni ) {
    throw std::invalid_argument{"input_handles.size() mismatch"};
  }
  for ( SizeType i = 0; i < ni; ++ i ) {
    auto node = network.input(i);
    auto id = node.id();
    auto h = input_handles[i];
    mNodeDict.emplace(id, h);
  }

  //////////////////////////////////////////////////////////////////////
  // 論理ノードを作成する．
  //////////////////////////////////////////////////////////////////////
  for ( auto node: network.logic_list() ) {
    auto id = node.id();
    // ファンインのノードに対応するハンドルを求める．
    auto h = node2aig(node);
    // 登録しておく．
    mNodeDict.emplace(id, h);
  }

  //////////////////////////////////////////////////////////////////////
  // 外部出力のマップを作成する．
  //////////////////////////////////////////////////////////////////////
  auto no = network.output_num();
  vector<FraigHandle> output_handles;
  output_handles.reserve(no);
  for ( auto node: network.output_list() ) {
    auto id = node.id();
    ASSERT_COND( mNodeDict.count(id) > 0 );
    output_handles.push_back(mNodeDict.at(id));
  }

  return output_handles;
}

// @brief BnNode の出力の論理関数を Fraig に変換する．
FraigHandle
FraigEnc::node2aig(
  const BnNode& node
)
{
  // ファンインのノードに対応するハンドルを求める．
  auto ni = node.fanin_num();
  mInputList.clear();
  mInputList.reserve(ni);
  for ( auto inode: node.fanin_list() ) {
    auto iid = inode.id();
    ASSERT_COND( mNodeDict.count(iid) > 0 );
    mInputList.push_back(mNodeDict.at(iid));
  }

  // 個々の関数タイプに従って fraig を生成する．
  auto node_type = node.type();
  switch ( node_type ) {
  case BnNodeType::PRIMITIVE:
    switch ( node.primitive_type() ) {
    case PrimType::C0:
      return mMgr.make_zero();

    case PrimType::C1:
      return mMgr.make_one();

    case PrimType::Buff:
      return mInputList[0];

    case PrimType::Not:
      return ~mInputList[0];

    case PrimType::And:
      return mMgr.make_and(mInputList);

    case PrimType::Nand:
      return ~mMgr.make_and(mInputList);

    case PrimType::Or:
      return mMgr.make_or(mInputList);

    case PrimType::Nor:
      return ~mMgr.make_or(mInputList);

    case PrimType::Xor:
      return mMgr.make_xor(mInputList);

    case PrimType::Xnor:
      return ~mMgr.make_xor(mInputList);

    case PrimType::None:
      ASSERT_NOT_REACHED;
      break;
    }
    break;

  case BnNodeType::FUNC:
    {
      auto func = node.local_func();
      switch ( func.type() ) {
      case BnFuncType::COVER:
      case BnFuncType::EXPR:
	return expr2aig(func.expr());

      case BnFuncType::TVFUNC:
	return tv2aig(func.tvfunc(), 0);

      case BnFuncType::BDD:
	return bdd2aig(func.bdd());

      case BnFuncType::NONE:
	ASSERT_NOT_REACHED;
      }
    }
    break;

  default:
    ASSERT_NOT_REACHED;
    break;
  }
  // ダミー
  return mMgr.make_zero();
}

// @brief 論理式に対応するノード(木)をつくる．
FraigHandle
FraigEnc::expr2aig(
  const Expr& expr
)
{
  if ( expr.is_zero() ) {
    return mMgr.make_zero();
  }
  if ( expr.is_one() ) {
    return mMgr.make_one();
  }
  if ( expr.is_posi_literal() ) {
    auto var = expr.varid();
    ASSERT_COND( var < mInputList.size() );
    return mInputList[var];
  }
  if ( expr.is_nega_literal() ) {
    auto var = expr.varid();
    ASSERT_COND( var < mInputList.size() );
    return ~mInputList[var];
  }

  vector<FraigHandle> input_list;
  input_list.reserve(expr.operand_num());
  for ( auto& opr: expr.operand_list() ) {
    auto ih = expr2aig(opr);
    input_list.push_back(ih);
  }
  if ( expr.is_and() ) {
    return mMgr.make_and(input_list);
  }
  if ( expr.is_or() ) {
    return mMgr.make_or(input_list);
  }
  if ( expr.is_xor() ) {
    return mMgr.make_xor(input_list);
  }
  ASSERT_NOT_REACHED;
  // ダミー
  return mMgr.make_zero();
}

// @brief TvFunc を AIG に変換する．
FraigHandle
FraigEnc::tv2aig(
  const TvFunc& func,
  SizeType var
)
{
  if ( func.is_zero() ) {
    return mMgr.make_zero();
  }
  if ( func.is_one() ) {
    return mMgr.make_one();
  }

  if ( mTvDict.count(func) > 0 ) {
    return mTvDict.at(func);
  }

  for ( ; var < mInputList.size(); ++ var ) {
    auto f0 = func.cofactor(var, true);
    auto f1 = func.cofactor(var, false);
    if ( f0 != f1 ) {
      auto c = mInputList[var];
      auto r0 = tv2aig(f0, var + 1);
      auto r1 = tv2aig(f1, var + 1);
      auto r = mMgr.make_mux(c, r0, r1);
      mTvDict.emplace(func, r);
      return r;
    }
  }
  ASSERT_NOT_REACHED;
  // ダミー
  return mMgr.make_zero();
}

// @brief BDD を AIG に変換する．
FraigHandle
FraigEnc::bdd2aig(
  const Bdd& bdd
)
{
  if ( bdd.is_zero() ) {
    return mMgr.make_zero();
  }
  if ( bdd.is_one() ) {
    return mMgr.make_one();
  }
  if ( bdd.is_posilit() ) {
    auto var = bdd.root_var();
    auto varid = var.id();
    return mInputList[varid];
  }
  if ( bdd.is_negalit() ) {
    auto var = bdd.root_var();
    auto varid = var.id();
    return ~mInputList[varid];
  }
  if ( mBddDict.count(bdd) > 0 ) {
    return mBddDict.at(bdd);
  }
  Bdd bdd0;
  Bdd bdd1;
  auto var = bdd.root_decomp(bdd0, bdd1);
  auto varid = var.id();
  auto sel = mInputList[varid];
  auto h0 = bdd2aig(bdd0);
  auto h1 = bdd2aig(bdd1);
  auto r = mMgr.make_mux(sel, h0, h1);
  mBddDict.emplace(bdd, r);
  return r;
}

END_NAMESPACE_EQUIV
