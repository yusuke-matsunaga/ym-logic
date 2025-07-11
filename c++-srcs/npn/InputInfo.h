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
  ///
  /// 空の状態で初期化される．
  InputInfo(
    SizeType input_num = 0 ///< [in] 入力数
  ) : mInputNum{input_num}
  {
    clear();
  }

  /// @brief デストラクタ
  ~InputInfo() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クリアする．
  void
  clear()
  {
    mGroupNum = 0;
    for ( SizeType i = 0; i < TvFunc::kMaxNi; ++ i ) {
      mElemListArray[i].clear();
    }
    mPolUndetNum = 0;
    mBiSymmBits = 0U;
  }

  /// @brief 入力数を設定する．
  void
  set_input_num(
    SizeType input_num ///< [in] 入力数
  )
  {
    mInputNum = input_num;
  }

  /// @brief 新しい等価グループを作る．
  ///
  /// id だけを要素として持つ新しいグループを作る．
  void
  new_group(
    SizeType id, ///< [in] 変数番号
    SizeType w1  ///< [in] Walsh の1次係数
  )
  {
    auto gid = mGroupNum;
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

  /// @brief 既存のグループに要素を追加する．
  void
  add_elem(
    SizeType gid, ///< [in] グループ番号 ( 0 <= gid < group_num() )
    SizeType id   ///< [in] 変数番号
  )
  {
    _check_gid(gid);
    mElemListArray[gid].push_back(id);
    mGroupIdArray[id] = gid;
    if ( !bisym(gid) ) {
      mInvBitsArray[gid] |= (1U << id);
    }
  }

  /// @brief 既存のグループに bi-symmetry フラグをつける．
  void
  set_bisym(
    SizeType gid ///< [in] グループ番号 ( 0 <= gid < group_num() )
  )
  {
    _check_gid(gid);
    mBiSymmBits |= (1U << gid);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力数を返す．
  SizeType
  input_num() const
  {
    return mInputNum;
  }

  /// @brief 等価グループ数を返す．
  SizeType
  group_num() const
  {
    return mGroupNum;
  }

  /// @brief 極性の決まっていないグループ数を返す．
  SizeType
  polundet_num() const
  {
    return mPolUndetNum;
  }

  /// @brief 極性の決まっていないグループ番号を返す．
  SizeType
  polundet_gid(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < polundet_num() )
  ) const
  {
    if ( pos < 0 || polundet_num() <= pos ) {
      throw std::out_of_range("pos is out of range");
    }
    return mPolUndetArray[pos];
  }

  /// @brief Walsh の 1次係数を返す．
  int
  w1(
    SizeType gid ///< [in] グループ番号 ( 0 <= gid < group_num() )
  ) const
  {
    _check_gid(gid);
    return mW1[gid];
  }

  /// @brief 等価グループの要素数を返す．
  SizeType
  elem_num(
    SizeType gid ///< [in] グループ番号 ( 0 <= gid < group_num() )
  ) const
  {
    _check_gid(gid);
    return mElemListArray[gid].size();
  }

  /// @brief 等価グループの要素を返す．
  SizeType
  elem(
    SizeType gid, ///< [in] グループ番号 ( 0 <= gid < group_num() )
    SizeType pos  ///< [in] 位置番号 ( 0 <= pos < elem_num(gid) )
  ) const
  {
    _check_gid(gid);
    if ( pos < 0 || elem_num(gid) <= pos ) {
      throw std::out_of_range("pos is out of range");
    }
    return mElemListArray[gid][pos];
  }

  /// @brief 等価グループの bi-symmetry フラグを返す．
  bool
  bisym(
    SizeType gid ///< [in] グループ番号 ( 0 <= gid < group_num() )
  ) const
  {
    _check_gid(gid);
    return static_cast<bool>((mBiSymmBits >> gid) & 1U);
  }

  /// @brief 等価グループの反転ビットパタンを求める．
  ///
  /// 普通の等価グループは全要素のビットを反転させる．
  /// bi-symmetry の等価グループは先頭のビットのみ反転させる．
  std::uint32_t
  inv_bits(
    SizeType gid ///< [in] グループ番号 ( 0 <= gid < group_num() )
  ) const
  {
    _check_gid(gid);
    return mInvBitsArray[gid];
  }

  /// @brief w1:elem_num:bisym を用いた比較関数
  bool
  w1gt(
    SizeType gid1,
    SizeType gid2
  ) const
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

  /// @brief w1:elem_num:bisym を用いた等価関数
  bool
  w1eq(
    SizeType gid1,
    SizeType gid2
  ) const
  {
    if ( w1(gid1) != w1(gid2) ) {
      return false;
    }
    if ( elem_num(gid1) != elem_num(gid2) ) {
      return false;
    }
    return bisym(gid1) == bisym(gid2);
  }

  /// @brief 内容を出力する．
  void
  display(
    std::ostream& s ///< [in] 出力先のストリーム
  ) const
  {
    for ( SizeType gid = 0; gid < group_num(); ++ gid ) {
      s << "G#" << gid << ": " << w1(gid) << ": ";
      if ( bisym(gid) ) {
	s << "*: ";
      }
      s << "{";
      for ( SizeType i = 0; i < elem_num(gid); ++ i ) {
	s << " " << elem(gid, i);
      }
      s << "}" << std::endl;
    }
    s << std::endl;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief gid のチェックを行う．
  void
  _check_gid(
    SizeType gid
  ) const
  {
    if ( gid < 0 || mGroupNum <= gid ) {
      throw std::out_of_range("gid is out of range");
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力数
  SizeType mInputNum;

  // 等価グループ数
  SizeType mGroupNum;

  // Walsh の1次係数
  // キーはグループ番号
  int mW1[TvFunc::kMaxNi];

  // 等価グループの要素のリストの配列
  std::vector<SizeType> mElemListArray[TvFunc::kMaxNi];

  // 極性の決まっていないグループ数
  SizeType mPolUndetNum;

  // 極性が決まっていないグループ番号のリスト
  SizeType mPolUndetArray[TvFunc::kMaxNi];

  // 反転ビットパタンの配列
  std::uint32_t mInvBitsArray[TvFunc::kMaxNi];

  // 等価グループの bi-symmetry フラグのビットベクタ
  SizeType mBiSymmBits;

  // 各入力変数のグループ番号
  SizeType mGroupIdArray[TvFunc::kMaxNi];

};

END_NAMESPACE_YM_LOGIC

#endif // INPUTINFO_H
