#ifndef YM_ALGCOVER_H
#define YM_ALGCOVER_H

/// @file ym/AlgCover.h
/// @brief AlgCover のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/Alg.h"
#include "ym/Literal.h"
#include "ym/HashFunc.h"


BEGIN_NAMESPACE_YM_LOGIC

//////////////////////////////////////////////////////////////////////
/// @class AlgCover AlgCover.h "ym/AlgCover.h"
/// @brief カバー(積和形論理式)を表すクラス
///
/// 中身は positional cube matrix 表現を用いたビットベクタ<br>
/// ビットベクタの内容は一旦，設定されたら変わることはない<br>
/// キューブはワードレベルの順序で整列しているものと仮定している<br>
/// この式は代数的(algebraic)なので他のキューブに包含されるキューブは
/// 許されない．もちろん重複したキューブも許されない．<br>
/// 通常の操作を行っているかぎり代数的な式しか生成されない．<br>
/// 実際の操作は AlgMgr が行う．
//////////////////////////////////////////////////////////////////////
class AlgCover
{
public:

  /// @brief コンストラクタ
  /// @param[in] variable_num 変数の数
  ///
  /// * 空のカバーとなる．
  explicit
  AlgCover(int variable_num);

  /// @brief コンストラクタ
  /// @param[in] variable_num 変数の数
  /// @param[in] cube_list キューブのリスト
  ///
  /// * cube_list が空の時は空のカバーとなる．
  /// * cube_list が空でない時は各キューブのサイズは variable_num
  ///   と等しくなければならない．
  /// * キューブの順番は変わる可能性がある．
  AlgCover(int variable_num,
	   const vector<AlgCube>& cube_list);

  /// @brief コンストラクタ
  /// @param[in] variable_num 変数の数
  /// @param[in] cube_list カバーを表すリテラルのリストのリスト
  ///
  /// * キューブの順番は変わる可能性がある．
  AlgCover(int variable_num,
	   const vector<vector<Literal>>& cube_list);

  /// @brief コンストラクタ
  /// @param[in] variable_num 変数の数
  /// @param[in] cube_list カバーを表すリテラルのリストのリスト
  ///
  /// * キューブの順番は変わる可能性がある．
  AlgCover(int variable_num,
	   std::initializer_list<std::initializer_list<Literal>>& cube_list);

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  AlgCover(const AlgCover& src);

  /// @brief コピー代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 代入後の自身の参照を返す．
  AlgCover&
  operator=(const AlgCover& src);

  /// @brief ムーブコンストラクタ
  /// @param[in] src ムーブ元のオブジェクト
  AlgCover(AlgCover&& src);

  /// @brief ムーブ代入演算子
  /// @param[in] src ムーブ元のオブジェクト
  /// @return 代入後の自身の参照を返す．
  AlgCover&
  operator=(AlgCover&& src);

  /// @brief キューブからのコピー変換コンストラクタ
  /// @param[in] cube 対象のキューブ
  ///
  /// 指定されたキューブのみのカバーとなる．
  explicit
  AlgCover(const AlgCube& cube);

  /// @brief キューブからのムーブ変換コンストラクタ
  /// @param[in] cube 対象のキューブ
  ///
  /// 指定されたキューブのみのカバーとなる．
  explicit
  AlgCover(AlgCube&& cube);

  /// @brief デストラクタ
  ///
  /// ここに属しているすべてのキューブは削除される．
  ~AlgCover();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数の数を返す．
  int
  variable_num() const;

  /// @brief キューブの数を返す．
  int
  cube_num() const;

  /// @brief リテラル数を返す．
  int
  literal_num() const;

  /// @brief 指定されたリテラルの出現回数を返す．
  /// @param[in] lit 対象のリテラル
  int
  literal_num(Literal lit) const;

  /// @brief 内容をリテラルのリストのリストに変換する．
  /// @param[in] cube_list リテラルのリストのリストを格納するベクタ
  void
  to_literal_list(vector<vector<Literal>>& cube_list) const;

#if 0
  /// @brief 内容を返す．
  /// @param[in] cube_id キューブ番号 ( 0 <= cube_id < cube_num() )
  /// @param[in] var_id 変数の位置番号 ( 0 <= var_id.val() < variable_num() )
  AlgPol
  get_pol(int cube_id,
	  VarId var_id) const;
#endif

