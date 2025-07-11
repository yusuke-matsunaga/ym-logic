#ifndef LITSET_H
#define LITSET_H

/// @file LitSet.h
/// @brief LitSet のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/Literal.h"
#include "ym/SopBase.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
/// @class LitSet LitSet.h "LitSet.h"
/// @brief Literal の集合を表すクラス
///
/// SopCube と異なり，相反するリテラル(x と x')を同時に含んでも良い．
/// Kernel の列挙用に使うだけなので使用する演算も限られている．
//////////////////////////////////////////////////////////////////////
class LitSet:
  public SopBase
{
public:

  /// @brief コンストラクタ
  ///
  /// * 空集合となる．
  explicit
  LitSet(
    SizeType variable_num ///< [in] 変数の数
  );

  /// @brief シングルトンのコンストラクタ
  LitSet(
    SizeType variable_num, ///< [in] 変数の数
    Literal lit            ///< [in] リテラル
  );

  /// @brief 一般的なコンストラクタ
  LitSet(
    SizeType variable_num,               ///< [in] 変数の数
    const std::vector<Literal>& lit_list ///< [in] リテラルのリスト
  );

  /// @brief 一般的なコンストラクタ
  LitSet(
    SizeType variable_num,                   ///< [in] 変数の数
    std::initializer_list<Literal>& lit_list ///< [in] リテラルのリスト
  );

  /// @brief コピーコンストラクタ
  LitSet(
    const LitSet& src ///< [in] コピー元のオブジェクト
  );

  /// @brief コピー代入演算子
  /// @return 代入後の自身への参照を返す．
  LitSet&
  operator=(
    const LitSet& src ///< [in] コピー元のオブジェクト
  );

  /// @brief ムーブコンストラクタ
  LitSet(
    LitSet&& src ///< [in] ムーブ元のオブジェクト
  );

  /// @brief ムーブ代入演算子
  /// @return 代入後の自身への参照を返す．
  LitSet&
  operator=(
    LitSet&& src ///< [in] ムーブ元のオブジェクト
  );

  /// @brief デストラクタ
  ~LitSet() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 該当するリテラルが含まれているか調べる．
  /// @retval true 含まれている．
  /// @retval false 含まれていない．
  bool
  is_in(
    Literal lit ///< [in] 対象のリテラル
  ) const;

  /// @brief ユニオン演算付き代入
  /// @return 自身への参照を返す．
  LitSet&
  operator+=(
    const LitSet& right ///< [in] オペランド
  );

  /// @brief 要素を足す．
  /// @return 自身への参照を返す．
  LitSet&
  operator+=(
    Literal lit ///< [in] 追加するリテラル
  );

  /// @brief 引数のキューブ中のリテラルをひとつでも含んでいたら true を返す．
  bool
  check_intersect(
    const SopCube& right ///< [in] 対象のキューブ
  ) const;

  /// @brief 内容を出力する．
  void
  print(
    std::ostream& s ///< [in] 出力先のストリーム
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 内容を表すビットベクタ
  Chunk mChunk;

};

/// @brief LitSet 同士の加算(ユニオン演算)
/// @return 演算結果を返す．
inline
LitSet
operator+(
  const LitSet& left, ///< [in] オペランド1
  const LitSet& right ///< [in] オペランド2
)
{
  return LitSet{left}.operator+=(right);
}

/// @brief LitSet 同士の加算(ユニオン演算)
/// @return 演算結果を返す．
inline
LitSet
operator+(
  LitSet&& left,      ///< [in] オペランド1
  const LitSet& right ///< [in] オペランド2
)
{
  return LitSet{std::move(left)}.operator+=(right);
}

/// @brief LitSet 同士の加算(ユニオン演算)
/// @return 演算結果を返す．
inline
LitSet
operator+(
  const LitSet& left, ///< [in] オペランド1
  LitSet&& right      ///< [in] オペランド2
)
{
  return LitSet{std::move(right)}.operator+=(left);
}

/// @brief LitSet 同士の加算(ユニオン演算)
/// @return 演算結果を返す．
inline
LitSet
operator+(
  LitSet&& left, ///< [in] オペランド1
  LitSet&& right ///< [in] オペランド2
)
{
  return LitSet{std::move(left)}.operator+=(right);
}

/// @brief 要素を足す．
/// @return 演算結果を返す．
inline
LitSet
operator+(
  const LitSet& lit_set, ///< [in] リテラル集合の本体
  Literal lit            ///< [in] 追加するリテラル
)
{
  return LitSet{lit_set}.operator+=(lit);
}

/// @brief 要素を足す．
/// @return 演算結果を返す．
inline
LitSet
operator+(
  const LitSet&& lit_set, ///< [in] リテラル集合の本体
  Literal lit             ///< [in] 追加するリテラル
)
{
  return LitSet{std::move(lit_set)}.operator+=(lit);
}

/// @brief ストリーム演算
inline
std::ostream&
operator<<(
  std::ostream& s,
  const LitSet& lit_set ///< [in] リテラル集合の本体
)
{
  lit_set.print(s);
  return s;
}

END_NAMESPACE_YM_SOP

#endif // LITSET_H
