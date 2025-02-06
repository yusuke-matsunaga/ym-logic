#ifndef YM_ALGCUBE_H
#define YM_ALGCUBE_H

/// @file ym/AlgCube.h
/// @brief AlgCube のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/AlgBase.h"
#include "ym/Expr.h"
#include "ym/Literal.h"


BEGIN_NAMESPACE_YM_ALG

//////////////////////////////////////////////////////////////////////
/// @ingroup AlgGroup
/// @class AlgCube AlgCube.h "ym/AlgCube.h"
/// @brief キューブ(積項)を表すクラス
///
/// * 離散数学的には Literal の集合だが，相反するリテラル(x と x')は
///   同時には含まない．
/// * 常に固定サイズのビット配列として実装する．
/// * そのため初期化の際に変数の数が必要となる．
/// * 1つの変数につき2ビットを使用する．
//////////////////////////////////////////////////////////////////////
class AlgCube:
  public AlgBase
{
public:

  /// @brief コンストラクタ
  ///
  /// * 空のキューブを作る．
  explicit
  AlgCube(
    SizeType variable_num = 0 ///< [in] 変数の数
  );

  /// @brief コンストラクタ
  ///
  /// * 単一のリテラルからなるキューブを作る．
  AlgCube(
    SizeType variable_num, ///< [in] 変数の数
    Literal lit            ///< [in] リテラル
  );

  /// @brief コンストラクタ
  AlgCube(
    SizeType variable_num,          ///< [in] 変数の数
    const vector<Literal>& lit_list ///< [in] キューブを表すリテラルのリスト
  );

  /// @brief コンストラクタ
  AlgCube(
    SizeType variable_num,              ///< [in] 変数の数
    initializer_list<Literal>& lit_list ///< [in] キューブを表すリテラルのリスト初期化子
  );

  /// @brief コピーコンストラクタ
  AlgCube(
    const AlgCube& src ///< [in] コピー元のオブジェクト
  );

  /// @brief コピー代入演算子
  /// @return 代入後の自身への参照を返す．
  AlgCube&
  operator=(
    const AlgCube& src ///< [in] コピー元のオブジェクト
  );

  /// @brief ムーブコンストラクタ
  AlgCube(
    AlgCube&& src ///< [in] ムーブ元のオブジェクト
  );

  /// @brief ムーブ代入演算子
  /// @return 代入後の自身への参照を返す．
  AlgCube&
  operator=(
    AlgCube&& src ///< [in] ムーブ元のオブジェクト
  );

  /// @brief 内容を指定するコンストラクタ
  AlgCube(
    SizeType variable_num, ///< [in] 変数の数
    Chunk&& chunk          ///< [in] キューブのパタンを表す本体
  );

  /// @brief デストラクタ
  ~AlgCube() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief リテラル数を返す．
  SizeType
  literal_num() const;

  /// @brief 空のキューブの時 true を返す．
  bool
  is_tautology() const;

  /// @brief 指定した変数のパタンを読み出す．
  /// @retval AlgPat::_X その変数は現れない．
  /// @retval AlgPat::_1 その変数が肯定のリテラルとして現れる．
  /// @retval AlgPat::_0 その変数が否定のリテラルとして現れる．
  AlgPat
  get_pat(
    SizeType var ///< [in] 変数( 0 <= var_id.val() < variable_num() )
  ) const;

  /// @brief 指定したリテラルを含んでいたら true を返す．
  bool
  check_literal(
    Literal lit ///< [in] 対象のリテラル
  ) const
  {
    auto varid = lit.varid();
    auto inv = lit.is_negative();
    return check_literal(varid, inv);
  }

  /// @brief 指定したリテラルを含んでいたら true を返す．
  bool
  check_literal(
    SizeType varid, ///< [in] 変数番号
    bool inv        ///< [in] 反転属性
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
    const AlgCube& right ///< [in] オペランドのキューブ
  ) const;

  /// @brief Expr に変換する．
  Expr
  expr() const;

  /// @brief 本体のビットベクタを返す．
  const Chunk&
  chunk() const
  {
    return mChunk;
  }

  /// @brief 本体のビットベクタを返す．
  Chunk&
  chunk()
  {
    return mChunk;
  }

  /// @brief ハッシュ値を返す．
  SizeType
  hash() const;

  /// @brief 内容をわかりやすい形で出力する．
  void
  print(
    ostream& s,                             ///< [in] 出力先のストリーム
    const vector<string>& varname_list = {} ///< [in] 変数名のリスト
  ) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 演算子のオーバーロード
  //////////////////////////////////////////////////////////////////////

  /// @brief キューブの論理積を計算する
  ///
  /// リテラル集合としてみると和集合となる<br>
  /// ただし，相反するリテラルが含まれていたら空キューブとなる．
  AlgCube
  operator*(
    const AlgCube& right ///< [in] オペランド
  ) const;

  /// @brief 論理積を計算し自身に代入する．
  /// @return 演算後の自身の参照を返す．
  ///
  /// リテラル集合とみなすとユニオンを計算することになる<br>
  /// ただし，相反するリテラルとの積があったら答は空のキューブとなる．
  AlgCube&
  operator*=(
    const AlgCube& right ///< [in] オペランドのキューブ
  );

  /// @brief キューブとリテラルの論理積を計算する
  ///
  /// リテラル集合としてみると和集合となる<br>
  /// ただし，相反するリテラルが含まれていたら空キューブとなる．
  AlgCube
  operator*(
    Literal right ///< [in] オペランドのリテラル
  ) const;

  /// @brief リテラルとの論理積を計算し自身に代入する．
  /// @return 演算後の自身の参照を返す．
  ///
  /// リテラル集合とみなすとユニオンを計算することになる<br>
  /// ただし，相反するリテラルとの積があったら答は空のキューブとなる．
  AlgCube&
  operator*=(
    Literal right ///< [in] オペランドのリテラル
  );

  /// @brief キューブによる商を計算する
  ///
  /// リテラル集合として考えると集合差を計算することになる<br>
  /// ただし，right のみに含まれるリテラルがあったら結果は空となる．
  AlgCube
  operator/(
    const AlgCube& right ///< [in] オペランド
  ) const;

  /// @brief キューブによる商を計算し自身に代入する．
  /// @return 演算後の自身の参照を返す．
  ///
  /// リテラル集合として考えると集合差を計算することになる<br>
  /// ただし，right のみに含まれるリテラルがあったら結果は空となる．
  AlgCube&
  operator/=(
    const AlgCube& right ///< [in] オペランドのキューブ
  );

  /// @brief リテラルによる商を計算する
  ///
  /// リテラル集合として考えると集合差を計算することになる<br>
  /// ただし，right のみに含まれるリテラルがあったら結果は空となる．
  AlgCube
  operator/(
    Literal right ///< [in] オペランドのリテラル
  ) const;

  /// @brief リテラルによる商を計算し自身に代入する．
  /// @return 演算後の自身の参照を返す．
  ///
  /// リテラル集合として考えると集合差を計算することになる<br>
  /// ただし，right のみに含まれるリテラルがあったら結果は空となる．
  AlgCube&
  operator/=(
    Literal right ///< [in] オペランドのリテラル
  );

  /// @brief AlgCubeの比較演算子(EQ)
  /// @retval true  left == right
  /// @retval false left != right
  bool
  operator==(
    const AlgCube& right ///< [in] 第2オペランド
  ) const
  {
    return _compare(right) == 0;
  }

  /// @brief AlgCubeの比較演算子(NE)
  /// @retval true  left != right
  /// @retval false left == right
  bool
  operator!=(
    const AlgCube& right ///< [in] 第2オペランド
  ) const
  {
    return _compare(right) != 0;
  }

  /// @brief AlgCubeの比較演算子(LT)
  /// @retval true  left < right
  /// @retval false left >= right
  bool
  operator<(
    const AlgCube& right ///< [in] 第2オペランド
  ) const
  {
    return _compare(right) < 0;
  }

  /// @brief AlgCubeの比較演算子(GT)
  /// @retval true  left > right
  /// @retval false left <= right
  bool
  operator>(
    const AlgCube& right ///< [in] 第2オペランド
  ) const
  {
    return _compare(right) > 0;
  }

  /// @brief AlgCubeの比較演算子(LE)
  /// @retval true  left < right
  /// @retval false left >= right
  bool
  operator<=(
    const AlgCube& right ///< [in] 第2オペランド
  ) const
  {
    return _compare(right) <= 0;
  }

  /// @brief AlgCubeの比較演算子(GE)
  /// @retval true  left < right
  /// @retval false left >= right
  bool
  operator>=(
    const AlgCube& right ///< [in] 第2オペランド
  ) const
  {
    return _compare(right) >= 0;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内部で用いられるキューブを得る．
  Cube
  _cube() const;

  /// @brief 内部で用いられるキューブを得る．
  DstCube
  _dst_cube();

  /// @brief AlgCubeの比較演算子
  /// @retval -1 left < right
  /// @retval  0 left = right
  /// @retval  1 left > right
  int
  _compare(
    const AlgCube& right ///< [in] 第2オペランド
  ) const;

  friend
  int
  compare(
    const AlgCube& left,
    const AlgCube& right
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 内容を表すビットベクタ
  Chunk mChunk;

};

/// @relates AlgCube
/// @brief キューブの論理積を計算する
///
/// リテラル集合としてみると和集合となる<br>
/// ただし，相反するリテラルが含まれていたら空キューブとなる．
inline
AlgCube
operator*(
  AlgCube&& left,      ///< [in] 第1オペランド(ムーブ参照)
  const AlgCube& right ///< [in] 第2オペランド
)
{
  return AlgCube{std::move(left)}.operator*=(right);
}

/// @relates AlgCube
/// @brief キューブの論理積を計算する
///
/// リテラル集合としてみると和集合となる<br>
/// ただし，相反するリテラルが含まれていたら空キューブとなる．
inline
AlgCube
operator*(
  const AlgCube& left, ///< [in] 第1オペランド
  AlgCube&& right      ///< [in] 第2オペランド(ムーブ参照)
)
{
  return AlgCube{std::move(right)}.operator*=(left);
}

/// @relates AlgCube
/// @brief キューブの論理積を計算する
///
/// リテラル集合としてみると和集合となる<br>
/// ただし，相反するリテラルが含まれていたら空キューブとなる．
inline
AlgCube
operator*(
  AlgCube&& left, ///< [in] 第1オペランド(ムーブ参照)
  AlgCube&& right ///< [in] 第2オペランド(ムーブ参照)
)
{
  return AlgCube{std::move(left)}.operator*=(right);
}

/// @relates AlgCube
/// @brief キューブとリテラルの論理積を計算する
///
/// リテラル集合としてみると和集合となる<br>
/// ただし，相反するリテラルが含まれていたら空キューブとなる．
inline
AlgCube
operator*(
  AlgCube&& left, ///< [in] 第1オペランド(ムーブ参照)
  Literal right	  ///< [in] 第2オペランド
)
{
  return AlgCube{std::move(left)}.operator*=(right);
}

/// @relates AlgCube
/// @brief Literal と AlgCube の論理積
/// @return 結果のキューブを返す．
inline
AlgCube
operator*(
  Literal left,        ///< [in] 第1オペランド
  const AlgCube& right ///< [in] 第2オペランド
)
{
  // 交換則を用いる．
  return right.operator*(left);
}

/// @relates AlgCube
/// @brief Literal と AlgCube の論理積
/// @return 結果のキューブを返す．
inline
AlgCube
operator*(
  Literal left,    ///< [in] 第1オペランド
  AlgCube&& right  ///< [in] 第2オペランド(ムーブ参照)
)
{
  // 交換則を用いる．
  return AlgCube{std::move(right)}.operator*=(left);
}

/// @relates AlgCube
/// @brief キューブの除算を計算する
///
/// リテラル集合としてみると集合差となる<br>
inline
AlgCube
operator/(
  AlgCube&& left,      ///< [in] 第1オペランド(ムーブ参照)
  const AlgCube& right ///< [in] 第2オペランド
)
{
  return AlgCube{std::move(left)}.operator/=(right);
}

/// @relates AlgCube
/// @brief キューブとリテラルの除算を計算する
///
/// リテラル集合としてみると集合差となる<br>
inline
AlgCube
operator/(
  AlgCube&& left, ///< [in] 第1オペランド(ムーブ参照)
  Literal right	  ///< [in] 第2オペランド
)
{
  return AlgCube{std::move(left)}.operator/=(right);
}

/// @relates AlgCube
/// @brief AlgCubeの比較演算子
/// @retval -1 left < right
/// @retval  0 left = right
/// @retval  1 left > right
inline
int
compare(
  const AlgCube& left, ///< [in] 第1オペランド
  const AlgCube& right ///< [in] 第2オペランド
)
{
  return left._compare(right);
}

/// @relates AlgCube
/// @brief AlgCube の内容を出力する．
///
/// cube->print(s) と等価
inline
ostream&
operator<<(
  ostream& s,         ///< [in] 出力先のストリーム
  const AlgCube& cube ///< [in] 対象のキューブ(のポインタ)
)
{
  cube.print(s);
  return s;
}

END_NAMESPACE_YM_ALG

BEGIN_NAMESPACE_STD

/// @breif AlgCube をキーにしたハッシュ関数クラスの定義
template <>
struct hash<YM_NAMESPACE::AlgCube>
{
  SizeType
  operator()(const YM_NAMESPACE::AlgCube& cube) const
  {
    return cube.hash();
  }
};

END_NAMESPACE_STD

#endif // YM_ALGCUBE_H
