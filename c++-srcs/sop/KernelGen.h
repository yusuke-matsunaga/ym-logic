#ifndef KERNELGEN_H
#define KERNELGEN_H

/// @file KernelGen.h
/// @brief KernelGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/sop_nsdef.h"
#include "ym/SopCover.h"
#include "ym/SopCube.h"


BEGIN_NAMESPACE_YM_SOP

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
  void
  all_kernel(
    const SopCover& cover,                        ///< [in] 対象のカバー
    vector<pair<SopCover, SopCover>>& kernel_list ///< [out] カーネルとコカーネルのペアのリスト
  );

  /// @brief 価値の最も高いカーネルを求める．
  SopCover
  best_kernel(
    const SopCover& cover ///< [in] 対象のカバー
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief カーネルとコカーネルを列挙する．
  void
  generate(
    const SopCover& cover ///< [in] 対象のカバー
  );

  /// @brief カーネルを求める下請け関数
  void
  kern_sub(
    const SopCover& cover,             ///< [in] 対象のカバー
    vector<Literal>::const_iterator p, ///< [in] 次のリテラルの位置
    const SopCube& ccube,              ///< [in] 今までに括りだされた共通のキューブ
    const LitSet& plits                ///< [in] すでに括られたリテラルの集合
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // カーネルをキーにしてコカーネルを持つハッシュ表のセル
  struct Cell
  {
    // コンストラクタ
    Cell(
      SopCover&& kernel,
      const SopCube& cokernel
    ) : mKernel{kernel},
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
  hash_add(
    SopCover&& kernel,
    const SopCube& cokernel
  );

  /// @brief ハッシュ表をリサイズする．
  void
  hash_resize(
    SizeType size
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // リテラルリストの末尾
  vector<Literal>::const_iterator mEnd;

  // ハッシュ表
  vector<Cell*> mHashTable;

  // ハッシュ表のサイズ
  SizeType mHashSize;

  // ハッシュ表を拡大する目安
  SizeType mNextLimit;

  // ハッシュ表のセルのリスト
  vector<Cell*> mCellList;

};

END_NAMESPACE_YM_SOP

#endif // KERNELGEN_H
