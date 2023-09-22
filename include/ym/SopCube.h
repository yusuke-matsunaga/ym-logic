#ifndef YM_SOPCUBE_H
#define YM_SOPCUBE_H

/// @file ym/SopCube.h
/// @brief SopCube のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/Expr.h"
#include "ym/Literal.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
/// @ingroup SopGroup
/// @class SopCube SopCube.h "ym/SopCube.h"
/// @brief キューブ(積項)を表すクラス
///
/// * 離散数学的には Literal の集合だが，相反するリテラル(x と x')は
///   同時には含まない．
/// * 常に固定サイズのビット配列として実装する．
/// * そのため初期化の際に変数の数が必要となる．
/// * 1つの変数につき2ビットを使用する．
//////////////////////////////////////////////////////////////////////
class SopCube
{
  friend class SopMgr;

public:

  /// @brief コンストラクタ
  ///
  /// * 空のキューブを作る．
  explicit
  SopCube(
    SizeType variable_num ///< [in] 変数の数
  );

  /// @brief コンストラクタ
  ///
  /// * 単一のリテラルからなるキューブを作る．
  SopCube(
    SizeType variable_num, ///< [in] 変数の数
    Literal lit            ///< [in] リテラル
  );

  /// @brief コンストラクタ
  SopCube(
    SizeType variable_num,          ///< [in] 変数の数
    const vector<Literal>& lit_list ///< [in] キューブを表すリテラルのリスト
  );

  /// @brief コンストラクタ
  SopCube(
    SizeType variable_num,              ///< [in] 変数の数
    initializer_list<Literal>& lit_list ///< [in] キューブを表すリテラルのリスト初期化子
  );

  /// @brief コピーコンストラクタ
  SopCube(
    const SopCube& src ///< [in] コピー元のオブジェクト
  );

  /// @brief コピー代入演算子
  /// @return 代入後の自身への参照を返す．
  SopCube&
  operator=(
    const SopCube& src ///< [in] コピー元のオブジェクト
  );

  /// @brief ムーブコンストラクタ
  SopCube(
    SopCube&& src ///< [in] ムーブ元のオブジェクト
  );

  /// @brief ムーブ代入演算子
  /// @return 代入後の自身への参照を返す．
  SopCube&
  operator=(
    SopCube&& src ///< [in] ムーブ元のオブジェクト
  );

  /// @brief デストラクタ
  ~SopCube();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数の数を返す．
  SizeType
  variable_num() const
  {
    return mVariableNum;
  }

  /// @brief リテラル数を返す．
  SizeType
  literal_num() const;

  /// @brief 空のキューブの時 true を返す．
  bool
  is_tautology() const;

  /// @brief 指定した変数のパタンを読み出す．
  /// @retval SopPat::_X その変数は現れない．
  /// @retval SopPat::_1 その変数が肯定のリテラルとして現れる．
  /// @retval SopPat::_0 その変数が否定のリテラルとして現れる．
  SopPat
  get_pat(
    SizeType var ///< [in] 変数( 0 <= var_id.val() < variable_num() )
  ) const;

  /// @brief 指定したリテラルを含んでいたら true を返す．
  bool
  check_literal(
    Literal lit ///< [in] 対象のリテラル
  ) const;

  /// @brief 内容をリテラルのリストに変換する．
  vector<Literal>
  literal_list() const;

  /// @brief オペランドのキューブに含まれていたら true を返す．
  ///
  /// * ここではキューブの表す論理関数の含意を考える．
  /// * だからリテラル集合としてはオペランドのキューブを含むことになる．
  bool
  check_containment(
    const SopCube& right ///< [in] オペランドのキューブ
  ) const;

  /// @brief 2つのキューブに共通なリテラルがあれば true を返す．
  bool
  check_intersect(
    const SopCube& right ///< [in] オペランドのキューブ
  ) const;

  /// @brief キューブの論理積を計算する
  ///
  /// リテラル集合としてみると和集合となる<br>
  /// ただし，相反するリテラルが含まれていたら空キューブとなる．
  SopCube
  operator*(
    const SopCube& right ///< [in] オペランド
  ) const;

  /// @brief 論理積を計算し自身に代入する．
  /// @return 演算後の自身の参照を返す．
  ///
  /// リテラル集合とみなすとユニオンを計算することになる<br>
  /// ただし，相反するリテラルとの積があったら答は空のキューブとなる．
  SopCube&
  operator*=(
    const SopCube& right ///< [in] オペランドのキューブ
  );

