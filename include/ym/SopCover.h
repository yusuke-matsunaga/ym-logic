#ifndef YM_SOPCOVER_H
#define YM_SOPCOVER_H

/// @file ym/SopCover.h
/// @brief SopCover のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/Sop.h"
#include "ym/Literal.h"


BEGIN_NAMESPACE_YM_LOGIC

//////////////////////////////////////////////////////////////////////
/// @class SopCover SopCover.h "ym/SopCover.h"
/// @brief カバー(積和形論理式)を表すクラス
///
/// 中身は positional cube matrix 表現を用いたビットベクタ<br>
/// ビットベクタの内容は一旦，設定されたら変わることはない<br>
/// キューブはワードレベルの順序で整列しているものと仮定している<br>
/// この式は代数的(algebraic)なので他のキューブに包含されるキューブは
/// 許されない．もちろん重複したキューブも許されない．<br>
/// 通常の操作を行っているかぎり代数的な式しか生成されない．<br>
/// 実際の操作は SopMgr が行う．
//////////////////////////////////////////////////////////////////////
class SopCover
{
public:

  /// @brief コンストラクタ
  /// @param[in] variable_num 変数の数
  ///
  /// * 空のカバーとなる．
  explicit
  SopCover(int variable_num);

  /// @brief コンストラクタ
  /// @param[in] variable_num 変数の数
  /// @param[in] cube_list キューブのリスト
  ///
  /// * cube_list 中の各キューブのサイズは variable_num
  ///   と等しくなければならない．
  /// * キューブの順番は変わる可能性がある．
  SopCover(int variable_num,
	   const vector<SopCube>& cube_list);

  /// @brief コンストラクタ
  /// @param[in] variable_num 変数の数
  /// @param[in] cube_list カバーを表すリテラルのリストのリスト
  ///
  /// * キューブの順番は変わる可能性がある．
  SopCover(int variable_num,
	   const vector<vector<Literal>>& cube_list);

  /// @brief コンストラクタ
  /// @param[in] variable_num 変数の数
  /// @param[in] cube_list カバーを表すリテラルのリストのリスト
  ///
  /// * キューブの順番は変わる可能性がある．
  SopCover(int variable_num,
	   initializer_list<initializer_list<Literal>>& cube_list);

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  SopCover(const SopCover& src);

  /// @brief コピー代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 代入後の自身の参照を返す．
  SopCover&
  operator=(const SopCover& src);

  /// @brief ムーブコンストラクタ
  /// @param[in] src ムーブ元のオブジェクト
  SopCover(SopCover&& src);

  /// @brief ムーブ代入演算子
  /// @param[in] src ムーブ元のオブジェクト
  /// @return 代入後の自身の参照を返す．
  SopCover&
  operator=(SopCover&& src);

  /// @brief キューブからのコピー変換コンストラクタ
  /// @param[in] cube 対象のキューブ
  ///
  /// 指定されたキューブのみのカバーとなる．
  explicit
  SopCover(const SopCube& cube);

  /// @brief キューブからのムーブ変換コンストラクタ
  /// @param[in] cube 対象のキューブ
  ///
  /// 指定されたキューブのみのカバーとなる．
  explicit
  SopCover(SopCube&& cube);

  /// @brief デストラクタ
  ///
  /// ここに属しているすべてのキューブは削除される．
  ~SopCover();


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

  /// @brief パタンを返す．
  /// @param[in] cube_id キューブ番号 ( 0 <= cube_id < cube_num() )
  /// @param[in] var 変数( 0 <= var_id.val() < variable_num() )
  /// @retval SopPat::_X その変数は現れない．
  /// @retval SopPat::_1 その変数が肯定のリテラルとして現れる．
  /// @retval SopPat::_0 その変数が否定のリテラルとして現れる．
  SopPat
  get_pat(int cube_id,
	  VarId var_id) const;

  /// @brief 論理和を計算する．
  /// @param[in] right オペランド
  /// @return 計算結果を返す．
  SopCover
  operator+(const SopCover& right) const;

  /// @brief 論理和を計算して代入する．
  /// @param[in] right オペランド
  /// @return 演算後の自身への参照を返す．
  SopCover&
  operator+=(const SopCover& right);

  /// @brief 論理和を計算する(キューブ版)．
  /// @param[in] right オペランド
  /// @return 計算結果を返す．
  SopCover
  operator+(const SopCube& right) const;

  /// @brief 論理和を計算して代入する(キューブ版)．
  /// @param[in] right オペランド
  /// @return 演算後の自身への参照を返す．
  SopCover&
  operator+=(const SopCube& right);

  /// @brief 差分を計算する．
  /// @param[in] right オペランド
  /// @return 計算結果を返す．
  ///
  /// right のみに含まれる要素があっても無視される．
  SopCover
  operator-(const SopCover& right) const;

