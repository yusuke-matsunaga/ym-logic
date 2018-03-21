#ifndef INPUTINFO_H
#define INPUTINFO_H

/// @file InputInfo.h
/// @brief InputInfo のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "ym/logic.h"
#include "ym/TvFunc.h"


BEGIN_NAMESPACE_YM_LOGIC

//////////////////////////////////////////////////////////////////////
/// @class InputInfo InputInfo.h "InputInfo.h"
/// @brief 対象な入力グループの情報を表すクラス
///
/// - 要素の入力番号のリスト
/// - Walsh の 1次係数
/// - bi-symmetry フラグ
///
/// の情報を持つ．
/// bi-symmetry とは x と y が対称でかつ x と y' も対称であること．
//////////////////////////////////////////////////////////////////////
class InputInfo
{
public:

  /// @brief コンストラクタ
  /// @param[in] input_num 入力数
  ///
  /// 空の状態で初期化される．
  InputInfo(int input_num = 0);

  /// @brief デストラクタ
  ~InputInfo();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クリアする．
  void
  clear();

  /// @brief 入力数を設定する．
  /// @param[in] input_num 入力数
  void
  set_input_num(int input_num);

  /// @brief 新しい等価グループを作る．
  /// @param[in] id 変数番号
  /// @param[in] w1 Walsh の1次係数
  ///
  /// id だけを要素として持つ新しいグループを作る．
  void
  new_group(int id,
	    int w1);

  /// @brief 既存のグループに要素を追加する．
  /// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
  /// @param[in] id 変数番号
  void
  add_elem(int gid,
	   int id);

  /// @brief 既存のグループに bi-symmetry フラグをつける．
  /// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
  void
  set_bisym(int gid);


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力数を返す．
  int
  input_num() const;

  /// @brief 等価グループ数を返す．
  int
  group_num() const;

  /// @brief 極性の決まっていないグループ数を返す．
  int
  polundet_num() const;

  /// @brief 極性の決まっていないグループ番号を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < polundet_num() )
  int
  polundet_gid(int pos) const;

  /// @brief Walsh の 1次係数を返す．
  /// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
  int
  w1(int gid) const;

  /// @brief 等価グループの要素数を返す．
  /// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
  int
  elem_num(int gid) const;

  /// @brief 等価グループの要素を返す．
  /// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
  /// @param[in] pos 位置番号 ( 0 <= pos < elem_num(gid) )
  int
  elem(int gid,
       int pos) const;

  /// @brief 等価グループの bi-symmetry フラグを返す．
  /// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
  bool
  bisym(int gid) const;

  /// @brief 等価グループの反転ビットパタンを求める．
  /// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
  ///
  /// 普通の等価グループは全要素のビットを反転させる．
  /// bi-symmetry の等価グループは先頭のビットのみ反転させる．
  int
  inv_bits(int gid) const;

  /// @brief w1:elem_num:bisym を用いた比較関数
  bool
  w1gt(int gid1,
       int gid2) const;

  /// @brief w1:elem_num:bisym を用いた等価関数
  bool
  w1eq(int gid1,
       int gid2) const;

