
/// @file NpnMgr.cc
/// @brief NpnMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2017 Yusuke Matsunaga
/// All rights reserved.


#include "NpnMgr.h"
#include "ym/TvFunc.h"
#include "InputInfo.h"
#include "IgPartition.h"
#include "PolConf.h"
#include "NpnConf.h"

BEGIN_NONAMESPACE

const int debug_ww0_refine  = 0x0001;
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

END_NONAMESPACE


BEGIN_NAMESPACE_YM_LOGIC

//////////////////////////////////////////////////////////////////////
// W1:cnum:bisym を用いた比較関数
//////////////////////////////////////////////////////////////////////
class W1CnumCmp
{
public:

  // コンストラクタ
  W1CnumCmp(const InputInfo& info) :
    mInfo(info)
  {
  }


  // 大小比較関数
  bool
  gt(ymuint pos1,
     ymuint pos2)
  {
    return mInfo.w1gt(pos1, pos2);
  }

  // 等価比較関数
  bool
  eq(ymuint pos1,
     ymuint pos2)
  {
    return mInfo.w1eq(pos1, pos2);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  const InputInfo& mInfo;

};


//////////////////////////////////////////////////////////////////////
// W2 を用いた大小比較関数
//////////////////////////////////////////////////////////////////////
class W2Cmp
{
public:

  // コンストラクタ
  W2Cmp(const NpnConf& conf,
	ymuint pos0) :
    mConf(conf),
    mPos0(pos0)
  {
  }


  // 大小比較関数
  bool
  gt(ymuint pos1,
     ymuint pos2)
  {
    int v1 = mConf.walsh_2(mPos0, pos1);
    int v2 = mConf.walsh_2(mPos0, pos2);
    return v1 >= v2;
  }

