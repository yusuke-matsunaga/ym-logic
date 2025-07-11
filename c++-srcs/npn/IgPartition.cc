
/// @file IgPartition.cc
/// @brief IgPartition の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "IgPartition.h"
#include "PolConf.h"
#include "ym/NpnMap.h"


BEGIN_NAMESPACE_YM_LOGIC

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// IgPartition::refine() 用の比較ファンクタクラス
//////////////////////////////////////////////////////////////////////
class W1GnumBisymCmp
{
public:

  // コンストラクタ
  W1GnumBisymCmp(
    const InputInfo& info
  ) : mInfo{info}
  {
  }

  // 大小比較関数
  bool
  gt(
    SizeType gid1,
    SizeType gid2
  )
  {
    return mInfo.w1gt(gid1, gid2);
  }

  // 等価比較関数
  bool
  eq(
    SizeType gid1,
    SizeType gid2
  )
  {
    return mInfo.w1eq(gid1, gid2);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  const InputInfo& mInfo;

};

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス IgPartition
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
IgPartition::IgPartition(
  const InputInfo& iinfo
) : mInputInfo{iinfo}
{
  // 最初は1つの分割から始める．
  mGroupNum = iinfo.group_num();
  for ( SizeType i = 0; i < mGroupNum; ++ i ) {
    mGidArray[i] = i;
  }
  mPartitionNum = 1;
  mBeginArray[0] = 0;
  mBeginArray[1] = mGroupNum;

  // w1:group_num:bisym による分割を行う．
  refine(0, W1GnumBisymCmp(iinfo));
}

// @brief コピーコンストラクタ
IgPartition::IgPartition(
  const IgPartition& src
) : mInputInfo{src.mInputInfo}
{
  mGroupNum = src.mGroupNum;
  mPartitionNum = src.mPartitionNum;
  for ( SizeType i = 0; i < mGroupNum; ++ i ) {
    mGidArray[i] = src.mGidArray[i];
  }
  for ( SizeType i = 0; i <= mPartitionNum; ++ i ) {
    mBeginArray[i] = src.mBeginArray[i];
  }
}

// @brief デストラクタ
IgPartition::~IgPartition()
{
}

// @brief すべての分割の要素数が1の時 true を返す．
bool
IgPartition::is_resolved() const
{
  for ( SizeType i = 0; i < partition_num(); ++ i ) {
    if ( !is_resolved(i) ) {
      return false;
    }
  }
  return true;
}

// @brief 現在の状態を NpnMap に変換する．
NpnMap
IgPartition::to_npnmap(
  const PolConf& polconf
) const
{
  NpnMap npnmap(mInputInfo.input_num());
  npnmap.set_oinv(polconf.oinv());
  SizeType dst_var = 0;
  for ( SizeType i = 0; i < group_num(); ++ i ) {
    auto gid = group_id(i);
    auto n = mInputInfo.elem_num(gid);
    for ( SizeType j = 0; j < n; ++ j, ++ dst_var ) {
      auto src_var = mInputInfo.elem(gid, j);
      npnmap.set_imap(src_var, dst_var, polconf.iinv(src_var));
    }
  }
  return npnmap;
}

// @brief 分割の要素数が1の分割を前に持ってくる．
void
IgPartition::reorder()
{
  if ( is_resolved() ) {
    return;
  }

  auto n = partition_num();

  // pid をキーにしてグループ番号のリストを持つ配列
  // 要するに mGidArray をデコードしたもの
  std::vector<std::vector<SizeType>> gid_lists(n);
  // 要素数が1の pid のリスト
  std::vector<SizeType> pid_list1;
  // 要素数が2以上の pid のリスト
  std::vector<SizeType> pid_list2;
  for ( SizeType pid = 0; pid < n; ++ pid ) {
    auto ps = partition_size(pid);
    gid_lists[pid].reserve(ps);
    if ( ps == 1 ) {
      pid_list1.push_back(pid);
    }
    else {
      pid_list2.push_back(pid);
    }
    for ( auto pos = partition_begin(pid); pos < partition_end(pid); ++ pos ) {
      auto gid = group_id(pos);
      gid_lists[pid].push_back(gid);
    }
  }

  // pid_list1, pid_list2, gid_lists をもとに結果をエンコードする．
  SizeType wpos = 0;
  SizeType pid = 0;
  for ( SizeType j = 0; j < pid_list1.size(); ++ j, ++ pid ) {
    auto old_pid = pid_list1[j];
    mBeginArray[pid] = wpos;
    for ( SizeType i = 0; i < gid_lists[old_pid].size(); ++ i ) {
      auto gid = gid_lists[old_pid][i];
      mGidArray[wpos] = gid;
      ++ wpos;
    }
  }
  for ( SizeType j = 0; j < pid_list2.size(); ++ j, ++ pid ) {
    auto old_pid = pid_list2[j];
    mBeginArray[pid] = wpos;
    for ( SizeType i = 0; i < gid_lists[old_pid].size(); ++ i ) {
      auto gid = gid_lists[old_pid][i];
      mGidArray[wpos] = gid;
      ++ wpos;
    }
  }
  mBeginArray[pid] = wpos;

  ASSERT_COND( pid == partition_num() );
  ASSERT_COND( wpos == group_num() );
}

// @brief 分割の要素を一つ取り出して独立した分割とする．
void
IgPartition::_refine(
  SizeType pid,
  SizeType pos
)
{
  ASSERT_COND( pid < partition_num() );
  ASSERT_COND( partition_begin(pid) <= pos );
  ASSERT_COND( pos < partition_end(pid) );

  // pid 以降の分割の情報を一つ右にずらす．
  for ( auto pid1 = mPartitionNum; pid1 > pid; -- pid1 ) {
    mBeginArray[pid1 + 1] = mBeginArray[pid1];
  }
  ++ mPartitionNum;
  mBeginArray[pid + 1] = mBeginArray[pid] + 1;

  // partition_begin(pid) から pos までの要素を右にずらす．
  auto gid = mGidArray[pos];
  auto pos0 = mBeginArray[pid];
  for ( auto pos1 = pos; pos1 > pos0; -- pos1 ) {
    mGidArray[pos1] = mGidArray[pos1 - 1];
  }
  mGidArray[pos0] = gid;
}

// @brief 内容を出力する．
void
IgPartition::display(
  std::ostream& s
) const
{
  for ( SizeType i = 0; i < mPartitionNum; ++ i ) {
    s << "|";
    for ( SizeType j = mBeginArray[i]; j < mBeginArray[i + 1]; ++ j ) {
      auto g = mGidArray[j];
      s << " W1: " << mInputInfo.w1(g);
      if ( mInputInfo.bisym(g) ) {
	s << "*";
      }
      s << " (";
      for ( SizeType k = 0; k < mInputInfo.elem_num(g); ++ k ) {
	s << " " << mInputInfo.elem(g, k);
      }
      s << ") ";
    }
  }
  s << "|";
}

END_NAMESPACE_YM_LOGIC