  /// @brief 差分を計算して代入する．
  /// @param[in] right オペランド
  /// @return 演算後の自身への参照を返す．
  SopCover&
  operator-=(const SopCover& right);

  /// @brief 差分を計算する(キューブ版)．
  /// @param[in] right オペランド
  /// @return 計算結果を返す．
  ///
  /// right のみに含まれる要素があっても無視される．
  SopCover
  operator-(const SopCube& right) const;

  /// @brief 差分を計算して代入する(キューブ版)．
  /// @param[in] right オペランド
  /// @return 演算後の自身への参照を返す．
  SopCover&
  operator-=(const SopCube& right);

  /// @brief 論理積を計算する．
  /// @param[in] right オペランド
  /// @return 計算結果を返す．
  SopCover
  operator*(const SopCover& right) const;

  /// @brief 論理積を計算して代入する．
  /// @param[in] right オペランド
  /// @return 演算後の自身への参照を返す．
  SopCover&
  operator*=(const SopCover& right);

  /// @brief 論理積を計算する(キューブ版)．
  /// @param[in] right オペランド
  /// @return 計算結果を返す．
  SopCover
  operator*(const SopCube& right) const;

  /// @brief 論理積を計算して代入する(キューブ版)．
  /// @param[in] right オペランド
  /// @return 演算後の自身への参照を返す．
  SopCover&
  operator*=(const SopCube& right);

  /// @brief 論理積を計算する(リテラル版)．
  /// @param[in] right オペランド
  /// @return 計算結果を返す．
  SopCover
  operator*(Literal right) const;

  /// @brief 論理積を計算して代入する(リテラル版)．
  /// @param[in] right オペランド
  /// @return 演算後の自身への参照を返す．
  SopCover&
  operator*=(Literal right);

  /// @brief algebraic division を計算する．
  /// @param[in] right オペランド
  /// @return 計算結果を返す．
  SopCover
  operator/(const SopCover& right) const;

  /// @brief algebraic division を行って代入する．
  /// @param[in] right オペランド
  /// @return 演算後の自身への参照を返す．
  SopCover&
  operator/=(const SopCover& right);

  /// @brief キューブによる商を計算する．
  /// @param[in] cube 対象のキューブ
  /// @return 計算結果を返す．
  SopCover
  operator/(const SopCube& cube) const;

  /// @brief キューブによる商を計算して代入する．
  /// @param[in] cube 対象のキューブ
  /// @return 演算後の自身への参照を返す．
  SopCover&
  operator/=(const SopCube& cube);

  /// @brief リテラルによる商を計算する．
  /// @param[in] lit 対象のリテラル
  /// @return 計算結果を返す．
  SopCover
  operator/(Literal lit) const;

  /// @brief リテラルによる商を計算して代入する．
  /// @param[in] lit 対象のリテラル
  /// @return 演算後の自身への参照を返す．
  SopCover&
  operator/=(Literal lit);

  /// @brief 共通なキューブを返す．
  ///
  /// 共通なキューブがない場合には空のキューブを返す．
  SopCube
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
  SopCover(int variable_num,
	   int cube_num,
	   int cube_cap,
	   SopBitVect* body);

  /// @brief キューブ容量を変更する．
  /// @param[in] req_cap 要求するキューブ容量
  ///
  /// 現在のキューブ容量が大きければ変更しない．
  void
  resize(int req_cap);

  /// @brief 内容を表す SopBlock を返す．
  SopBlock
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
  compare(const SopCover& left,
	  const SopCover& right);


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
  SopBitVect* mBody;

};

/// @relates SopCover
/// @brief キューブとカバーの加算
/// @param[in] left 第1オペランド
/// @param[in] right 第2オペランド
/// @return 結果を返す．
SopCover
operator+(const SopCube& left,
	  const SopCover& right);

/// @relates SopCover
/// @brief カバーの減算
/// @param[in] left, right オペランド
/// @return 結果を返す．
SopCover
operator-(SopCover&& left,
	  const SopCover& right);

/// @relates SopCover
/// @brief カバーとキューブの減算
/// @param[in] left, right オペランド
/// @return 結果を返す．
SopCover
operator-(SopCover&& left,
	  const SopCube& right);

/// @relates SopCover, SopCube
/// @brief カバーとキューブの乗算
/// @param[in] left, right オペランド
/// @return 結果を返す．
SopCover
operator*(SopCover&& left,
	  const SopCube& right);

/// @relates SopCover, SopCube
/// @brief キューブとカバーの乗算
/// @param[in] left 第1オペランド
/// @param[in] right 第2オペランド
/// @return 結果を返す．
SopCover
operator*(const SopCube& left,
	  const SopCover& right);

