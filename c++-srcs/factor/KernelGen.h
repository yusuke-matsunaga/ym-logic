#ifndef KERNELGEN_H
#define KERNELGEN_H

/// @file KernelGen.h
/// @brief KernelGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
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
  KernelGen() = default;

  /// @brief デストラクタ
  ~KernelGen() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief カーネルとコカーネルを列挙する．
  std::vector<std::pair<SopCover, std::vector<SopCube>>>
  all_kernels(
    const SopCover& cover ///< [in] 対象のカバー
  );

  /// @brief 価値の最も高いカーネルを求める．
  SopCover
  best_kernel(
    const SopCover& cover ///< [in] 対象のカバー
  );

  /// @brief 価値の最も高いカーネルを求める．
  SopCover
  best_kernel(
    const SopCover& cover, ///< [in] 対象のカバー
    std::function<int(const SopCover&,
		      const std::vector<SopCube>&)> eval_func ///< [in] 評価関数
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
    const SopCover& cover,                  ///< [in] 対象のカバー
    std::vector<Literal>::const_iterator p, ///< [in] 次のリテラルの位置
    const SopCube& ccube,                   ///< [in] 今までに括りだされた共通のキューブ
    const LitSet& plits                     ///< [in] すでに括られたリテラルの集合
  );

  /// @brief 出現頻度の昇順にならべたリテラルのリストを作る．
  std::vector<Literal>
  gen_literal_list(
    const SopCover& cover ///< [in] 対象のカバー
  );


private:
  //////////////////////////////////////////////////////////////////////
  // ハッシュ表に関する操作
  //////////////////////////////////////////////////////////////////////

  /// @brief ハッシュ表をクリアする．
  void
  hash_clear()
  {
    mKernelDict.clear();
  }

  /// @brief ハッシュ表に登録する．
  void
  hash_add(
    const SopCover& kernel,
    const SopCube& cokernel
  )
  {
    if ( mKernelDict.count(kernel) == 0 ) {
      mKernelDict.emplace(kernel, std::vector<SopCube>{cokernel});
    }
    else {
      auto& cokernels = mKernelDict.at(kernel);
      cokernels.push_back(cokernel);
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // リテラルリストの末尾
  std::vector<Literal>::const_iterator mEnd;

  // ハッシュ表
  // カーネルをキーにしてコカーネルのリストを格納する．
  std::unordered_map<SopCover, std::vector<SopCube>> mKernelDict;

};

END_NAMESPACE_YM_SOP

#endif // KERNELGEN_H