  /// @brief 内容を出力する．
  /// @param[in] s 出力先のストリーム
  void
  display(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力数
  int mInputNum;

  // 等価グループ数
  int mGroupNum;

  // Walsh の1次係数
  // キーはグループ番号
  int mW1[TvFunc::kMaxNi];

  // 等価グループの要素のリストの配列
  vector<int> mElemListArray[TvFunc::kMaxNi];

  // 極性の決まっていないグループ数
  int mPolUndetNum;

  // 極性が決まっていないグループ番号のリスト
  int mPolUndetArray[TvFunc::kMaxNi];

  // 反転ビットパタンの配列
  int mInvBitsArray[TvFunc::kMaxNi];

  // 等価グループの bi-symmetry フラグのビットベクタ
  int mBiSymmBits;

  // 各入力変数のグループ番号
  int mGroupIdArray[TvFunc::kMaxNi];

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] input_num 入力数
//
// 空の状態で初期化される．
// 空の状態で初期化される．
inline
InputInfo::InputInfo(int input_num) :
  mInputNum(input_num)
{
  clear();
}

// @brief デストラクタ
inline
InputInfo::~InputInfo()
{
}

// @brief クリアする．
inline
void
InputInfo::clear()
{
  mGroupNum = 0;
  for (int i = 0; i < TvFunc::kMaxNi; ++ i) {
    mElemListArray[i].clear();
  }
  mPolUndetNum = 0;
  mBiSymmBits = 0U;
}

// @brief 入力数を設定する．
inline
void
InputInfo::set_input_num(int input_num)
{
  mInputNum = input_num;
}

// @brief 新しい等価グループを作る．
// @param[in] id 変数番号
// @param[in] w1 Walsh の1次係数
//
// id だけを要素として持つ新しいグループを作る．
inline
void
InputInfo::new_group(int id,
		     int w1)
{
  int gid = mGroupNum;
  ++ mGroupNum;
  mW1[gid] = w1;
  mElemListArray[gid].clear();
  if ( w1 == 0 ) {
    mPolUndetArray[mPolUndetNum] = gid;
    ++ mPolUndetNum;
    mInvBitsArray[gid] = (1U << id);
  }
  add_elem(gid, id);
}

// @brief 既存のグループに要素を追加する．
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
// @param[in] id 変数番号
inline
void
InputInfo::add_elem(int gid,
		    int id)
{
  ASSERT_COND( gid < mGroupNum );
  mElemListArray[gid].push_back(id);
  mGroupIdArray[id] = gid;
  if ( !bisym(gid) ) {
    mInvBitsArray[gid] |= (1U << id);
  }
}

// @brief 既存のグループに bi-symmetry フラグをつける．
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
inline
void
InputInfo::set_bisym(int gid)
{
  ASSERT_COND( gid < mGroupNum );
  mBiSymmBits |= (1U << gid);
}

// @brief 入力数を返す．
inline
int
InputInfo::input_num() const
{
  return mInputNum;
}

// @brief 等価グループ数を返す．
inline
int
InputInfo::group_num() const
{
  return mGroupNum;
}

// @brief 等価グループの要素数を返す．
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
inline
int
InputInfo::elem_num(int gid) const
{
  ASSERT_COND( gid < mGroupNum );
  return mElemListArray[gid].size();
}

// @brief 等価グループの要素を返す．
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
// @param[in] pos 位置番号 ( 0 <= pos < elem_num(gid) )
inline
int
InputInfo::elem(int gid,
		int pos) const
{
  ASSERT_COND( gid < mGroupNum );
  ASSERT_COND( pos < elem_num(gid) );
  return mElemListArray[gid][pos];
}

// @brief 極性の決まっていないグループ数を返す．
inline
int
InputInfo::polundet_num() const
{
  return mPolUndetNum;
}

// @brief 極性の決まっていないグループ番号を返す．
// @param[in] pos 位置番号 ( 0 <= pos < polundet_num() )
inline
int
InputInfo::polundet_gid(int pos) const
{
  ASSERT_COND( pos < polundet_num() );
  return mPolUndetArray[pos];
}

// @brief Walsh の 1次係数を返す．
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
inline
int
InputInfo::w1(int gid) const
{
  ASSERT_COND( gid < mGroupNum );
  return mW1[gid];
}

// @brief 等価グループの bi-symmetry フラグを返す．
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
inline
bool
InputInfo::bisym(int gid) const
{
  ASSERT_COND( gid < mGroupNum );
  return static_cast<bool>((mBiSymmBits >> gid) & 1U);
}

// @brief 等価グループの反転ビットパタンを求める．
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
//
// 普通の等価グループは全要素のビットを反転させる．
// bi-symmetry の等価グループは先頭のビットのみ反転させる．
inline
int
InputInfo::inv_bits(int gid) const
{
  ASSERT_COND( gid < mGroupNum );
  return mInvBitsArray[gid];
}

// @brief w1:elem_num:bisym を用いた比較関数
inline
bool
InputInfo::w1gt(int gid1,
		int gid2) const
{
  int diff1 = w1(gid1) - w1(gid2);
  if ( diff1 > 0 ) {
    return true;
  }
  else if ( diff1 < 0 ) {
    return false;
  }

  int diff2 = elem_num(gid1) - elem_num(gid2);
  if ( diff2 > 0 ) {
    return true;
  }
  else if ( diff2 < 0 ) {
    return false;
  }
  if ( bisym(gid1) && !bisym(gid2) ) {
    return true;
  }
  return false;
}

// @brief w1:elem_num:bisym を用いた等価関数
inline
bool
InputInfo::w1eq(int gid1,
		int gid2) const
{
  if ( w1(gid1) != w1(gid2) ) {
    return false;
  }
  if ( elem_num(gid1) != elem_num(gid2) ) {
    return false;
  }
  return bisym(gid1) == bisym(gid2);
}

// @brief 内容を出力する．
// @param[in] s 出力先のストリーム
inline
void
InputInfo::display(ostream& s) const
{
  for (int gid = 0; gid < group_num(); ++ gid) {
    s << "G#" << gid << ": " << w1(gid) << ": ";
    if ( bisym(gid) ) {
      s << "*: ";
    }
    s << "{";
    for (int i = 0; i < elem_num(gid); ++ i) {
      s << " " << elem(gid, i);
    }
    s << "}" << endl;
  }
  s << endl;
}

END_NAMESPACE_YM_LOGIC

#endif // INPUTINFO_H
