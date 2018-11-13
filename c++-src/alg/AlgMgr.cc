
/// @file AlgMgr.cc
/// @brief AlgMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "AlgMgr.h"
#include "AlgBlock.h"
#include "ym/AlgCube.h"
#include "ym/Literal.h"


BEGIN_NAMESPACE_YM_LOGIC

BEGIN_NONAMESPACE

inline
AlgBitVect
lit2bv(Literal lit)
{
  return static_cast<AlgBitVect>(lit.is_positive() ? AlgPol::P : AlgPol::N);
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス AlgMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] variable_num 変数の数
AlgMgr::AlgMgr(int variable_num) :
  mVarNum(variable_num)
{
  _init_buff();
}

// @brief デストラクタ
AlgMgr::~AlgMgr()
{
  delete_body(mTmpBuff, mTmpBuffSize);
}

BEGIN_NONAMESPACE

// 表引きを使ってリテラル数の計算を行う．
int
_count(AlgBitVect pat)
{
  int table[] = {
    // utils/gen_bitcount_tbl.py で生成
    0,  1,  1,  0,  1,  2,  2,  0,  1,  2,  2,  0,  0,  0,  0,  0,
    1,  2,  2,  0,  2,  3,  3,  0,  2,  3,  3,  0,  0,  0,  0,  0,
    1,  2,  2,  0,  2,  3,  3,  0,  2,  3,  3,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    1,  2,  2,  0,  2,  3,  3,  0,  2,  3,  3,  0,  0,  0,  0,  0,
    2,  3,  3,  0,  3,  4,  4,  0,  3,  4,  4,  0,  0,  0,  0,  0,
    2,  3,  3,  0,  3,  4,  4,  0,  3,  4,  4,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    1,  2,  2,  0,  2,  3,  3,  0,  2,  3,  3,  0,  0,  0,  0,  0,
    2,  3,  3,  0,  3,  4,  4,  0,  3,  4,  4,  0,  0,  0,  0,  0,
    2,  3,  3,  0,  3,  4,  4,  0,  3,  4,  4,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

  };

  return table[pat];
}

END_NONAMESPACE

// @brief ビットベクタ上のリテラル数を数える．
// @param[in] src 対象のブロック
int
AlgMgr::literal_num(const AlgBlock& src)
{
  // 8 ビットごとに区切って表引きで計算する．
  const AlgBitVect* bv = src.bitvect();
  int n = src.cube_num() * _cube_size();
  const AlgBitVect* bv_end = bv + n;
  int ans = 0;
  for ( ; bv != bv_end; ++ bv ) {
    AlgBitVect pat = *bv;
    AlgBitVect p1 = pat & 0xFFUL;
    ans += _count(p1);
    AlgBitVect p2 = (pat >> 8) & 0xFFUL;
    ans += _count(p2);
    AlgBitVect p3 = (pat >> 16) & 0xFFUL;
    ans += _count(p3);
    AlgBitVect p4 = (pat >> 24) & 0xFFUL;
    ans += _count(p4);
    AlgBitVect p5 = (pat >> 32) & 0xFFUL;
    ans += _count(p5);
    AlgBitVect p6 = (pat >> 40) & 0xFFUL;
    ans += _count(p6);
    AlgBitVect p7 = (pat >> 48) & 0xFFUL;
    ans += _count(p7);
    AlgBitVect p8 = (pat >> 56);
    ans += _count(p8);
  }
  return ans;
}

// @brief ビットベクタ上の特定のリテラルの出現頻度を数える．
// @param[in] src 対象のブロック
// @param[in] lit 対象のリテラル
int
AlgMgr::literal_num(const AlgBlock& src,
		    Literal lit)
{
  VarId var_id = lit.varid();
  int blk = _block_pos(var_id);
  int sft = _shift_num(var_id);
  AlgBitVect pat = lit2bv(lit);
  AlgBitVect mask = pat << sft;
  const AlgBitVect* bv = src.bitvect();
  int n = src.cube_num() * _cube_size();
  const AlgBitVect* bv_end = bv + n;
  int ans = 0;
  for ( ; bv != bv_end; bv += _cube_size() ) {
    if ( (bv[blk] & mask) == mask ) {
      ++ ans;
    }
  }
  return ans;
}

// @brief キューブ/カバー用の領域を確保する．
// @param[in] cube_num キューブ数
//
// キューブの時は cube_num = 1 とする．
AlgBitVect*
AlgMgr::new_body(int cube_num)
{
  int size = _cube_size() * cube_num;
  AlgBitVect* body = new AlgBitVect[size];
  for ( int i = 0; i < size; ++ i ) {
    body[i] = 0ULL;
  }
  return body;
}

