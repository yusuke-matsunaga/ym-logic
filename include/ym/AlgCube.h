#ifndef YM_ALGCUBE_H
#define YM_ALGCUBE_H

/// @file ym/AlgCube.h
/// @brief AlgCube のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/Alg.h"
#include "ym/Literal.h"
#include "ym/HashFunc.h"


BEGIN_NAMESPACE_YM_LOGIC

//////////////////////////////////////////////////////////////////////
/// @ingroup AlgGroup
/// @class AlgCube AlgCube.h "ym/AlgCube.h"
/// @brief キューブ(積項)を表すクラス
///
/// * 離散数学的には Literal の集合だが，相反するリテラル(x と x')は
///   同時には含まない．
/// * 常に固定サイズのビット配列として実装する．
/// * 1つの変数につき2ビットを使用する．
//////////////////////////////////////////////////////////////////////
class AlgCube
{
  friend class AlgCover;
  friend class AlgMgr;

public:

  /// @brief コンストラクタ
  /// @param[in] variable_num 変数の数
  ///
  /// * 空のキューブを作る．
  explicit
  AlgCube(int variable_num);

  /// @brief コンストラクタ
  /// @param[in] variable_num 変数の数
  /// @param[in] lit リテラル
  ///
  /// * 単一のリテラルからなるキューブを作る．
  AlgCube(int variable_num,
	  Literal lit);

  /// @brief コンストラクタ
  /// @param[in] variable_num 変数の数
  /// @param[in] lit_list キューブを表すリテラルのリスト
  AlgCube(int variable_num,
	  const vector<Literal>& lit_list);

  /// @brief コンストラクタ
  /// @param[in] variable_num 変数の数
  /// @param[in] lit_list キューブを表すリテラルのリスト初期化子
  AlgCube(int variable_num,
	  std::initializer_list<Literal> lit_list);

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  AlgCube(const AlgCube& src);

  /// @brief コピー代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 代入後の自身への参照を返す．
  AlgCube&
  operator=(const AlgCube& src);

  /// @brief ムーブコンストラクタ
  /// @param[in] src ムーブ元のオブジェクト
  AlgCube(AlgCube&& src);

  /// @brief ムーブ代入演算子
  /// @param[in] src ムーブ元のオブジェクト
  /// @return 代入後の自身への参照を返す．
  AlgCube&
  operator=(AlgCube&& src);

  /// @brief デストラクタ
  ~AlgCube();


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

  /// @brief 内容をリテラルのリストに変換する．
  /// @param[in] lit_list 結果を格納するベクタ
  void
  to_literal_list(vector<Literal>& lit_list) const;

  /// @brief 指定したリテラルを含んでいたら true を返す．
  /// @param[in] lit 対象のリテラル
  bool
  has_literal(Literal lit) const;

  /// @brief オペランドのキューブに含まれていたら true を返す．
  /// @param[in] right オペランドのキューブ
  ///
  /// ここではキューブの表す論理関数の含意を考える<br>
  /// だからリテラル集合としてはオペランドのキューブを含むことになる．
  bool
  check_containment(const AlgCube& right) const;

  /// @brief 2つのキューブに共通なリテラルがあれば true を返す．
  /// @param[in] right オペランドのキューブ
  bool
  check_intersect(const AlgCube& right) const;

  /// @brief 引数のリテラルをひとつでも含んでいたら true を返す．
  /// @param[in] right 対象のリテラル集合
  bool
  contains(const AlgLitSet& right) const;

  /// @brief キューブの論理積を計算する
  /// @param[in] right オペランド
  ///
  /// リテラル集合としてみると和集合となる<br>
  /// ただし，相反するリテラルが含まれていたら空キューブとなる．
  AlgCube
  operator*(const AlgCube& right) const;

  /// @brief 論理積を計算し自身に代入する．
  /// @param[in] right オペランドのキューブ
  /// @return 演算後の自身の参照を返す．
  ///
  /// リテラル集合とみなすとユニオンを計算することになる<br>
  /// ただし，相反するリテラルとの積があったら答は空のキューブとなる．
  AlgCube&
  operator*=(const AlgCube& right);

  /// @brief キューブとリテラルの論理積を計算する
  /// @param[in] right オペランドのリテラル
  ///
  /// リテラル集合としてみると和集合となる<br>
  /// ただし，相反するリテラルが含まれていたら空キューブとなる．
  AlgCube
  operator*(Literal right) const;

  /// @brief リテラルとの論理積を計算し自身に代入する．
  /// @param[in] right オペランドのリテラル
  /// @return 演算後の自身の参照を返す．
  ///
  /// リテラル集合とみなすとユニオンを計算することになる<br>
  /// ただし，相反するリテラルとの積があったら答は空のキューブとなる．
  AlgCube&
  operator*=(Literal right);

  /// @brief キューブによる商を計算する
  /// @param[in] right オペランド
  AlgCube
  operator/(const AlgCube& right) const;

  /// @brief キューブによる商を計算し自身に代入する．
  /// @param[in] right オペランドのキューブ
  /// @return 演算後の自身の参照を返す．
  ///
  /// リテラル集合として考えると集合差を計算することになる<br>
  /// ただし，right のみに含まれるリテラルがあったら結果は空となる．
  AlgCube&
  operator/=(const AlgCube& right);

  /// @brief リテラルによる商を計算する
  /// @param[in] right オペランドのリテラル
  AlgCube
  operator/(Literal right) const;

