
/// @file NpnMgr.cc
/// @brief NpnMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2017 Yusuke Matsunaga
/// All rights reserved.


#include "NpnMgr.h"
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
walsh01_normalize(const TvFunc& func,
		  NpnMap& xmap,
		  InputInfo& input_info);

extern
void
walsh_w0_refine(const TvFunc& func,
		vector<PolConf>& polconf_list);


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

// @brief 重み別 w1 を用いて極性を確定させる．
// @param[in] pos 位置番号
// @param[in] var 対象の変数
// @param[in] polconf_list 極性割当候補のリスト
void
NpnMgr::walsh_w1_refine(ymuint pos,
			VarId var,
			vector<PolConf>& polconf_list)
{
  ymuint ni = mBaseFunc.input_num();

  if ( debug & debug_ww1_refine ) {
    cout << "before walsh_w1_refine(" << pos << ")" << endl;
    print_polconf_list(cout, polconf_list, ni);
    cout << endl;
  }

  // 重み別 w0 係数を用いて極性の決定を行う．
  for (ymuint w = 0; w <= ni; ++ w) {
    int max_d0 = 0;
    ymuint wpos = 0;
    for (ymuint i = 0; i < polconf_list.size(); ++ i) {
      PolConf polconf = polconf_list[i];
      int d0 = mBaseFunc.walsh_w1(var, w, polconf.oinv(), polconf.iinv_bits());
      if ( debug & debug_ww1_refine ) {
	cout << " walsh_w1(" << var << ", " << w << ") = " << d0 << endl;
      }
      int stat = mMaxW1[pos][w] - d0;
      if ( stat <= 0 ) {
	if ( stat < 0 ) {
	  wpos = 0;
	  mMaxW1[pos][w] = d0;
	  clear_max(pos, w + 1);
	}
	polconf_list[wpos] = polconf;
	++ wpos;
      }
    }
    if ( wpos < polconf_list.size() ) {
      polconf_list.erase(polconf_list.begin() + wpos, polconf_list.end());
    }
  }

  if ( debug & debug_ww1_refine ) {
    cout << "after walsh_w1_refine()" << endl;
    print_polconf_list(cout, polconf_list, ni);
    cout << endl;
  }
}

// @brief func の正規化を行う．
TvFunc
NpnMgr::cannonical(const TvFunc& func)
{
  mMaxList.clear();
  mW2max_count = 0;
  mTvmax_count = 0;

  // まず独立な変数を取り除く
  mXmap0 = func.shrink_map();
  TvFunc func0 = func.xform(mXmap0);

  ymuint ni0 = func0.input_num();

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
    cout << "NpnMgr::cannonical: func0 = " << func0 << endl;
  }

  // Walsh の0次と1次の係数を用いて正規化する．
  NpnMap map1;
  InputInfo iinfo;
  bool opol_fixed = walsh01_normalize(func0, map1, iinfo);
  mXmap0 = mXmap0 * map1;

  // map1 に従って func0 を変換する．
  mBaseFunc = func0.xform(map1);

  // w1:group_num:bisym でグループを分割する．
  IgPartition igpart(iinfo);

  if ( debug ) {
    cout << "  after walsh01_normalize" << endl
	 << "    func1  = " << mBaseFunc << endl
	 << "    map1   = " << map1 << endl
	 << "    igpart = " << igpart << endl
	 << endl;
    }

  if ( opol_fixed && iinfo.polundet_num() == 0 && igpart.is_resolved() ) {
    // 解決した．
    NpnMap map = igpart.to_npnmap(PolConf());
    add_map(map);
    if ( debug ) {
      cout << "  resolved" << endl
	   << "  func2 = " << mBaseFunc.xform(map) << endl
	   << "  map2  = " << map << endl;
    }
    return mBaseFunc.xform(map);
  }

  // 入力の等価グループは iinfo に入っている．
  // 以降は等価グループごとに極性と順序を考えればよい．
  // 極性に関しては
  // - 通常の対称グループ: そのままか全部反転
  // - bi-symmetry の対称グループ: そのままか先頭だけ反転
  // となる．

  // polundet_num の指数乗だけ極性の割り当てがある．
  //
  // 改良のヒント: ここで一旦 polconf_list を作ってから
  // walsh_w0_refine でフィルタリングするのではなく，
  // walsh_w0_refine で polconf_list を作ったほうが良いかもしれない．
  ymuint nug = iinfo.polundet_num();
  ymuint nug_exp = 1U << nug;
  ymuint n = nug_exp;
  if ( !opol_fixed ) {
    n *= 2;
  }
  vector<PolConf> polconf_list(n);
  ymuint input_mask = 0U;
  for (ymuint i = 0; i < ni0; ++ i) {
    input_mask |= (1U << i);
  }
  for (ymuint p = 0; p < nug_exp; ++ p) {
    ymuint input_bits = 0U;
    for (ymuint i = 0; i < nug; ++ i) {
      ymuint gid = iinfo.polundet_gid(i);
      if ( p & (1U << i) ) {
	input_bits |= iinfo.inv_bits(gid);
      }
    }
    if ( opol_fixed ) {
      polconf_list[p] = PolConf(false, input_bits);
    }
    else {
      polconf_list[p * 2 + 0] = PolConf(false, input_bits);
      polconf_list[p * 2 + 1] = PolConf(true , input_bits ^ input_mask);
    }
  }

  // 極性割り当ての候補を重み別 Walsh_0 でフィルタリングする．
  walsh_w0_refine(mBaseFunc, polconf_list);

  if ( debug ) {
    cout << "  # of polarity candidates: " << polconf_list.size() << endl;
    for (ymuint i = 0; i < polconf_list.size(); ++ i) {
      cout << "    ";
      print_polconf(cout, polconf_list[i], ni0);
    }
    cout << "    Partition: " << igpart << endl
	 << endl;
  }

  clear_max(0, 0);

  // 残りはすべて展開して真理値ベクタが最大となるものを探す．
  mMaxList.clear();
  tvmax_recur(igpart, 0, polconf_list);

  if ( debug ) {
    cout << "  final result" << endl
	 << "  cfunc = " << mMaxFunc << endl
	 << "  cmap  = " << mMaxList[0] << endl;
  }
  return mMaxFunc;
}