// @brief キューブ/カバー用の領域を削除する．
// @param[in] p 領域を指すポインタ
// @param[in] cube_num キューブ数
//
// キューブの時は cube_num = 1 とする．
void
AlgMgr::delete_body(AlgBitVect* p,
		    int cube_num)
{
  delete [] p;
}

// @brief 2つのカバーの論理和を計算する．
// @param[in] dst_bv 結果を格納するビットベクタ
// @param[in] src1 1つめのブロック
// @param[in] src2 2つめのブロック
// @return 結果のキューブ数を返す．
//
// * dst_bv には十分な容量があると仮定する．
// * dst_bv != src1.bitvect() を仮定する．
// * dst_bv != src2.bitvect() を仮定する．
int
AlgMgr::sum(AlgBitVect* dst_bv,
	    const AlgBlock& src1,
	    const AlgBlock& src2)
{
  const AlgBitVect* bv1 = src1.bitvect();
  ASSERT_COND ( dst_bv != bv1 );
  const AlgBitVect* bv1_end = bv1 + src1.cube_num() * _cube_size();

  const AlgBitVect* bv2 = src2.bitvect();
  ASSERT_COND ( dst_bv != bv2 );
  const AlgBitVect* bv2_end = bv2 + src2.cube_num() * _cube_size();

  int nc = 0;
  while ( bv1 != bv1_end && bv2 != bv2_end ) {
    int res = cube_compare(bv1, bv2);
    if ( res > 0 ) {
      cube_copy(dst_bv, bv1);
      bv1 += _cube_size();
    }
    else if ( res < 0 ) {
      cube_copy(dst_bv, bv2);
      bv2 += _cube_size();
    }
    else {
      cube_copy(dst_bv, bv1);
      bv1 += _cube_size();
      bv2 += _cube_size();
    }
    dst_bv += _cube_size();
    ++ nc;
  }
  while ( bv1 != bv1_end ) {
    cube_copy(dst_bv, bv1);
    bv1 += _cube_size();
    dst_bv += _cube_size();
    ++ nc;
  }
  while ( bv2 != bv2_end ) {
    cube_copy(dst_bv, bv2);
    bv2 += _cube_size();
    dst_bv += _cube_size();
    ++ nc;
  }

  return nc;
}

// @brief 2つのカバーの差分を計算する．
// @param[in] dst_bv 結果を格納するビットベクタ
// @param[in] src1 1つめのブロック
// @param[in] src2 2つめのブロック
// @return 結果のキューブ数を返す．
//
// * dst_bv には十分な容量があると仮定する．
// * dst_bv != src2.bitvect() を仮定する．
// * dst_bv == src1.bitvect() でも正しく動く
int
AlgMgr::diff(AlgBitVect* dst_bv,
	     const AlgBlock& src1,
	     const AlgBlock& src2)
{
  const AlgBitVect* bv1 = src1.bitvect();
  const AlgBitVect* bv1_end = bv1 + src1.cube_num() * _cube_size();
  const AlgBitVect* bv2 = src2.bitvect();
  ASSERT_COND ( dst_bv != bv2 );
  const AlgBitVect* bv2_end = bv2 + src2.cube_num() * _cube_size();

  int nc = 0;
  while ( bv1 != bv1_end && bv2 != bv2_end ) {
    int res = cube_compare(bv1, bv2);
    if ( res > 0 ) {
      cube_copy(dst_bv, bv1);
      bv1 += _cube_size();
      dst_bv += _cube_size();
      ++ nc;
    }
    else if ( res < 0 ) {
      bv2 += _cube_size();
    }
    else {
      bv1 += _cube_size();
      bv2 += _cube_size();
    }
  }
  while ( bv1 != bv1_end ) {
    cube_copy(dst_bv, bv1);
    bv1 += _cube_size();
    dst_bv += _cube_size();
    ++ nc;
  }
  return nc;
}

