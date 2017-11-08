
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


BEGIN_NONAMESPACE

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
// @param[in] func 対象の関数
// @param[in] polconf_list 極性割当候補のリスト
void
NpnMgr::walsh_w0_refine(const TvFunc& func,
			vector<PolConf>& polconf_list)
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
NpnMgr::walsh_w1_refine(const TvFunc& func,
			VarId var,
			vector<PolConf>& polconf_list)
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
  for (ymuint w = 0; w <= ni && polconf_list.size() > 1; ++ w) {
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

// @brief 重み別 w1 を用いて極性を確定させる．
// @param[in] polconf_list 極性割当候補のリスト
// @param[in] func 対象の関数
// @param[in] var 対象の変数
void
NpnMgr::walsh_w1_refine(const TvFunc& func,
			const vector<VarId>& var_list,
			vector<PolConf>& polconf_list)
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
  for (ymuint w = 0; w <= ni && polconf_list.size() > 1; ++ w) {
    bool first = true;
    int max_d0 = 0;
    ymuint wpos = 0;
    for (ymuint i = 0; i < polconf_list.size(); ++ i) {
      PolConf polconf = polconf_list[i];
      int d0 = 0;
      vector<VarId> new_var_list;
      for (ymuint j = 0; j < var_list.size(); ++ j) {
	VarId var = var_list[j];
	int d1 = func.walsh_w1(var, w, polconf.oinv(), polconf.iinv_bits());
	if ( j == 0 || d0 < d1 ) {
	  d0 = d1;
	  new_var_list.push_back(var);
	}
	else if ( d0 == d1 ) {
	  new_var_list.push_back(var);
	}
      }

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

  // まず独立な変数を取り除く
  NpnMap map0 = func.shrink_map();
  TvFunc func0 = func.xform(map0);

  ymuint ni0 = func0.input_num();

  // 特例
  if ( ni0 == 0 ) {
    if ( func0.value(0) == 0 ) {
      // 定数0関数
      mMaxList.push_back(NpnMap::identity(0));
    }
    else {
      // 定数1関数
      mMaxList.push_back(NpnMap::identity(0, true));
    }
    return;
  }
  if ( ni == 1 ) {
    if ( func.value(0) == 0 ) {
      ASSERT_COND ( func.value(1) == 1 );

      // 肯定のリテラル関数
      mMaxList.push_back(NpnMap::identity(1));
    }
    else {
      ASSERT_COND( func.value(1) == 0 );
      // 否定のリテラル関数
      mMaxList.push_back(NpnMap::identity(1, true));
    }
    return;
  }

  // Walsh の0次と1次の係数を用いて正規化する．
  NpnMap xmap0;
  InputInfo input_info;
  bool opol_fixed = walsh01_normalize(func0, xmap0, input_info);

  // w1:group_num:bisym でグループを分割する．
  IgPartition igpart(input_info);

  if ( opol_fixed && input_info.polundet_num() == 0 && igpart.is_resolved() ) {
    // 解決した．
    mMaxList.push_back(map0 * xmap0);
    return;
  }

  // 入力の等価グループは input_info に入っている．
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

  // xmap0 に従って func0 を変換する．
  TvFunc func1 = func0.xform(xmap0);

  // 極性割り当ての候補を重み別 Walsh_0 でフィルタリングする．
  walsh_w0_refine(func1, polconf_list);

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
      walsh_w1_refine(func1, VarId(iid), polconf_list);

      if ( debug ) {
	cout << "After walsh_w1_refine(" << VarId(iid) << ")" << endl;
	cout << "# of polarity candidates: " << polconf_list.size() << endl;
      }
    }
    else {
      // この分割は確定していない．
      vector<VarId> var_list;
      for (ymuint pos = igpart.partition_begin(pid);
	   pos < igpart.partition_end(pid); ++ pos) {
	ymuint gid = igpart.group_id(pos);
	ymuint iid = input_info.elem(gid, 0);
	var_list.push_back(VarId(iid));
      }
      walsh_w1_refine(func1, var_list, polconf_list);
    }
  }

  // 残りはすべて展開して真理値ベクタが最大となるものを探す．
#if 0
  mMaxFunc = TvFunc::const_zero(ni);
  mMaxList.clear();
  for (vector<PolConf>::iterator p = polconf_list.begin();
       p != polconf_list.end(); ++ p) {
    mPolConf = *p;
    tvmax_recur(igpart, 0);
  }
#endif
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

END_NAMESPACE_YM_LOGIC
