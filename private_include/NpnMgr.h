﻿#ifndef NPNMGR_H
#define NPNMGR_H

/// @file NpnMgr.h
/// @brief NpnMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym/NpnMap.h"


BEGIN_NAMESPACE_YM_LOGIC

class InputInfo;
class IgPartition;
class PolConf;

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
  tvmax_recur(const TvFunc& func,
	      const NpnMap& map1,
	      const IgPartition& igpart,
	      ymuint pid);

  void
  add_map(const NpnMap& map);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 元の関数(デバッグ用)
  TvFunc mOrigFunc;

  // 正規化マップ
  NpnMap mXmap0;

  // 正規化後の関数
  TvFunc mBaseFunc;

  // tvmax_recur で用いる最大の関数
  TvFunc mMaxFunc;

  // w2max_recur で用いる現在の最適解のリスト
  vector<NpnMap> mMaxList;

  // 1回の cannonical あたりの w2max_recur の起動回数
  ymuint64 mW2max_count;

  // 1回の cannonical あたりの tvmax_recur の起動回数
  ymuint64 mTvmax_count;

};

END_NAMESPACE_YM_LOGIC

#endif // NPNMGR_H
