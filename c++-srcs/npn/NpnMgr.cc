
/// @file NpnMgr.cc
/// @brief NpnMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2017, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "npn/NpnMgr.h"
#include "ym/TvFunc.h"
#include "IgPartition.h"
#include "PolConf.h"


BEGIN_NONAMESPACE

const int debug_ww0_refine  = 0x0001;
const int debug_ww1_refine  = 0x0002;
const int debug_w2max_recur = 0x0010;
const int debug_w2refine    = 0x0040;
const int debug_none        = 0x0000;
const int debug_all         = 0xffff;

#ifdef YM_DEBUG
#define DEBUG_FLAG debug_all
#endif

#ifndef DEBUG_FLAG
#define DEBUG_FLAG debug_none
#endif

int debug = DEBUG_FLAG;
//int debug = debug_all;

END_NONAMESPACE


BEGIN_NAMESPACE_YM_LOGIC

extern
bool
walsh_01_normalize(
  const TvFunc& func,
  NpnMap& xmap,
  InputInfo& input_info
);

extern
void
walsh_w0_refine(
  const TvFunc& func,
  std::vector<PolConf>& polconf_list
);

extern
void
walsh_w1_refine(
  const TvFunc& func,
  SizeType var,
  std::vector<PolConf>& polconf_list
);

extern
void
walsh_w1_refine(
  const TvFunc& func,
  IgPartition& igpart
);

extern
void
walsh_2_refine(
  const TvFunc& func,
  SizeType var,
  IgPartition& igpart
);


//////////////////////////////////////////////////////////////////////
// クラス NpnMgr
//////////////////////////////////////////////////////////////////////

// コンストラクタ
NpnMgr::NpnMgr()
{
}

// デストラクタ
NpnMgr::~NpnMgr()
{
}

// @brief func の正規化を行う．
TvFunc
NpnMgr::cannonical(
  const TvFunc& func
)
{
  mMaxList.clear();
  mTvmax_count = 0;

  // まず独立な変数を取り除く
  mXmap0 = func.shrink_map();
  auto func0 = func.xform(mXmap0);

  auto ni0 = func0.input_num();

  // 特例
  if ( ni0 == 0 ) {
    if ( func0.value(0) == 0 ) {
      // 定数0関数
      add_map(NpnMap::identity(0, true));
      return ~func0;
    }
    else {
      // 定数1関数
      add_map(NpnMap::identity(0, false));
      return func0;
    }
  }
  if ( ni0 == 1 ) {
    if ( func0.value(0) == 0 ) {
      ASSERT_COND ( func0.value(1) == 1 );

      // 肯定のリテラル関数
      add_map(NpnMap::identity(1, false));
      return func0;
    }
    else {
      ASSERT_COND( func0.value(1) == 0 );

      // 否定のリテラル関数
      add_map(NpnMap::identity(1, true));
      return ~func0;
    }
  }

  if ( debug ) {
    if ( ni0 < 10 ) {
      std::cout << "NpnMgr::cannonical: func0 = " << func0 << std::endl;
    }
  }

  // Walsh の0次と1次の係数を用いて正規化する．
  NpnMap map1;
  InputInfo iinfo;
  bool opol_fixed = walsh_01_normalize(func0, map1, iinfo);
  mXmap0 = mXmap0 * map1;

  // map1 に従って func0 を変換する．
  auto func1 = func0.xform(map1);

  // w1:group_num:bisym でグループを分割する．
  IgPartition igpart(iinfo);

  if ( debug ) {
    std::cout << "  after walsh01_normalize" << std::endl;
    if ( ni0 < 10 ) {
      std::cout << "    func1  = " << func1 << std::endl;
    }
    std::cout << "    map1   = " << map1 << std::endl
	      << "    igpart = " << igpart << std::endl
	      << std::endl;
    }

  if ( opol_fixed && iinfo.polundet_num() == 0 && igpart.is_resolved() ) {
    // 解決した．
    auto map = igpart.to_npnmap();
    add_map(map);
    auto func2 = func1.xform(map);
    if ( debug ) {
      std::cout << "  resolved" << std::endl;
      if ( ni0 < 10 ) {
	std::cout << "  func2 = " << func2 << std::endl;
      }
      std::cout << "  map2  = " << map << std::endl;
    }
    return func2;
  }

  // 入力の等価グループは iinfo に入っている．
  // 以降は等価グループごとに極性と順序を考えればよい．
  // 極性に関しては
  // - 通常の対称グループ: そのままか全部反転
  // - bi-symmetry の対称グループ: そのままか先頭だけ反転
  // となる．

  // polundet_num の指数乗だけ極性の割り当てがある．
  auto nug = iinfo.polundet_num();
  auto nug_exp = 1U << nug;
  auto n = nug_exp;
  if ( !opol_fixed ) {
    n *= 2;
  }
  std::vector<PolConf> polconf_list;
  polconf_list.reserve(n);
  int input_mask = 0U;
  for ( int i = 0; i < ni0; ++ i ) {
    input_mask |= (1U << i);
  }
  for ( int p = 0; p < nug_exp; ++ p ) {
    int input_bits = 0U;
    for (int i = 0; i < nug; ++ i) {
      int gid = iinfo.polundet_gid(i);
      if ( p & (1U << i) ) {
	input_bits |= iinfo.inv_bits(gid);
      }
    }
    polconf_list.push_back(PolConf(false, input_bits));
    if ( !opol_fixed ) {
      polconf_list.push_back(PolConf(true , input_bits ^ input_mask));
    }
  }

  // 極性割り当ての候補を重み別 Walsh_0 でフィルタリングする．
  walsh_w0_refine(func1, polconf_list);

  // 極性割り当ての候補を確定した入力の重み別 Walsh_1 でフィルタリングする．
  for ( int pid = 0;
	polconf_list.size() > 1 && pid < igpart.partition_num();
	++ pid ) {
    if ( igpart.is_resolved(pid) ) {
      auto pos = igpart.partition_begin(pid);
      auto gid = igpart.group_id(pos);
      auto iid = igpart.input_id(gid);
      walsh_w1_refine(func1, iid, polconf_list);
    }
  }

  if ( debug ) {
    std::cout << "  # of polarity candidates: " << polconf_list.size() << std::endl;
    for ( int i = 0; i < polconf_list.size(); ++ i ) {
      std::cout << "    ";
      print_polconf(std::cout, polconf_list[i], ni0);
    }
    std::cout << "    Partition: " << igpart << std::endl
	      << std::endl;
  }

  // 残りはすべて展開して真理値ベクタが最大となるものを探す．
  mMaxFunc = TvFunc::zero(ni0);
  mMaxList.clear();

  auto xmap0_orig = mXmap0;
  for ( auto polconf: polconf_list ) {
    // まずpolconfの内容を func1 に適用する．
    auto map2 = polconf.to_npnmap(ni0);
    auto func2 = func1.xform(map2);
    mXmap0 = xmap0_orig * map2;

    if ( debug ) {
      std::cout << std::endl
		<< "current PolConf: ";
      print_polconf(std::cout, polconf, ni0);
      std::cout << std::endl
		<< " map(polconf) = " << map2 << std::endl;
      if ( ni0 < 10 ) {
	std::cout << " func2 = " << func2 << std::endl;
      }
    }

    // この極性割り当てのもとで重み別 walsh_1 が最大になる変数順のみを試す．
    IgPartition igpart1(igpart);
    walsh_w1_refine(func2, igpart1);

    // あとは全ての順番を試す．
    std::vector<bool> w1_mark(ni0, false);
    tvmax_recur(func2, igpart1, w1_mark);
  }

  if ( debug ) {
    if ( ni0 < 10 ) {
      std::cout << "  final result" << std::endl
		<< "  cfunc = " << mMaxFunc << std::endl
		<< "  cmap  = " << mMaxList[0] << std::endl;
    }
  }
  return mMaxFunc;
}