  /// @brief リテラルによる商を計算し自身に代入する．
  /// @param[in] right オペランドのリテラル
  /// @return 演算後の自身の参照を返す．
  ///
  /// リテラル集合として考えると集合差を計算することになる<br>
  /// ただし，right のみに含まれるリテラルがあったら結果は空となる．
  AlgCube&
  operator/=(Literal right);

  /// @breif AlgBlock を返す．
  AlgBlock
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
  AlgCube(int variable_num,
	  AlgBitVect* body);

  /// @brief mBody を削除する．
  void
  delete_body();

  // friend 関数の宣言
  friend
  int
  compare(const AlgCube& left,
	  const AlgCube& right);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数の数
  int mVariableNum;

  // 内容を表すビットベクタ
  AlgBitVect* mBody;

};

/// @relates AlgCube
/// @brief Literal と AlgCube の論理積
/// @param[in] left 第1オペランド
/// @param[in] right 第2オペランド
/// @return 結果のキューブを返す．
AlgCube
operator*(Literal left,
	  const AlgCube& right);

/// @relates AlgCube
/// @brief AlgCubeの比較演算子
/// @param[in] left, right オペランド
/// @retval -1 left < right
/// @retval  0 left = right
/// @retval  1 left > right
int
compare(const AlgCube& left,
	const AlgCube& right);

/// @relates AlgCube
/// @brief AlgCubeの比較演算子(EQ)
/// @param[in] left, right オペランド
/// @retval true  left == right
/// @retval false left != right
bool
operator==(const AlgCube& left,
	   const AlgCube& right);

/// @relates AlgCube
/// @brief AlgCubeの比較演算子(NE)
/// @param[in] left, right オペランド
/// @retval true  left != right
/// @retval false left == right
bool
operator!=(const AlgCube& left,
	   const AlgCube& right);

/// @relates AlgCube
/// @brief AlgCubeの比較演算子(LT)
/// @param[in] left, right オペランド
/// @retval true  left < right
/// @retval false left >= right
bool
operator<(const AlgCube& left,
	  const AlgCube& right);

/// @relates AlgCube
/// @brief AlgCubeの比較演算子(GT)
/// @param[in] left, right オペランド
/// @retval true  left > right
/// @retval false left <= right
bool
operator>(const AlgCube& left,
	  const AlgCube& right);

/// @relates AlgCube
/// @brief AlgCubeの比較演算子(LE)
/// @param[in] left, right オペランド
/// @retval true  left < right
/// @retval false left >= right
bool
operator<=(const AlgCube& left,
	   const AlgCube& right);

/// @relates AlgCube
/// @brief AlgCubeの比較演算子(GE)
/// @param[in] left, right オペランド
/// @retval true  left < right
/// @retval false left >= right
bool
operator>=(const AlgCube& left,
	   const AlgCube& right);

/// @relates AlgCube
/// @brief AlgCube の内容を出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] cube 対象のキューブ(のポインタ)
///
/// cube->print(s) と等価
ostream&
operator<<(ostream& s,
	   const AlgCube& cube);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 変数の数を返す．
inline
int
AlgCube::variable_num() const
{
  return mVariableNum;
}

// @relates AlgCube
// @brief Literal と AlgCube の論理積
// @param[in] left 第1オペランド
// @param[in] right 第2オペランド
// @return 結果のキューブを返す．
inline
AlgCube
operator*(Literal left,
	  const AlgCube& right)
{
  // 交換則を用いる．
  return right.operator*(left);
}

// @relates AlgCube
// @brief AlgCubeの比較演算子(EQ)
// @param[in] left, right オペランド
// @retval true  left == right
// @retval false left != right
inline
bool
operator==(const AlgCube& left,
	   const AlgCube& right)
{
  return compare(left, right) == 0;
}

// @relates AlgCube
// @brief AlgCubeの比較演算子(NE)
// @param[in] left, right オペランド
// @retval true  left != right
// @retval false left == right
inline
bool
operator!=(const AlgCube& left,
	   const AlgCube& right)
{
  return compare(left, right) != 0;
}

// @brief AlgCubeの比較演算子(LT)
// @param[in] left, right オペランド
// @retval true  left < right
// @retval false left >= right
inline
bool
operator<(const AlgCube& left,
	  const AlgCube& right)
{
  return compare(left, right) < 0;
}

// @brief AlgCubeの比較演算子(GT)
// @param[in] left, right オペランド
// @retval true  left > right
// @retval false left <= right
inline
bool
operator>(const AlgCube& left,
	  const AlgCube& right)
{
  return compare(left, right) > 0;
}

// @brief AlgCubeの比較演算子(LE)
// @param[in] left, right オペランド
// @retval true  left < right
// @retval false left >= right
inline
bool
operator<=(const AlgCube& left,
	   const AlgCube& right)
{
  return compare(left, right) <= 0;
}

// @brief AlgCubeの比較演算子(GE)
// @param[in] left, right オペランド
// @retval true  left < right
// @retval false left >= right
inline
bool
operator>=(const AlgCube& left,
	   const AlgCube& right)
{
  return compare(left, right) >= 0;
}

// @brief AlgCube の内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] cube 対象のキューブ
//
// cube.print(s) と等価
inline
ostream&
operator<<(ostream& s,
	   const AlgCube& cube)
{
  cube.print(s);
  return s;
}

END_NAMESPACE_YM_LOGIC

BEGIN_NAMESPACE_YM

/// @breif AlgCube をキーにしたハッシュ関数クラスの定義
template <>
struct HashFunc<AlgCube>
{
  SizeType
  operator()(const AlgCube& cube) const
  {
    return cube.hash();
  }
};

END_NAMESPACE_YM

#endif // YM_ALGCUBE_H