  /// @brief 論理和を計算する．
  /// @param[in] right オペランド
  /// @return 計算結果を返す．
  AlgCover
  operator+(const AlgCover& right) const;

  /// @brief 論理和を計算して代入する．
  /// @param[in] right オペランド
  /// @return 演算後の自身への参照を返す．
  AlgCover&
  operator+=(const AlgCover& right);

  /// @brief 論理和を計算する(キューブ版)．
  /// @param[in] right オペランド
  /// @return 計算結果を返す．
  AlgCover
  operator+(const AlgCube& right) const;

  /// @brief 論理和を計算して代入する(キューブ版)．
  /// @param[in] right オペランド
  /// @return 演算後の自身への参照を返す．
  AlgCover&
  operator+=(const AlgCube& right);

  /// @brief 差分を計算する．
  /// @param[in] right オペランド
  /// @return 計算結果を返す．
  ///
  /// right のみに含まれる要素があっても無視される．
  AlgCover
  operator-(const AlgCover& right) const;

  /// @brief 差分を計算して代入する．
  /// @param[in] right オペランド
  /// @return 演算後の自身への参照を返す．
  AlgCover&
  operator-=(const AlgCover& right);

  /// @brief 差分を計算する(キューブ版)．
  /// @param[in] right オペランド
  /// @return 計算結果を返す．
  ///
  /// right のみに含まれる要素があっても無視される．
  AlgCover
  operator-(const AlgCube& right) const;

  /// @brief 差分を計算して代入する(キューブ版)．
  /// @param[in] right オペランド
  /// @return 演算後の自身への参照を返す．
  AlgCover&
  operator-=(const AlgCube& right);

  /// @brief 論理積を計算する．
  /// @param[in] right オペランド
  /// @return 計算結果を返す．
  AlgCover
  operator*(const AlgCover& right) const;

  /// @brief 論理積を計算して代入する．
  /// @param[in] right オペランド
  /// @return 演算後の自身への参照を返す．
  AlgCover&
  operator*=(const AlgCover& right);

  /// @brief 論理積を計算する(キューブ版)．
  /// @param[in] right オペランド
  /// @return 計算結果を返す．
  AlgCover
  operator*(const AlgCube& right) const;

  /// @brief 論理積を計算して代入する(キューブ版)．
  /// @param[in] right オペランド
  /// @return 演算後の自身への参照を返す．
  AlgCover&
  operator*=(const AlgCube& right);

  /// @brief 論理積を計算する(リテラル版)．
  /// @param[in] right オペランド
  /// @return 計算結果を返す．
  AlgCover
  operator*(Literal right) const;

  /// @brief 論理積を計算して代入する(リテラル版)．
  /// @param[in] right オペランド
  /// @return 演算後の自身への参照を返す．
  AlgCover&
  operator*=(Literal right);

  /// @brief algebraic division を計算する．
  /// @param[in] right オペランド
  /// @return 計算結果を返す．
  AlgCover
  operator/(const AlgCover& right) const;

  /// @brief algebraic division を行って代入する．
  /// @param[in] right オペランド
  /// @return 演算後の自身への参照を返す．
  AlgCover&
  operator/=(const AlgCover& right);

  /// @brief キューブによる商を計算する．
  /// @param[in] cube 対象のキューブ
  /// @return 計算結果を返す．
  AlgCover
  operator/(const AlgCube& cube) const;

  /// @brief キューブによる商を計算して代入する．
  /// @param[in] cube 対象のキューブ
  /// @return 演算後の自身への参照を返す．
  AlgCover&
  operator/=(const AlgCube& cube);

  /// @brief リテラルによる商を計算する．
  /// @param[in] lit 対象のリテラル
  /// @return 計算結果を返す．
  AlgCover
  operator/(Literal lit) const;

  /// @brief リテラルによる商を計算して代入する．
  /// @param[in] lit 対象のリテラル
  /// @return 演算後の自身への参照を返す．
  AlgCover&
  operator/=(Literal lit);