// @brief 2つのカバーの論理積を計算する．
// @param[in] dst_bv 結果を格納するビットベクタ
// @param[in] src1 1つめのブロック
// @param[in] src2 2つめのブロック
// @return 結果のキューブ数を返す．
//
// * dst_bv には十分な容量があると仮定する．
// * dst_bv != src2.bitvect() を仮定する．
// * src2.cubenum() > 1 の時は dst_bv != src1.bitvect()
//   を仮定する．
// * src2.cubenum() == 1 の時は dst_bv == src1.bitvect()
//   でも正しく動く．
int
AlgMgr::product(AlgBitVect* dst_bv,
		const AlgBlock& src1,
		const AlgBlock& src2)
{
  const AlgBitVect* bv1 = src1.bitvect();
  ASSERT_COND( dst_bv != bv1 || src2.cube_num() == 1 );
  const AlgBitVect* bv2_start = src2.bitvect();
  ASSERT_COND( dst_bv != bv2_start );

  // 単純には答の積項数は2つの積項数の積だが
  // 相反するリテラルを含む積は数えない．
  int nc = 0;
  const AlgBitVect* bv1_end = bv1 + src1.cube_num() * _cube_size();
  const AlgBitVect* bv2_end = bv2_start + src2.cube_num() * _cube_size();
  for ( ; bv1 != bv1_end; bv1 += _cube_size() ) {
    for ( const AlgBitVect* bv2 = bv2_start; bv2 != bv2_end; bv2 += _cube_size() ) {
      if ( cube_product(dst_bv, bv1, bv2) ) {
	dst_bv += _cube_size();
	++ nc;
      }
    }
  }
  return nc;
}

// @brief カバーとリテラルとの論理積を計算する．
// @param[in] dst_bv 結果を格納するビットベクタ
// @param[in] src1 1つめのブロック
// @param[in] lit 対象のリテラル
// @return 結果のキューブ数を返す．
//
// * dst_bv には十分な容量があると仮定する．
// * dst_bv == src1.bitvect() でも正しく動く．
int
AlgMgr::product(AlgBitVect* dst_bv,
		const AlgBlock& src1,
		Literal lit)
{
  VarId var_id = lit.varid();
  int blk = _block_pos(var_id);
  int sft = _shift_num(var_id);
  AlgBitVect pat = lit2bv(lit);
  AlgBitVect pat1 = pat << sft;
  AlgBitVect mask = 3UL << sft;
  int nb = _cube_size();

  const AlgBitVect* bv1 = src1.bitvect();
  const AlgBitVect* bv1_end = bv1 + src1.cube_num() * nb;
  // 単純には答の積項数は2つの積項数の積だが
  // 相反するリテラルを含む積は数えない．
  int nc = 0;
  for ( ; bv1 != bv1_end; bv1 += nb ) {
    AlgBitVect tmp = bv1[blk] | pat1;
    if ( (tmp & mask) == mask ) {
      // 相反するリテラルがあった．
      continue;
    }
    cube_copy(dst_bv, bv1);
    dst_bv[blk] = tmp;
    dst_bv += nb;
    ++ nc;
  }

  return nc;
}

// @brief カバーの代数的除算を行う．
// @param[in] dst_bv 結果を格納するビットベクタ
// @param[in] src1 1つめのブロック
// @param[in] src2 2つめのブロック
// @return 結果のキューブ数を返す．
//
// * dst_bv には十分な容量があると仮定する．
// * dst_bv == src1.bitvect() でも正しく動く．
// * dst_bv == src2.bitvect() でも正しく動く．
int
AlgMgr::quotient(AlgBitVect* dst_bv,
		 const AlgBlock& src1,
		 const AlgBlock& src2)
{
  int nc1 = src1.cube_num();
  int nc2 = src2.cube_num();

  // 作業領域のビットベクタを確保する．
  _resize_buff(nc1);

  const AlgBitVect* bv1 = src1.bitvect();
  const AlgBitVect* bv1_end = bv1 + nc1 * _cube_size();
  const AlgBitVect* bv2 = src2.bitvect();
  const AlgBitVect* bv2_end = bv2 + nc2 * _cube_size();

  // bv1 の各キューブは高々1つのキューブでしか割ることはできない．
  // ただし，除数も被除数も algebraic expression の場合
  // ので bv1 の各キューブについて bv2 の各キューブで割ってみて
  // 成功した場合，その商を記録する．
  vector<bool> mark(nc1, false);
  AlgBitVect* tmp_dst = mTmpBuff;
  const AlgBitVect* bv1_tmp = bv1;
  for ( int i = 0; i < nc1; ++ i ) {
    for ( const AlgBitVect* bv2_tmp = bv2; bv2_tmp != bv2_end; bv2_tmp += _cube_size() ) {
      if ( cube_quotient(tmp_dst, bv1_tmp, bv2_tmp) ) {
	mark[i] = true;
	break;
      }
    }
    tmp_dst += _cube_size();
    bv1_tmp += _cube_size();
  }

  // 商のキューブは tmp 中に nc2 回現れているはず．
  vector<int> pos_list;
  pos_list.reserve(nc1);
  for ( int i = 0; i < nc1; ++ i ) {
    if ( !mark[i] ) {
      // 対象ではない．
      continue;
    }
    const AlgBitVect* tmp1 = mTmpBuff + i * _cube_size();

    int c = 1;
    vector<int> tmp_list;
    for ( int i2 = i + 1; i2 < nc1; ++ i2 ) {
      const AlgBitVect* tmp2 = mTmpBuff + i2 * _cube_size();
      if ( mark[i2] && cube_compare(tmp1, tmp2) == 0 ) {
	++ c;
	// i 番目のキューブと等しかったキューブ位置を記録する．
	tmp_list.push_back(i2);
      }
    }
    if ( c == nc2 ) {
      // 見つけた
      pos_list.push_back(i);
      // tmp_list に含まれるキューブはもう調べなくて良い．
      for ( int pos: tmp_list ) {
	mark[pos] = false;
      }
    }
  }

  for ( int pos: pos_list ) {
    const AlgBitVect* tmp = mTmpBuff + pos * _cube_size();
    cube_copy(dst_bv, tmp);
    dst_bv += _cube_size();
  }

  return pos_list.size();
}