void
NpnMgr::tvmax_recur(
  const TvFunc& func,
  const IgPartition& igpart,
  std::vector<bool>& w1_mark
)
{
  if ( debug ) {
    std::cout << std::endl
	      << "tvmax_recur(" << igpart << ")" << std::endl;
  }

  if ( igpart.is_resolved() ) {
    // 全ての入力順が解決している．
    ++ mTvmax_count;
    auto map = igpart.to_npnmap();
    auto func1 = func.xform(map);
    if ( debug ) {
      if ( func.input_num() < 10 ) {
	std::cout << "func1 = " << func1 << std::endl
		  << "map   = " << map << std::endl;
      }
    }
    // 真理値表が最大のものを記録する．
    if ( mMaxFunc <= func1 ) {
      if ( mMaxFunc < func1 ) {
	mMaxFunc = func1;
	mMaxList.clear();
	if ( debug ) {
	  if ( func.input_num() < 10 ) {
	    std::cout << "mMaxFunc = " << func1 << std::endl;
	  }
	  else {
	    std::cout << "mMaxFunc updated" << std::endl;
	  }
	}
      }
      add_map(map);
      if ( debug ) {
	std::cout << "map      += " << map << std::endl;
      }
    }
  }
  else {
    int pid = 0;
    int iid = 0;
    for ( ; pid < igpart.partition_num(); ++ pid) {
      if ( igpart.is_resolved(pid) ) {
	// 解決している分割を使って残りの分割を細分化する．
	auto pos = igpart.partition_begin(pid);
	auto gid = igpart.group_id(pos);
	iid = igpart.input_id(gid);
	if ( w1_mark[iid] ) {
	  // すでに処理済み
	  continue;
	}
	else {
	  // この変数で細分化する．
	  break;
	}
      }
      else {
	// 解決していない分割を見つけた．
	break;
      }
    }
    if ( igpart.is_resolved(pid) ) {
      // この分割の変数との Walsh_2 係数で細分化する．
      w1_mark[iid] = true;
      IgPartition igpart1(igpart);
      walsh_2_refine(func, iid, igpart1);

      tvmax_recur(func, igpart1, w1_mark);
      w1_mark[iid] = false;
    }
    else {
      // この分割は解決していないので一つづつを細分化して再帰する．
      for ( int pos = igpart.partition_begin(pid);
	    pos < igpart.partition_end(pid); ++ pos ) {
	auto gid = igpart.group_id(pos);
	auto iid = igpart.input_id(gid);
	IgPartition igpart1(igpart);
	igpart1._refine(pid, pos);
	walsh_2_refine(func, iid, igpart1);

	tvmax_recur(func, igpart1, w1_mark);
      }
    }
  }
}

// @brief 直前の cannonical の呼び出しにおける NpnMap の全候補を返す．
void
NpnMgr::all_cmap(
  std::vector<NpnMap>& map_list
) const
{
  map_list.clear();
  map_list.reserve(mMaxList.size());
  for ( auto map: mMaxList ) {
    map_list.push_back(map);
  }
}

// @brief 直前の cannonical の呼び出しにおける w2max_recur の起動回数を返す．
std::uint64_t
NpnMgr::tvmax_count() const
{
  return mTvmax_count;
}

void
NpnMgr::add_map(
  const NpnMap& map
)
{
  mMaxList.push_back(mXmap0 * map);
}

END_NAMESPACE_YM_LOGIC
