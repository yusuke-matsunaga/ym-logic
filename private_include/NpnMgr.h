#ifndef NPNMGR_H
#define NPNMGR_H

/// @file NpnMgr.h
/// @brief NpnMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym/NpnMap.h"
#include "PolConf.h"


BEGIN_NAMESPACE_YM_LOGIC

class InputInfo;
class IgPartition;

//////////////////////////////////////////////////////////////////////
/// @class NpnMgr NpnMgr.h "NpnMgr.h"
/// @brief NPN同値類の正規形を求めるためのクラス
//////////////////////////////////////////////////////////////////////
class NpnMgr
{
public:

  /// @brief コンストラクタ
  NpnMgr();

  /// @brief デストラクタ
  ~NpnMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部に公開している関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 論理関数の正規化を行う．
  /// @param[in] func 対象の論理関数
  /// @param[in] algorithm アルゴリズムの種類を表す番号
  void
  cannonical(const TvFunc& func,
	     int algorithm = 0);

  /// @brief Walsh の0次/1次係数を用いた正規化を行う．
  /// @param[in] func 対象の論理関数
  /// @param[out] xmap 変換マップ
  /// @param[out] input_info 入力グループの情報
  /// @return 出力極性が決まっていたら true を返す．
  static
  bool
  walsh01_normalize(const TvFunc& func,
		    NpnMap& xmap,
		    InputInfo& input_info);

  /// @brief 重み別 w0 を用いて極性を確定させる．
  /// @param[in] func 対象の関数
  /// @param[inout] polconf_list 極性割当候補のリスト
  ///
  /// 結果として有効な polconf_list の要素のみが残る．
  static
  void
  walsh_w0_refine(const TvFunc& func,
		  vector<PolConf>& polconf_list);

  /// @brief 重み別 w1 を用いて極性を確定させる．
  /// @param[in] func 対象の関数
  /// @param[in] var 対象の変数
  /// @param[inout] polconf_list 極性割当候補のリスト
  static
  void
  walsh_w1_refine(const TvFunc& func,
		  VarId var,
		  vector<PolConf>& polconf_list);

  /// @brief 重み別 w1 を用いて極性を確定させる．
  /// @param[in] func 対象の関数
  /// @param[in] var_list 対象の変数のリスト
  /// @param[inout] polconf_list 極性割当候補のリスト
  static
  void
  walsh_w1_refine(const TvFunc& func,
		  const vector<VarId>& var_list,
		  vector<PolConf>& polconf_list);

  /// @brief 正規化マップの先頭を返す．
  NpnMap
  cmap() const;

  /// @brief すべての正規化マップを返す．
  /// @param[out] map_list 変換マップを格納するリスト
  void
  all_cmap(vector<NpnMap>& map_list) const;

  /// @brief w2max_recur の起動回数を返す．
  /// 直前の cannonical の呼び出しにおける
  /// w2max_recur の起動回数を返す．
  ymulong
  w2max_count() const;

  /// @brief w2max_recur の起動回数を返す．
  /// 直前の cannonical の呼び出しにおける
  /// w2max_recur の起動回数を返す．
  ymulong
  tvmax_count() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

#if 0
  /// @brief DFS
  void
  algorithm0(const NpnConf& conf);

  /// @brief BFS
  void
  algorithm1(const NpnConf& conf);

  /// @brief 極性を展開した後で DFS
  void
  algorithm2(const NpnConf& conf);

  /// @brief 極性を展開した後で BFS
  void
  algorithm3(const NpnConf& conf);

  /// @brief シグネチャが最大になるように極性と順序を決める．
  /// @param[out] conf
  /// @param[in] g0 調べ始める最初のグループ番号
  void
  w2max_recur(NpnConf& conf,
	      ymuint g0);

  /// @brief w2シグネチャが最大になるように極性と順序を決める．
  /// @param[in] conf_list
  /// @param[in] g0 調べ始める最初のグループ番号
  void
  w2max_recur(vector<NpnConf>& conf_list,
	      ymuint g0);

  void
  tvmax_recur(const IgPartition& igpart,
	      ymuint pid0);

  /// @brief mMaxFunc を設定する．
  void
  set_maxfunc(const TvFunc& func);

  /// @brief NpnConf を取り出す．
  NpnConf*
  new_npnconf();

  /// @brief NpnConf を使用可能リストに戻す．
  void
  free_npnconf(NpnConf* conf);
#endif

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // w2max_recur で用いる現在の最適解のリスト
  vector<NpnMap> mMaxList;

  // 1回の cannonical あたりの w2max_recur の起動回数
  ymuint64 mW2max_count;

  // 1回の cannonical あたりの tvmax_recur の起動回数
  ymuint64 mTvmax_count;

  // w2max_recur で用いる現在の最大値
  TvFunc mMaxFunc;

  // w2max_recur で用いる現在の w2 ベクタ
  int mMaxW2[TvFunc::kMaxNi * TvFunc::kMaxNi];

  bool mMaxW2Valid;

#if 0
  // NpnConf 用のアロケータ
  UnitAlloc mAlloc;
#endif

};

END_NAMESPACE_YM_LOGIC

#endif // NPNMGR_H