  /// @brief 共通なキューブを返す．
  ///
  /// 共通なキューブがない場合には空のキューブを返す．
  AlgCube
  common_cube() const;

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

  /// @brief 内容を指定したコンストラクタ
  /// @param[in] variable_num 変数の数
  /// @param[in] cube_num キューブ数
  /// @param[in] cube_cap キューブ容量
  /// @param[in] body 内容のパタンを表す本体
  ///
  /// この関数は危険なので普通は使わないこと
  AlgCover(int variable_num,
	   int cube_num,
	   int cube_cap,
	   AlgBitVect* body);

  /// @brief キューブ容量を変更する．
  /// @param[in] req_cap 要求するキューブ容量
  ///
  /// 現在のキューブ容量が大きければ変更しない．
  void
  resize(int req_cap);

  /// @brief 内容を表す AlgBlock を返す．
  AlgBlock
  block() const;

  /// @brief キューブ容量を計算する．
  static
  int
  get_capacity(int cube_num);

  /// @brief 比較演算子(rich compare)
  /// @param[in] right オペランド
  /// @return 比較結果を返す．
  ///
  /// 比較方法はキューブごとの辞書式順序
  friend
  int
  compare(const AlgCover& left,
	  const AlgCover& right);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数の数
  int mVariableNum;

  // キューブ数
  int mCubeNum;

  // mBody の実際に確保されているキューブ容量
  int mCubeCap;

  // 内容を表すビットベクタ
  AlgBitVect* mBody;

};

/// @relates AlgCover
/// @brief キューブとカバーの加算
/// @param[in] left 第1オペランド
/// @param[in] right 第2オペランド
/// @return 結果を返す．
AlgCover
operator+(const AlgCube& left,
	  const AlgCover& right);

/// @relates AlgCover
/// @brief カバーの減算
/// @param[in] left, right オペランド
/// @return 結果を返す．
AlgCover
operator-(AlgCover&& left,
	  const AlgCover& right);

/// @relates AlgCover
/// @brief カバーとキューブの減算
/// @param[in] left, right オペランド
/// @return 結果を返す．
AlgCover
operator-(AlgCover&& left,
	  const AlgCube& right);

/// @relates AlgCover, AlgCube
/// @brief カバーとキューブの乗算
/// @param[in] left, right オペランド
/// @return 結果を返す．
AlgCover
operator*(AlgCover&& left,
	  const AlgCube& right);

/// @relates AlgCover, AlgCube
/// @brief キューブとカバーの乗算
/// @param[in] left 第1オペランド
/// @param[in] right 第2オペランド
/// @return 結果を返す．
AlgCover
operator*(const AlgCube& left,
	  const AlgCover& right);

/// @relates AlgCover, AlgCube
/// @brief キューブとカバーの乗算
/// @param[in] left 第1オペランド
/// @param[in] right 第2オペランド
/// @return 結果を返す．
AlgCover
operator*(const AlgCube& left,
	  AlgCover&& right);

/// @relates AlgCover, Literal
/// @brief カバーとリテラルの乗算
/// @param[in] left, right オペランド
/// @return 結果を返す．
AlgCover
operator*(AlgCover&& left,
	  Literal right);

/// @relates AlgCover, Literal
/// @brief リテラルとカバーの乗算
/// @param[in] left 第1オペランド
/// @param[in] right 第2オペランド
/// @return 結果を返す．
AlgCover
operator*(Literal left,
	  const AlgCover& right);

/// @relates AlgCover, Literal
/// @brief リテラルとカバーの乗算
/// @param[in] left 第1オペランド
/// @param[in] right 第2オペランド
/// @return 結果を返す．
AlgCover
operator*(Literal left,
	  AlgCover&& right);

/// @relates AlgCover
/// @brief カバーの除算
/// @param[in] left, right オペランド
/// @return 結果を返す．
AlgCover
operator/(AlgCover&& left,
	  const AlgCover& right);

/// @relates AlgCover, AlgCube
/// @brief カバーとキューブの除算
/// @param[in] left, right オペランド
/// @return 結果を返す．
AlgCover
operator/(AlgCover&& left,
	  const AlgCube& right);

