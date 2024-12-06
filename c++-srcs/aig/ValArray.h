#ifndef VALARRAY_H
#define VALARRAY_H

/// @file ValArray.h
/// @brief ValArray のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/aig.h"
#include "AigNode.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
/// @class ValArray ValArray.h "ValArray.h"
/// @brief AigMgrImpl::eval() で用いる値の配列
///
/// 中身はただの vector<AigBitVect> だが AigEdge をインデックスとして
/// 用いる．
//////////////////////////////////////////////////////////////////////
class ValArray
{
public:

  /// @brief コンストラクタ
  ValArray(
    SizeType node_num ///< [in] ノード数
  ) : mArray(node_num, 0UL)
  {
  }

  /// @brief デストラクタ
  ~ValArray() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を設定する．
  void
  put_val(
    const AigNode* node, ///< [in] ノード
    AigBitVect val       ///< [in] 値
  )
  {
    mArray[node->id()] = val;
  }

  /// @brief 値を読み出す．
  AigBitVect
  get_val(
    const AigEdge& edge ///< [in] 枝
  ) const
  {
    auto node = edge.node();
    auto inv = edge.inv();
    auto bv = mArray[node->id()];
    if ( inv ) {
      bv = ~bv;
    }
    return bv;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値の配列
  vector<AigBitVect> mArray;

};

END_NAMESPACE_YM_AIG

#endif // VALARRAY_H
