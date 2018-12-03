#ifndef YM_SOPCUBE_H
#define YM_SOPCUBE_H

/// @file ym/SopCube.h
/// @brief SopCube のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/Sop.h"
#include "ym/Literal.h"
#include "ym/HashFunc.h"


BEGIN_NAMESPACE_YM_LOGIC

//////////////////////////////////////////////////////////////////////
/// @ingroup SopGroup
/// @class SopCube SopCube.h "ym/SopCube.h"
/// @brief キューブ(積項)を表すクラス
///
/// * 離散数学的には Literal の集合だが，相反するリテラル(x と x')は
///   同時には含まない．
/// * 常に固定サイズのビット配列として実装する．
/// * 1つの変数につき2ビットを使用する．
//////////////////////////////////////////////////////////////////////
class SopCube
{
  friend class SopMgr;

public:

  /// @brief コンストラクタ
  /// @param[in] variable_num 変数の数
  ///
  /// * 空のキューブを作る．
  explicit
  SopCube(int variable_num);

  /// @brief コンストラクタ
  /// @param[in] variable_num 変数の数
  /// @param[in] lit リテラル
  ///
  /// * 単一のリテラルからなるキューブを作る．
  SopCube(int variable_num,
	  Literal lit);

  /// @brief コンストラクタ
  /// @param[in] variable_num 変数の数
  /// @param[in] lit_list キューブを表すリテラルのリスト
  SopCube(int variable_num,
	  const vector<Literal>& lit_list);

  /// @brief コンストラクタ
  /// @param[in] variable_num 変数の数
  /// @param[in] lit_list キューブを表すリテラルのリスト初期化子
  SopCube(int variable_num,
	  std::initializer_list<Literal>& lit_list);

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  SopCube(const SopCube& src);

  /// @brief コピー代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 代入後の自身への参照を返す．
  SopCube&
  operator=(const SopCube& src);

  /// @brief ムーブコンストラクタ
  /// @param[in] src ムーブ元のオブジェクト
  SopCube(SopCube&& src);

  /// @brief ムーブ代入演算子
  /// @param[in] src ムーブ元のオブジェクト
  /// @return 代入後の自身への参照を返す．
  SopCube&
  operator=(SopCube&& src);

  /// @brief デストラクタ
  ~SopCube();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数の数を返す．
  int
  variable_num() const;

  /// @brief リテラル数を返す．
  int
  literal_num() const;

  /// @brief 指定した変数のパタンを読み出す．
  /// @param[in] var 変数( 0 <= var_id.val() < variable_num() )
  /// @retval SopPat::_X その変数は現れない．
  /// @retval SopPat::_1 その変数が肯定のリテラルとして現れる．
  /// @retval SopPat::_0 その変数が否定のリテラルとして現れる．
  SopPat
  get_pat(VarId var) const;

  /// @brief 指定したリテラルを含んでいたら true を返す．
  /// @param[in] lit 対象のリテラル
  bool
  has_literal(Literal lit) const;

  /// @brief 内容をリテラルのリストに変換する．
  /// @param[in] lit_list 結果を格納するベクタ
  void
  to_literal_list(vector<Literal>& lit_list) const;

  /// @brief オペランドのキューブに含まれていたら true を返す．
  /// @param[in] right オペランドのキューブ
  ///
  /// * ここではキューブの表す論理関数の含意を考える．
  /// * だからリテラル集合としてはオペランドのキューブを含むことになる．
  bool
  check_containment(const SopCube& right) const;

  /// @brief 2つのキューブに共通なリテラルがあれば true を返す．
  /// @param[in] right オペランドのキューブ
  bool
  check_intersect(const SopCube& right) const;

  /// @brief キューブの論理積を計算する
  /// @param[in] right オペランド
  ///
  /// リテラル集合としてみると和集合となる<br>
  /// ただし，相反するリテラルが含まれていたら空キューブとなる．
  SopCube
  operator*(const SopCube& right) const;

