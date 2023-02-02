#ifndef YM_SOPCOVER_H
#define YM_SOPCOVER_H

/// @file ym/SopCover.h
/// @brief SopCover のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/sop_nsdef.h"
#include "ym/Expr.h"
#include "ym/Literal.h"


BEGIN_NAMESPACE_YM_SOP

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
  friend class SopMgr;

public:

  /// @brief コンストラクタ
  ///
  /// * 空のカバーとなる．
  explicit
  SopCover(
    SizeType variable_num ///< [in] 変数の数
  );

  /// @brief コンストラクタ
  ///
  /// * cube_list 中の各キューブのサイズは variable_num
  ///   と等しくなければならない．
  /// * キューブの順番は変わる可能性がある．
  SopCover(
    SizeType variable_num,           ///< [in] 変数の数
    const vector<SopCube>& cube_list ///< [in] キューブのリスト
  );

  /// @brief コンストラクタ
  ///
  /// * キューブの順番は変わる可能性がある．
  SopCover(
    SizeType variable_num,                   ///< [in] 変数の数
    const vector<vector<Literal>>& cube_list ///< [in] カバーを表すリテラルのリストのリスト
  );

  /// @brief コンストラクタ
  ///
  /// * キューブの順番は変わる可能性がある．
  SopCover(
    SizeType variable_num,                                 ///< [in] 変数の数
    initializer_list<initializer_list<Literal>>& cube_list ///< [in] カバーを表すリテラルのリストのリスト
  );

  /// @brief コピーコンストラクタ
  SopCover(
    const SopCover& src ///< [in] コピー元のオブジェクト
  );

  /// @brief コピー代入演算子
  /// @return 代入後の自身の参照を返す．
  SopCover&
  operator=(
    const SopCover& src ///< [in] コピー元のオブジェクト
  );

  /// @brief ムーブコンストラクタ
  SopCover(
    SopCover&& src ///< [in] ムーブ元のオブジェクト
  );

  /// @brief ムーブ代入演算子
  /// @return 代入後の自身の参照を返す．
  SopCover&
  operator=(
    SopCover&& src ///< [in] ムーブ元のオブジェクト
  );

  /// @brief キューブからのコピー変換コンストラクタ
  ///
  /// 指定されたキューブのみのカバーとなる．
  explicit
  SopCover(
    const SopCube& cube ///< [in] 対象のキューブ
  );

  /// @brief キューブからのムーブ変換コンストラクタ
  ///
  /// 指定されたキューブのみのカバーとなる．
  explicit
  SopCover(
    SopCube&& cube ///< [in] 対象のキューブ
  );

  /// @brief デストラクタ
  ///
  /// ここに属しているすべてのキューブは削除される．
  ~SopCover();


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

  /// @brief キューブの数を返す．
  SizeType
  cube_num() const
  {
    return mCubeNum;
  }

  /// @brief リテラル数を返す．
  SizeType
  literal_num() const;

  /// @brief 指定されたリテラルの出現回数を返す．
  SizeType
  literal_num(
    Literal lit ///< [in] 対象のリテラル
  ) const;

  /// @brief 内容をリテラルのリストのリストに変換する．
  vector<vector<Literal>>
  literal_list() const;

  /// @brief パタンを返す．
  /// @retval SopPat::_X その変数は現れない．
  /// @retval SopPat::_1 その変数が肯定のリテラルとして現れる．
  /// @retval SopPat::_0 その変数が否定のリテラルとして現れる．
  SopPat
  get_pat(
    SizeType cube_id, ///< [in] キューブ番号 ( 0 <= cube_id < cube_num() )
    SizeType var_id   ///< [in] 変数( 0 <= var_id.val() < variable_num() )
  ) const;

  /// @brief 否定を計算する．
  SopCover
  operator~() const;

  /// @brief 論理和を計算する．
  /// @return 計算結果を返す．
  SopCover
  operator+(
    const SopCover& right ///< [in] オペランド
  ) const;

  /// @brief 論理和を計算して代入する．
  /// @return 演算後の自身への参照を返す．
  SopCover&
  operator+=(
    const SopCover& right ///< [in] オペランド
  );

  /// @brief 論理和を計算する(キューブ版)．
  /// @return 計算結果を返す．
  SopCover
  operator+(
    const SopCube& right ///< [in] オペランド
  ) const;

  /// @brief 論理和を計算して代入する(キューブ版)．
  /// @return 演算後の自身への参照を返す．
  SopCover&
  operator+=(
    const SopCube& right ///< [in] オペランド
  );

  /// @brief 差分を計算する．
  /// @return 計算結果を返す．
  ///
  /// right のみに含まれる要素があっても無視される．
  SopCover
  operator-(
    const SopCover& right ///< [in] オペランド
  ) const;

  /// @brief 差分を計算して代入する．
  /// @return 演算後の自身への参照を返す．
  SopCover&
  operator-=(
    const SopCover& right ///< [in] オペランド
  );

  /// @brief 差分を計算する(キューブ版)．
  /// @return 計算結果を返す．
  ///
  /// right のみに含まれる要素があっても無視される．
  SopCover
  operator-(
    const SopCube& right ///< [in] オペランド
  ) const;

  /// @brief 差分を計算して代入する(キューブ版)．
  /// @return 演算後の自身への参照を返す．
  SopCover&
  operator-=(
    const SopCube& right ///< [in] オペランド
  );

  /// @brief 論理積を計算する．
  /// @return 計算結果を返す．
  SopCover
  operator*(
    const SopCover& right ///< [in] オペランド
  ) const;

  /// @brief 論理積を計算して代入する．
  /// @return 演算後の自身への参照を返す．
  SopCover&
  operator*=(
    const SopCover& right ///< [in] オペランド
  );

  /// @brief 論理積を計算する(キューブ版)．
  /// @return 計算結果を返す．
  SopCover
  operator*(
    const SopCube& right ///< [in] オペランド
  ) const;

  /// @brief 論理積を計算して代入する(キューブ版)．
  /// @return 演算後の自身への参照を返す．
  SopCover&
  operator*=(
    const SopCube& right ///< [in] オペランド
  );

  /// @brief 論理積を計算する(リテラル版)．
  /// @return 計算結果を返す．
  SopCover
  operator*(
    Literal right ///< [in] オペランド
  ) const;

  /// @brief 論理積を計算して代入する(リテラル版)．
  /// @return 演算後の自身への参照を返す．
  SopCover&
  operator*=(
    Literal right ///< [in] オペランド
  );

  /// @brief algebraic division を計算する．
  /// @return 計算結果を返す．
  SopCover
  operator/(
    const SopCover& right ///< [in] オペランド
  ) const;

  /// @brief algebraic division を行って代入する．
  /// @return 演算後の自身への参照を返す．
  SopCover&
  operator/=(
    const SopCover& right ///< [in] オペランド
  );

  /// @brief キューブによる商を計算する．
  /// @return 計算結果を返す．
  SopCover
  operator/(
    const SopCube& cube ///< [in] 対象のキューブ
  ) const;

  /// @brief キューブによる商を計算して代入する．
  /// @return 演算後の自身への参照を返す．
  SopCover&
  operator/=(
    const SopCube& cube ///< [in] 対象のキューブ
  );

  /// @brief リテラルによる商を計算する．
  /// @return 計算結果を返す．
  SopCover
  operator/(
    Literal lit ///< [in] 対象のリテラル
  ) const;

  /// @brief リテラルによる商を計算して代入する．
  /// @return 演算後の自身への参照を返す．
  SopCover&
  operator/=(
    Literal lit ///< [in] 対象のリテラル
  );

  /// @brief 共通なキューブを返す．
  ///
  /// 共通なキューブがない場合には空のキューブを返す．
  SopCube
  common_cube() const;

  /// @brief Expr に変換する．
  Expr
  expr() const;

  /// @brief ハッシュ値を返す．
  SizeType
  hash() const;

  /// @brief 内容をわかりやすい形で出力する．
  void
  print(
    ostream& s,                             ///< [in] 出力先のストリーム
    const vector<string>& varname_list = {} ///< [in] 変数名のリスト
  ) const;

  /// @brief 内容をデバッグ用に出力する．
  void
  debug_print(
    ostream& s ///< [in] 出力先のストリーム
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を指定したコンストラクタ
  ///
  /// この関数は危険なので普通は使わないこと
  SopCover(
    SizeType variable_num, ///< [in] 変数の数
    SizeType cube_num,     ///< [in] キューブ数
    SizeType cube_cap,     ///< [in] キューブ容量
    SopBitVect* body       ///< [in] 内容のパタンを表す本体
  );

  /// @brief キューブ容量を変更する．
  ///
  /// 現在のキューブ容量が大きければ変更しない．
  void
  resize(
    SizeType req_cap ///< [in] 要求するキューブ容量
  );

  /// @brief 内容を表す SopBlock を返す．
  SopBlock
  block() const;

  /// @brief キューブ容量を計算する．
  static
  SizeType
  get_capacity(
    SizeType cube_num ///< [in] キューブ数
  );

  /// @brief 比較演算子(rich compare)
  /// @return 比較結果を返す．
  ///
  /// 比較方法はキューブごとの辞書式順序
  friend
  int
  compare(
    const SopCover& left, ///< [in] 第1オペランド
    const SopCover& right ///< [in] 第2オペランド
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数の数
  SizeType mVariableNum;

  // キューブ数
  SizeType mCubeNum;

  // mBody の実際に確保されているキューブ容量
  SizeType mCubeCap;

  // 内容を表すビットベクタ
  SopBitVect* mBody;

};

/// @relates SopCover
/// @brief キューブとカバーの加算
/// @return 結果を返す．
inline
SopCover
operator+(
  const SopCube& left,  ///< [in] 第1オペランド
  const SopCover& right ///< [in] 第2オペランド
)
{
  // 交換則を用いる．
  return right.operator+(left);
}

/// @relates SopCover
/// @brief カバーの減算
/// @return 結果を返す．
inline
SopCover
operator-(
  SopCover&& left,      ///< [in] 第1オペランド
  const SopCover& right ///< [in] 第2オペランド
)
{
  return SopCover{move(left)}.operator-=(right);
}

/// @relates SopCover
/// @brief カバーとキューブの減算
/// @return 結果を返す．
inline
SopCover
operator-(
  SopCover&& left,     ///< [in] 第1オペランド
  const SopCube& right ///< [in] 第2オペランド
)
{
  return SopCover{move(left)}.operator-=(right);
}

/// @relates SopCover, SopCube
/// @brief カバーとキューブの乗算
/// @return 結果を返す．
inline
SopCover
operator*(
  SopCover&& left,     ///< [in] 第1オペランド
  const SopCube& right ///< [in] 第2オペランド
)
{
  return SopCover{move(left)}.operator*=(right);
}

/// @relates SopCover, SopCube
/// @brief キューブとカバーの乗算
/// @return 結果を返す．
inline
SopCover
operator*(
  const SopCube& left,  ///< [in] 第1オペランド
  const SopCover& right ///< [in] 第2オペランド
)
{
  // 交換則を用いる．
  return right.operator*(left);
}

/// @relates SopCover, SopCube
/// @brief キューブとカバーの乗算
/// @return 結果を返す．
inline
SopCover
operator*(
  const SopCube& left, ///< [in] 第1オペランド
  SopCover&& right     ///< [in] 第2オペランド
)
{
  // 交換則を用いる．
  return SopCover{move(right)}.operator*=(left);
}

/// @relates SopCover, Literal
/// @brief カバーとリテラルの乗算
/// @return 結果を返す．
inline
SopCover
operator*(
  SopCover&& left, ///< [in] 第1オペランド
  Literal right	   ///< [in] 第2オペランド
)
{
  return SopCover{move(left)}.operator*=(right);
}

/// @relates SopCover, Literal
/// @brief リテラルとカバーの乗算
/// @return 結果を返す．
inline
SopCover
operator*(
  Literal left,         ///< [in] 第1オペランド
  const SopCover& right ///< [in] 第2オペランド
)
{
  // 交換則を用いる．
  return right.operator*(left);
}

/// @relates SopCover, Literal
/// @brief リテラルとカバーの乗算
/// @return 結果を返す．
inline
SopCover
operator*(
  Literal left,    ///< [in] 第1オペランド
  SopCover&& right ///< [in] 第2オペランド
)
{
  // 交換則を用いる．
  return SopCover{move(right)}.operator*=(left);
}

/// @relates SopCover
/// @brief カバーの除算
/// @return 結果を返す．
inline
SopCover
operator/(
  SopCover&& left,      ///< [in] 第1オペランド
  const SopCover& right ///< [in] 第2オペランド
)
{
  return SopCover{move(left)}.operator/=(right);
}

/// @relates SopCover, SopCube
/// @brief カバーとキューブの除算
/// @return 結果を返す．
inline
SopCover
operator/(
  SopCover&& left,     ///< [in] 第1オペランド
  const SopCube& right ///< [in] 第2オペランド
)
{
  return SopCover{move(left)}.operator/=(right);
}

/// @relates SopCover, Literal
/// @brief カバーとリテラルの除算
/// @return 結果を返す．
inline
SopCover
operator/(
  SopCover&& left, ///< [in] 第1オペランド
  Literal right	   ///< [in] 第2オペランド
)
{
  return SopCover{move(left)}.operator/=(right);
}

/// @relates SopCover
/// @brief 比較演算子(rich compare)
/// @return 比較結果を返す．
///
/// 比較方法はキューブごとの辞書式順序
int
compare(
  const SopCover& left, ///< [in] 第1オペランド
  const SopCover& right ///< [in] 第2オペランド
);

/// @relates SopCover
/// @brief 比較演算子 (EQ)
/// @return 等しい時に true を返す．
inline
bool
operator==(
  const SopCover& left, ///< [in] 第1オペランド
  const SopCover& right ///< [in] 第2オペランド
)
{
  return compare(left, right) == 0;
}

/// @relates SopCover
/// @brief 比較演算子 (NE)
/// @return 等しくない時に true を返す．
inline
bool
operator!=(
  const SopCover& left, ///< [in] 第1オペランド
  const SopCover& right ///< [in] 第2オペランド
)
{
  return compare(left, right) != 0;
}

/// @relates SopCover
/// @brief 比較演算子 (LT)
/// @return left が right より小さい時に true を返す．
inline
bool
operator<(
  const SopCover& left, ///< [in] 第1オペランド
  const SopCover& right ///< [in] 第2オペランド
)
{
  return compare(left, right) < 0;
}

/// @relates SopCover
/// @brief 比較演算子 (GT)
/// @return left が right より大きい時に true を返す．
inline
bool
operator>(
  const SopCover& left, ///< [in] 第1オペランド
  const SopCover& right ///< [in] 第2オペランド
)
{
  return compare(left, right) > 0;
}

/// @relates SopCover
/// @brief 比較演算子 (LE)
/// @return left が right と等しいか小さい時に true を返す．
inline
bool
operator<=(
  const SopCover& left, ///< [in] 第1オペランド
  const SopCover& right ///< [in] 第2オペランド
)
{
  return compare(left, right) <= 0;
}

/// @relates SopCover
/// @brief 比較演算子 (GE)
/// @return left が right と等しいか大きい時に true を返す．
inline
bool
operator>=(
  const SopCover& left, ///< [in] 第1オペランド
  const SopCover& right ///< [in] 第2オペランド
)
{
  return compare(left, right) >= 0;
}

/// @relates SopCover
/// @brief SopCover の内容を出力する．
///
/// cover.print(s) と等価
inline
ostream&
operator<<(
  ostream& s,           ///< [in] 出力先のストリーム
  const SopCover& cover ///< [in] 対象のカバー
)
{
  cover.print(s);
  return s;
}

END_NAMESPACE_YM_SOP

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
