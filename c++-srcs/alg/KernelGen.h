#ifndef KERNELGEN_H
#define KERNELGEN_H

/// @file KernelGen.h
/// @brief KernelGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/AlgCover.h"
#include "ym/AlgCube.h"


BEGIN_NAMESPACE_YM_ALG

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
  /// @return カーネルとコカーネル集合のペアのリストを返す．
  vector<pair<AlgCover, AlgCover>>
  all_kernels(
    const AlgCover& cover ///< [in] 対象のカバー
  );

  /// @brief 価値の最も高いカーネルを求める．
  AlgCover
  best_kernel(
    const AlgCover& cover ///< [in] 対象のカバー
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief カーネルとコカーネルを列挙する．
  void
  generate(
    const AlgCover& cover ///< [in] 対象のカバー
  );

  /// @brief カーネルを求める下請け関数
  void
  kern_sub(
    const AlgCover& cover,             ///< [in] 対象のカバー
    vector<Literal>::const_iterator p, ///< [in] 次のリテラルの位置
    const AlgCube& ccube,              ///< [in] 今までに括りだされた共通のキューブ
    const LitSet& plits                ///< [in] すでに括られたリテラルの集合
  );

  /// @brief 出現頻度の昇順にならべたリテラルのリストを作る．
  vector<Literal>
  gen_literal_list(
    const AlgCover& cover ///< [in] 対象のカバー
  );

#if 0
private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // カーネルをキーにしてコカーネルを持つハッシュ表のセル
  struct Cell
  {
    // コンストラクタ
    Cell(
      AlgCover&& kernel,
      const AlgCube& cokernel
    ) : mKernel{kernel},
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
#endif

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
    const AlgCover& kernel,
    const AlgCube& cokernel
  )
  {
    auto tmp_kernel = kernel;
    hash_add(std::move(tmp_kernel), cokernel);
  }

  /// @brief ハッシュ表に登録する．
  void
  hash_add(
    AlgCover&& kernel,
    const AlgCube& cokernel
  );

#if 0
  /// @brief ハッシュ表をリサイズする．
  void
  hash_resize(
    SizeType size
  );
#endif

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // リテラルリストの末尾
  vector<Literal>::const_iterator mEnd;

  // ハッシュ表
  // カーネルをキーにしてコカーネルのリストを格納する．
  std::unordered_map<AlgCover, AlgCover> mKernelDict;

#if 0
  vector<Cell*> mHashTable;

  // ハッシュ表のサイズ
  SizeType mHashSize;

  // ハッシュ表を拡大する目安
  SizeType mNextLimit;

  // ハッシュ表のセルのリスト
  vector<Cell*> mCellList;
#endif

};

END_NAMESPACE_YM_ALG

#endif // KERNELGEN_H