// @brief カバーをリテラルで割る．
// @param[in] dst_bv 結果を格納するビットベクタ
// @param[in] src1 1つめのブロック
// @param[in] lit 対象のリテラル
// @return 結果のキューブ数を返す．
//
// * dst_bv には十分な容量があると仮定する．
// * dst_bv == src1.bitvect() でも正しく動く．
int
AlgMgr::quotient(AlgBitVect* dst_bv,
		 const AlgBlock& src1,
		 Literal lit)
{
  VarId var_id = lit.varid();
  int blk = _block_pos(var_id);
  int sft = _shift_num(var_id);
  AlgBitVect pat = lit2bv(lit);
  AlgBitVect pat1 = pat << sft;
  AlgBitVect mask = 3UL << sft;
  AlgBitVect nmask = ~mask;
  int nb = _cube_size();
  int nc = 0;
  const AlgBitVect* bv1 = src1.bitvect();
  const AlgBitVect* bv1_end = bv1 + (nb * src1.cube_num());
  for ( ; bv1 != bv1_end; bv1 += nb ) {
    if ( (bv1[blk] & mask) == pat1 ) {
      cube_copy(dst_bv, bv1);
      dst_bv[blk] &= nmask;
      dst_bv += nb;
      ++ nc;
    }
  }

  return nc;
}

// @brief カバー中のすべてのキューブの共通部分を求める．
// @param[in] dst_bv 結果を格納するビットベクタ
// @param[in] src1 1つめのブロック
//
// * 共通部分がないときは空のキューブとなる．
// * dst_bv には十分な容量があると仮定する．
void
AlgMgr::common_cube(AlgBitVect* dst_bv,
		    const AlgBlock& src1)
{
  int nb = _cube_size();

  const AlgBitVect* bv1 = src1.bitvect();
  const AlgBitVect* bv1_end = bv1 + src1.cube_num() * _cube_size();

  // 最初のキューブをコピーする．
  cube_copy(dst_bv, bv1);

  // 2番目以降のキューブとの共通部分を求める．
  bv1 += _cube_size();
  for ( ; bv1 != bv1_end; bv1 += _cube_size() ) {
    AlgBitVect tmp = 0ULL;
    for ( int i = 0; i < nb; ++ i ) {
      dst_bv[i] &= bv1[i];
      tmp |= dst_bv[i];
    }
    if ( tmp == 0ULL ) {
      // 空になった．
      break;
    }
  }
}

// @brief Literal のリストからキューブ(を表すビットベクタ)のコピーを行う．
// @param[in] dst_bv コピー先のビットベクタ
// @param[in] lit_list キューブを表すリテラルのリスト
//
// * dst_bv には十分な容量があると仮定する．
void
AlgMgr::cube_set(AlgBitVect* dst_bv,
		 const vector<Literal>& lit_list)
{
  for ( auto lit: lit_list ) {
    VarId var_id = lit.varid();
    int blk = _block_pos(var_id);
    int sft = _shift_num(var_id);
    AlgBitVect pat = lit2bv(lit);
    AlgBitVect mask = pat << sft;
    dst_bv[blk] |= mask;
  }
}