  // 等価比較関数
  bool
  eq(ymuint pos1,
     ymuint pos2)
  {
    int v1 = mConf.walsh_2(mPos0, pos1);
    int v2 = mConf.walsh_2(mPos0, pos2);
    return v1 == v2;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  const NpnConf& mConf;

  ymuint32 mPos0;

};


//////////////////////////////////////////////////////////////////////
// クラス NpnMgr
//////////////////////////////////////////////////////////////////////

// コンストラクタ
NpnMgr::NpnMgr() :
  mAlloc(sizeof(NpnConf), 1024)
{
}

// デストラクタ
NpnMgr::~NpnMgr()
{
}


BEGIN_NONAMESPACE

#if 0
// @brief 重み別 w0 を用いて極性を確定させる．
void
ww0_refine(vector<NpnConf>& pollist)
{
  const NpnConf& conf0 = pollist.front();
  ymuint ni = conf0.input_num();

  if ( debug & debug_ww0_refine ) {
    cout << "before ww0_refine()" << endl;
    for (ymuint i = 0; i < pollist.size(); ++ i) {
      const NpnConf& conf = pollist[i];
      conf.print(cout);
    }
    cout << endl;
  }

  // 重み別 w0 係数を用いて極性の決定を行う．
  ymuint w = 0;
  for (w = 0; w <= ni && pollist.size() > 1; ++ w) {
    bool first = true;
    int max_d0 = 0;
    ymuint wpos = 0;
    for (ymuint i = 0; i < pollist.size(); ++ i) {
      const NpnConf& conf = pollist[i];
      int d0 = conf.walsh_w0(w);

      int stat = -1;
      if ( first ) {
	first = false;
      }
      else {
	stat = max_d0 - d0;
      }
      if ( stat <= 0 ) {
	if ( stat < 0 ) {
	  wpos = 0;
	  max_d0 = d0;
	}
	pollist[wpos] = conf;
	++ wpos;
      }
    }
    if ( wpos < pollist.size() ) {
      pollist.erase(pollist.begin() + wpos, pollist.end());
    }
  }

  if ( debug & debug_ww0_refine ) {
    cout << "after ww0_refine()" << endl;
    for (ymuint i = 0; i < pollist.size(); ++ i) {
      const NpnConf& conf = pollist[i];
      conf.print(cout);
    }
    cout << endl;
  }
}
#endif

void
print_polconf(const PolConf& polconf,
	      ymuint ni)
{
  ymuint inv_bits = polconf.iinv_bits();
  for (ymuint i = 0; i < ni; ++ i) {
    if ( inv_bits & (1U << i) ) {
      cout << "N";
    }
    else {
      cout << "P";
    }
  }
  cout << "|";
  if ( polconf.oinv() ) {
    cout << "N";
  }
  else {
    cout << "P";
  }
  cout << endl;
}

END_NONAMESPACE


// @brief Walsh の0次/1次係数を用いた正規化を行う．
// @param[in] func 対象の論理関数
// @param[out] xmap 変換マップ
// @param[out] input_info 入力グループの情報
// @return 出力極性が決まっていたら true を返す．
bool
NpnMgr::walsh01_normalize(const TvFunc& func,
			  NpnMap& xmap,
			  InputInfo& input_info)
{
  // Walsh の0次と1次の係数を計算する．
  int w0;
  int w1[TvFunc::kMaxNi];
  w0 = func.walsh_01(w1);

  ymuint ni = func.input_num();

  xmap.clear();
  xmap.resize(ni);

  bool opol_fixed = false;

  // w0 が非負になるように出力極性を決める．
  if ( w0 < 0 ) {
    xmap.set_oinv(true);
    opol_fixed = true;
    w0 = -w0;
    // w1 も反転させる．
    for (ymuint i = 0; i < ni; ++ i) {
      w1[i] = -w1[i];
    }
  }
  else if ( w0 > 0 ) {
    // そのままの極性で固定する．
    xmap.set_oinv(false);
    opol_fixed = true;
  }
  else {
    // とりあえずそのままの極性で固定する．
    xmap.set_oinv(false);
  }

  // w1 に従って入力極性の調整を行う．
  for (ymuint i = 0; i < ni; ++ i) {
    VarId var(i);
    if ( w1[i] < 0 ) {
      // 反転させる．
      xmap.set(var, var, true);
      w1[i] = -w1[i];
    }
    else if ( w1[i] > 0 ) {
      // そのままの極性で固定する．
      xmap.set(var, var, false);
    }
    else { // w1[i] == 0
      // 独立な変数かどうか調べる．
      if ( func.check_sup(var) ) {
	// とりあえずそのままの極性で固定する．
	xmap.set(var, var, false);
      }
      else {
	// 独立だった．
	;
      }
    }
  }

  // xmap に従って関数を変換する．
  TvFunc func0 = func.xform(xmap);

  // 等価な入力グループを探す．
  // 副産物として入力の極性が決まる場合がある．
  input_info.clear();
  for (ymuint i = 0; i < ni; ++ i) {
    bool found = false;
    VarId var(i);
    for (ymuint gid = 0; gid < input_info.group_num(); ++ gid) {
      if ( w1[i] != input_info.w1(gid) ) {
	// w1 の値が異なる．
	continue;
      }

      // 対称性のチェックを行う．
      ymuint pos1 = input_info.elem(gid, 0);
      VarId var1(pos1);
      bool stat1 = func0.check_sym(var, var1, false);
      if ( stat1 ) {
	// 対称だった．
	found = true;
	if ( w1[pos1] == 0 && input_info.elem_num(gid) == 1 ) {
	  // 係数が0で最初の等価対の場合には bi-simmetry のチェックを行う．
	  bool stat2 = func0.check_sym(var, var1, true);
	  if ( stat2 ) {
	    // bi-symmetry の印を付けておく．
	    input_info.set_bisym(gid);
	  }
	}
	// 要素を追加しておく．
	input_info.add_elem(gid, i);
	break;
      }
      else if ( w1[pos1] == 0 ) {
	// 逆極性で対称の場合もあるのでチェックしておく．
	bool stat3 = func0.check_sym(var, var1, true);
	if ( stat3 ) {
	  // 逆相で対称だった．
	  found = true;
	  input_info.add_elem(gid, i);
	  xmap.set(var, var, true);
	  break;
	}
      }
    }
    if ( !found ) {
      // 新しい等価グループを作る．
      input_info.new_group(i, w1[i]);
    }
  }

  return opol_fixed;
}

// @brief 重み別 w0 を用いて極性を確定させる．
// @param[in] polconf_list 極性割当候補のリスト
// @param[in] func 対象の関数
void
NpnMgr::walsh_w0_refine(vector<PolConf>& polconf_list,
			const TvFunc& func)
{
  ymuint ni = func.input_num();

  if ( debug & debug_ww0_refine ) {
    cout << "before walsh_w0_refine()" << endl;
    for (ymuint i = 0; i < polconf_list.size(); ++ i) {
      print_polconf(polconf_list[i], ni);
    }
    cout << endl;
  }

  // 重み別 w0 係数を用いて極性の決定を行う．
  ymuint w = 0;
  for (w = 0; w <= ni && polconf_list.size() > 1; ++ w) {
    bool first = true;
    int max_d0 = 0;
    ymuint wpos = 0;
    for (ymuint i = 0; i < polconf_list.size(); ++ i) {
      PolConf polconf = polconf_list[i];
      int d0 = func.walsh_w0(w, polconf.oinv(), polconf.iinv_bits());

      int stat = -1;
      if ( first ) {
	first = false;
      }
      else {
	stat = max_d0 - d0;
      }
      if ( stat <= 0 ) {
	if ( stat < 0 ) {
	  wpos = 0;
	  max_d0 = d0;
	}
	polconf_list[wpos] = polconf;
	++ wpos;
      }
    }
    if ( wpos < polconf_list.size() ) {
      polconf_list.erase(polconf_list.begin() + wpos, polconf_list.end());
    }
  }

  if ( debug & debug_ww0_refine ) {
    cout << "after walsh_w0_refine()" << endl;
    for (ymuint i = 0; i < polconf_list.size(); ++ i) {
      print_polconf(polconf_list[i], ni);
    }
    cout << endl;
  }
}

// @brief 重み別 w1 を用いて極性を確定させる．
// @param[in] polconf_list 極性割当候補のリスト
// @param[in] func 対象の関数
// @param[in] var 対象の変数
void
NpnMgr::walsh_w1_refine(vector<PolConf>& polconf_list,
			const TvFunc& func,
			VarId var)
{
  ymuint ni = func.input_num();

  if ( debug & debug_ww0_refine ) {
    cout << "before walsh_w1_refine()" << endl;
    for (ymuint i = 0; i < polconf_list.size(); ++ i) {
      print_polconf(polconf_list[i], ni);
    }
    cout << endl;
  }

  // 重み別 w0 係数を用いて極性の決定を行う．
  ymuint w = 0;
  for (w = 0; w <= ni && polconf_list.size() > 1; ++ w) {
    bool first = true;
    int max_d0 = 0;
    ymuint wpos = 0;
    for (ymuint i = 0; i < polconf_list.size(); ++ i) {
      PolConf polconf = polconf_list[i];
      int d0 = func.walsh_w1(var, w, polconf.oinv(), polconf.iinv_bits());

      int stat = -1;
      if ( first ) {
	first = false;
      }
      else {
	stat = max_d0 - d0;
      }
      if ( stat <= 0 ) {
	if ( stat < 0 ) {
	  wpos = 0;
	  max_d0 = d0;
	}
	polconf_list[wpos] = polconf;
	++ wpos;
      }
    }
    if ( wpos < polconf_list.size() ) {
      polconf_list.erase(polconf_list.begin() + wpos, polconf_list.end());
    }
  }

  if ( debug & debug_ww0_refine ) {
    cout << "after walsh_w1_refine()" << endl;
    for (ymuint i = 0; i < polconf_list.size(); ++ i) {
      print_polconf(polconf_list[i], ni);
    }
    cout << endl;
  }
}

#if 0
// @brief 重み別 w1 を用いて極性を確定させる．
// @param[in] polconf_list 極性割当候補のリスト
// @param[in] func 対象の関数
// @param[in] var_list 対象の変数のリスト
void
NpnMgr::walsh_w1_refine(vector<PolConf>& polconf_list,
			const TvFunc& func,
			const vector<VarId>& var_list)
{
  ymuint nv = var_list.size();
  vector<PolConf> tmp_polconf_list(nv);
  for (ymuint i = 0; i < nv; ++ i) {
    // 個々の変数ごとに walsh_w1_refine() を適用してみる．
    tmp_polconf_list[i] = polconf_list[i];
    walsh_w1_refine(tmp_polconf_list, func, var_list[i]);
  }
  // その中で重み別 w1 が最大のものを求める．
  ymuint ni = func.input_num();
  vector<int> max_ww1(ni + 1);
  {
    VarId var = var_list[0];
    const PolConf& polconf = tmp_polconf_list[0][0];
    for (ymuint w = 0; w <= ni; ++ w) {
      max_ww1[w] = func.walsh_w1(var, w, polconf.oinv(), polconf.iinv_bits());
    }
  }
  vector<ymuint> max_pos_list;
  max_pos_list.push_back(0);
  for (ymuint i = 1; i < nv; ++ i) {
    VarId var = var_list[i];
    const PolConf& polconf = tmp_polconf_list[i][0];
    vector<int> tmp_ww1(ni + 1);
    for (ymuint w = 0; w <= ni; ++ w) {
      int tmp_ww1 = func.walsh_w1(var, w, polconf.oinv(), polconf.iinv_bits());
      int diff = max_ww1[w] - tmp_ww1;
      if ( diff < 0 ) {
	max_ww1[w] = tmp_ww1;
	max_pos_list.clear();
      }
      if ( diff <= 0 ) {
	max_pos_list.push_back(i);
      }
    }
  }

  ymuint ni = func.input_num();

  if ( debug & debug_ww0_refine ) {
    cout << "before walsh_w1_refine()" << endl;
    for (ymuint i = 0; i < polconf_list.size(); ++ i) {
      print_polconf(polconf_list[i], ni);
    }
    cout << endl;
  }

  // 重み別 w0 係数を用いて極性の決定を行う．
  ymuint w = 0;
  for (w = 0; w <= ni && polconf_list.size() > 1; ++ w) {
    bool first = true;
    int max_d0 = 0;
    ymuint wpos = 0;
    for (ymuint i = 0; i < polconf_list.size(); ++ i) {
      PolConf polconf = polconf_list[i];
      int d0 = func.walsh_w1(var, w, polconf.oinv(), polconf.iinv_bits());

      int stat = -1;
      if ( first ) {
	first = false;
      }
      else {
	stat = max_d0 - d0;
      }
      if ( stat <= 0 ) {
	if ( stat < 0 ) {
	  wpos = 0;
	  max_d0 = d0;
	}
	polconf_list[wpos] = polconf;
	++ wpos;
      }
    }
    if ( wpos < polconf_list.size() ) {
      polconf_list.erase(polconf_list.begin() + wpos, polconf_list.end());
    }
  }

  if ( debug & debug_ww0_refine ) {
    cout << "after walsh_w1_refine()" << endl;
    for (ymuint i = 0; i < polconf_list.size(); ++ i) {
      print_polconf(polconf_list[i], ni);
    }
    cout << endl;
  }
}
#endif

// @brief func の正規化を行う．
void
NpnMgr::cannonical(const TvFunc& func,
		   int algorithm)
{
  mMaxList.clear();
  mW2max_count = 0;
  mTvmax_count = 0;
  mMaxW2Valid = false;

  ymuint ni = func.input_num();

  // 特例
  if ( ni == 0 ) {
    if ( func.value(0) == 0 ) {
      // 定数0関数
      mMaxList.push_back(NpnMap(0));
    }
    else {
      // 定数1関数
      mMaxList.push_back(NpnMap(0, true));
    }
    return;
  }
  if ( ni == 1 ) {
    if ( func.value(0) == 0 ) {
      if ( func.value(1) == 0 ) {
	// 1入力の定数0関数
	mMaxList.push_back(NpnMap(1));
      }
      else {
	// 肯定のリテラル関数
	mMaxList.push_back(NpnMap(1));
      }
    }
    else {
      if ( func.value(1) == 0 ) {
	// 否定のリテラル関数
	mMaxList.push_back(NpnMap(1, true));
      }
      else {
	// 1入力の定数1関数
	mMaxList.push_back(NpnMap(1, true));
      }
    }
    return;
  }

  // Walsh の0次と1次の係数を用いて正規化する．
  NpnMap xmap0;
  InputInfo input_info;
  bool opol_fixed = walsh01_normalize(func, xmap0, input_info);

  // w1:group_num:bisym でグループを分割する．
  IgPartition igpart(input_info);

  if ( opol_fixed && input_info.polundet_num() == 0 && igpart.is_resolved() ) {
    // 解決した．
    mMaxList.push_back(xmap0);
    return;
  }

  // 入力の等価グループは input_info に入っている．
  // 以降は等価グループごとに極性と順序を考えればよい．
  // 極性に関しては
  // - 通常の対称グループ: そのままか全部反転
  // - bi-symmetry の対称グループ: そのままか先頭だけ反転
  // となる．

  // polundet_num の指数乗だけ極性の割り当てがある．
  ymuint nug = input_info.polundet_num();
  ymuint nug_exp = 1U << nug;
  ymuint n = nug_exp;
  if ( !opol_fixed ) {
    n *= 2;
  }
  vector<PolConf> polconf_list(n);
  for (ymuint p = 0; p < nug_exp; ++ p) {
    ymuint input_bits = 0U;
    for (ymuint i = 0; i < nug; ++ i) {
      ymuint gid = input_info.polundet_gid(i);
      if ( p & (1U << i) ) {
	input_bits |= input_info.inv_bits(gid);
      }
    }
    if ( opol_fixed ) {
      polconf_list[p] = PolConf(false, input_bits);
    }
    else {
      polconf_list[p * 2 + 0] = PolConf(false, input_bits);
      polconf_list[p * 2 + 1] = PolConf(true, input_bits);
    }
  }

  // xmap0 に従って func を変換する．
  mBaseFunc = func.xform(xmap0);

  // 極性割り当ての候補を重み別 Walsh_0 でフィルタリングする．
  walsh_w0_refine(polconf_list, mBaseFunc);

  if ( debug ) {
    cout << "# of polarity candidates: " << polconf_list.size() << endl;
    cout << "Partition: ";
    for (ymuint pid = 0; pid < igpart.partition_num(); ++ pid) {
      cout << " " << igpart.partition_size(pid);
    }
    cout << endl;
  }

  // 順序の確定している変数を用いて重み別 Walsh_1 でフィルタリングする．
  vector<ymuint> pid_list;
  for (ymuint pid = 0; pid < igpart.partition_num(); ++ pid) {
    ymuint n = igpart.partition_size(pid);
    if ( n == 1 ) {
      // この入力は確定している．
      ymuint pos = igpart.partition_begin(pid);
      ymuint gid = igpart.group_id(pos);
      ymuint iid = input_info.elem(gid, 0);
      walsh_w1_refine(polconf_list, mBaseFunc, VarId(iid));

      if ( debug ) {
	cout << "After walsh_w1_refine(" << VarId(iid) << ")" << endl;
	cout << "# of polarity candidates: " << polconf_list.size() << endl;
      }
    }
    else {
      // この分割は確定していない．
      pid_list.push_back(pid);
    }
  }

  // 残りはすべて展開して真理値ベクタが最大となるものを探す．
  mMaxFunc = TvFunc::const_zero(ni);
  mMaxList.clear();
  for (vector<PolConf>::iterator p = polconf_list.begin();
       p != polconf_list.end(); ++ p) {
    mPolConf = *p;
    tvmax_recur(igpart, 0);
  }


#if 0
  NpnConf conf0(base_conf);

  if ( !conf0.is_resolvoed() && conf0.nc() > 1 ) {
    if ( debug ) {
      cout << "Before step1" << endl;
      conf0.print(cout);
    }

    // W1:cnum:bisym を用いてグループわけを行う．
    conf0.refine(0, W1CnumCmp(base_conf));

    if ( debug ) {
      cout << "After step1" << endl;
      conf0.print(cout);
    }
  }

  if ( conf0.is_resolved() ) {
    mMaxList.push_back(conf0.conv_to_npnmap());
  }
  else {
    mMaxFunc = TvFunc::const_zero(func.input_num());

    switch ( algorithm ) {
    case 0:
      algorithm0(conf0);
      break;

    case 1:
      algorithm1(conf0);
      break;

    case 2:
      algorithm2(conf0);
      break;

    case 3:
      algorithm3(conf0);
      break;

    default:
      algorithm0(conf0);
      break;
    }
  }

  if ( debug ) {
    cout << "Final Result" << endl;
    cout << " algorithm:   " << algorithm << endl;
    cout << " w2max_count: " << mW2max_count << endl;
    cout << " tvmax_count: " << mTvmax_count << endl;
    for (vector<NpnMap>::iterator p = mMaxList.begin();
	 p != mMaxList.end(); ++ p) {
      const NpnMap& map = *p;
      TvFunc func1 = func.xform(map);
      cout << "map     : " << map << endl
	   << "rep func: " << func1 << endl
	   << endl;
    }
    cout << endl;
  }
#endif
}

void
NpnMgr::tvmax_recur(const IgPartition& igpart,
		    ymuint pid0)
{
  ymuint pid;
  for (pid = pid0; pid < igpart.partition_num(); ++ pid) {
    if ( igpart.partition_size(pid) > 1 ) {
      break;
    }
  }
  ASSERT_COND( pid < igpart.partition_num() );

  // pid の分割から要素を一つ取り出し，分離する．
  ymuint begin = igpart.partition_begin(pid);
  ymuint end = igpart.partition_end(pid);
  for (ymuint i = begin; i < end; ++ i) {
    IgPartition igpart1(igpart);
    igpart1._refine(pid, i);
    if ( igpart1.is_resolved() ) {
      NpnMap xmap = igpart1.to_npnmap(mBaseFunc.input_num(), mInputInfo, mPolConf);
      TvFunc func1 = mBaseFunc.xform(xmap);
      int diff = compare(mMaxFunc, func1);
      if ( diff < 0 ) {
	mMaxFunc = func1;
	mMaxList.clear();
      }
      if ( diff <= 0 ) {
	mMaxList.push_back(xmap);
      }
    }
    else {
      tvmax_recur(igpart1, pid + 1);
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

#if 0
// @brief DFS
void
NpnMgr::algorithm0(const NpnConf& conf)
{
  // 以降の処理は単純化するために出力極性を固定で考える．
  if ( conf.is_opol_fixed() ) {
    NpnConf conf1(conf);
    w2max_recur(conf1, 0);
  }
  else {
    // 出力極性が決まっていなければ両方の極性で考える．
    NpnConf conf_p(conf, 1);
    w2max_recur(conf_p, 0);

    NpnConf conf_n(conf, 2);
    w2max_recur(conf_n, 0);
  }
}

// @brief BFS
void
NpnMgr::algorithm1(const NpnConf& conf)
{
  vector<NpnConf> conf_list;
  if ( conf.is_opol_fixed() ) {
    conf_list.push_back(conf);
  }
  else {
    NpnConf conf_p(conf, 1);
    conf_list.push_back(conf_p);

    NpnConf conf_n(conf, 2);
    conf_list.push_back(conf_n);
  }
  w2max_recur(conf_list, 0);
}

// @brief 極性を展開した後で DFS
void
NpnMgr::algorithm2(const NpnConf& conf)
{
  // 最初に全ての極性を展開してしまう．
  vector<NpnConf> pollist;
  ymuint ulist[TvFunc::kMaxNi];
  ymuint unum = 0;
  for (ymuint c = 0; c < conf.nc(); ++ c) {
    if ( conf.ic_pol(c) == 0 ) {
      ulist[unum] = c;
      ++ unum;
    }
  }
  ymuint unum_pow = 1UL << unum;
  if ( conf.is_opol_fixed() ) {
    pollist.reserve(unum_pow);
    for (ymuint p = 0; p < unum_pow; ++ p) {
      NpnConf conf1(conf);
      for (ymuint i = 0; i < unum; ++ i) {
	ymuint c = ulist[i];
	if ( p & (1 << i) ) {
	  // c のクラスを反転させる．
	  conf1.set_ic_pol(c, 2);
	}
	else {
	  conf1.set_ic_pol(c, 1);
	}
      }
      pollist.push_back(conf1);
    }
  }
  else {
    pollist.reserve(unum_pow * 2);
    for (int opol = 1; opol <= 2; ++ opol) {
      for (ymuint p = 0; p < unum_pow; ++ p) {
	NpnConf conf1(conf, opol);
	for (ymuint i = 0; i < unum; ++ i) {
	  ymuint c = ulist[i];
	  if ( p & (1 << i) ) {
	    // c のクラスを反転させる．
	    conf1.set_ic_pol(c, 2);
	  }
	  else {
	    conf1.set_ic_pol(c, 1);
	  }
	}
	pollist.push_back(conf1);
      }
    }
  }
  // それを WW0 を用いて制限する．
  ww0_refine(pollist);
  for (ymuint i = 0; i < pollist.size(); ++ i) {
    NpnConf& conf = pollist[i];
    w2max_recur(conf, 0);
  }
}

// @brief 極性を展開した後で BFS
void
NpnMgr::algorithm3(const NpnConf& conf)
{
  // 最初に全ての極性を展開してしまう．
  vector<NpnConf> pollist;
  ymuint ulist[TvFunc::kMaxNi];
  ymuint unum = 0;
  for (ymuint c = 0; c < conf.nc(); ++ c) {
    if ( conf.ic_pol(c) == 0 ) {
      ulist[unum] = c;
      ++ unum;
    }
  }
  ymuint unum_pow = 1UL << unum;
  if ( conf.is_opol_fixed() ) {
    pollist.reserve(unum_pow);
    for (ymuint p = 0; p < unum_pow; ++ p) {
      NpnConf conf1(conf);
      for (ymuint i = 0; i < unum; ++ i) {
	ymuint c = ulist[i];
	if ( p & (1 << i) ) {
	  // c のクラスを反転させる．
	  conf1.set_ic_pol(c, 2);
	}
	else {
	  conf1.set_ic_pol(c, 1);
	}
      }
      pollist.push_back(conf1);
    }
  }
  else {
    pollist.reserve(unum_pow * 2);
    for (int opol = 1; opol <= 2; ++ opol) {
      for (ymuint p = 0; p < unum_pow; ++ p) {
	NpnConf conf1(conf, opol);
	for (ymuint i = 0; i < unum; ++ i) {
	  ymuint c = ulist[i];
	  if ( p & (1 << i) ) {
	    // c のクラスを反転させる．
	    conf1.set_ic_pol(c, 2);
	  }
	  else {
	    conf1.set_ic_pol(c, 1);
	  }
	}
	pollist.push_back(conf1);
      }
    }
  }
  // それを WW0 を用いて制限する．
  ww0_refine(pollist);
  w2max_recur(pollist, 0);
}


BEGIN_NONAMESPACE

// @brief 確定した入力との w2 を用いて細分化する．
void
w2refine(NpnConf& conf,
	 ymuint g0)
{
  // Walsh の 2次の係数を用いた細分化

  // 確定した入力をキューに積む．
  ymuint queue[TvFunc::kMaxNi];
  ymuint rp = 0;
  ymuint wp = 0;
  for (ymuint g = g0; g < conf.group_num(); ++ g) {
    ymuint s = conf.group_begin(g);
    ymuint e = conf.group_end(g);
    if ( e - s == 1 && conf.ic_pol(s) != 0 ) {
      queue[wp] = s;
      ++ wp;
    }
  }

  while ( wp > rp ) {
    ymuint s = queue[rp];
    ymuint pos0 = conf.ic_rep(s);
    ++ rp;

    ymuint c = 0;
    for (ymuint g1 = g0; g1 < conf.group_num(); ++ g1) {
      ymuint b = conf.group_begin(g1);
      ymuint e = conf.group_end(g1);
      if ( e - b == 1 ) {
	// 順序が確定している場合

	if ( conf.ic_pol(b) == 0 ) {
	  // ても極性は確定していない場合
	  ymuint pos1 = conf.ic_rep(b);
	  int w2_1 = conf.walsh_2(pos0, pos1);
	  if ( w2_1 < 0 ) {
	    conf.set_ic_pol(b, 2);
	    queue[wp] = b;
	    ++ wp;
	  }
	  else if ( w2_1 > 0 ) {
	    conf.set_ic_pol(b, 1);
	    queue[wp] = b;
	    ++ wp;
	  }
	  else {
	    // ここでは確定しなかった．
	    ++ c;
	  }
	}
      }
      else {
	// 順序が確定していない場合

	// 極性が確定していない場合には2次係数で極性を制限する．
	for (ymuint i = b; i < e; ++ i) {
	  if ( conf.ic_pol(i) == 0 ) {
	    ymuint pos1 = conf.ic_rep(i);
	    int w2 = conf.walsh_2(pos0, pos1);
	    if ( w2 > 0 ) {
	      conf.set_ic_pol(i, 1);
	    }
	    else if ( w2 < 0 ) {
	      conf.set_ic_pol(i, 2);
	    }
	  }
	}

	// 2次係数の降べきの順に並べ替える．
	ymuint d = conf.refine(g1, W2Cmp(conf, pos0));

	// 新しく増えたグループで確定したものがあるか調べる．
	for (ymuint j = g1; j < g1 + d; ++ j) {
	  ymuint s = conf.group_begin(j);
	  if ( conf.group_size(j) == 1 && conf.ic_pol(s) != 0 ) {
	    queue[wp] = s;
	    ++ wp;
	  }
	  else {
	    ++ c;
	  }
	}
	g1 += d;
      }
    }
    if ( c == 0 ) {
      break;
    }
  }
}

END_NONAMESPACE


// @brief シグネチャが最大になるように極性と順序を決める．
void
NpnMgr::w2max_recur(NpnConf& conf,
		    ymuint g0)
{
  if ( debug & debug_w2max_recur ) {
    cout << "w2max_recur(" << g0 << ")" << endl;
    conf.print(cout);
  }

  ++ mW2max_count;

  if ( !conf.is_resolved() ) {
    w2refine(conf, g0);
  }
  if ( conf.is_resolved() ) {
#if 0
    TvFunc func1;
    {
      NpnMap map1 = conf.conv_to_npnmap();
      func1 = conf.func().xform(map1);
    }
    cout << "func1 = " << func1 << endl
	 << "w2 = {";
    ymuint ni = conf.input_num();
    for (ymuint i = 1; i < ni; ++ i) {
      for (ymuint j = 0; j < i; ++ j) {
	int w2 = func1.walsh_2(i, j);
	cout << " " << w2;
      }
    }
    cout << "}" << endl;
#endif
    if ( mMaxList.empty() ) {
      NpnMap map1 = conf.conv_to_npnmap();
      mMaxList.push_back(map1);
      set_maxfunc(conf.func().xform(map1));
    }
    else {
      int diff = 0;

      ymuint ni = conf.input_num();
      if ( !mMaxW2Valid ) {
	for (ymuint i = 1; i < ni; ++ i) {
	  ymuint base = i * ni;
	  for (ymuint j = 0; j < i; ++ j) {
	    mMaxW2[base + j] = mMaxFunc.walsh_2(VarId(i), VarId(j));
	  }
	}
	mMaxW2Valid = true;
      }
      for (ymuint i = 1; i < ni; ++ i) {
	ymuint base = i * ni;
	for (ymuint j = 0; j < i; ++ j) {
	  int w2_1 = mMaxW2[base + j];
	  int w2_2 = conf.walsh_2i(i, j);
	  diff = w2_1 - w2_2;
	  if ( diff != 0 ) {
	    if ( diff < 0 ) {
	      NpnMap map1 = conf.conv_to_npnmap();
	      mTmpFunc = conf.func().xform(map1);
	    }
	    goto loop_exit;
	  }
	}
      }
    loop_exit:
      if ( diff == 0 ) {
	// ここまでで決着が付かなければ真理値表ベクタの辞書式順序で比較する．
	++ mTvmax_count;
	{
	  NpnMap map1 = conf.conv_to_npnmap();
	  mTmpFunc = conf.func().xform(map1);
	}
	if ( mMaxFunc < mTmpFunc ) {
	  diff = -1;
	}
	else if ( mMaxFunc > mTmpFunc ) {
	  diff = 1;
	}
      }
      if ( diff <= 0 ) {
	if ( diff < 0 ) {
	  // 最大値の更新
	  mMaxList.clear();
	  set_maxfunc(mTmpFunc);
	}
	mMaxList.push_back(conf.conv_to_npnmap());
      }
    }
    return;
  }

  // 確定している入力グループをスキップする．
  for ( ; g0 < conf.group_num(); ++ g0) {
    if ( conf.group_size(g0) > 1 ) {
      // g0 内に複数のクラスがあって順序は未確定
      ymuint b = conf.group_begin(g0);
      ymuint e = conf.group_end(g0);
      for (ymuint i = b; i < e; ++ i) {
	NpnConf conf_i(conf, g0, i);
	w2max_recur(conf_i, g0);
      }
      break;
    }
    ymuint b = conf.group_begin(g0);
    if ( conf.ic_pol(b) == 0 ) {
      // g0 の唯一のクラスの極性が未定
      NpnConf conf_p(conf);
      ymuint c = conf.group_begin(g0);
      conf_p.set_ic_pol(c, 1);
      w2max_recur(conf_p, g0);

      conf.set_ic_pol(c, 2);
      w2max_recur(conf, g0);
      break;
    }
  }
}

// @brief mMaxFunc を設定する．
void
NpnMgr::set_maxfunc(const TvFunc& func)
{
  mMaxFunc = func;
  mMaxW2Valid = false;
#if 0
  ymuint ni = func.input_num();
  for (ymuint i = 1; i < ni; ++ i) {
    ymuint base = i * ni;
    for (ymuint j = 0; j < i; ++ j) {
      mMaxW2[base + j] = func.walsh_2(i, j);
    }
  }
#endif
#if 0
  cout << "mMaxFunc = " << mMaxFunc << endl
       << "w2 = {";
  ymuint ni = conf.input_num();
  for (ymuint i = 1; i < ni; ++ i) {
    ymuint base = i * ni;
    for (ymuint j = 0; j < i; ++ j) {
      int w2 = mMaxW2[base + j];
      cout << " " << w2;
    }
  }
  cout << "}" << endl;
#endif
}


BEGIN_NONAMESPACE

void
w2refine2_sub(NpnConf& conf,
	      ymuint g0)
{
  if ( debug & debug_w2refine) {
    cout << "conf" << endl;
    conf.print(cout);
  }

  // 極性の確定を行う．
  ymuint c0 = conf.group_begin(g0);
  ymuint pos0 = conf.ic_rep(c0);
  ymuint cend = conf.group_end(conf.group_num() - 1);
  for (ymuint c = c0 + 1; c < cend; ++ c) {
    if ( conf.ic_pol(c) == 0 ) {
      int w2 = conf.walsh_2(pos0, conf.ic_rep(c));
      if ( w2 < 0 ) {
	conf.set_ic_pol(c, 2);
      }
      else if ( w2 > 0 ) {
	conf.set_ic_pol(c, 1);
      }
    }
  }
  // 並べ替え＆グループ化を行う．
  for (ymuint g = g0 + 1; g < conf.group_num(); ++ g) {
    ymuint delta = conf.refine(g, W2Cmp(conf, pos0));
    g += delta;
  }
  if ( debug & debug_w2refine) {
    cout << " =>" << endl;
    conf.print(cout);
    cout << endl;
  }
}

// @brief 確定した入力との w2 を用いて細分化する．
void
w2refine2(const NpnConf& conf,
	  ymuint g0,
	  list<NpnConf>& conf_list)
{
  // Walsh の 2次の係数を用いた細分化
  if ( debug & debug_w2refine ) {
    cout << endl
	 << "w2refine2(g0 = " << g0 << ")" << endl;
  }

  ASSERT_COND( conf.group_size(g0) == 1 );
  ymuint c0 = conf.group_begin(g0);
  if ( conf.ic_pol(c0) == 0 ) {
    // 極性が確定していない．
    NpnConf conf_p(conf);
    conf_p.set_ic_pol(c0, 1);
    w2refine2_sub(conf_p, g0);
    conf_list.push_back(conf_p);

    NpnConf conf_n(conf);
    conf_n.set_ic_pol(c0, 2);
    w2refine2_sub(conf_n, g0);
    conf_list.push_back(conf_n);
  }
  else {
    // 2次係数の降べきの順に並べ替える．
    NpnConf conf1(conf);
    w2refine2_sub(conf1, g0);
    conf_list.push_back(conf1);
  }
}

END_NONAMESPACE


// @brief シグネチャが最大になるように極性と順序を決める．
void
NpnMgr::w2max_recur(vector<NpnConf>& conf_list,
		    ymuint g0)
{
  ymuint cend;
  {
    ASSERT_COND( !conf_list.empty() );
    const NpnConf& conf = conf_list.front();
    cend = conf.group_end(conf.group_num() - 1);
  }

  while ( !conf_list.empty() ) {
    if ( debug & debug_w2max_recur ) {
      cout << endl
	   << "w2max_recur(g0 = " << g0 << ")" << endl;
      for (vector<NpnConf>::const_iterator p = conf_list.begin();
	   p != conf_list.end(); ++ p) {
	const NpnConf& conf = *p;
	conf.print(cout);
      }
      cout << endl;
    }

    ++ mW2max_count;

#if 0
    for (ymuint c1 = 0; c1 < cend; ++ c1) {
      mMaxW2[c1] = -INT_MAX;
    }
#endif
    bool first = true;
    list<NpnConf> conf_list1;
    for (vector<NpnConf>::const_iterator p = conf_list.begin();
	 p != conf_list.end(); ++ p) {
      const NpnConf& conf = *p;
      if ( conf.is_resolved() ) {
	++ mTvmax_count;

	// 真理値表ベクタの辞書式順序で比較する．
	NpnMap map1 = conf.conv_to_npnmap();
	mTmpFunc = conf.func().xform(map1);
	int diff = 1;
	if ( mMaxList.empty() ) {
	  diff = -1;
	}
	else {
	  if ( mMaxFunc < mTmpFunc ) {
	    diff = -1;
	  }
	  else if ( mMaxFunc == mTmpFunc ) {
	    diff = 0;
	  }
	}
	if ( diff <= 0 ) {
	  if ( diff < 0 ) {
	    // 最大値の更新
	    mMaxList.clear();
	    mMaxFunc = mTmpFunc;
	  }
	  mMaxList.push_back(conf.conv_to_npnmap());
	}
	continue;
      }

      list<NpnConf> tmp_list;
      if ( conf.group_size(g0) > 1 ) {
	ymuint b = conf.group_begin(g0);
	ymuint e = conf.group_end(g0);
	for (ymuint c0 = b; c0 < e; ++ c0) {
	  NpnConf conf1(conf, g0, c0);
	  w2refine2(conf1, g0, tmp_list);
	}
      }
      else {
	w2refine2(conf, g0, tmp_list);
      }

      for (list<NpnConf>::iterator p = tmp_list.begin();
	   p != tmp_list.end(); ++ p) {
	const NpnConf& conf = *p;
	ymuint c0 = conf.group_begin(g0);
	ymuint pos0 = conf.ic_rep(c0);
	ymuint pol0 = conf.ic_pol(c0);
	ASSERT_COND( pol0 != 0 );
	int tmp_w2[TvFunc::kMaxNi];
	for (ymuint c1 = c0 + 1; c1 < cend; ++ c1) {
	  ymuint pos1 = conf.ic_rep(c1);
	  tmp_w2[c1] = conf.walsh_2(pos0, pos1);
	}
	if ( debug & debug_w2max_recur ) {
	  cout << "tmp_w2 = ";
	  for (ymuint c1 = c0 + 1; c1 < cend; ++ c1) {
	    cout << " " << tmp_w2[c1];
	  }
	  cout << endl;
	}
	int diff = 0;
	if ( first ) {
	  first = false;
	  diff = -1;
	}
	else {
	  for (ymuint c1 = c0 + 1; c1 < cend; ++ c1) {
	    int diff1 = mMaxW2[c1] - tmp_w2[c1];
	    if ( diff1 != 0 ) {
	      diff = diff1;
	      break;
	    }
	  }
	}
	if ( diff <= 0 ) {
	  if ( diff < 0 ) {
	    conf_list1.clear();
	    for (ymuint c1 = c0 + 1; c1 < cend; ++ c1) {
	      mMaxW2[c1] = tmp_w2[c1];
	    }
	  }
	  conf_list1.push_back(conf);
	}
      }
    }

    if ( debug & debug_w2max_recur ) {
      cout << "conf_list1 = " << endl;
      for (list<NpnConf>::iterator p = conf_list1.begin();
	   p != conf_list1.end(); ++ p) {
	const NpnConf& conf = *p;
	conf.print(cout);
      }
      cout << endl;
    }

    conf_list.clear();
    for (list<NpnConf>::iterator p = conf_list1.begin();
	 p != conf_list1.end(); ++ p) {
      const NpnConf& conf = *p;
      conf_list.push_back(conf);
    }
    ++ g0;
  }
}
#endif

// @brief NpnConf を取り出す．
NpnConf*
NpnMgr::new_npnconf()
{
  void* p = mAlloc.get_memory(sizeof(NpnConf));
  return new (p) NpnConf;
}

// @brief NpnConf を使用可能リストに戻す．
void
NpnMgr::free_npnconf(NpnConf* conf)
{
  mAlloc.put_memory(sizeof(NpnConf), conf);
}

END_NAMESPACE_YM_LOGIC
