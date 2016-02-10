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

class NpnMgrImpl;

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
  /// @param[out] cmap 正規化するための変換マップ(の一つ)
  /// @param[in] algorithm アルゴリズムの種類を表す番号
  /// @note func.xform(cmap) の結果が正規化された関数となる．
  /// @note 同位体変換がある時は次の all_map() で取得する．
  void
  cannonical(const TvFunc& func,
	     NpnMap& cmap,
	     int algorithm = 0);

  /// @brief 直前の cannonical の呼び出しにおける NpnMap の全候補を返す．
  /// @param[out] map_list 変換マップを格納するリスト
  void
  all_map(vector<NpnMap>& map_list) const;

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
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際の処理を行う実装クラス
  NpnMgrImpl* mImpl;

};

END_NAMESPACE_YM_LOGIC

#endif // NPNMGR_H