  /// @brief 論理積を計算し自身に代入する．
  /// @param[in] right オペランドのキューブ
  /// @return 演算後の自身の参照を返す．
  ///
  /// リテラル集合とみなすとユニオンを計算することになる<br>
  /// ただし，相反するリテラルとの積があったら答は空のキューブとなる．
  SopCube&
  operator*=(const SopCube& right);

  /// @brief キューブとリテラルの論理積を計算する
  /// @param[in] right オペランドのリテラル
  ///
  /// リテラル集合としてみると和集合となる<br>
  /// ただし，相反するリテラルが含まれていたら空キューブとなる．
  SopCube
  operator*(Literal right) const;

  /// @brief リテラルとの論理積を計算し自身に代入する．
  /// @param[in] right オペランドのリテラル
  /// @return 演算後の自身の参照を返す．
  ///
  /// リテラル集合とみなすとユニオンを計算することになる<br>
  /// ただし，相反するリテラルとの積があったら答は空のキューブとなる．
  SopCube&
  operator*=(Literal right);

  /// @brief キューブによる商を計算する
  /// @param[in] right オペランド
  SopCube
  operator/(const SopCube& right) const;

  /// @brief キューブによる商を計算し自身に代入する．
  /// @param[in] right オペランドのキューブ
  /// @return 演算後の自身の参照を返す．
  ///
  /// リテラル集合として考えると集合差を計算することになる<br>
  /// ただし，right のみに含まれるリテラルがあったら結果は空となる．
  SopCube&
  operator/=(const SopCube& right);

  /// @brief リテラルによる商を計算する
  /// @param[in] right オペランドのリテラル
  SopCube
  operator/(Literal right) const;

  /// @brief リテラルによる商を計算し自身に代入する．
  /// @param[in] right オペランドのリテラル
  /// @return 演算後の自身の参照を返す．
  ///
  /// リテラル集合として考えると集合差を計算することになる<br>
  /// ただし，right のみに含まれるリテラルがあったら結果は空となる．
  SopCube&
  operator/=(Literal right);

  /// @breif SopBlock を返す．
  SopBlock
  block() const;

  /// @brief ハッシュ値を返す．
  SizeType
  hash() const;

  /// @brief 内容をわかりやすい形で出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] varname_list 変数名のリスト
  void
  print(ostream& s,
	const vector<string>& varname_list = vector<string>()) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を指定するコンストラクタ
  /// @param[in] variable_num 変数の数
  /// @param[in] body キューブのパタンを表す本体
  ///
  /// 危険なので普通は使わないように
  SopCube(int variable_num,
	  SopBitVect* body);

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
  compare(const SopCube& left,
	  const SopCube& right);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数の数
  int mVariableNum;

  // 内容を表すビットベクタ
  SopBitVect* mBody;

};

/// @relates SopCube
/// @brief キューブの論理積を計算する
/// @param[in] left, right オペランド
///
/// リテラル集合としてみると和集合となる<br>
/// ただし，相反するリテラルが含まれていたら空キューブとなる．
SopCube
operator*(SopCube&& left,
	  const SopCube& right);

/// @relates SopCube
/// @brief キューブの論理積を計算する
/// @param[in] left, right オペランド
///
/// リテラル集合としてみると和集合となる<br>
/// ただし，相反するリテラルが含まれていたら空キューブとなる．
SopCube
operator*(const SopCube& left,
	  SopCube&& right);

/// @relates SopCube
/// @brief キューブの論理積を計算する
/// @param[in] left, right オペランド
///
/// リテラル集合としてみると和集合となる<br>
/// ただし，相反するリテラルが含まれていたら空キューブとなる．
SopCube
operator*(SopCube&& left,
	  SopCube&& right);

/// @relates SopCube
/// @brief キューブとリテラルの論理積を計算する
/// @param[in] right オペランドのリテラル
///
/// リテラル集合としてみると和集合となる<br>
/// ただし，相反するリテラルが含まれていたら空キューブとなる．
SopCube
operator*(SopCube&& left,
	  Literal right);