/// @relates SopCover, SopCube
/// @brief キューブとカバーの乗算
/// @param[in] left 第1オペランド
/// @param[in] right 第2オペランド
/// @return 結果を返す．
SopCover
operator*(const SopCube& left,
	  SopCover&& right);

/// @relates SopCover, Literal
/// @brief カバーとリテラルの乗算
/// @param[in] left, right オペランド
/// @return 結果を返す．
SopCover
operator*(SopCover&& left,
	  Literal right);

/// @relates SopCover, Literal
/// @brief リテラルとカバーの乗算
/// @param[in] left 第1オペランド
/// @param[in] right 第2オペランド
/// @return 結果を返す．
SopCover
operator*(Literal left,
	  const SopCover& right);

/// @relates SopCover, Literal
/// @brief リテラルとカバーの乗算
/// @param[in] left 第1オペランド
/// @param[in] right 第2オペランド
/// @return 結果を返す．
SopCover
operator*(Literal left,
	  SopCover&& right);

/// @relates SopCover
/// @brief カバーの除算
/// @param[in] left, right オペランド
/// @return 結果を返す．
SopCover
operator/(SopCover&& left,
	  const SopCover& right);

/// @relates SopCover, SopCube
/// @brief カバーとキューブの除算
/// @param[in] left, right オペランド
/// @return 結果を返す．
SopCover
operator/(SopCover&& left,
	  const SopCube& right);

/// @relates SopCover, Literal
/// @brief カバーとリテラルの除算
/// @param[in] left, right オペランド
/// @return 結果を返す．
SopCover
operator/(SopCover&& left,
	  Literal right);

/// @relates SopCover
/// @brief 比較演算子(rich compare)
/// @param[in] left, right オペランド
/// @return 比較結果を返す．
///
/// 比較方法はキューブごとの辞書式順序
int
compare(const SopCover& left,
	const SopCover& right);

/// @relates SopCover
/// @brief 比較演算子 (EQ)
/// @param[in] left, right オペランド
/// @return 等しい時に true を返す．
bool
operator==(const SopCover& left,
	   const SopCover& right);

/// @relates SopCover
/// @brief 比較演算子 (NE)
/// @param[in] left, right オペランド
/// @return 等しくない時に true を返す．
bool
operator!=(const SopCover& left,
	   const SopCover& right);

/// @relates SopCover
/// @brief 比較演算子 (LT)
/// @param[in] left, right オペランド
/// @return left が right より小さい時に true を返す．
bool
operator<(const SopCover& left,
	  const SopCover& right);

/// @relates SopCover
/// @brief 比較演算子 (GT)
/// @param[in] left, right オペランド
/// @return left が right より大きい時に true を返す．
bool
operator>(const SopCover& left,
	  const SopCover& right);

/// @relates SopCover
/// @brief 比較演算子 (LE)
/// @param[in] left, right オペランド
/// @return left が right と等しいか小さい時に true を返す．
bool
operator<=(const SopCover& left,
	   const SopCover& right);

/// @relates SopCover
/// @brief 比較演算子 (GE)
/// @param[in] left, right オペランド
/// @return left が right と等しいか大きい時に true を返す．
bool
operator>=(const SopCover& left,
	   const SopCover& right);

/// @relates SopCover
/// @brief SopCover の内容を出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] cover 対象のカバー
///
/// cover.print(s) と等価
ostream&
operator<<(ostream& s,
	   const SopCover& cover);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 変数の数を返す．
inline
int
SopCover::variable_num() const
{
  return mVariableNum;
}

// @brief キューブの数を返す．
inline
int
SopCover::cube_num() const
{
  return mCubeNum;
}

// @relates SopCover
// @brief キューブとカバーの加算
// @param[in] left 第1オペランド
// @param[in] right 第2オペランド
// @return 結果を返す．
inline
SopCover
operator+(const SopCube& left,
	  const SopCover& right)
{
  // 交換則を用いる．
  return right.operator+(left);
}

/// @relates SopCover
/// @brief カバーの減算
/// @param[in] left, right オペランド
/// @return 結果を返す．
inline
SopCover
operator-(SopCover&& left,
	  const SopCover& right)
{
  return SopCover(move(left)).operator-=(right);
}

// @relates SopCover
// @brief カバーとキューブの減算
// @param[in] left, right オペランド
// @return 結果を返す．
inline
SopCover
operator-(SopCover&& left,
	  const SopCube& right)
{
  return SopCover(move(left)).operator-=(right);
}

// @relates SopCover, SopCube
// @brief カバーとキューブの乗算
// @param[in] left, right オペランド
// @return 結果を返す．
inline
SopCover
operator*(SopCover&& left,
	  const SopCube& right)
{
  return SopCover(move(left)).operator*=(right);
}