// @brief Literal のリストからキューブ(を表すビットベクタ)のコピーを行う．
// @param[in] dst_bv コピー先のビットベクタ
// @param[in] lit_list キューブを表すリテラルのリスト初期化子
//
// * dst_bv には十分な容量があると仮定する．
void
AlgMgr::cube_set(AlgBitVect* dst_bv,
		 std::initializer_list<Literal> lit_list)
{
  for ( auto lit: lit_list ) {
    VarId var_id = lit.varid();
    int blk = _block_pos(var_id);
    int sft = _shift_num(var_id);
    AlgBitVect pat = lit2bv(lit);
    AlgBitVect mask = pat << sft;
    dst_bv[blk] |= mask;
  }
}

// @brief マージソートを行う下請け関数
// @param[in] bv 対象のビットベクタ
// @param[in] start 開始位置
// @param[in] end 終了位置
//
// bv[start] - bv[end - 1] の領域をソートする．
void
AlgMgr::_sort(AlgBitVect* bv,
	      int start,
	      int end)
{
  int n = end - start;
  if ( n <= 1 ) {
    return;
  }
  if ( n == 2 ) {
    // (0, 1) と (1, 0) の2通りだけ
    AlgBitVect* bv0 = bv + start * _cube_size();
    AlgBitVect* bv1 = bv0 + _cube_size();
    if ( cube_compare(bv0, bv1) < 0 ) {
      // (1, 0) だったので交換する．
      _resize_buff(1);
      cube_swap(bv0, bv1);
    }
    return;
  }
  if ( n == 3 ) {
    // (0, 1, 2), (0, 2, 1), (1, 0, 2), (1, 2, 0), (2, 0, 1), (2, 1, 0)
    // の6通りなので虱潰し
    AlgBitVect* bv0 = bv + start * _cube_size();
    AlgBitVect* bv1 = bv0 + _cube_size();
    AlgBitVect* bv2 = bv1 + _cube_size();
    if ( cube_compare(bv0, bv1) < 0 ) {
      // (1, 0, 2), (1, 2, 0), (2, 1, 0)
      if ( cube_compare(bv0, bv2) < 0 ) {
	// (1, 2, 0), (2, 1, 0)
	if ( cube_compare(bv1, bv2) < 0 ) {
	  // (2, 1, 0)
	  // 0 と 2 を交換
	  _resize_buff(1);
	  cube_swap(bv0, bv2);
	}
	else {
	  // (1, 2, 0)
	  // 0 <- 1, 1 <- 2, 2 <- 0
	  _resize_buff(1);
	  cube_rotate3(bv0, bv1, bv2);
	}
      }
      else {
	// (1, 0, 2)
	// 0 <-> 1
	_resize_buff(1);
	cube_swap(bv0, bv1);
      }
    }
    else {
      // (0, 1, 2), (0, 2, 1), (2, 0, 1)
      if ( cube_compare(bv0, bv2) < 0 ) {
	// (2, 0, 1)
	// 0 <- 2, 2 <- 1, 1 <- 0
	_resize_buff(1);
	cube_rotate3(bv0, bv2, bv1);
      }
      else {
	// (0, 1, 2), (0, 2, 1)
	if ( cube_compare(bv1, bv2) < 0 ) {
	  // (0, 2, 1)
	  // 1 <-> 2
	  _resize_buff(1);
	  cube_swap(bv1, bv2);
	}
	else {
	  // (0, 1, 2)
	  // そのまま
	}
      }
    }
    return;
  }
  if ( n == 4 ) {
    AlgBitVect* bv0 = bv + start * _cube_size();
    AlgBitVect* bv1 = bv0 + _cube_size();
    AlgBitVect* bv2 = bv1 + _cube_size();
    AlgBitVect* bv3 = bv2 + _cube_size();
    _resize_buff(1);
    // 0 と 1 を整列
    if ( cube_compare(bv0, bv1) < 0 ) {
      cube_swap(bv0, bv1);
    }
    // 2 と 3 を整列
    if ( cube_compare(bv2, bv3) < 0 ) {
      cube_swap(bv2, bv3);
    }
    // 0 と 2 を比較
    if ( cube_compare(bv0, bv2) < 0 ) {
      if ( cube_compare(bv0, bv3) < 0 ) {
	// (0, 1) と (2, 3) を交換
	cube_swap(bv0, bv2);
	cube_swap(bv1, bv3);
      }
      else if ( cube_compare(bv1, bv3) < 0 ) {
	// 0 <- 2, 2 <- 3, 3 <- 1, 1 <- 0
	cube_rotate4(bv0, bv2, bv3, bv1);
      }
      else {
	// 0 <- 2, 2 <- 1, 1 <- 0
	cube_rotate3(bv0, bv2, bv1);
      }
    }
    else if ( cube_compare(bv1, bv2) < 0 ) {
      if ( cube_compare(bv1, bv3) < 0 ) {
	// 1 <- 2, 2 <- 3, 3 <- 1
	cube_rotate3(bv1, bv2, bv3);
      }
      else {
	// 1 <- 2, 2 <- 1
	cube_swap(bv1, bv2);
      }
    }
    else {
      // そのまま
    }
    return;
  }

  // 半分に分割してそれぞれソートする．
  int hn = (n + 1) / 2;
  int start1 = start;
  int end1 = start + hn;
  int start2 = end1;
  int end2 = end;
  _sort(bv, start1, end1);
  _sort(bv, start2, end2);

  // trivial case
  // 前半部分の末尾が後半部分の先頭より大きければ
  // すでに整列している．
  const AlgBitVect* bv_end1 = bv + (end1 - 1) * _cube_size();
  const AlgBitVect* bv_start2 = bv + start2 * _cube_size();
  if ( cube_compare(bv_end1, bv_start2) > 0 ) {
    return;
  }

  // マージする．
  // 前半部分を一旦 mTmpBuff にコピーする．
  _resize_buff(hn);
  _copy(mTmpBuff, bv + start1 * _cube_size(), hn);
  AlgBitVect* bv1 = mTmpBuff;
  AlgBitVect* bv1_end = bv1 + hn * _cube_size();
  AlgBitVect* bv2 = bv + start2 * _cube_size();
  AlgBitVect* bv2_end = bv + end2 * _cube_size();
  AlgBitVect* dst_bv = bv + start * _cube_size();
  while ( bv1 != bv1_end && bv2 != bv2_end ) {
    int comp_res = cube_compare(bv1, bv2);
    if ( comp_res > 0 ) {
      cube_copy(dst_bv, bv1);
      bv1 += _cube_size();
      dst_bv += _cube_size();
    }
    else if ( comp_res < 0 ) {
      cube_copy(dst_bv, bv2);
      bv2 += _cube_size();
      dst_bv += _cube_size();
    }
    else {
      // 重複したキューブはエラー
      ASSERT_NOT_REACHED;
    }
  }
  while ( bv1 != bv1_end ) {
    cube_copy(dst_bv, bv1);
    bv1 += _cube_size();
    dst_bv += _cube_size();
  }
  // 後半部分が残っている時はそのままでいいはず．
  ASSERT_COND( bv2 == dst_bv );
}