/// @relates SopCube
/// @brief Literal と SopCube の論理積
/// @param[in] left 第1オペランド
/// @param[in] right 第2オペランド
/// @return 結果のキューブを返す．
SopCube
operator*(Literal left,
	  const SopCube& right);

/// @relates SopCube
/// @brief Literal と SopCube の論理積
/// @param[in] left 第1オペランド
/// @param[in] right 第2オペランド
/// @return 結果のキューブを返す．
SopCube
operator*(Literal left,
	  SopCube&& right);

/// @relates SopCube
/// @brief キューブの除算を計算する
/// @param[in] left, right オペランド
///
/// リテラル集合としてみると集合差となる<br>
SopCube
operator/(SopCube&& left,
	  const SopCube& right);

/// @relates SopCube
/// @brief キューブとリテラルの除算を計算する
/// @param[in] left, right オペランド
///
/// リテラル集合としてみると集合差となる<br>
SopCube
operator/(SopCube&& left,
	  Literal right);

/// @relates SopCube
/// @brief SopCubeの比較演算子
/// @param[in] left, right オペランド
/// @retval -1 left < right
/// @retval  0 left = right
/// @retval  1 left > right
int
compare(const SopCube& left,
	const SopCube& right);

/// @relates SopCube
/// @brief SopCubeの比較演算子(EQ)
/// @param[in] left, right オペランド
/// @retval true  left == right
/// @retval false left != right
bool
operator==(const SopCube& left,
	   const SopCube& right);

/// @relates SopCube
/// @brief SopCubeの比較演算子(NE)
/// @param[in] left, right オペランド
/// @retval true  left != right
/// @retval false left == right
bool
operator!=(const SopCube& left,
	   const SopCube& right);

/// @relates SopCube
/// @brief SopCubeの比較演算子(LT)
/// @param[in] left, right オペランド
/// @retval true  left < right
/// @retval false left >= right
bool
operator<(const SopCube& left,
	  const SopCube& right);

/// @relates SopCube
/// @brief SopCubeの比較演算子(GT)
/// @param[in] left, right オペランド
/// @retval true  left > right
/// @retval false left <= right
bool
operator>(const SopCube& left,
	  const SopCube& right);

/// @relates SopCube
/// @brief SopCubeの比較演算子(LE)
/// @param[in] left, right オペランド
/// @retval true  left < right
/// @retval false left >= right
bool
operator<=(const SopCube& left,
	   const SopCube& right);

/// @relates SopCube
/// @brief SopCubeの比較演算子(GE)
/// @param[in] left, right オペランド
/// @retval true  left < right
/// @retval false left >= right
bool
operator>=(const SopCube& left,
	   const SopCube& right);

/// @relates SopCube
/// @brief SopCube の内容を出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] cube 対象のキューブ(のポインタ)
///
/// cube->print(s) と等価
ostream&
operator<<(ostream& s,
	   const SopCube& cube);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 変数の数を返す．
inline
int
SopCube::variable_num() const
{
  return mVariableNum;
}

// @relates SopCube
// @brief キューブの論理積を計算する
// @param[in] left, right オペランド
//
// リテラル集合としてみると和集合となる<br>
// ただし，相反するリテラルが含まれていたら空キューブとなる．
inline
SopCube
operator*(SopCube&& left,
	  const SopCube& right)
{
  return SopCube(std::move(left)).operator*=(right);
}

// @relates SopCube
// @brief キューブの論理積を計算する
// @param[in] left, right オペランド
//
// リテラル集合としてみると和集合となる<br>
// ただし，相反するリテラルが含まれていたら空キューブとなる．
inline
SopCube
operator*(const SopCube& left,
	  SopCube&& right)
{
  return SopCube(std::move(right)).operator*=(left);
}

// @relates SopCube
// @brief キューブの論理積を計算する
// @param[in] left, right オペランド
//
// リテラル集合としてみると和集合となる<br>
// ただし，相反するリテラルが含まれていたら空キューブとなる．
inline
SopCube
operator*(SopCube&& left,
	  SopCube&& right)
{
  return SopCube(std::move(left)).operator*=(right);
}

