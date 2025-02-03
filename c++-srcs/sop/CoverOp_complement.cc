
/// @file CoverOp_complement.cc
/// @brief complement() 関係の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CoverOp.h"
#include "SopBlock.h"
#include "ym/Literal.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
// クラス CoverOp
//////////////////////////////////////////////////////////////////////

// @brief 否定のカバーを求める．
SopBlock
CoverOp::_compl_recur(
  const SopBlockRef& src
)
{
  auto src_bv = src.body;
  auto src_nc = src.cube_num;

  if ( src_nc == 0 ) {
    // 結果は tautology
    return new_cover(1);
  }
  if ( src_nc == 1 ) {
    // キューブに対する否定
    return cube_complement(_cube_begin(src_bv));
  }

  // 空のキューブ(universal cube)があるかチェックする．
  auto cube = _cube_begin(src_bv);
  auto end = _cube_end(cube, src_nc);
  for ( ; cube != end; cube += _cube_size() ) {
    if ( cube_check_null(cube) ) {
      // 結果は空
      return SopBlock{0, 0, nullptr};
    }
  }

  auto cc_bv = common_cube(src);
  SopBlock r{0, 0, nullptr};
  auto cc = _cube_begin(cc_bv);
  if ( !cube_check_null(cc) ) {
    r = cube_complement(cc);
    quotient_int(src, cc);
  }

  SizeType j = binate_select(src);
  if ( j == variable_num() ) {
    // unate cover だった．
    return _unate_compl_recur(src);
  }

  auto f0 = cofactor(src, Literal{j, true});
  auto f0_compl = _compl_recur(f0);
  delete_cover(f0);
  auto f1 = cofactor(src, Literal{j, false});
  auto f1_compl = _compl_recur(f1);
  delete_cover(f1);

  auto ans = merge(j, f0_compl, f1_compl, r);
  delete_cover(f0_compl);
  delete_cover(f1_compl);
  delete_cover(r);
  return ans;
}

// @brief unate cover の否定を求める．
SopBlock
CoverOp::_unate_compl_recur(
  SopBlock& src
)
{
  auto bv1 = src.body;
  auto nc1 = src.cube_num;

  if ( nc1 == 0 ) {
    // 結果は tautology
    return new_cover(1);
  }
  if ( nc1 == 1 ) {
    // キューブに対する否定
    return cube_complement(src.body);
  }
  for ( SizeType c = 0; c < nc1; ++ c ) {
    if ( cube_check_null(bv1, c) ) {
      // 結果は空
      return SopBlock{0, 0, nullptr};
    }
  }

  // splitting variable を選ぶ．
  SizeType j = ucomp_select(src);

}

// @brief MOST_BINATE variable を選ぶ．
SizeType
CoverOp::binate_select(
  const SopBlock& src
)
{
  // 各変数について肯定と否定のリテラル数を数える．
  vector<SizeType> p_count(variable_num(), 0);
  vector<SizeType> n_count(variable_num(), 0);
  SizeType nc = src.cube_num;
  auto src_top = src.body;
  for ( SizeType v = 0; v < variable_num(); ++ v ) {
    // vanilla version
    for ( SizeType i = 0; i < nc; ++ i ) {
      auto pat = get_pat(src_top, i, v);
      if ( pat == SopPat::_0 ) {
	++ n_count[v];
      }
      else if ( pat == SopPat::_1 ) {
	++ p_count[v];
      }
    }
  }
  // max(min(p_count[v], n_count[v])) となる v を求める．
  int max_c = 0;
  SizeType max_v = variable_num();
  for ( SizeType v = 0; v < variable_num(); ++ v ) {
    int c = std::min(p_count[v], n_count[v]);
    if ( max_c < c ) {
      max_c = c;
      max_v = v;
    }
  }
  // もしも src が unate cover だった場合は
  // max_v は variable_num() のままのはず．
  return max_v;
}

// @brief unate cover 用の変数選択
SizeType
CoverOp::ucomp_select(
  const SopBlock& src
)
{
}

// @brief Shanon 展開のマージを行う．
SopBlock
CoverOp::merge(
  SizeType var,
  const SopBlock& f0,
  const SopBlock& f1,
  const SopBlock& r
)
{
  // f0 と f1 の共通部分の求める．
  SizeType nc0 = f0.cube_num;
  SizeType nc1 = f1.cube_num;
  vector<SizeType> f0_list; f0_list.reserve(nc0);
  vector<SizeType> f1_list; f1_list.reserve(nc1);
  vector<SizeType> f2_list; f2_list.reserve(nc0);
  SizeType i0 = 0;
  SizeType i1 = 0;
  auto bv0_top = f0.body;
  auto bv1_top = f1.body;
  auto bv0 = _calc_offset(bv0_top, i0);
  auto bv1 = _calc_offset(bv1_top, i1);
  while ( i0 < nc0 && i1 < nc1 ) {
    auto r = cube_compare(bv0, bv1);
    if ( r < 0 ) {
      f0_list.push_back(i0);
      ++ i0;
      bv0 = _calc_offset(bv0_top, i0);
    }
    else if ( r > 0 ) {
      f1_list.push_back(i1);
      ++ i1;
      bv1 = _calc_offset(bv1_top, i1);
    }
    else {
      f2_list.push_back(i0);
      ++ i0;
      bv0 = _calc_offset(bv0_top, i0);
      ++ i1;
      bv1 = _calc_offset(bv1_top, i1);
    }
  }

  // f0 & var' + f1 & var + f0 + r が答となる．

  // トータルのキューブ数を数える．
  SizeType nc = f0_list.size() + f1_list.size() + f2_list.size() + r.cube_num;
  auto dst = new_cover(nc);
  auto dst_top = dst.body;
  nc = 0;
  Literal l0{var, true};
  for ( auto pos: f0_list ) {
    auto bv = _calc_offset(bv0_top, pos);
    cube_product_int(bv, l0);
    auto dst1 = _calc_offset(dst_top, nc);
    cube_copy(dst1, bv);
    ++ nc;
  }
  Literal l1{var, false};
  for ( auto pos: f1_list ) {
    auto bv = _calc_offset(bv1_top, pos);
    cube_product_int(bv, l1);
    auto dst1 = _calc_offset(dst_top, nc);
    cube_copy(dst1, bv);
    ++ nc;
  }
  for ( auto pos: f2_list ) {
    auto bv = _calc_offset(bv0_top, pos);
    auto dst1 = _calc_offset(dst_top, nc);
    cube_copy(dst1, bv);
    ++ nc;
  }
  auto r_top = r.body;
  auto r_end = _calc_offset(r_top, r.cube_num);
  for ( ; r_top != r_end; r_top += _cube_size() ) {
    auto dst1 = _calc_offset(dst_top, nc);
    cube_copy(dst1, r_top);
    ++ nc;
  }
  _sort(dst_top, 0, nc);
  dst.cube_num = nc;
  return dst;
}

END_NAMESPACE_YM_SOP