void
NpnMgr::tvmax_recur(const IgPartition& igpart,
		    ymuint pid,
		    const vector<PolConf>& polconf_list)
{
  if ( debug ) {
    cout << "tvmax_recur(" << igpart << ", pid = " << pid << ")" << endl;
    print_polconf_list(cout, polconf_list, mBaseFunc.input_num());
    cout << endl;
  }
  if ( igpart.is_resolved() ) {
    for (vector<PolConf>::const_iterator p = polconf_list.begin();
	 p != polconf_list.end(); ++ p) {
      const PolConf& polconf = *p;
      NpnMap map = igpart.to_npnmap(polconf);
      TvFunc func1 = mBaseFunc.xform(map);
      if ( mMaxFunc < func1 ) {
	mMaxFunc = func1;
	mMaxList.clear();
	add_map(map);
	if ( debug ) {
	  cout << "mMaxFunc = " << func1 << endl
	       << "map      = " << map << endl;
	}
      }
      else if ( mMaxFunc == func1 ) {
	add_map(map);
	if ( debug ) {
	  cout << "mMaxFunc = " << func1 << endl
	       << "map      += " << map << endl;
	}
      }
    }
  }
  else {
    for (ymuint pos = igpart.partition_begin(pid);
	 pos < igpart.partition_end(pid); ++ pos) {
      ymuint gid = igpart.group_id(pos);
      ymuint iid = igpart.input_id(gid);
      vector<PolConf> polconf_list1(polconf_list);
      walsh_w1_refine(pid, VarId(iid), polconf_list1);
      if ( !polconf_list1.empty() ) {
	if ( igpart.is_resolved(pid) ) {
	  tvmax_recur(igpart, pid + 1, polconf_list1);
	}
	else {
	  IgPartition igpart1(igpart);
	  igpart1._refine(pid, pos);
	  tvmax_recur(igpart1, pid + 1, polconf_list1);
	}
      }
    }
  }
}

// @brief 正規化マップの先頭を返す．
NpnMap
NpnMgr::cmap() const
{
  ASSERT_COND( !mMaxList.empty() );
  return mMaxList.front();
}

// @brief 直前の cannonical の呼び出しにおける NpnMap の全候補を返す．
void
NpnMgr::all_cmap(vector<NpnMap>& map_list) const
{
  map_list.clear();
  map_list.reserve(mMaxList.size());
  for (vector<NpnMap>::const_iterator p = mMaxList.begin();
       p != mMaxList.end(); ++ p) {
    const NpnMap& map = *p;
    map_list.push_back(map);
  }
}

// @brief 直前の cannonical の呼び出しにおける w2max_recur の起動回数を返す．
ymulong
NpnMgr::w2max_count() const
{
  return mW2max_count;
}

// @brief 直前の cannonical の呼び出しにおける w2max_recur の起動回数を返す．
ymulong
NpnMgr::tvmax_count() const
{
  return mTvmax_count;
}

void
NpnMgr::clear_max(ymuint pos,
		  ymuint w)
{
  if ( debug ) {
    cout << "clear_max(" << pos << ", " << w << ")" << endl;
  }
  ymuint ni = mBaseFunc.input_num();
  mMaxFunc = TvFunc::const_zero(ni);
  for (ymuint i = pos; i < ni; ++ i) {
    ymuint w0 = (i == pos) ? w : 0;
    for (ymuint j = w0; j <= ni; ++ j) {
      mMaxW1[i][j] = -INT_MAX / 2;
    }
  }
}

void
NpnMgr::add_map(const NpnMap& map)
{
  mMaxList.push_back(mXmap0 * map);
}

END_NAMESPACE_YM_LOGIC
