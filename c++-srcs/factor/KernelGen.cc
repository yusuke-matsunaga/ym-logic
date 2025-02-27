
/// @file KernelGen.cc
/// @brief KernelGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "KernelGen.h"
#include "LitSet.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
// クラス SopCover
//////////////////////////////////////////////////////////////////////

// @brief すべてのカーネルとコカーネルペアを列挙する．
vector<pair<SopCover, vector<SopCube>>>
SopCover::all_kernels() const
{
  KernelGen kg;
  return kg.all_kernels(*this);
}

// @brief 最も価値の高いカーネルを求める．
SopCover
SopCover::best_kernel() const
{
  KernelGen kg;
  return kg.best_kernel(*this);
}

// @brief 最も価値の高いカーネルを求める．
SopCover
SopCover::best_kernel(
  std::function<int(const SopCover&, const vector<SopCube>&)> eval_func
) const
{
  KernelGen kg;
  return kg.best_kernel(*this, eval_func);
}


//////////////////////////////////////////////////////////////////////
// クラス KernelGen
//////////////////////////////////////////////////////////////////////

// @brief カーネルとコカーネルを列挙する．
vector<pair<SopCover, vector<SopCube>>>
KernelGen::all_kernels(
  const SopCover& cover
)
{
  generate(cover);

  // kernel_list に設定する．
  vector<pair<SopCover, vector<SopCube>>> kernel_list;
  kernel_list.reserve(mKernelDict.size());

  // この処理は破壊的なので以降は mKernelDict は使えない．
  for ( auto& p: mKernelDict ) {
    auto& kernel = p.first;
    auto& cokernels = p.second;
    kernel_list.push_back(make_pair(std::move(kernel),
				    std::move(cokernels)));
  }

  std::sort(kernel_list.begin(), kernel_list.end());

  return kernel_list;
}

BEGIN_NONAMESPACE

// デフォルトの評価関数
int
eval_func(
  const SopCover& kernel,
  const vector<SopCube>& cokernels
)
{
  auto k_nc = kernel.cube_num();
  auto k_nl = kernel.literal_num();
  auto c_nc = cokernels.size();
  auto c_nl = SopCube::literal_num(cokernels);
  int value = (k_nc - 1) * c_nl + (c_nc - 1) * k_nl;
  return value;
}

END_NONAMESPACE


// @brief 最も価値の高いカーネルを返す．
SopCover
KernelGen::best_kernel(
  const SopCover& cover
)
{
  return best_kernel(cover, eval_func);
}

// @brief 最も価値の高いカーネルを返す．
SopCover
KernelGen::best_kernel(
  const SopCover& cover,
  std::function<int(const SopCover&, const vector<SopCube>&)> eval_func
)
{
  // 特例1: cover が single cube の場合は空のカバーを返す．
  if ( cover.cube_num() <= 1 ) {
    return SopCover(cover.variable_num());
  }

  generate(cover);

  // 特例2: 自身がレベル０カーネルの場合は空のカバーを返す．
  if ( mKernelDict.size() == 1 ) {
    auto& cokernels = mKernelDict.begin()->second;
    if ( cokernels.size() == 1 &&
	 cokernels.front().literal_num() == 0 ) {
      return SopCover(cover.variable_num());
    }
  }

  // 価値の最も大きいカーネルを求める．
  // この処理は破壊的なので以降は mKernelDict を使えない．
  int max_value = -1;
  auto max_p = mKernelDict.end();
  for ( auto p = mKernelDict.begin();
	p != mKernelDict.end(); ++ p ) {
    auto& kernel = p->first;
    auto& cokernels = p->second;
    int value = eval_func(kernel, cokernels);
    if ( max_value < value ) {
      max_value = value;
      max_p = p;
    }
  }

  auto& kernel = max_p->first;
  return kernel;
}

// @brief カーネルとコカーネルを列挙する．
void
KernelGen::generate(
  const SopCover& cover
)
{
  // 2回以上現れるリテラルの（出現頻度でソートされた）リストを作る．
  auto literal_list = gen_literal_list(cover);

  mEnd = literal_list.end();
  hash_clear();

  SizeType nv = cover.variable_num();
  auto ccube0 = SopCube{nv}; // 空のキューブ
  auto plits = LitSet{nv}; // 空集合
  kern_sub(cover, literal_list.begin(), ccube0, plits);

  // 特例：自分自身がカーネルとなっているか調べる．
  auto ccube = cover.common_cube();
  if ( ccube.literal_num() == 0 ) {
    hash_add(cover, ccube);
  }
}

// @brief 出現頻度の昇順にならべたリテラルのリストを作る．
vector<Literal>
KernelGen::gen_literal_list(
  const SopCover& cover ///< [in] 対象のカバー
)
{
  // cover に2回以上現れるリテラルとその出現頻度のリストを作る．
  SizeType nv = cover.variable_num();
  vector<pair<SizeType, Literal>> tmp_list;
  tmp_list.reserve(nv * 2);
  for ( SizeType var: Range(nv) ) {
    for ( auto lit: {Literal(var, false), Literal(var, true)} ) {
      auto n = cover.literal_num(lit);
      if ( n >= 2 ) {
	tmp_list.push_back(make_pair(n, lit));
      }
    }
  }

  // 出現頻度の昇順にソートする．
  // c++-11 のラムダ式を使っている．
  sort(tmp_list.begin(), tmp_list.end(),
       [](const pair<SizeType, Literal>& a,
	  const pair<SizeType, Literal>& b) -> bool
       { return a.first < b.first; });

  // リテラルだけを literal_list に移す．
  auto n = tmp_list.size();
  vector<Literal> literal_list;
  literal_list.reserve(n);
  for ( auto& p: tmp_list ) {
    auto lit = p.second;
    literal_list.push_back(lit);
  }
  return literal_list;
}

// @brief カーネルを求める下請け関数
void
KernelGen::kern_sub(
  const SopCover& cover,
  vector<Literal>::const_iterator p,
  const SopCube& ccube,
  const LitSet& plits
)
{
  auto plits1 = plits;
  for ( ; p != mEnd; ++ p ) {
    auto lit = *p;

    if ( cover.literal_num(lit) <= 1 ) {
      // 2回以上現れていなければスキップする．
      continue;
    }

    // まず lit で割る．
    auto cover1 = cover.algdiv(lit);
    // 共通なキューブを求める．
    auto ccube1 = cover1.common_cube();
    if ( plits1.check_intersect(ccube1) ) {
      // plits1 にはすでに処理したリテラルが入っている．
      // それと ccube1 が共通部分をもっていたということは
      // cover1 はすでに処理されている．
      continue;
    }

    // cover1 を cube-free にする．
    cover1.algdiv_int(ccube1);

    // ccube1 を cover1 を導出したキューブにする．
    ccube1 &= ccube;
    ccube1 &= lit;

    // plits1 を更新する．
    plits1 += lit;

    // 再帰する．
    kern_sub(cover1, p + 1, ccube1, plits1);

    // cover1/ccube1 を記録．
    hash_add(std::move(cover1), ccube1);
  }
}

END_NAMESPACE_YM_SOP
