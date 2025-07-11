#ifndef NPNMGR_H
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
  /// @return 正規化された関数を返す．
  TvFunc
  cannonical(
    const TvFunc& func ///< [in] 対象の論理関数
  );

  /// @brief すべての正規化マップを返す．
  void
  all_cmap(
    std::vector<NpnMap>& map_list ///< [out] 変換マップを格納するリスト
  ) const;

  /// @brief w2max_recur の起動回数を返す．
  /// 直前の cannonical の呼び出しにおける
  /// w2max_recur の起動回数を返す．
  std::uint64_t
  tvmax_count() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  void
  tvmax_recur(
    const TvFunc& func,
    const IgPartition& igpart,
    std::vector<bool>& w1_mark
  );

  void
  add_map(
    const NpnMap& map
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 正規化マップ
  // add_map() 中で常に適用される．
  NpnMap mXmap0;

  // tvmax_recur で用いる最大の関数
  TvFunc mMaxFunc;

  // w2max_recur で用いる現在の最適解のリスト
  std::vector<NpnMap> mMaxList;

  // 1回の cannonical あたりの tvmax_recur の起動回数
  std::uint64_t mTvmax_count;

};

END_NAMESPACE_YM_LOGIC

#endif // NPNMGR_H
