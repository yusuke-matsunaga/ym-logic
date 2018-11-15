
/// @file AlgKernelGen.cc
/// @brief AlgKernelGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "AlgKernelGen.h"
#include "AlgLitSet.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_LOGIC

//////////////////////////////////////////////////////////////////////
// クラス AlgKernelGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AlgKernelGen::AlgKernelGen()
{
}

// @brief デストラクタ
AlgKernelGen::~AlgKernelGen()
{
}

// @brief カーネルとコカーネルを列挙する．
// @param[in] cover 対象のカバー
// @param[out] kernel_list 結果を格納するリスト
void
AlgKernelGen::generate(const AlgCover& cover,
		       vector<AlgKernelInfo>& kernel_list)
{
  // cover に2回以上現れるリテラルとそのの出現頻度のリストを作る．
  int nv = cover.variable_num();
  vector<pair<int, Literal>> tmp_list;
  tmp_list.reserve(nv * 2);
  for ( int i: Range(nv) ) {
    VarId var(i);
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

  AlgCube ccube0(nv); // 空のキューブ
  AlgLitSet plits(nv); // 空集合
  kern_sub(cover, literal_list.begin(), ccube0, plits, kernel_list);

  // 特例：自分自身がカーネルとなっているか調べる．
  AlgCube ccube = cover.common_cube();
  if ( ccube.literal_num() == 0 ) {
    kernel_list.push_back(AlgKernelInfo{cover, ccube});
  }
}

// @brief カーネルを求める下請け関数
// @param[in] cover 対象のカバー
// @param[in] pos mLitList 上の位置
// @param[in] ccube 今までに括りだされた共通のキューブ
// @param[in] plits mLitList[0]〜mLitList[pos - 1] までをまとめたリテラル集合
void
AlgKernelGen::kern_sub(const AlgCover& cover,
		       vector<Literal>::const_iterator p,
		       const AlgCube& ccube,
		       const AlgLitSet& plits,
		       vector<AlgKernelInfo>& kernel_list)
{
  AlgLitSet plits1(plits);
  while ( p != mEnd ) {
    Literal lit = *p;
    ++ p;

    if ( cover.literal_num(lit) <= 1 ) {
      // 2回以上現れていなければスキップする．
      continue;
    }

    // まず lit で割る．
    AlgCover cover1 = cover / lit;
    // 共通なキューブを求める．
    AlgCube ccube1 = cover1.common_cube();
    if ( ccube1.check_intersect(plits1) ) {
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
    kern_sub(cover1, p, ccube1, plits1, kernel_list);

    // cover1/ccube1/level を記録．
    kernel_list.push_back(AlgKernelInfo{std::move(cover1), std::move(ccube1)});
  }
}

END_NAMESPACE_YM_LOGIC
