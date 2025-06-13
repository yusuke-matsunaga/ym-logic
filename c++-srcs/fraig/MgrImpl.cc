
/// @file MgrImpl.cc
/// @brief MgrImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "MgrImpl.h"
#include "FraigNode.h"
#include "ym/Range.h"
#include "ym/Timer.h"


#if defined(YM_DEBUG)
#define DEBUG_FLAG 1
#endif

#if !defined(DEBUG_FLAG)
#define DEBUG_FLAG 0
#endif


BEGIN_NAMESPACE_YM_FRAIG

BEGIN_NONAMESPACE

const int debug = DEBUG_FLAG;

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// MgrImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MgrImpl::MgrImpl(
  SizeType sig_size,
  const SatInitParam& init_param
) : mSolver{init_param},
    mLogLevel{0},
    mLogStream{new ofstream("/dev/null")}
{
  FraigNode::mPatSize = sig_size * 2;
  FraigNode::mPatUsed = sig_size;
}

// @brief デストラクタ
MgrImpl::~MgrImpl()
{
  if ( mLogStream != &cout ) {
    delete mLogStream;
  }
}

// @brief 外部入力を作る．
FraigHandle
MgrImpl::make_input()
{
  if ( debug ) {
    cout << "make_input ...";
  }

  auto id = mAllNodes.size();
  auto iid = mInputNodes.size();
  // 乱数で初期化したパタンを作る．
  auto init_pat = std::vector<std::uint64_t>(FraigNode::mPatUsed);
  auto rd = std::uniform_int_distribution<std::uint64_t>{};
  for ( auto i: Range(FraigNode::mPatUsed) ) {
    init_pat[i] = rd(mRandGen);
  }
  auto node_ptr = std::unique_ptr<FraigNode>{new FraigNode(id, iid, init_pat)};
  auto node = node_ptr.get();
  reg_node(std::move(node_ptr));

  mInputNodes.push_back(node);

  auto ans = FraigHandle(node, false);

  if ( debug ) {
    cout << " -> " << ans << endl;
  }

  return ans;
}

// @brief 2つのノードの AND を取る．
FraigHandle
MgrImpl::make_and(
  FraigHandle handle1,
  FraigHandle handle2
)
{
  if ( debug ) {
    cout << "make_and(" << handle1 << ", " << handle2 << ") ..." << endl;
  }

  FraigHandle ans;

  // trivial な場合の処理
  if ( handle1.is_zero() || handle2.is_zero() ) {
    ans = make_zero();
  }
  else if ( handle1.is_one() ) {
    ans = handle2;
  }
  else if ( handle2.is_one() ) {
    ans = handle1;
  }
  else if ( handle1 == handle2 ) {
    ans = handle1;
  }
  else if ( handle1.node_id() == handle2.node_id() ) {
    // handle1.inv != handle2.inv() のはず
    ans = make_zero();
  }
  else {
    // 順番の正規化
    if ( handle1.node_id() < handle2.node_id() ) {
      std::swap(handle1, handle2);
    }

    if ( debug ) {
      cout << "  after normalize: " << handle1 << ", " << handle2 << endl;
    }

    // 同じ構造を持つノードが既にないか調べる．
    auto key = FraigNode(0, handle1, handle2);
    auto p = mStructTable.find(&key);
    if ( p != mStructTable.end() ) {
      // 等価なノードが存在した．
      auto node = *p;
      ans = FraigHandle(node->id(), false);
    }
    else {
      // ノードを作る．
      SizeType id = mAllNodes.size();
      auto node_ptr = std::unique_ptr<FraigNode>{new FraigNode(id, handle1, handle2)};
      auto node = node_ptr.get();
      reg_node(std::move(node_ptr));

      // 構造ハッシュに追加する．
      mStructTable.insert(node);

      // 入出力の関係を表す CNF を作る．
      mSolver.make_cnf(node);

      if ( debug ) {
	cout << "  new node: " << FraigHandle(node, false) << endl;
      }

      // 縮退検査を行う．
      if ( verify_const(node, ans) == SatBool3::True ) {
	// 縮退していた．
	goto exit;
      }

      // 等価なノードを探す．
      PatEq eq;
      while ( true ) {
	auto range = mPatTable.equal_range(node);
	auto inv0 = node->pat_hash_inv();
	auto change = false;
	auto end = range.second;
	for ( auto p = range.first; p != end; ++ p ) {
	  auto node1 = *p;
	  auto inv = inv0 != node1->pat_hash_inv();
	  if ( eq(node, node1) ) {
	    // node と node1 が等価かどうか調べる．
	    auto stat = mSolver.check_equiv(node, node1, inv);
	    if ( stat == SatBool3::True ) {
	      // 等価なノードが見つかった．
	      ans = FraigHandle{node1, inv};
	      goto exit;
	    }
	    else if ( stat == SatBool3::False ) {
	      // 反例をパタンに加えて再ハッシュする．
	      add_pat(node);

	      ASSERT_COND( !eq(node, node1) );
	      change = true;
	      break;
	    }
	  }
	}
	if ( !change ) {
	  ans = FraigHandle{node, false};
	  break;
	}
      }
    }
  }

 exit:
  if ( debug ) {
    cout << "  -> " << ans << endl;
  }

  return ans;
}

