#ifndef EVALOP_H
#define EVALOP_H

/// @file EvalOp.h
/// @brief EvalOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/aig.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
/// @class EvalOp EvalOp.h "EvalOp.h"
/// @brief AigMgrImpl::eval() 用の補助クラス
//////////////////////////////////////////////////////////////////////
class EvalOp
{
public:

  /// @brief コンストラクタ
  EvalOp(
    const std::unordered_map<SizeType, AigBitVect>& ival_dict ///< [in] 入力値の辞書
  ) : mIvalDict{ival_dict}
  {
  }

  /// @brief デストラクタ
  ~EvalOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を求める．
  AigBitVect
  eval(
    AigEdge edge ///< [in] 対象の枝
  );

  /// @brief eval() の別名
  AigBitVect
  operator()(
    AigEdge edge ///< [in] 対象の枝
  )
  {
    return eval(edge);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力番号をキーにして値を保持する辞書
  std::unordered_map<SizeType, AigBitVect> mIvalDict;

  // ノード番号をキーにして値を保持する辞書
  std::unordered_map<SizeType, AigBitVect> mValDict;

};

END_NAMESPACE_YM_AIG

#endif // EVALOP_H
