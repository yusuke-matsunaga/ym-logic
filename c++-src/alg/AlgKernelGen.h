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
/// @class カーネル(とコカーネル)の情報を表す構造体
//////////////////////////////////////////////////////////////////////
struct AlgKernelInfo
{

  /// @brief 内容を指定したコンストラクタ
  AlgKernelInfo(const AlgCover& kernel,
		const AlgCube& cokernel) :
    mKernel{kernel},
    mCoKernel{cokernel}
  {
  }

  /// @brief 内容を指定したコンストラクタ
  AlgKernelInfo(AlgCover&& kernel,
		AlgCube&& cokernel) :
    mKernel{kernel},
    mCoKernel{cokernel}
  {
  }

  /// @brief カーネル
  AlgCover mKernel;

  /// @brief コカーネル
  AlgCube mCoKernel;

};


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
  /// @param[out] kernel_list 結果を格納するリスト
  void
  generate(const AlgCover& cover,
	   vector<AlgKernelInfo>& kernel_list);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

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
	   const AlgLitSet& plits,
	   vector<AlgKernelInfo>& kernel_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // リテラルリストの末尾
  vector<Literal>::const_iterator mEnd;

};

END_NAMESPACE_YM_LOGIC

#endif // ALGKERNELGEN_H
