
/// @file walsh_2_refine.cc
/// @brief walsh_2_refine の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2017, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/TvFunc.h"
#include "IgPartition.h"
#include "PolConf.h"


BEGIN_NONAMESPACE

#ifdef YM_DEBUG
#define DEBUG_FLAG true
#endif

#ifndef DEBUG_FLAG
#define DEBUG_FLAG false
#endif

int debug = DEBUG_FLAG;
//int debug = true;

END_NONAMESPACE


BEGIN_NAMESPACE_YM_LOGIC

BEGIN_NONAMESPACE

class W2Cmp
{
public:

  W2Cmp(
    SizeType n
  ) : mW2Array(n)
  {
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief w2 を設定する．
  void
  set_w2(
    SizeType gid,
    int w2
  )
  {
    mW2Array[gid] = w2;
  }

  /// @brief 等価比較演算子
  bool
  eq(
    SizeType gid1,
    SizeType gid2
  ) const
  {
    return mW2Array[gid1] == mW2Array[gid2];
  }

  /// @brief 大小比較演算子
  bool
  gt(
    SizeType gid1,
    SizeType gid2
  ) const
  {
    return mW2Array[gid1] > mW2Array[gid2];
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  std::vector<int> mW2Array;

};

END_NONAMESPACE

// @brief 重み別 w2 を用いて変数順序を確定させる．
// @param[in] var_list 対象の変数リスト
void
walsh_2_refine(
  const TvFunc& func,
  SizeType var,
  IgPartition& igpart
)
{
  auto ni = func.input_num();

  if ( debug ) {
    std::cout << "before walsh_w2_refine(" << var << ")" << std::endl
	      << igpart << std::endl;
  }

  // walsh_2 係数を用いて極性の決定を行う．
  for ( SizeType pid = 0; pid < igpart.partition_num(); ++ pid ) {
    if ( igpart.is_resolved(pid) ) {
      continue;
    }
    W2Cmp w2_cmp(igpart.group_num());
    for ( SizeType pos = igpart.partition_begin(pid);
	  pos < igpart.partition_end(pid); ++ pos ) {
      auto gid = igpart.group_id(pos);
      auto iid = igpart.input_id(gid);
      int w2 = func.walsh_2(var, iid);
      w2_cmp.set_w2(gid, w2);
    }
    auto delta = igpart.refine(pid, w2_cmp);
    pid += delta;
  }

  // 確定した分割を前に持ってくる．
  igpart.reorder();

  if ( debug ) {
    std::cout << "after walsh_w2_refine" << std::endl
	      << igpart << std::endl;
  }
}

END_NAMESPACE_YM_LOGIC
