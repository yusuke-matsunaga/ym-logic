#ifndef NPNMGR_H
#define NPNMGR_H

/// @file NpnMgr.h
/// @brief NpnMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym/NpnMap.h"
#include "InputInfo.h"
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
  /// @return 正規化された関数を返す．
  TvFunc
  cannonical(const TvFunc& func);

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
  /// @param[in] pos 位置番号
  /// @param[in] var 対象の変数
  /// @param[inout] polconf_list 極性割当候補のリスト
  void
  walsh_w1_refine(ymuint pos,
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

  void
  tvmax_recur(const IgPartition& igpart,
	      ymuint pid,
	      const vector<PolConf>& polconf_list);

  void
  clear_max(ymuint pos,
	    ymuint w);

  void
  add_map(const NpnMap& map);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 正規化マップ
  NpnMap mXmap0;

  // 正規化後の関数
  TvFunc mBaseFunc;

  // tvmax_recur で用いる最大の関数
  TvFunc mMaxFunc;

  // tvmax_recur で用いる weighted_walsh1 の最大値
  int mMaxW1[TvFunc::kMaxNi][TvFunc::kMaxNi + 1];

  // w2max_recur で用いる現在の最適解のリスト
  vector<NpnMap> mMaxList;

  // 1回の cannonical あたりの w2max_recur の起動回数
  ymuint64 mW2max_count;

  // 1回の cannonical あたりの tvmax_recur の起動回数
  ymuint64 mTvmax_count;

};

END_NAMESPACE_YM_LOGIC

#endif // NPNMGR_H
