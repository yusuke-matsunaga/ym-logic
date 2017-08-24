#ifndef IGPARTITION_H
#define IGPARTITION_H

/// @file IgPartition.h
/// @brief IgPartition のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "ym/ym_logic.h"
#include "ym/TvFunc.h"


BEGIN_NAMESPACE_YM_LOGIC

class InputInfo;
class PolConf;

//////////////////////////////////////////////////////////////////////
/// @class IgPartition IgPartition.h "IgPartition.h"
/// @brief 入力グループの分割を表すクラス
//////////////////////////////////////////////////////////////////////
class IgPartition
{
public:

  /// @brief コンストラクタ
  /// @param[in] iinfo 等価入力グループの情報
  ///
  /// w1:group_num:bisym の情報で初期分割を行う．
  IgPartition(const InputInfo& iinfo);

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のソース
  IgPartition(const IgPartition& src);

  /// @brief デストラクタ
  ~IgPartition();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief グループ数を返す．
  ymuint
  group_num() const;

  /// @brief グループ番号を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < group_num() )
  ymuint
  group_id(ymuint pos) const;

  /// @brief 分割数を返す．
  ymuint
  partition_num() const;

  /// @brief 分割のサイズを返す．
  /// @param[in] pid 分割番号 ( 0 <= pid < partition_num() )
  ymuint
  partition_size(ymuint pid) const;

  /// @brief 分割の開始位置を返す．
  /// @param[in] pid 分割番号 ( 0 <= pid < partition_num() )
  ymuint
  partition_begin(ymuint pid) const;

  /// @brief 分割の終了位置を返す．
  /// @param[in] pid 分割番号 ( 0 <= pid < partition_num() )
  ///
  /// この位置は分割には含まれない．
  ymuint
  partition_end(ymuint pid) const;

  /// @brief すべての分割の要素数が1の時 true を返す．
  bool
  is_resolved() const;

  /// @brief 指定した分割の要素数が1の時 true を返す．
  /// @param[in] pid 分割番号 ( 0 <= pid < partition_num() )
  bool
  is_resolved(ymuint pid) const;

  /// @brief 現在の状態を NpnMap に変換する．
  /// @param[in] ni 入力数
  /// @param[in] iinfo 入力グループの情報
  /// @param[in] polconf 極性情報
  NpnMap
  to_npnmap(ymuint ni,
	    const InputInfo& iinfo,
	    const PolConf& polconf) const;

  /// @brief 分割の細分化を行う．
  /// @param[in] pid0 対象の分割番号
  /// @param[in] cmp 2つの入力グループの大小比較関数オブジェクト
  /// @return 増えたグループ数を返す．
  template <typename T>
  ymuint
  refine(ymuint pid0,
	 T cmp);

  /// @brief 分割の要素を一つ取り出して独立した分割とする．
  /// @param[in] pid 分轄番号 ( 0 <= pid << partition_num() )
  /// @param[in] pos 要素の位置番号 ( partition_begin(pid) <= pos < partition_end(pid) )
  void
  _refine(ymuint pid,
	  ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  /// @brief グループ数
  ymuint mGroupNum;

  /// @brief 分割数
  ymuint mPartitionNum;

  /// @brief グループ番号の配列
  ymuint mGidArray[TvFunc::kMaxNi];

  /// @brief 分割の開始位置
  ymuint mBeginArray[TvFunc::kMaxNi];

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief グループ数を返す．
inline
ymuint
IgPartition::group_num() const
{
  return mGroupNum;
}

// @brief グループ番号を返す．
// @param[in] pos 位置番号 ( 0 <= pos < group_num() )
inline
ymuint
IgPartition::group_id(ymuint pos) const
{
  ASSERT_COND( pos < group_num() );
  return mGidArray[pos];
}

// @brief 分割数を返す．
inline
ymuint
IgPartition::partition_num() const
{
  return mPartitionNum;
}

// @brief 分割のサイズを返す．
// @param[in] pid 分割番号 ( 0 <= pid < partition_num() )
inline
ymuint
IgPartition::partition_size(ymuint pid) const
{
  return partition_end(pid) - partition_begin(pid);
}

// @brief 分割の開始位置を返す．
// @param[in] pid 分割番号 ( 0 <= pid < partition_num() )
inline
ymuint
IgPartition::partition_begin(ymuint pid) const
{
  ASSERT_COND( pid < partition_num() );
  return mBeginArray[pid];
}

// @brief 分割の終了位置を返す．
// @param[in] pid 分割番号 ( 0 <= pid < partition_num() )
//
// この位置は分割には含まれない．
inline
ymuint
IgPartition::partition_end(ymuint pid) const
{
  ASSERT_COND( pid < partition_num() );
  return mBeginArray[pid + 1];
}

// @brief 指定した分割の要素数が1の時 true を返す．
// @param[in] pid 分割番号 ( 0 <= pid < partition_num() )
inline
bool
IgPartition::is_resolved(ymuint pid) const
{
  return partition_size(pid) == 1;
}

// @brief 分割の細分化を行う．
// @param[in] pid0 対象の分割番号
// @param[in] cmp 2つの入力クラスの大小比較関数オブジェクト
// @return 増えたグループ数を返す．
template <typename T>
inline
ymuint
IgPartition::refine(ymuint pid0,
		    T cmp)
{
  ymuint old_size = partition_num();
  ymuint s = partition_begin(pid0);
  ymuint e = partition_end(pid0);
  // mGidArray[s] 〜 mGidArray[e - 1] までを
  // cmp の降順に整列させる．
  // つまり
  // forall i, cmp.gt(mGidArray[i], mGidArray[i + 1]) == true
  // が成り立つ．
  // 要素数は多くないので単純なバブルソートを用いる．
  for (ymuint i = s; i < e - 1; ++ i) {
    ymuint max_gid = mGidArray[i];
    ymuint max_pos = i;
    for (ymuint j = i + 1; j < e; ++ j) {
      ymuint gid1 = mGidArray[j];
      if ( cmp.gt(gid1, max_gid) ) {
	max_gid = gid1;
	max_pos = j;
      }
    }
    if ( max_pos != i ) {
      // i + 1 から max_pos までを1つずらす．
      for (ymuint j = max_pos; j > i; -- j) {
	mGidArray[j] = mGidArray[j - 1];
      }
      mGidArray[i] = max_gid;
    }
  }

  // 等価なグループをまとめる．
  ymuint prev_gid = mGidArray[s];
  for (ymuint i = s + 1; i < e; ++ i) {
    ymuint cur_gid = mGidArray[i];
    if ( !cmp.eq(prev_gid, cur_gid) ) {
      // 新しい分割を作る．
      // pid0 以降の分割の情報を一つ右にずらす．
      for (ymuint pid = mPartitionNum; pid > pid0; -- pid) {
	mBeginArray[pid + 1] = mBeginArray[pid];
      }
      ++ mPartitionNum;
      ymuint pid1 = pid0 + 1;
      mBeginArray[pid1] = i;
      prev_gid = cur_gid;
    }
  }

  return mPartitionNum - old_size;
}

END_NAMESPACE_YM_LOGIC

#endif // IGPARTITION_H