// @relates SopCover
// @brief キューブとカバーの乗算
// @param[in] left 第1オペランド
// @param[in] right 第2オペランド
// @return 結果を返す．
inline
SopCover
operator*(const SopCube& left,
	  const SopCover& right)
{
  // 交換則を用いる．
  return right.operator*(left);
}

// @relates SopCover, SopCube
// @brief キューブとカバーの乗算
// @param[in] left 第1オペランド
// @param[in] right 第2オペランド
// @return 結果を返す．
inline
SopCover
operator*(const SopCube& left,
	  SopCover&& right)
{
  // 交換則を用いる．
  return SopCover(move(right)).operator*=(left);
}

// @relates SopCover, Literal
// @brief カバーとリテラルの乗算
// @param[in] left, right オペランド
// @return 結果を返す．
inline
SopCover
operator*(SopCover&& left,
	  Literal right)
{
  return SopCover(move(left)).operator*=(right);
}

// @relates SopCover
// @brief リテラルとカバーの乗算
// @param[in] left 第1オペランド
// @param[in] right 第2オペランド
// @return 結果を返す．
inline
SopCover
operator*(Literal left,
	  const SopCover& right)
{
  // 交換則を用いる．
  return right.operator*(left);
}

// @relates SopCover, Literal
// @brief リテラルとカバーの乗算
// @param[in] left 第1オペランド
// @param[in] right 第2オペランド
// @return 結果を返す．
inline
SopCover
operator*(Literal left,
	  SopCover&& right)
{
  // 交換則を用いる．
  return SopCover(move(right)).operator*=(left);
}

// @relates SopCover
// @brief カバーの除算
// @param[in] left, right オペランド
// @return 結果を返す．
inline
SopCover
operator/(SopCover&& left,
	  const SopCover& right)
{
  return SopCover(move(left)).operator/=(right);
}

// @relates SopCover, SopCube
// @brief カバーとキューブの除算
// @param[in] left, right オペランド
// @return 結果を返す．
inline
SopCover
operator/(SopCover&& left,
	  const SopCube& right)
{
  return SopCover(move(left)).operator/=(right);
}

// @relates SopCover, Literal
// @brief カバーとリテラルの除算
// @param[in] left, right オペランド
// @return 結果を返す．
inline
SopCover
operator/(SopCover&& left,
	  Literal right)
{
  return SopCover(move(left)).operator/=(right);
}

// @relates SopCover
// @brief 比較演算子 (EQ)
// @param[in] left, right オペランド
// @return 等しい時に true を返す．
inline
bool
operator==(const SopCover& left,
	   const SopCover& right)
{
  return compare(left, right) == 0;
}

// @relates SopCover
// @brief 比較演算子 (NE)
// @param[in] left, right オペランド
// @return 等しくない時に true を返す．
inline
bool
operator!=(const SopCover& left,
	   const SopCover& right)
{
  return compare(left, right) != 0;
}

// @relates SopCover
// @brief 比較演算子 (LT)
// @param[in] left, right オペランド
// @return left が right より小さい時に true を返す．
inline
bool
operator<(const SopCover& left,
	  const SopCover& right)
{
  return compare(left, right) < 0;
}

// @relates SopCover
// @brief 比較演算子 (GT)
// @param[in] left, right オペランド
// @return left が right より大きい時に true を返す．
inline
bool
operator>(const SopCover& left,
	  const SopCover& right)
{
  return compare(left, right) > 0;
}

// @relates SopCover
// @brief 比較演算子 (LE)
// @param[in] left, right オペランド
// @return left が right と等しいか小さい時に true を返す．
inline
bool
operator<=(const SopCover& left,
	   const SopCover& right)
{
  return compare(left, right) <= 0;
}

// @relates SopCover
// @brief 比較演算子 (GE)
// @param[in] left, right オペランド
// @return left が right と等しいか大きい時に true を返す．
inline
bool
operator>=(const SopCover& left,
	   const SopCover& right)
{
  return compare(left, right) >= 0;
}

// @brief SopCover の内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] cover 対象のカバー
//
// cover.print(s) と等価
inline
ostream&
operator<<(ostream& s,
	   const SopCover& cover)
{
  cover.print(s);
  return s;
}

END_NAMESPACE_YM_LOGIC

BEGIN_NAMESPACE_STD

/// @breif SopCover をキーにしたハッシュ関数クラスの定義
template <>
struct hash<YM_NAMESPACE::SopCover>
{
  SizeType
  operator()(const YM_NAMESPACE::SopCover& cover) const
  {
    return cover.hash();
  }
};

END_NAMESPACE_STD

#endif // YM_SOPCOVER_H