/// @relates AlgCover, Literal
/// @brief カバーとリテラルの除算
/// @param[in] left, right オペランド
/// @return 結果を返す．
AlgCover
operator/(AlgCover&& left,
	  Literal right);

/// @relates AlgCover
/// @brief 比較演算子(rich compare)
/// @param[in] left, right オペランド
/// @return 比較結果を返す．
///
/// 比較方法はキューブごとの辞書式順序
int
compare(const AlgCover& left,
	const AlgCover& right);

/// @relates AlgCover
/// @brief 比較演算子 (EQ)
/// @param[in] left, right オペランド
/// @return 等しい時に true を返す．
bool
operator==(const AlgCover& left,
	   const AlgCover& right);

/// @relates AlgCover
/// @brief 比較演算子 (NE)
/// @param[in] left, right オペランド
/// @return 等しくない時に true を返す．
bool
operator!=(const AlgCover& left,
	   const AlgCover& right);

/// @relates AlgCover
/// @brief 比較演算子 (LT)
/// @param[in] left, right オペランド
/// @return left が right より小さい時に true を返す．
bool
operator<(const AlgCover& left,
	  const AlgCover& right);

/// @relates AlgCover
/// @brief 比較演算子 (GT)
/// @param[in] left, right オペランド
/// @return left が right より大きい時に true を返す．
bool
operator>(const AlgCover& left,
	  const AlgCover& right);

/// @relates AlgCover
/// @brief 比較演算子 (LE)
/// @param[in] left, right オペランド
/// @return left が right と等しいか小さい時に true を返す．
bool
operator<=(const AlgCover& left,
	   const AlgCover& right);

/// @relates AlgCover
/// @brief 比較演算子 (GE)
/// @param[in] left, right オペランド
/// @return left が right と等しいか大きい時に true を返す．
bool
operator>=(const AlgCover& left,
	   const AlgCover& right);

/// @relates AlgCover
/// @brief AlgCover の内容を出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] cover 対象のカバー
///
/// cover.print(s) と等価
ostream&
operator<<(ostream& s,
	   const AlgCover& cover);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 変数の数を返す．
inline
int
AlgCover::variable_num() const
{
  return mVariableNum;
}

// @brief キューブの数を返す．
inline
int
AlgCover::cube_num() const
{
  return mCubeNum;
}

// @relates AlgCover
// @brief キューブとカバーの加算
// @param[in] left 第1オペランド
// @param[in] right 第2オペランド
// @return 結果を返す．
inline
AlgCover
operator+(const AlgCube& left,
	  const AlgCover& right)
{
  // 交換則を用いる．
  return right.operator+(left);
}

/// @relates AlgCover
/// @brief カバーの減算
/// @param[in] left, right オペランド
/// @return 結果を返す．
inline
AlgCover
operator-(AlgCover&& left,
	  const AlgCover& right)
{
  return AlgCover(std::move(left)).operator-=(right);
}

// @relates AlgCover
// @brief カバーとキューブの減算
// @param[in] left, right オペランド
// @return 結果を返す．
inline
AlgCover
operator-(AlgCover&& left,
	  const AlgCube& right)
{
  return AlgCover(std::move(left)).operator-=(right);
}

// @relates AlgCover, AlgCube
// @brief カバーとキューブの乗算
// @param[in] left, right オペランド
// @return 結果を返す．
inline
AlgCover
operator*(AlgCover&& left,
	  const AlgCube& right)
{
  return AlgCover(std::move(left)).operator*=(right);
}

// @relates AlgCover
// @brief キューブとカバーの乗算
// @param[in] left 第1オペランド
// @param[in] right 第2オペランド
// @return 結果を返す．
inline
AlgCover
operator*(const AlgCube& left,
	  const AlgCover& right)
{
  // 交換則を用いる．
  return right.operator*(left);
}

// @relates AlgCover, AlgCube
// @brief キューブとカバーの乗算
// @param[in] left 第1オペランド
// @param[in] right 第2オペランド
// @return 結果を返す．
inline
AlgCover
operator*(const AlgCube& left,
	  AlgCover&& right)
{
  // 交換則を用いる．
  return AlgCover(std::move(right)).operator*=(left);
}