// @brief カバー(を表すビットベクタ)の比較を行う．
// @param[in] src1 1つめのブロック
// @param[in] src2 2つめのブロック
// @retval -1 src1 <  src2
// @retval  0 src1 == src2
// @retval  1 src1 >  src2
//
// 比較はキューブごとの辞書式順序で行う．
int
AlgMgr::compare(const AlgBlock& src1,
		const AlgBlock& src2)
{
  const AlgBitVect* bv1 = src1.bitvect();
  const AlgBitVect* bv1_end = bv1 + src1.cube_num() + _cube_size();
  const AlgBitVect* bv2 = src2.bitvect();
  const AlgBitVect* bv2_end = bv2 + src2.cube_num() + _cube_size();
  for ( ; bv1 != bv1_end && bv2 != bv2_end; bv1 += _cube_size(), bv2 += _cube_size() ) {
    int res = cube_compare(bv1, bv2);
    if ( res != 0 ) {
      return res;
    }
  }
  if ( bv1 != bv1_end ) {
    // bv2 == bv2_end なので bv1 が大きい
    return 1;
  }
  if ( bv2 != bv2_end ) {
    // bv1 == bv1_end なので bv2 が大きい
    return -1;
  }
  // 等しい
  return 0;
}

// @brief ビットベクタからハッシュ値を計算する．
// @param[in] src1 1つめのブロック
SizeType
AlgMgr::hash(const AlgBlock& src1)
{
  // キューブは常にソートされているので
  // 順番は考慮する必要はない．
  // ただおなじキューブの中のビットに関しては
  // 本当は区別しなければならないがどうしようもないので
  // 16ビットに区切って単純に XOR を取る．
  int n = src1.cube_num() * _cube_size();
  SizeType ans = 0;
  const AlgBitVect* bv = src1.bitvect();
  const AlgBitVect* bv_end = bv + n;
  for ( ; bv != bv_end; ++ bv ) {
    AlgBitVect pat = *bv;
    ans ^= (pat & 0xFFFFULL); pat >>= 16;
    ans ^= (pat & 0xFFFFULL); pat >>= 16;
    ans ^= (pat & 0xFFFFULL); pat >>= 16;
    ans ^= pat;
  }
  return ans;
}

