#ifndef ALGKERNELGEN_H
#define ALGKERNELGEN_H

/// @file AlgKernelGen.h
/// @brief AlgKernelGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/Alg.h"
#include "ym/AlgCover.h"
#include "ym/AlgCube.h"


BEGIN_NAMESPACE_YM_LOGIC

//////////////////////////////////////////////////////////////////////
/// @class AlgKernelGen AlgKernelGen.h "AlgKernelGen.h"
/// @brief カーネルを求めるクラス
//////////////////////////////////////////////////////////////////////
class AlgKernelGen
{
public:

  /// @brief コンストラクタ
  AlgKernelGen();

  /// @brief デストラクタ
  ~AlgKernelGen();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief カーネルとコカーネルを列挙する．
  /// @param[in] cover 対象のカバー
  /// @param[out] kernel_list カーネルとコカーネルのペアのリスト
  void
  all_kernel(const AlgCover& cover,
	     vector<pair<AlgCover, AlgCover>>& kernel_list);

  /// @brief 価値の最も高いカーネルを求める．
  /// @param[in] cover 対象のカバー
  AlgCover
  best_kernel(const AlgCover& cover);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief カーネルとコカーネルを列挙する．
  /// @param[in] cover 対象のカバー
  void
  generate(const AlgCover& cover);

  /// @brief カーネルを求める下請け関数
  /// @param[in] cover 対象のカバー
  /// @param[in] all_kernel すべてのカーネルを求める．
  /// @param[in] pos mLitList 上の位置
  /// @param[in] ccube 今までに括りだされた共通のキューブ
  /// @param[in] plits mLitList[0]〜mLitList[pos - 1] までをまとめたリテラル集合
  void
  kern_sub(const AlgCover& cover,
	   vector<Literal>::const_iterator p,
	   const AlgCube& ccube,
	   const AlgLitSet& plits);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // カーネルをキーにしてコカーネルを持つハッシュ表のセル
  struct Cell
  {
    // コンストラクタ
    Cell(AlgCover&& kernel,
	 const AlgCube& cokernel) :
      mKernel{kernel},
      mCoKernels{cokernel}
    {
    }

    // カーネル
    AlgCover mKernel;

    // コカーネルのリストを表すカバー
    AlgCover mCoKernels;

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
  hash_add(AlgCover&& kernel,
	   const AlgCube& cokernel);

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

#endif // ALGKERNELGEN_H
