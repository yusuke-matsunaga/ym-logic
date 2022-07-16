#ifndef CHECKSYMOP_H
#define CHECKSYMOP_H

/// @file CheckSymOp.h
/// @brief CheckSymOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ApplyKey.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @class CheckSymOp CheckSymOp.h "CheckSymOp.h"
/// @brief 対称変数のチェックを行う．
//////////////////////////////////////////////////////////////////////
class CheckSymOp
{
public:

  /// @brief コンストラクタ
  CheckSymOp(
    SizeType index1, ///< [in] インデックス1
    SizeType index2, ///< [in] インデックス2
    bool inv = false ///< [in] 極性
  ) : mIndex1{std::min(index1, index2)},
      mIndex2{std::max(index1, index2)},
      mInv{inv}
  {
  }

  /// @brief デストラクタ
  ~CheckSymOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 対称性のチェックを行う．
  bool
  op_step(
    BddEdge edge ///< [in] 枝
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief op_step の下請け関数
  bool
  op_step2(
    BddEdge edge0, ///< [in] mIndex1 の 0 のコファクター
    BddEdge edge1  ///< [in] mIndex1 の 1 のコファクター
  );

  /// @brief op_step の下請け関数
  ///
  /// mIndex2 に依存していないときに true を返す．
  bool
  op_step3(
    BddEdge edge ///< [in] 枝
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // インデックス1
  SizeType mIndex1;

  // インデックス2
  SizeType mIndex2;

  // 極性
  bool mInv;

  // 演算結果テーブル1
  unordered_map<BddNode*, bool> mTable1;

  // 演算結果テーブル2
  unordered_map<ApplyKey, bool> mTable2;
};

END_NAMESPACE_YM_BDD

#endif // CHECKSYMOP_H