// @brief キューブ(を表すビットベクタ)の比較を行う．
// @param[in] bv1 1つめのキューブを表すビットベクタ
// @param[in] bv2 2つめのキューブを表すビットベクタ
// @retval -1 bv1 <  bv2
// @retval  0 bv1 == bv2
// @retval  1 bv1 >  bv2
int
AlgMgr::cube_compare(const AlgBitVect* bv1,
		     const AlgBitVect* bv2)
{
  const AlgBitVect* bv1_end = bv1 + _cube_size();
  while ( bv1 != bv1_end ) {
    AlgBitVect pat1 = *bv1;
    AlgBitVect pat2 = *bv2;
    if ( pat1 < pat2 ) {
      return -1;
    }
    else if ( pat1 > pat2 ) {
      return 1;
    }
    ++ bv1;
    ++ bv2;
  }
  return 0;
}

// @brief 2つのキューブの積が空でない時 true を返す．
// @param[in] bv1 1つめのキューブを表すビットベクタ
// @param[in] bv2 2つめのキューブを表すビットベクタ
bool
AlgMgr::cube_check_conflict(const AlgBitVect* bv1,
			    const AlgBitVect* bv2)
{
  const AlgBitVect* bv1_end = bv1 + _cube_size();
  while ( bv1 != bv1_end ) {
    AlgBitVect tmp = *bv1 | *bv2;
    AlgBitVect mask1 = 0x5555555555555555ULL;
    AlgBitVect mask2 = 0xAAAAAAAAAAAAAAAAULL;
    AlgBitVect tmp1 = tmp & mask1;
    AlgBitVect tmp2 = tmp & mask2;
    if ( (tmp1 & (tmp2 >> 1)) != 0ULL ) {
      // 同じ変数の異なる極性のリテラルがあった．
      return true;
    }
    ++ bv1;
    ++ bv2;
  }
  return false;
}

// @brief 一方のキューブが他方のキューブに含まれているか調べる．
// @param[in] bv1 1つめのキューブを表すビットベクタ
// @param[in] bv2 2つめのキューブを表すビットベクタ
// @return 1つめのキューブが2つめのキューブ に含まれていたら true を返す．
bool
AlgMgr::cube_check_containment(const AlgBitVect* bv1,
			       const AlgBitVect* bv2)
{
  const AlgBitVect* bv1_end = bv1 + _cube_size();
  while ( bv1 != bv1_end ) {
    if ( (~(*bv1) & *bv2) != 0ULL ) {
      return false;
    }
    ++ bv1;
    ++ bv2;
  }
  return true;
}

// @brief ２つのキューブに共通なリテラルがあれば true を返す．
// @param[in] bv1 1つめのキューブを表すビットベクタ
// @param[in] bv2 2つめのキューブを表すビットベクタ
// @return ２つのキューブに共通なリテラルがあれば true を返す．
bool
AlgMgr::cube_check_intersect(const AlgBitVect* bv1,
			     const AlgBitVect* bv2)
{
  const AlgBitVect* bv1_end = bv1 + _cube_size();
  while ( bv1 != bv1_end ) {
    if ( (*bv1 & *bv2) != 0ULL ) {
      return true;
    }
    ++ bv1;
    ++ bv2;
  }
  return false;
}

// @brief キューブ(を表すビットベクタ)をクリアする．
// @param[in] dst_bv 対象のビットベクタ
void
AlgMgr::cube_clear(AlgBitVect* dst_bv)
{
  AlgBitVect* dst_bv_end = dst_bv + _cube_size();
  while ( dst_bv != dst_bv_end ) {
    *dst_bv = 0ULL;
    ++ dst_bv;
  }
}

