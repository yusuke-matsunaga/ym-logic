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
    bool xor_flag,
    std::uint16_t tv,
    std::uint8_t size,
    std::uint8_t level,
    const PatNode* child0,
    bool inv0,
    const PatNode* child1,
    bool inv1,
    bool input0,
    bool input1,
    bool input2,
    bool input3
  ) : mId{id},
      mTv{tv},
      mSize{size},
      mLevel{level},
      mChild0{child0},
      mChild1{child1}
  {
    if ( xor_flag ) {
      mFlags.set(0);
    }
    if ( inv0 ) {
      mFlags.set(1);
    }
    if ( inv1 ) {
      mFlags.set(2);
    }
    if ( input0 ) {
      mFlags.set(3);
    }
    if ( input1 ) {
      mFlags.set(4);
    }
    if ( input2 ) {
      mFlags.set(5);
    }
    if ( input3 ) {
      mFlags.set(6);
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

  /// @brief EXOR ノードの時 true を返す．
  bool
  is_xor() const
  {
    return mFlags.test(0);
  }

  /// @brief AND ノードの時 true を返す．
  bool
  is_and() const
  {
    return !is_xor();
  }

  /// @brief このノードの表す論理関数の真理値表のパタンを返す．
  std::uint16_t
  tv() const
  {
    return mTv;
  }

  /// @brief このノードを根とするパタンのサイズを返す．
  SizeType
  size() const
  {
    return mSize;
  }

  /// @brief このノードを根とするパタンのレベルを返す．
  SizeType
  level() const
  {
    return mLevel;
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
    return mFlags.test(1);
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
    return mFlags.test(2);
  }

  /// @brief 入力0フラグ
  bool
  input0() const
  {
    return mFlags.test(3);
  }

  /// @brief 入力1フラグ
  bool
  input1() const
  {
    return mFlags.test(4);
  }

  /// @brief 入力2フラグ
  bool
  input2() const
  {
    return mFlags.test(5);
  }

  /// @brief 入力4フラグ
  bool
  input3() const
  {
    return mFlags.test(6);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  SizeType mId;

  // いくつかのフラグ
  // 0: EXOR フラグ
  // 1: mChild0 の反転フラグ
  // 2: mChild1 の反転フラグ
  // 3: input0 フラグ
  // 4: input1 フラグ
  // 5: input2 フラグ
  // 6: input3 フラグ
  std::bitset<7> mFlags{0};

  // このノードの関数を表す真理値表
  std::uint16_t mTv;

  // このノードを根とするパタンのサイズ
  std::uint8_t mSize;

  // このノードを根とするパタンのレベル
  std::uint8_t mLevel;

  // 左の子供
  const PatNode* mChild0;

  // 右の子供
  const PatNode* mChild1;

};

END_NAMESPACE_YM_AIG

#endif // PATNODE_H
