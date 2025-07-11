#ifndef TVFUNCTEST_H
#define TVFUNCTEST_H

/// @file TvFuncTest.h
/// @brief TvFuncTest のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/TvFunc.h"
#include <random>


BEGIN_NAMESPACE_YM

const bool debug = false;

//////////////////////////////////////////////////////////////////////
/// @class TvFuncTestWithParam TvFuncTestWithParam.h "TvFuncTestWithParam.h"
/// @brief 入力数をパラメータに持つ TvFunc 用のテストフィクスチャ
//////////////////////////////////////////////////////////////////////
class TvFuncTestWithParam :
public ::testing::TestWithParam<int>
{
public:

  // コンストラクタ
  TvFuncTestWithParam(
  ) : mRandDist(0, 1)
  {
  }

  void
  init_values()
  {
    auto ni = GetParam();
    auto ni_exp = 1 << ni;
    mValues.clear();
    mValues.resize(ni_exp, 0);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ベクタの値を格納するバッファ
  std::vector<int> mValues;

  // 乱数生成器
  std::mt19937 mRandGen;

  // 0, 1 の一様分布生成器
  std::uniform_int_distribution<int> mRandDist;

};

END_NAMESPACE_YM

#endif // TVFUNCTEST_H