  /// @brief キューブとリテラルの論理積を計算する
  ///
  /// リテラル集合としてみると和集合となる<br>
  /// ただし，相反するリテラルが含まれていたら空キューブとなる．
  SopCube
  operator*(
    Literal right ///< [in] オペランドのリテラル
  ) const;

  /// @brief リテラルとの論理積を計算し自身に代入する．
  /// @return 演算後の自身の参照を返す．
  ///
  /// リテラル集合とみなすとユニオンを計算することになる<br>
  /// ただし，相反するリテラルとの積があったら答は空のキューブとなる．
  SopCube&
  operator*=(
    Literal right ///< [in] オペランドのリテラル
  );

  /// @brief キューブによる商を計算する
  ///
  /// リテラル集合として考えると集合差を計算することになる<br>
  /// ただし，right のみに含まれるリテラルがあったら結果は空となる．
  SopCube
  operator/(
    const SopCube& right ///< [in] オペランド
  ) const;

  /// @brief キューブによる商を計算し自身に代入する．
  /// @return 演算後の自身の参照を返す．
  ///
  /// リテラル集合として考えると集合差を計算することになる<br>
  /// ただし，right のみに含まれるリテラルがあったら結果は空となる．
  SopCube&
  operator/=(
    const SopCube& right ///< [in] オペランドのキューブ
  );

  /// @brief リテラルによる商を計算する
  ///
  /// リテラル集合として考えると集合差を計算することになる<br>
  /// ただし，right のみに含まれるリテラルがあったら結果は空となる．
  SopCube
  operator/(
    Literal right ///< [in] オペランドのリテラル
  ) const;

  /// @brief リテラルによる商を計算し自身に代入する．
  /// @return 演算後の自身の参照を返す．
  ///
  /// リテラル集合として考えると集合差を計算することになる<br>
  /// ただし，right のみに含まれるリテラルがあったら結果は空となる．
  SopCube&
  operator/=(
    Literal right ///< [in] オペランドのリテラル
  );

  /// @brief Expr に変換する．
  Expr
  expr() const;

  /// @breif SopBlock に変換する．
  ///
  /// variable_num() の情報は失われる．
  SopBlock
  block() const;

  /// @brief ハッシュ値を返す．
  SizeType
  hash() const;

