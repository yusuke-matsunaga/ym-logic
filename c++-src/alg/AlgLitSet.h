#ifndef ALGLITSET_H
#define ALGLITSET_H

/// @file AlgLitSet.h
/// @brief AlgLitSet のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/Alg.h"
#include "ym/Literal.h"


BEGIN_NAMESPACE_YM_LOGIC

//////////////////////////////////////////////////////////////////////
/// @class AlgLitSet AlgLitSet.h "AlgLitSet.h"
/// @brief Literal の集合を表すクラス
///
/// AlgCube と異なり，相反するリテラル(x と x')を同時に含んでも良い．
//////////////////////////////////////////////////////////////////////
class AlgLitSet
{
  friend class AlgCube; // AlgCube::check_intersect() のため
  friend class AlgMgr;

public:

  /// @brief コンストラクタ
  /// @param[in] variable_num 変数の数
  ///
  /// * 空集合となる．
  explicit
  AlgLitSet(int variable_num);

  /// @brief シングルトンのコンストラクタ
  /// @param[in] variable_num 変数の数
  /// @param[in] lit リテラル
  AlgLitSet(int variable_num,
	    Literal lit);

  /// @brief 一般的なコンストラクタ
  /// @param[in] variable_num 変数の数
  /// @param[in] lit_list リテラルのリスト
  AlgLitSet(int variable_num,
	    const vector<Literal>& lit_list);

  /// @brief 一般的なコンストラクタ
  /// @param[in] variable_num 変数の数
  /// @param[in] lit_list リテラルのリスト
  AlgLitSet(int variable_num,
	    std::initializer_list<Literal>& lit_list);

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  AlgLitSet(const AlgLitSet& src);

  /// @brief コピー代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 代入後の自身への参照を返す．
  AlgLitSet&
  operator=(const AlgLitSet& src);

  /// @brief ムーブコンストラクタ
  /// @param[in] src ムーブ元のオブジェクト
  AlgLitSet(AlgLitSet&& src);

  /// @brief ムーブ代入演算子
  /// @param[in] src ムーブ元のオブジェクト
  /// @return 代入後の自身への参照を返す．
  AlgLitSet&
  operator=(AlgLitSet&& src);

  /// @brief デストラクタ
  ~AlgLitSet();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数の数を返す．
  int
  variable_num() const;

  /// @brief 該当するリテラルが含まれているか調べる．
  /// @param[in] lit 対象のリテラル
  /// @retval true 含まれている．
  /// @retval false 含まれていない．
  bool
  is_in(Literal lit) const;

  /// @brief ユニオン演算付き代入
  /// @param[in] right オペランド
  /// @return 自身への参照を返す．
  AlgLitSet&
  operator+=(const AlgLitSet& right);

  /// @brief 要素を足す．
  /// @param[in] lit 追加するリテラル
  /// @return 自身への参照を返す．
  AlgLitSet&
  operator+=(Literal lit);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数の数
  int mVariableNum;

  // 内容を表すビットベクタ
  AlgBitVect* mBody;

};

/// @brief AlgLitSet 同士の加算(ユニオン演算)
/// @param[in] left, right
/// @return 演算結果を返す．
AlgLitSet
operator+(const AlgLitSet& left,
	  const AlgLitSet& right);

/// @brief AlgLitSet 同士の加算(ユニオン演算)
/// @param[in] left, right
/// @return 演算結果を返す．
AlgLitSet
operator+(AlgLitSet&& left,
	  const AlgLitSet& right);

/// @brief AlgLitSet 同士の加算(ユニオン演算)
/// @param[in] left, right
/// @return 演算結果を返す．
AlgLitSet
operator+(const AlgLitSet& left,
	  AlgLitSet&& right);

/// @brief AlgLitSet 同士の加算(ユニオン演算)
/// @param[in] left, right
/// @return 演算結果を返す．
AlgLitSet
operator+(AlgLitSet&& left,
	  AlgLitSet&& right);

/// @brief 要素を足す．
/// @param[in] lit_set リテラル集合の本体
/// @param[in] lit 追加するリテラル
/// @return 演算結果を返す．
AlgLitSet
operator+(const AlgLitSet& lit_set,
	  Literal lit);

/// @brief 要素を足す．
/// @param[in] lit_set リテラル集合の本体
/// @param[in] lit 追加するリテラル
/// @return 演算結果を返す．
AlgLitSet
operator+(const AlgLitSet&& lit_set,
	  Literal lit);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 変数の数を返す．
inline
int
AlgLitSet::variable_num() const
{
  return mVariableNum;
}

// @brief AlgLitSet 同士の加算(ユニオン演算)
// @param[in] left, right
// @return 演算結果を返す．
inline
AlgLitSet
operator+(const AlgLitSet& left,
	  const AlgLitSet& right)
{
  return AlgLitSet(left).operator+=(right);
}

// @brief AlgLitSet 同士の加算(ユニオン演算)
// @param[in] left, right
// @return 演算結果を返す．
inline
AlgLitSet
operator+(AlgLitSet&& left,
	  const AlgLitSet& right)
{
  return AlgLitSet(left).operator+=(right);
}

// @brief AlgLitSet 同士の加算(ユニオン演算)
// @param[in] left, right
// @return 演算結果を返す．
inline
AlgLitSet
operator+(const AlgLitSet& left,
	  AlgLitSet&& right)
{
  return AlgLitSet(right).operator+=(left);
}

// @brief AlgLitSet 同士の加算(ユニオン演算)
// @param[in] left, right
// @return 演算結果を返す．
inline
AlgLitSet
operator+(AlgLitSet&& left,
	  AlgLitSet&& right)
{
  return AlgLitSet(left).operator+=(right);
}

// @brief 要素を足す．
// @param[in] lit_set リテラル集合の本体
// @param[in] lit 追加するリテラル
// @return 演算結果を返す．
inline
AlgLitSet
operator+(const AlgLitSet& lit_set,
	  Literal lit)
{
  return AlgLitSet(lit_set).operator+=(lit);
}

// @brief 要素を足す．
// @param[in] lit_set リテラル集合の本体
// @param[in] lit 追加するリテラル
// @return 演算結果を返す．
inline
AlgLitSet
operator+(const AlgLitSet&& lit_set,
	  Literal lit)
{
  return AlgLitSet(lit_set).operator+=(lit);
}

END_NAMESPACE_YM_LOGIC

#endif // ALGLITSET_H