// @relates SopCube
// @brief キューブとリテラルの論理積を計算する
// @param[in] right オペランドのリテラル
//
// リテラル集合としてみると和集合となる<br>
// ただし，相反するリテラルが含まれていたら空キューブとなる．
inline
SopCube
operator*(SopCube&& left,
	  Literal right)
{
  return SopCube(std::move(left)).operator*=(right);
}

// @relates SopCube
// @brief Literal と SopCube の論理積
// @param[in] left 第1オペランド
// @param[in] right 第2オペランド
// @return 結果のキューブを返す．
inline
SopCube
operator*(Literal left,
	  const SopCube& right)
{
  // 交換則を用いる．
  return right.operator*(left);
}

// @relates SopCube
// @brief Literal と SopCube の論理積
// @param[in] left 第1オペランド
// @param[in] right 第2オペランド
// @return 結果のキューブを返す．
inline
SopCube
operator*(Literal left,
	  SopCube&& right)
{
  // 交換則を用いる．
  return SopCube(std::move(right)).operator*=(left);
}

// @relates SopCube
// @brief キューブの除算を計算する
// @param[in] left, right オペランド
//
// リテラル集合としてみると集合差となる<br>
inline
SopCube
operator/(SopCube&& left,
	  const SopCube& right)
{
  return SopCube(std::move(left)).operator/=(right);
}

// @relates SopCube
// @brief キューブとリテラルの除算を計算する
// @param[in] left, right オペランド
//
// リテラル集合としてみると集合差となる<br>
inline
SopCube
operator/(SopCube&& left,
	  Literal right)
{
  return SopCube(std::move(left)).operator/=(right);
}

// @relates SopCube
// @brief SopCubeの比較演算子(EQ)
// @param[in] left, right オペランド
// @retval true  left == right
// @retval false left != right
inline
bool
operator==(const SopCube& left,
	   const SopCube& right)
{
  return compare(left, right) == 0;
}

// @relates SopCube
// @brief SopCubeの比較演算子(NE)
// @param[in] left, right オペランド
// @retval true  left != right
// @retval false left == right
inline
bool
operator!=(const SopCube& left,
	   const SopCube& right)
{
  return compare(left, right) != 0;
}

// @brief SopCubeの比較演算子(LT)
// @param[in] left, right オペランド
// @retval true  left < right
// @retval false left >= right
inline
bool
operator<(const SopCube& left,
	  const SopCube& right)
{
  return compare(left, right) < 0;
}

// @brief SopCubeの比較演算子(GT)
// @param[in] left, right オペランド
// @retval true  left > right
// @retval false left <= right
inline
bool
operator>(const SopCube& left,
	  const SopCube& right)
{
  return compare(left, right) > 0;
}

// @brief SopCubeの比較演算子(LE)
// @param[in] left, right オペランド
// @retval true  left < right
// @retval false left >= right
inline
bool
operator<=(const SopCube& left,
	   const SopCube& right)
{
  return compare(left, right) <= 0;
}

// @brief SopCubeの比較演算子(GE)
// @param[in] left, right オペランド
// @retval true  left < right
// @retval false left >= right
inline
bool
operator>=(const SopCube& left,
	   const SopCube& right)
{
  return compare(left, right) >= 0;
}

// @brief SopCube の内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] cube 対象のキューブ
//
// cube.print(s) と等価
inline
ostream&
operator<<(ostream& s,
	   const SopCube& cube)
{
  cube.print(s);
  return s;
}

END_NAMESPACE_YM_LOGIC

BEGIN_NAMESPACE_YM

/// @breif SopCube をキーにしたハッシュ関数クラスの定義
template <>
struct HashFunc<SopCube>
{
  SizeType
  operator()(const SopCube& cube) const
  {
    return cube.hash();
  }
};

END_NAMESPACE_YM

#endif // YM_SOPCUBE_H
