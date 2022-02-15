#ifndef KERNELGEN_H
#define KERNELGEN_H

/// @file KernelGen.h
/// @brief KernelGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/Sop.h"
#include "ym/SopCover.h"
#include "ym/SopCube.h"


BEGIN_NAMESPACE_YM_LOGIC

class LitSet;

//////////////////////////////////////////////////////////////////////
/// @class KernelGen KernelGen.h "KernelGen.h"
/// @brief カーネルを求めるクラス
//////////////////////////////////////////////////////////////////////
class KernelGen
{
public:

  /// @brief コンストラクタ
  KernelGen();

  /// @brief デストラクタ
  ~KernelGen();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief カーネルとコカーネルを列挙する．
  /// @param[in] cover 対象のカバー
  /// @param[out] kernel_list カーネルとコカーネルのペアのリスト
  void
  all_kernel(const SopCover& cover,
	     vector<pair<SopCover, SopCover>>& kernel_list);

  /// @brief 価値の最も高いカーネルを求める．
  /// @param[in] cover 対象のカバー
  SopCover
  best_kernel(const SopCover& cover);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief カーネルとコカーネルを列挙する．
  /// @param[in] cover 対象のカバー
  void
  generate(const SopCover& cover);

  /// @brief カーネルを求める下請け関数
  /// @param[in] cover 対象のカバー
  /// @param[in] all_kernel すべてのカーネルを求める．
  /// @param[in] pos mLitList 上の位置
  /// @param[in] ccube 今までに括りだされた共通のキューブ
  /// @param[in] plits mLitList[0]〜mLitList[pos - 1] までをまとめたリテラル集合
  void
  kern_sub(const SopCover& cover,
	   vector<Literal>::const_iterator p,
	   const SopCube& ccube,
	   const LitSet& plits);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // カーネルをキーにしてコカーネルを持つハッシュ表のセル
  struct Cell
  {
    // コンストラクタ
    Cell(SopCover&& kernel,
	 const SopCube& cokernel) :
      mKernel{kernel},
      mCoKernels{cokernel}
    {
    }

    // カーネル
    SopCover mKernel;

    // コカーネルのリストを表すカバー
    SopCover mCoKernels;

    // 次のセルを指すリンクポインタ
    Cell* mLink;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // ハッシュ表に関する操作
  //////////////////////////////////////////////////////////////////////

  /// @brief ハッシュ表をクリアする．
  void
  hash_clear();

  /// @brief ハッシュ表に登録する．
  void
  hash_add(SopCover&& kernel,
	   const SopCube& cokernel);

  /// @brief ハッシュ表をリサイズする．
  void
  hash_resize(int size);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // リテラルリストの末尾
  vector<Literal>::const_iterator mEnd;

  // ハッシュ表
  vector<Cell*> mHashTable;

  // ハッシュ表のサイズ
  int mHashSize;

  // ハッシュ表を拡大する目安
  int mNextLimit;

  // ハッシュ表のセルのリスト
  vector<Cell*> mCellList;

};

END_NAMESPACE_YM_LOGIC

#endif // KERNELGEN_H