// @brief make_and() の下請け関数
FraigHandle
MgrImpl::make_and_sub(
  const vector<FraigHandle>& edge_list,
  SizeType start_pos,
  SizeType end_pos,
  bool iinv
)
{
  ASSERT_COND( start_pos < end_pos );

  auto n = end_pos - start_pos;
  if ( n == 1 ) {
    auto h = edge_list[start_pos];
    if ( iinv ) {
      return ~h;
    }
    else {
      return h;
    }
  }
  // n >= 2
  auto mid_pos = start_pos + (n + 1) / 2;
  auto h0 = make_and_sub(edge_list, start_pos, mid_pos, iinv);
  auto h1 = make_and_sub(edge_list, mid_pos, end_pos, iinv);
  return make_and(h0, h1);
}

// @brief ２つのノードの XOR を作る．
FraigHandle
MgrImpl::make_xor(
  FraigHandle edge1,
  FraigHandle edge2
)
{
  auto e01 = make_and(~edge1, edge2);
  auto e10 = make_and(edge1, ~edge2);
  return make_or(e01, e10);
}

// @brief make_xor() の下請け関数
FraigHandle
MgrImpl::make_xor_sub(
  const vector<FraigHandle>& edge_list,
  SizeType start_pos,
  SizeType end_pos
)
{
  ASSERT_COND( start_pos < end_pos );

  auto n = end_pos - start_pos;
  if ( n == 1 ) {
    auto h = edge_list[start_pos];
    return h;
  }
  // n >= 2
  auto mid_pos = start_pos + (n + 1) / 2;
  auto h0 = make_xor_sub(edge_list, start_pos, mid_pos);
  auto h1 = make_xor_sub(edge_list, mid_pos, end_pos);
  return make_xor(h0, h1);
}

// @brief MUX を作る
FraigHandle
MgrImpl::make_mux(
  FraigHandle sel_edge,
  FraigHandle edge0,
  FraigHandle edge1
)
{
  if ( edge0.is_zero() ) {
    if ( edge1.is_zero() ) {
      return edge0;
    }
    if ( edge1.is_one() ) {
      return sel_edge;
    }
    return make_and(sel_edge, edge1);
  }
  if ( edge0.is_one() ) {
    if ( edge1.is_zero() ) {
      return ~sel_edge;
    }
    if ( edge1.is_one() ) {
      return edge0;
    }
    return make_or(~sel_edge, edge1);
  }
  if ( edge1.is_zero() ) {
    return make_and(~sel_edge, edge0);
  }
  if ( edge1.is_one() ) {
    return make_or(sel_edge, edge0);
  }
  auto tmp0 = make_and(~sel_edge, edge0);
  auto tmp1 = make_and(sel_edge, edge1);
  auto ans = make_or(tmp0, tmp1);
  return ans;
}

// @brief コファクターを計算する．
FraigHandle
MgrImpl::make_cofactor(
  FraigHandle edge,
  SizeType input_id,
  bool inv
)
{
  if ( edge.is_const() ) {
    // edge が定数の時は変更なし
    return edge;
  }

  auto node = edge.node();
  FraigHandle ans;
  if ( node->is_input() ) {
    // 入力ノード時は番号が input_id どうかで処理が変わる．
    if ( node->input_id() == input_id ) {
      if ( inv ) {
	ans = make_zero();
      }
      else {
	ans = make_one();
      }
    }
    else {
      ans = FraigHandle{node, false};
    }
  }
  else {
    // AND ノードの場合
    // 2つの子供に再帰的な処理を行って結果の AND を計算する．
    auto tmp0 = make_cofactor(node->fanin0_handle(), input_id, inv);
    auto tmp1 = make_cofactor(node->fanin1_handle(), input_id, inv);
    ans = make_and(tmp0, tmp1);
  }
  return ans ^ edge.inv();
}

