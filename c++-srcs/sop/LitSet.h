#ifndef LITSET_H
#define LITSET_H

/// @file LitSet.h
/// @brief LitSet のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/Sop.h"
#include "ym/Literal.h"


BEGIN_NAMESPACE_YM_LOGIC

//////////////////////////////////////////////////////////////////////
/// @class LitSet LitSet.h "LitSet.h"
/// @brief Literal の集合を表すクラス
///
/// SopCube と異なり，相反するリテラル(x と x')を同時に含んでも良い．
//////////////////////////////////////////////////////////////////////
class LitSet
{
  //friend class AlgMgr;

public:

  /// @brief コンストラクタ
  /// @param[in] variable_num 変数の数
  ///
  /// * 空集合となる．
  explicit
  LitSet(int variable_num);

  /// @brief シングルトンのコンストラクタ
  /// @param[in] variable_num 変数の数
  /// @param[in] lit リテラル
  LitSet(int variable_num,
	    Literal lit);

  /// @brief 一般的なコンストラクタ
  /// @param[in] variable_num 変数の数
  /// @param[in] lit_list リテラルのリスト
  LitSet(int variable_num,
	    const vector<Literal>& lit_list);

  /// @brief 一般的なコンストラクタ
  /// @param[in] variable_num 変数の数
  /// @param[in] lit_list リテラルのリスト
  LitSet(int variable_num,
	 initializer_list<Literal>& lit_list);

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  LitSet(const LitSet& src);

  /// @brief コピー代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 代入後の自身への参照を返す．
  LitSet&
  operator=(const LitSet& src);

  /// @brief ムーブコンストラクタ
  /// @param[in] src ムーブ元のオブジェクト
  LitSet(LitSet&& src);

  /// @brief ムーブ代入演算子
  /// @param[in] src ムーブ元のオブジェクト
  /// @return 代入後の自身への参照を返す．
  LitSet&
  operator=(LitSet&& src);

  /// @brief デストラクタ
  ~LitSet();


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
  LitSet&
  operator+=(const LitSet& right);

  /// @brief 要素を足す．
  /// @param[in] lit 追加するリテラル
  /// @return 自身への参照を返す．
  LitSet&
  operator+=(Literal lit);

  /// @brief 引数のキューブ中のリテラルをひとつでも含んでいたら true を返す．
  /// @param[in] right 対象のキューブ
  bool
  check_intersect(const SopCube& right) const;


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
  SopBitVect* mBody;

};

/// @brief LitSet 同士の加算(ユニオン演算)
/// @param[in] left, right
/// @return 演算結果を返す．
LitSet
operator+(const LitSet& left,
	  const LitSet& right);

/// @brief LitSet 同士の加算(ユニオン演算)
/// @param[in] left, right
/// @return 演算結果を返す．
LitSet
operator+(LitSet&& left,
	  const LitSet& right);

/// @brief LitSet 同士の加算(ユニオン演算)
/// @param[in] left, right
/// @return 演算結果を返す．
LitSet
operator+(const LitSet& left,
	  LitSet&& right);

/// @brief LitSet 同士の加算(ユニオン演算)
/// @param[in] left, right
/// @return 演算結果を返す．
LitSet
operator+(LitSet&& left,
	  LitSet&& right);

/// @brief 要素を足す．
/// @param[in] lit_set リテラル集合の本体
/// @param[in] lit 追加するリテラル
/// @return 演算結果を返す．
LitSet
operator+(const LitSet& lit_set,
	  Literal lit);

/// @brief 要素を足す．
/// @param[in] lit_set リテラル集合の本体
/// @param[in] lit 追加するリテラル
/// @return 演算結果を返す．
LitSet
operator+(const LitSet&& lit_set,
	  Literal lit);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 変数の数を返す．
inline
int
LitSet::variable_num() const
{
  return mVariableNum;
}

// @brief LitSet 同士の加算(ユニオン演算)
// @param[in] left, right
// @return 演算結果を返す．
inline
LitSet
operator+(const LitSet& left,
	  const LitSet& right)
{
  return LitSet(left).operator+=(right);
}

// @brief LitSet 同士の加算(ユニオン演算)
// @param[in] left, right
// @return 演算結果を返す．
inline
LitSet
operator+(LitSet&& left,
	  const LitSet& right)
{
  return LitSet(left).operator+=(right);
}

// @brief LitSet 同士の加算(ユニオン演算)
// @param[in] left, right
// @return 演算結果を返す．
inline
LitSet
operator+(const LitSet& left,
	  LitSet&& right)
{
  return LitSet(right).operator+=(left);
}

// @brief LitSet 同士の加算(ユニオン演算)
// @param[in] left, right
// @return 演算結果を返す．
inline
LitSet
operator+(LitSet&& left,
	  LitSet&& right)
{
  return LitSet(left).operator+=(right);
}

// @brief 要素を足す．
// @param[in] lit_set リテラル集合の本体
// @param[in] lit 追加するリテラル
// @return 演算結果を返す．
inline
LitSet
operator+(const LitSet& lit_set,
	  Literal lit)
{
  return LitSet(lit_set).operator+=(lit);
}

// @brief 要素を足す．
// @param[in] lit_set リテラル集合の本体
// @param[in] lit 追加するリテラル
// @return 演算結果を返す．
inline
LitSet
operator+(const LitSet&& lit_set,
	  Literal lit)
{
  return LitSet(lit_set).operator+=(lit);
}

END_NAMESPACE_YM_LOGIC

#endif // LITSET_H
