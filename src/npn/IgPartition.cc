
/// @file IgPartition.cc
/// @brief IgPartition の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
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
  W1GnumBisymCmp(const InputInfo& info) :
    mInfo(info)
  {
  }

  // 大小比較関数
  bool
  gt(ymuint gid1,
     ymuint gid2)
  {
    return mInfo.w1gt(gid1, gid2);
  }

  // 等価比較関数
  bool
  eq(ymuint gid1,
     ymuint gid2)
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
// @param[in] iinfo 等価入力グループの情報
//
// w1:group_num:bisym の情報で初期分割を行う．
IgPartition::IgPartition(const InputInfo& iinfo) :
  mInputInfo(iinfo)
{
  // 最初は1つの分割から始める．
  mGroupNum = iinfo.group_num();
  for (ymuint i = 0; i < mGroupNum; ++ i) {
    mGidArray[i] = i;
  }
  mPartitionNum = 1;
  mBeginArray[0] = 0;
  mBeginArray[1] = mGroupNum;

  // w1:group_num:bisym による分割を行う．
  refine(0, W1GnumBisymCmp(iinfo));
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のソース
IgPartition::IgPartition(const IgPartition& src) :
  mInputInfo(src.mInputInfo)
{
  mGroupNum = src.mGroupNum;
  mPartitionNum = src.mPartitionNum;
  for (ymuint i = 0; i < mGroupNum; ++ i) {
    mGidArray[i] = src.mGidArray[i];
  }
  for (ymuint i = 0; i <= mPartitionNum; ++ i) {
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
  for (ymuint i = 0; i < partition_num(); ++ i) {
    if ( !is_resolved(i) ) {
      return false;
    }
  }
  return true;
}

// @brief 現在の状態を NpnMap に変換する．
// @param[in] polconf 極性情報
NpnMap
IgPartition::to_npnmap(const PolConf& polconf) const
{
  NpnMap npnmap(mInputInfo.input_num());
  npnmap.set_oinv(polconf.oinv());
  ymuint dst_id = 0;
  for (ymuint i = 0; i < group_num(); ++ i) {
    ymuint gid = group_id(i);
    ymuint n = mInputInfo.elem_num(gid);
    for (ymuint j = 0; j < n; ++ j, ++ dst_id) {
      VarId dst_var(dst_id);
      ymuint src_id = mInputInfo.elem(gid, j);
      VarId src_var(src_id);
      npnmap.set(src_var, dst_var, polconf.iinv(src_id));
    }
  }
  return npnmap;
}

// @brief 分割の要素を一つ取り出して独立した分割とする．
// @param[in] pid 分轄番号 ( 0 <= pid << partition_num() )
// @param[in] pos 要素の位置番号 ( partition_begin(pid) <= pos < partition_end(pid) )
void
IgPartition::_refine(ymuint pid,
		     ymuint pos)
{
  ASSERT_COND( pid < partition_num() );
  ASSERT_COND( partition_begin(pid) <= pos );
  ASSERT_COND( pos < partition_end(pid) );

  // pid 以降の分割の情報を一つ右にずらす．
  for (ymuint pid1 = mPartitionNum; pid1 > pid; -- pid1) {
    mBeginArray[pid1 + 1] = mBeginArray[pid1];
  }
  ++ mPartitionNum;
  mBeginArray[pid + 1] = mBeginArray[pid] + 1;

  // partition_begin(pid) から pos までの要素を右にずらす．
  ymuint gid = mGidArray[pos];
  ymuint pos0 = mBeginArray[pid];
  for (ymuint pos1 = pos; pos1 > pos0; -- pos1) {
    mGidArray[pos1] = mGidArray[pos1 - 1];
  }
  mGidArray[pos0] = gid;
}

// @brief 内容を出力する．
// @param[in] s 出力先のストリーム
void
IgPartition::display(ostream& s) const
{
  for (ymuint i = 0; i < mPartitionNum; ++ i) {
    s << "|";
    for (ymuint j = mBeginArray[i]; j < mBeginArray[i + 1]; ++ j) {
      ymuint g = mGidArray[j];
      s << " W1: " << mInputInfo.w1(g);
      if ( mInputInfo.bisym(g) ) {
	s << "*";
      }
      s << " (";
      for (ymuint k = 0; k < mInputInfo.elem_num(g); ++ k) {
	s << " " << mInputInfo.elem(g, k);
      }
      s << ") ";
    }
  }
  s << "|";
}

// @brief IgPart のストリーム出力演算子
ostream&
operator<<(ostream& s,
	   const IgPartition& igpart)
{
  igpart.display(s);
  return s;
}

END_NAMESPACE_YM_LOGIC