// @brief 2つのハンドルが等価かどうか調べる．
SatBool3
MgrImpl::check_equiv(
  FraigHandle aig1,
  FraigHandle aig2
)
{
  if ( aig1 == aig2 ) {
    // もっとも簡単なパタン
    return SatBool3::True;
  }

  auto node1 = aig1.node();
  auto node2 = aig2.node();

  if ( node1 == node2 ) {
    // ということは逆極性なので絶対に等価ではない．
    return SatBool3::False;
  }

  auto inv1 = aig1.inv();
  auto inv2 = aig2.inv();

  if ( aig1.is_zero () ) {
    // 上のチェックで aig2 は定数でないことは明らか
    auto stat = mSolver.check_const(node2, inv2);
    return stat;
  }

  if ( aig1.is_one() ) {
    // 上のチェックで aig2 は定数でないことは明らか
    auto stat = mSolver.check_const(node2, !inv2);
    return stat;
  }

  if ( aig2.is_zero() ) {
    // 上のチェックで aig1 は定数でないことは明らか
    auto stat = mSolver.check_const(node1, inv1);
    return stat;
  }

  if ( aig2.is_one() ) {
    // 上のチェックで aig1 は定数でないことは明らか
    auto stat = mSolver.check_const(node1, !inv1);
    return stat;
  }

  auto inv = inv1 ^ inv2;
  auto stat = mSolver.check_equiv(node1, node2, inv);
  return stat;
}

// @brief 0縮退検査を行う．
SatBool3
MgrImpl::verify_const(
  FraigNode* node,
  FraigHandle& ans
)
{
  auto stat = SatBool3::False;
  if ( !node->check_1mark() ) {
    // 定数0の可能性があるか調べる．
    stat = mSolver.check_const(node, false);
    if ( stat == SatBool3::True ) {
      // 定数0と等価だった．
      ans = make_zero();
    }
    else if ( stat == SatBool3::False ) {
      // 反例をパタンに加えておく．
      add_pat(node);

      ASSERT_COND( node->check_1mark() );
    }
  }
  if ( !node->check_0mark() ) {
    // 定数1の可能性があるか調べる．
    stat = mSolver.check_const(node, true);
    if ( stat == SatBool3::True ) {
      // 定数1と等価だった．
      ans = make_one();
    }
    else if ( stat == SatBool3::False ) {
      // 反例をパタンに加えておく．
      add_pat(node);

      ASSERT_COND( node->check_0mark() );
    }
  }

  return stat;
}

// @breif 直前の SAT の反例を加えて再ハッシュする．
void
MgrImpl::add_pat(
  FraigNode* node
)
{
  if ( FraigNode::mPatSize <= FraigNode::mPatUsed ) {
    resize_pat(FraigNode::mPatSize * 2);
  }
  mPatTable.clear();

  // 反例をパタンに加える．
  auto rd100 = std::uniform_int_distribution<int>(0, 99);
  for ( auto& node1_ptr: mAllNodes ) {
    auto node1 = node1_ptr.get();
    if ( node1->is_input() ) {
      auto pat = std::uint64_t{0UL};
      if ( mSolver.model_val(node1) == SatBool3::True ) {
	pat = ~0UL;
      }
      else {
	pat = 0UL;
      }
      // ただし一度に64個のパタンを加えるので63個は
      // 適当にばらつかせる．
      for ( int b = 1; b < 64; ++ b ) {
	if ( rd100(mRandGen) <= 3 ) {
	  pat ^= (1UL << b);
	}
      }
      node1->add_pat(pat);
    }
    else {
      node1->calc_pat(FraigNode::mPatUsed, FraigNode::mPatUsed + 1);
    }

    if ( node1 != node ) {
      mPatTable.insert(node1);
    }
  }
  ++ FraigNode::mPatUsed;
}

// @brief ノードを登録する．
void
MgrImpl::reg_node(
  std::unique_ptr<FraigNode>&& node
)
{
  mSolver.reg_node(node.get());
  mAllNodes.push_back(std::move(node));
}

// @brief ログレベルを設定する．
void
MgrImpl::set_loglevel(
  SizeType level
)
{
  mLogLevel = level;
}

// @brief ログ出力用ストリームを設定する．
void
MgrImpl::set_logstream(
  ostream* out
)
{
  if ( mLogStream != &cout ) {
    delete mLogStream;
  }
  mLogStream = out;
}

// @brief 全ノードのシミュレーションパタン用配列を拡大する．
void
MgrImpl::resize_pat(
  SizeType size
)
{
  for ( auto& node: mAllNodes ) {
    node->resize_pat(size);
  }
  FraigNode::mPatSize = size;
}

// @brief 内部の統計情報を出力する．
void
MgrImpl::dump_stats(
  ostream& s
)
{
  mSolver.dump_stats(s);
}

END_NAMESPACE_YM_FRAIG