  /// @brief 内容をわかりやすい形で出力する．
  void
  print(
    ostream& s,                             ///< [in] 出力先のストリーム
    const vector<string>& varname_list = {} ///< [in] 変数名のリスト
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を指定するコンストラクタ
  SopCube(
    SizeType variable_num, ///< [in] 変数の数
    SopBitVect* body       ///< [in] キューブのパタンを表す本体
  );

  /// @brief SopBlock からのムーブコンストラクタ
  SopCube(
    SizeType variable_num, ///< [in] 変数の数
    SopBlock&& block       ///< [in] 変換元ののブロック
  );

  /// @brief mBody を削除する．
  void
  delete_body();


private:
  //////////////////////////////////////////////////////////////////////
  // friend 関数の宣言
  // private: 指定は実は関係ない．
  //////////////////////////////////////////////////////////////////////

  friend
  int
  compare(
    const SopCube& left,
    const SopCube& right
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数の数
  SizeType mVariableNum;

  // 内容を表すビットベクタ
  SopBitVect* mBody;

};

/// @relates SopCube
/// @brief キューブの論理積を計算する
///
/// リテラル集合としてみると和集合となる<br>
/// ただし，相反するリテラルが含まれていたら空キューブとなる．
inline
SopCube
operator*(
  SopCube&& left,      ///< [in] 第1オペランド
  const SopCube& right ///< [in] 第2オペランド
)
{
  return SopCube{move(left)}.operator*=(right);
}

/// @relates SopCube
/// @brief キューブの論理積を計算する
///
/// リテラル集合としてみると和集合となる<br>
/// ただし，相反するリテラルが含まれていたら空キューブとなる．
inline
SopCube
operator*(
  const SopCube& left, ///< [in] 第1オペランド
  SopCube&& right      ///< [in] 第2オペランド
)
{
  return SopCube{move(right)}.operator*=(left);
}

/// @relates SopCube
/// @brief キューブの論理積を計算する
///
/// リテラル集合としてみると和集合となる<br>
/// ただし，相反するリテラルが含まれていたら空キューブとなる．
inline
SopCube
operator*(
  SopCube&& left, ///< [in] 第1オペランド
  SopCube&& right ///< [in] 第2オペランド
)
{
  return SopCube{move(left)}.operator*=(right);
}

/// @relates SopCube
/// @brief キューブとリテラルの論理積を計算する
///
/// リテラル集合としてみると和集合となる<br>
/// ただし，相反するリテラルが含まれていたら空キューブとなる．
inline
SopCube
operator*(
  SopCube&& left, ///< [in] 第1オペランド
  Literal right	  ///< [in] 第2オペランド
)
{
  return SopCube{move(left)}.operator*=(right);
}

/// @relates SopCube
/// @brief Literal と SopCube の論理積
/// @return 結果のキューブを返す．
inline
SopCube
operator*(
  Literal left,        ///< [in] 第1オペランド
  const SopCube& right ///< [in] 第2オペランド
)
{
  // 交換則を用いる．
  return right.operator*(left);
}

/// @relates SopCube
/// @brief Literal と SopCube の論理積
/// @return 結果のキューブを返す．
inline
SopCube
operator*(
  Literal left,    ///< [in] 第1オペランド
  SopCube&& right  ///< [in] 第2オペランド
)
{
  // 交換則を用いる．
  return SopCube{move(right)}.operator*=(left);
}

/// @relates SopCube
/// @brief キューブの除算を計算する
///
/// リテラル集合としてみると集合差となる<br>
inline
SopCube
operator/(
  SopCube&& left,      ///< [in] 第1オペランド
  const SopCube& right ///< [in] 第2オペランド
)
{
  return SopCube{move(left)}.operator/=(right);
}

/// @relates SopCube
/// @brief キューブとリテラルの除算を計算する
///
/// リテラル集合としてみると集合差となる<br>
inline
SopCube
operator/(
  SopCube&& left, ///< [in] 第1オペランド
  Literal right	  ///< [in] 第2オペランド
)
{
  return SopCube{move(left)}.operator/=(right);
}

/// @relates SopCube
/// @brief SopCubeの比較演算子
/// @retval -1 left < right
/// @retval  0 left = right
/// @retval  1 left > right
int
compare(
  const SopCube& left, ///< [in] 第1オペランド
  const SopCube& right ///< [in] 第2オペランド
);

/// @relates SopCube
/// @brief SopCubeの比較演算子(EQ)
/// @retval true  left == right
/// @retval false left != right
inline
bool
operator==(
  const SopCube& left, ///< [in] 第1オペランド
  const SopCube& right ///< [in] 第2オペランド
)
{
  return compare(left, right) == 0;
}

/// @relates SopCube
/// @brief SopCubeの比較演算子(NE)
/// @retval true  left != right
/// @retval false left == right
inline
bool
operator!=(
  const SopCube& left, ///< [in] 第1オペランド
  const SopCube& right ///< [in] 第2オペランド
)
{
  return compare(left, right) != 0;
}

/// @relates SopCube
/// @brief SopCubeの比較演算子(LT)
/// @retval true  left < right
/// @retval false left >= right
inline
bool
operator<(
  const SopCube& left, ///< [in] 第1オペランド
  const SopCube& right ///< [in] 第2オペランド
)
{
  return compare(left, right) < 0;
}

/// @relates SopCube
/// @brief SopCubeの比較演算子(GT)
/// @retval true  left > right
/// @retval false left <= right
inline
bool
operator>(
  const SopCube& left, ///< [in] 第1オペランド
  const SopCube& right ///< [in] 第2オペランド
)
{
  return compare(left, right) > 0;
}

/// @relates SopCube
/// @brief SopCubeの比較演算子(LE)
/// @retval true  left < right
/// @retval false left >= right
inline
bool
operator<=(
  const SopCube& left, ///< [in] 第1オペランド
  const SopCube& right ///< [in] 第2オペランド
)
{
  return compare(left, right) <= 0;
}

/// @relates SopCube
/// @brief SopCubeの比較演算子(GE)
/// @retval true  left < right
/// @retval false left >= right
inline
bool
operator>=(
  const SopCube& left, ///< [in] 第1オペランド
  const SopCube& right ///< [in] 第2オペランド
)
{
  return compare(left, right) >= 0;
}

/// @relates SopCube
/// @brief SopCube の内容を出力する．
///
/// cube->print(s) と等価
inline
ostream&
operator<<(
  ostream& s,         ///< [in] 出力先のストリーム
  const SopCube& cube ///< [in] 対象のキューブ(のポインタ)
)
{
  cube.print(s);
  return s;
}

END_NAMESPACE_YM_SOP

BEGIN_NAMESPACE_STD

/// @breif SopCube をキーにしたハッシュ関数クラスの定義
template <>
struct hash<YM_NAMESPACE::SopCube>
{
  SizeType
  operator()(const YM_NAMESPACE::SopCube& cube) const
  {
    return cube.hash();
  }
};

END_NAMESPACE_STD

#endif // YM_SOPCUBE_H