// @brief 2つのキューブの積を計算する．
// @param[in] dst_bv コピー先のビットベクタ
// @param[in] bv1 1つめのキューブを表すビットベクタ
// @param[in] bv2 2つめのキューブを表すビットベクタ
// @retval true 積が空でなかった．
// @retval false 積が空だった．
bool
AlgMgr::cube_product(AlgBitVect* dst_bv,
		     const AlgBitVect* bv1,
		     const AlgBitVect* bv2)
{
  AlgBitVect* dst_bv_end = dst_bv + _cube_size();
  while ( dst_bv != dst_bv_end ) {
    AlgBitVect tmp = *bv1 | *bv2;
    AlgBitVect mask1 = 0x5555555555555555ULL;
    AlgBitVect mask2 = 0xAAAAAAAAAAAAAAAAULL;
    AlgBitVect tmp1 = tmp & mask1;
    AlgBitVect tmp2 = tmp & mask2;
    if ( (tmp1 & (tmp2 >> 1)) != 0ULL ) {
      // 同じ変数の異なる極性のリテラルがあった．
      return false;
    }
    *dst_bv = tmp;
    ++ dst_bv;
    ++ bv1;
    ++ bv2;
  }
  return true;
}

// @brief キューブによる商を求める．
// @param[in] dst_bv コピー先のビットベクタ
// @param[in] bv1 1つめのキューブを表すビットベクタ
// @param[in] bv2 2つめのキューブを表すビットベクタ
// @return 正しく割ることができたら true を返す．
bool
AlgMgr::cube_quotient(AlgBitVect* dst_bv,
		      const AlgBitVect* bv1,
		      const AlgBitVect* bv2)
{
  AlgBitVect* dst_bv_end = dst_bv + _cube_size();
  while ( dst_bv != dst_bv_end ) {
    AlgBitVect pat1 = *bv1;
    AlgBitVect pat2 = *bv2;
    if ( (~pat1 & pat2) != 0ULL ) {
      // この場合の dst の値は不定
      return false;
    }
    *dst_bv = pat1 & ~pat2;
    ++ dst_bv;
    ++ bv1;
    ++ bv2;
  }
  return true;
}

// @brief 要素のチェック
// @param[in] bv ビットベクタ
// @param[in] lit 対象のリテラル
// @return lit を含んでいたら true を返す．
bool
AlgMgr::is_in(AlgBitVect* bv,
	      Literal lit)
{
  VarId var_id = lit.varid();
  int blk = _block_pos(var_id);
  int sft = _shift_num(var_id);
  AlgBitVect pat = lit2bv(lit);
  AlgBitVect mask = pat << sft;
  if ( bv[blk] & mask ) {
    return true;
  }
  else {
    return false;
  }
}

// @brief 要素の追加
// @param[in] bv ビットベクタ
// @param[in] lit 対象のリテラル
void
AlgMgr::add_lit(AlgBitVect* bv,
		Literal lit)
{
  VarId var_id = lit.varid();
  int blk = _block_pos(var_id);
  int sft = _shift_num(var_id);
  AlgBitVect pat = lit2bv(lit);
  AlgBitVect mask = pat << sft;
  bv[blk] |= mask;
}

// @brief カバー/キューブの内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] bv カバー/キューブを表すビットベクタ
// @param[in] start キューブの開始位置
// @param[in] end キューブの終了位置
// @param[in] varname_list 変数名のリスト
//
// end は実際の末尾 + 1 を指す．
void
AlgMgr::print(ostream& s,
	      const AlgBitVect* bv,
	      int start,
	      int end,
	      const vector<string>& varname_list)
{
  const char* plus = "";
  for ( int i = start; i < end; ++ i ) {
    s << plus;
    plus = " + ";
    const char* spc = "";
    for ( int j = 0; j < variable_num(); ++ j ) {
      string varname = varname_list[j];
      AlgPol pol = get_pol(bv, i, VarId(j));
      if ( pol == AlgPol::P ) {
	s << spc << varname;
	spc = " ";
      }
      else if ( pol == AlgPol::N ) {
	s << spc << varname << "'";
	spc = " ";
      }
    }
  }
}

// @brief mTmpBuff を初期化する．
void
AlgMgr::_init_buff()
{
  // とりあえず 128 を初期値とする．
  mTmpBuffSize = 128;
  mTmpBuff = new_body(mTmpBuffSize);
}

// @brief mTmpBuff に必要な領域を確保する．
// @param[in] req_size 要求するキューブ数
void
AlgMgr::_resize_buff(int req_size)
{
  int old_size = mTmpBuffSize;
  while ( mTmpBuffSize < req_size ) {
    mTmpBuffSize <<= 1;
  }

  if ( old_size < mTmpBuffSize ) {
    delete_body(mTmpBuff, old_size);
  }
  mTmpBuff = new_body(mTmpBuffSize);
}

END_NAMESPACE_YM_LOGIC
