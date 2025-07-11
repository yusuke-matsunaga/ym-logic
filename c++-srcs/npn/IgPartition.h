#ifndef IGPARTITION_H
#define IGPARTITION_H

/// @file IgPartition.h
/// @brief IgPartition のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/TvFunc.h"
#include "InputInfo.h"
#include "PolConf.h"


BEGIN_NAMESPACE_YM_LOGIC

//////////////////////////////////////////////////////////////////////
/// @class IgPartition IgPartition.h "IgPartition.h"
/// @brief 入力グループの分割を表すクラス
//////////////////////////////////////////////////////////////////////
class IgPartition
{
public:

  /// @brief コンストラクタ
  ///
  /// w1:group_num:bisym の情報で初期分割を行う．
  IgPartition(
    const InputInfo& iinfo ///< [in] 等価入力グループの情報
  );

  /// @brief コピーコンストラクタ
  IgPartition(
    const IgPartition& src ///< [in] コピー元のソース
  );

  /// @brief デストラクタ
  ~IgPartition();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief グループ数を返す．
  SizeType
  group_num() const
  {
    return mGroupNum;
  }

  /// @brief グループ番号を返す．
  SizeType
  group_id(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < group_num() )
  ) const
  {
    if ( pos < 0 || group_num() <= pos ) {
      throw std::out_of_range("pos is out of range");
    }
    return mGidArray[pos];
  }

  /// @brief グループの先頭の入力番号を返す．
  SizeType
  input_id(
    SizeType gid///< [in] グループ番号
  ) const
  {
    return mInputInfo.elem(gid, 0);
  }

  /// @brief 分割数を返す．
  SizeType
  partition_num() const
  {
    return mPartitionNum;
  }

  /// @brief 分割のサイズを返す．
  SizeType
  partition_size(
    SizeType pid ///< [in] 分割番号 ( 0 <= pid < partition_num() )
  ) const
  {
    return partition_end(pid) - partition_begin(pid);
  }

  /// @brief 分割の開始位置を返す．
  SizeType
  partition_begin(
    SizeType pid ///< [in] 分割番号 ( 0 <= pid < partition_num() )
  ) const
  {
    if ( pid < 0 || partition_num() <= pid ) {
      throw std::out_of_range("pid is out of range");
    }
    return mBeginArray[pid];
  }

  /// @brief 分割の終了位置を返す．
  ///
  /// この位置は分割には含まれない．
  SizeType
  partition_end(
    SizeType pid ///< [in] 分割番号 ( 0 <= pid < partition_num() )
  ) const
  {
    if ( pid < 0 || partition_num() <= pid ) {
      throw std::out_of_range("pid is out of range");
    }
    return mBeginArray[pid + 1];
  }

  /// @brief すべての分割の要素数が1の時 true を返す．
  bool
  is_resolved() const;

  /// @brief 指定した分割の要素数が1の時 true を返す．
  bool
  is_resolved(
    SizeType pid ///< [in] 分割番号 ( 0 <= pid < partition_num() )
  ) const
  {
    return partition_size(pid) == 1;
  }

  /// @brief 現在の状態を NpnMap に変換する．
  ///
  /// polconf が省略された時は極性の反転はなしとみなす．
  NpnMap
  to_npnmap(
    const PolConf& polconf = PolConf() ///< [in] 極性情報
  ) const;

  /// @brief 分割の細分化を行う．
  /// @return 増えたグループ数を返す．
  template <typename T>
  SizeType
  refine(
    int pid0, ///< [in] 対象の分割番号
    T cmp     ///< [in] 2つの入力グループの大小比較関数オブジェクト
  )
  {
    auto old_size = partition_num();
    auto s = partition_begin(pid0);
    auto e = partition_end(pid0);
    // mGidArray[s] 〜 mGidArray[e - 1] までを
    // cmp の降順に整列させる．
    // つまり
    // forall i, cmp.gt(mGidArray[i], mGidArray[i + 1]) == true
    // が成り立つ．
    // 要素数は多くないので単純なバブルソートを用いる．
    for ( auto i = s; i < e - 1; ++ i ) {
      auto max_gid = mGidArray[i];
      auto max_pos = i;
      for ( auto j = i + 1; j < e; ++ j ) {
	auto gid1 = mGidArray[j];
	if ( cmp.gt(gid1, max_gid) ) {
	  max_gid = gid1;
	  max_pos = j;
	}
      }
      if ( max_pos != i ) {
	// i + 1 から max_pos までを1つずらす．
	for ( auto j = max_pos; j > i; -- j ) {
	  mGidArray[j] = mGidArray[j - 1];
	}
	mGidArray[i] = max_gid;
      }
    }

    // 等価なグループをまとめる．
    auto prev_gid = mGidArray[s];
    for ( auto i = s + 1; i < e; ++ i ) {
      auto cur_gid = mGidArray[i];
      if ( !cmp.eq(prev_gid, cur_gid) ) {
	// 新しい分割を作る．
	// pid0 以降の分割の情報を一つ右にずらす．
	for ( auto pid = mPartitionNum; pid > pid0; -- pid ) {
	  mBeginArray[pid + 1] = mBeginArray[pid];
	}
	++ mPartitionNum;
	++ pid0;
	mBeginArray[pid0] = i;
	prev_gid = cur_gid;
      }
    }

    return mPartitionNum - old_size;
  }

  /// @brief 分割の要素数が1の分割を前に持ってくる．
  void
  reorder();

  /// @brief 分割の要素を一つ取り出して独立した分割とする．
  /// @param[in] pid
  /// @param[in] pos
  void
  _refine(
    SizeType pid, ///< [in] 分轄番号 ( 0 <= pid << partition_num() )
    SizeType pos  ///< [in] 要素の位置番号
                  ///<    ( partition_begin(pid) <= pos < partition_end(pid) )
  );

  /// @brief 内容を出力する．
  void
  display(
    std::ostream& s ///< [in] 出力先のストリーム
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力の情報
  InputInfo mInputInfo;

  /// @brief グループ数
  SizeType mGroupNum;

  /// @brief 分割数
  SizeType mPartitionNum;

  /// @brief グループ番号の配列
  SizeType mGidArray[TvFunc::kMaxNi];

  /// @brief 分割の開始位置
  SizeType mBeginArray[TvFunc::kMaxNi];

};

/// @brief IgPart のストリーム出力演算子
inline
std::ostream&
operator<<(
  std::ostream& s,
  const IgPartition& igpart
)
{
  igpart.display(s);
  return s;
}

END_NAMESPACE_YM_LOGIC

#endif // IGPARTITION_H
