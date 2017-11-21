
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

extern
void
walsh_w1_refine(const TvFunc& func,
		VarId var,
		vector<PolConf>& polconf_list);

extern
void
walsh_w1_refine(const TvFunc& func,
		IgPartition& igpart);

extern
void
walsh_2_refine(const TvFunc& func,
	       VarId var,
	       IgPartition& igpart);


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
NpnMgr::cannonical(const TvFunc& func)
{
  mMaxList.clear();
  mW2max_count = 0;
  mTvmax_count = 0;

  mOrigFunc = func;

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

  // 極性割り当ての候補を確定した入力の重み別 Walsh_1 でフィルタリングする．
  for (ymuint pid = 0;
       polconf_list.size() > 1 && pid < igpart.partition_num();
       ++ pid) {
    if ( igpart.is_resolved(pid) ) {
      ymuint pos = igpart.partition_begin(pid);
      ymuint gid = igpart.group_id(pos);
      ymuint iid = igpart.input_id(gid);
      walsh_w1_refine(mBaseFunc, VarId(iid), polconf_list);
    }
  }

  if ( debug ) {
    cout << "  # of polarity candidates: " << polconf_list.size() << endl;
    for (ymuint i = 0; i < polconf_list.size(); ++ i) {
      cout << "    ";
      print_polconf(cout, polconf_list[i], ni0);
    }
    cout << "    Partition: " << igpart << endl
	 << endl;
  }

  // 残りはすべて展開して真理値ベクタが最大となるものを探す．
  mMaxFunc = TvFunc::const_zero(ni0);
  mMaxList.clear();

  for (vector<PolConf>::const_iterator p = polconf_list.begin();
       p != polconf_list.end(); ++ p) {
    const PolConf& polconf = *p;

    // まずpolconfの内容を mBaseFunc に適用する．
    NpnMap map1 = polconf.to_npnmap(ni0);
    TvFunc func1 = mBaseFunc.xform(map1);

    if ( debug ) {
      cout << endl;
      cout << "current PolConf: ";
      print_polconf(cout, polconf, ni0);
      cout << endl
	   << " map(polconf) = " << map1 << endl
	   << " func1 = " << func1 << endl;
    }

    // この極性割り当てのもとで重み別 walsh_1 が最大になる変数順のみを試す．
    IgPartition igpart1(igpart);
    walsh_w1_refine(func1, igpart1);

    // 入力順の確定したグループを前に持ってくる．
    igpart1.reorder();

    // 入力順の確定している入力との Walsh_2 で細分化する．
    for (ymuint pid = 0; pid < igpart1.partition_num(); ++ pid) {
      if ( igpart1.is_resolved() ) {
	break;
      }
      if ( igpart1.is_resolved(pid) ) {
	ymuint pos = igpart1.partition_begin(pid);
	ymuint gid = igpart1.group_id(pos);
	ymuint iid = igpart1.input_id(gid);
	walsh_2_refine(func1, VarId(iid), igpart1);
	// igpart1 の細分化は pid より後ろで起こるので pid は修正の必要はない．
      }
    }

    tvmax_recur(func1, map1, igpart1, 0);
  }

  if ( debug ) {
    cout << "  final result" << endl
	 << "  cfunc = " << mMaxFunc << endl
	 << "  cmap  = " << mMaxList[0] << endl;
  }
  return mMaxFunc;
}

void
NpnMgr::tvmax_recur(const TvFunc& func,
		    const NpnMap& map1,
		    const IgPartition& igpart,
		    ymuint pid)
{
  if ( debug ) {
    cout << "tvmax_recur(" << igpart << ", pid = " << pid << ")" << endl;
  }

  if ( igpart.is_resolved() ) {
    ++ mTvmax_count;
    NpnMap map = igpart.to_npnmap();
    TvFunc func1 = func.xform(map);
    if ( debug ) {
      cout << "func1 = " << func1 << endl
	   << "map   = " << map << endl;
    }
    if ( mMaxFunc <= func1 ) {
      if ( mMaxFunc < func1 ) {
	mMaxFunc = func1;
	mMaxList.clear();
      }
      add_map(map1 * map);
      if ( debug ) {
	cout << "mMaxFunc = " << func1 << endl
	     << "map      = " << map << endl;
      }
    }
  }
  else if ( igpart.is_resolved(pid) ) {
    tvmax_recur(func, map1, igpart, pid + 1);
  }
  else {
    for (ymuint pos = igpart.partition_begin(pid);
	 pos < igpart.partition_end(pid); ++ pos) {
      ymuint gid = igpart.group_id(pos);
      ymuint iid = igpart.input_id(gid);
      IgPartition igpart1(igpart);
      igpart1._refine(pid, pos);
      walsh_2_refine(func, VarId(iid), igpart1);
      tvmax_recur(func, map1, igpart1, pid + 1);
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
NpnMgr::add_map(const NpnMap& map)
{
  mMaxList.push_back(mXmap0 * map);
}

END_NAMESPACE_YM_LOGIC
