
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
// クラス KernelGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
KernelGen::KernelGen()
{
}

// @brief デストラクタ
KernelGen::~KernelGen()
{
  hash_clear();
}

// @brief カーネルとコカーネルを列挙する．
void
KernelGen::all_kernel(
  const SopCover& cover,
  vector<pair<SopCover, SopCover>>& kernel_list
)
{
  generate(cover);

  // kernel_list に設定する．
  // この処理は破壊的なので以降は mHashTable は使えない．
  for ( auto cell: mCellList ) {
    kernel_list.push_back(make_pair(std::move(cell->mKernel), std::move(cell->mCoKernels)));
  }
  hash_clear();
}

// @brief 最も価値の高いカーネルを返す．
SopCover
KernelGen::best_kernel(
  const SopCover& cover
)
{
  generate(cover);

  // 特例: 自身がレベル０カーネルの場合は空のカバーを返す．
  if ( mCellList.size() == 1 && mCellList[0]->mCoKernels.literal_num() == 0 ) {
    hash_clear();
    return SopCover(cover.variable_num());
  }

  // 価値の最も大きいカーネルを求める．
  int max_value = -1;
  Cell* max_cell = nullptr;
  for ( auto cell: mCellList ) {
    const SopCover& kernel = cell->mKernel;
    int k_nc = kernel.cube_num();
    int k_nl = kernel.literal_num();
    const SopCover& cokernels = cell->mCoKernels;
    int c_nc = cokernels.cube_num();
    int c_nl = cokernels.literal_num();
    int value = (k_nc - 1) * c_nl + (c_nc - 1) * k_nl;
    if ( max_value < value ) {
      max_value = value;
      max_cell = cell;
    }
  }
  SopCover ans{std::move(max_cell->mKernel)};
  hash_clear();

  return ans;
}

// @brief カーネルとコカーネルを列挙する．
void
KernelGen::generate(
  const SopCover& cover
)
{
  // cover に2回以上現れるリテラルとその出現頻度のリストを作る．
  SizeType nv = cover.variable_num();
  vector<pair<int, Literal>> tmp_list;
  tmp_list.reserve(nv * 2);
  for ( int var: Range(nv) ) {
    for ( auto lit: {Literal(var, false), Literal(var, true)} ) {
      int n = cover.literal_num(lit);
      if ( n >= 2 ) {
	tmp_list.push_back(make_pair(n, lit));
      }
    }
  }

  // 出現頻度の昇順にソートする．
  // c++-11 のラムダ式を使っている．
  sort(tmp_list.begin(), tmp_list.end(),
       [](const pair<int, Literal>& a,
	  const pair<int, Literal>& b) -> bool
       { return a.first < b.first; });

  // リテラルだけを literal_list に移す．
  int n = tmp_list.size();
  vector<Literal> literal_list(n);
  for ( int i: Range(n) ) {
    literal_list[i] = tmp_list[i].second;
  }

  mEnd = literal_list.end();
  hash_clear();
  hash_resize(32);

  SopCube ccube0(nv); // 空のキューブ
  LitSet plits(nv); // 空集合
  kern_sub(cover, literal_list.begin(), ccube0, plits);

  // 特例：自分自身がカーネルとなっているか調べる．
  SopCube ccube = cover.common_cube();
  if ( ccube.literal_num() == 0 ) {
    SopCover cover1(cover);
    hash_add(std::move(cover1), ccube);
  }
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
  LitSet plits1(plits);
  while ( p != mEnd ) {
    Literal lit = *p;
    ++ p;

    if ( cover.literal_num(lit) <= 1 ) {
      // 2回以上現れていなければスキップする．
      continue;
    }

    // まず lit で割る．
    SopCover cover1 = cover / lit;
    // 共通なキューブを求める．
    SopCube ccube1 = cover1.common_cube();
    if ( plits1.check_intersect(ccube1) ) {
      // plits にはすでに処理したリテラルが入っている．
      // それと ccube1 が共通部分をもっていたということは
      // cover1 はすでに処理されている．
      continue;
    }

    // cover1 を cube-free にする．
    cover1 /= ccube1;

    // ccube1 を cover1 を導出したキューブにする．
    ccube1 *= ccube;
    ccube1 *= lit;

    // plits1 を更新する．
    plits1 += lit;

    // 再帰する．
    kern_sub(cover1, p, ccube1, plits1);

    // cover1/ccube1 を記録．
    hash_add(std::move(cover1), ccube1);
  }
}

// @brief ハッシュ表をクリアする．
void
KernelGen::hash_clear()
{
  for ( auto cell: mCellList ) {
    delete cell;
  }
  mHashTable.clear();
  mCellList.clear();
}

// @brief ハッシュ表に登録する．
void
KernelGen::hash_add(
  SopCover&& kernel,
  const SopCube& cokernel
)
{
  SizeType hash = kernel.hash();
  int index = hash % mHashSize;
  bool found = false;
  for ( auto cell = mHashTable[index];
	cell != nullptr;
	cell = cell->mLink ) {
    if ( cell->mKernel == kernel ) {
      // 同じカーネルがあった．
      cell->mCoKernels += cokernel;
      found = true;
      break;
    }
  }
  if ( !found ) {
    if ( mCellList.size() >= mNextLimit ) {
      hash_resize(mHashSize * 2);
      index = hash % mHashSize;
    }
    auto cell = new Cell{std::move(kernel), cokernel};
    cell->mLink = mHashTable[index];
    mHashTable[index] = cell;
    mCellList.push_back(cell);
  }
}

// @brief ハッシュ表をリサイズする．
void
KernelGen::hash_resize(
  SizeType size
)
{
  vector<Cell*> new_table(size, nullptr);
  for ( auto cell: mHashTable ) {
    while ( cell != nullptr ) {
      auto next = cell->mLink;
      SizeType hash = cell->mKernel.hash();
      int index = hash % size;
      cell->mLink = new_table[index];
      new_table[index] = cell;
    }
  }
  mHashTable.swap(new_table);
  mHashSize = size;
  mNextLimit = static_cast<int>(mHashSize * 1.8);
}

END_NAMESPACE_YM_SOP