// @relates AlgCover, Literal
// @brief カバーとリテラルの乗算
// @param[in] left, right オペランド
// @return 結果を返す．
inline
AlgCover
operator*(AlgCover&& left,
	  Literal right)
{
  return AlgCover(std::move(left)).operator*=(right);
}

// @relates AlgCover
// @brief リテラルとカバーの乗算
// @param[in] left 第1オペランド
// @param[in] right 第2オペランド
// @return 結果を返す．
inline
AlgCover
operator*(Literal left,
	  const AlgCover& right)
{
  // 交換則を用いる．
  return right.operator*(left);
}

// @relates AlgCover, Literal
// @brief リテラルとカバーの乗算
// @param[in] left 第1オペランド
// @param[in] right 第2オペランド
// @return 結果を返す．
inline
AlgCover
operator*(Literal left,
	  AlgCover&& right)
{
  // 交換則を用いる．
  return AlgCover(std::move(right)).operator*=(left);
}

// @relates AlgCover
// @brief カバーの除算
// @param[in] left, right オペランド
// @return 結果を返す．
inline
AlgCover
operator/(AlgCover&& left,
	  const AlgCover& right)
{
  return AlgCover(std::move(left)).operator/=(right);
}

// @relates AlgCover, AlgCube
// @brief カバーとキューブの除算
// @param[in] left, right オペランド
// @return 結果を返す．
inline
AlgCover
operator/(AlgCover&& left,
	  const AlgCube& right)
{
  return AlgCover(std::move(left)).operator/=(right);
}

// @relates AlgCover, Literal
// @brief カバーとリテラルの除算
// @param[in] left, right オペランド
// @return 結果を返す．
inline
AlgCover
operator/(AlgCover&& left,
	  Literal right)
{
  return AlgCover(std::move(left)).operator/=(right);
}

// @relates AlgCover
// @brief 比較演算子 (EQ)
// @param[in] left, right オペランド
// @return 等しい時に true を返す．
inline
bool
operator==(const AlgCover& left,
	   const AlgCover& right)
{
  return compare(left, right) == 0;
}

// @relates AlgCover
// @brief 比較演算子 (NE)
// @param[in] left, right オペランド
// @return 等しくない時に true を返す．
inline
bool
operator!=(const AlgCover& left,
	   const AlgCover& right)
{
  return compare(left, right) != 0;
}

// @relates AlgCover
// @brief 比較演算子 (LT)
// @param[in] left, right オペランド
// @return left が right より小さい時に true を返す．
inline
bool
operator<(const AlgCover& left,
	  const AlgCover& right)
{
  return compare(left, right) < 0;
}

// @relates AlgCover
// @brief 比較演算子 (GT)
// @param[in] left, right オペランド
// @return left が right より大きい時に true を返す．
inline
bool
operator>(const AlgCover& left,
	  const AlgCover& right)
{
  return compare(left, right) > 0;
}

// @relates AlgCover
// @brief 比較演算子 (LE)
// @param[in] left, right オペランド
// @return left が right と等しいか小さい時に true を返す．
inline
bool
operator<=(const AlgCover& left,
	   const AlgCover& right)
{
  return compare(left, right) <= 0;
}

// @relates AlgCover
// @brief 比較演算子 (GE)
// @param[in] left, right オペランド
// @return left が right と等しいか大きい時に true を返す．
inline
bool
operator>=(const AlgCover& left,
	   const AlgCover& right)
{
  return compare(left, right) >= 0;
}

// @brief AlgCover の内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] cover 対象のカバー
//
// cover.print(s) と等価
inline
ostream&
operator<<(ostream& s,
	   const AlgCover& cover)
{
  cover.print(s);
  return s;
}

END_NAMESPACE_YM_LOGIC

BEGIN_NAMESPACE_YM

/// @breif AlgCover をキーにしたハッシュ関数クラスの定義
template <>
struct HashFunc<AlgCover>
{
  SizeType
  operator()(const AlgCover& cover) const
  {
    return cover.hash();
  }
};

END_NAMESPACE_YM

#endif // YM_ALGCOVER_H
