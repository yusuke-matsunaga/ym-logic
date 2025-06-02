#ifndef PATNODE_H
#define PATNODE_H

/// @file PatNode.h
/// @brief PatNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/aig.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
/// @class PatNode PatNode.h "PatNode.h"
/// @brief Rewrite 用のパタンを表すノード
//////////////////////////////////////////////////////////////////////
class PatNode
{
public:

  /// @brief 空のコンストラクタ
  ///
  /// 内容は不定
  PatNode() = default;

  /// @brief コンストラクタ
  PatNode(
    SizeType id,
    const PatNode* child0,
    bool inv0,
    const PatNode* child1,
    bool inv1
  ) : mId{id},
      mChild0{child0},
      mChild1{child1}
  {
    if ( inv0 ) {
      mFlags.set(0);
    }
    if ( inv1 ) {
      mFlags.set(1);
    }
  }

  /// @brief デストラクタ
  ~PatNode() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  SizeType
  id() const
  {
    return mId;
  }

  /// @brief 左の子供を返す．
  const PatNode*
  child0() const
  {
    return mChild0;
  }

  /// @brief 左の子供の反転フラグを返す．
  bool
  inv0() const
  {
    return mFlags.test(0);
  }

  /// @brief 右の子供を返す．
  const PatNode*
  child1() const
  {
    return mChild1;
  }

  /// @brief 右の子供の反転フラグを返す．
  bool
  inv1() const
  {
    return mFlags.test(1);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  SizeType mId;

  // いくつかのフラグ
  // 0: mChild0 の反転フラグ
  // 1: mChild1 の反転フラグ
  std::bitset<2> mFlags{0};

  // 左の子供
  const PatNode* mChild0;

  // 右の子供
  const PatNode* mChild1;

};

END_NAMESPACE_YM_AIG

#endif // PATNODE_H
