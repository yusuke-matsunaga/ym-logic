#ifndef YM_SOPCUBE_H
#define YM_SOPCUBE_H

/// @file ym/SopCube.h
/// @brief SopCube のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/SopBase.h"
#include "ym/Expr.h"
#include "ym/TvFunc.h"
#include "ym/Literal.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
/// @ingroup SopGroup
/// @class SopCube SopCube.h "ym/SopCube.h"
/// @brief キューブ(積項)を表すクラス
///
/// * Literal の論理積を表す．
/// * 常に固定サイズのビット配列として実装する．
/// * そのため初期化の際に変数の数が必要となる．
/// * 1つの変数につき2ビットを使用する．
/// * 相反するリテラルの論理積の結果は不正なキューブとなる．
/// * 不正なキューブを使った演算結果はすべて不正なキューブとなる．
//////////////////////////////////////////////////////////////////////
class SopCube:
  public SopBase
{
public:
  //////////////////////////////////////////////////////////////////////
  /// @name コンストラクタ/デストラクタ
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  ///
  /// * 空のキューブを作る．
  explicit
  SopCube(
    SizeType var_num = 0 ///< [in] 変数の数
  ) : SopBase{var_num},
      mChunk(_cube_size(), SOP_ALL1)
  {
  }

  /// @brief コンストラクタ
  ///
  /// * 単一のリテラルからなるキューブを作る．
  SopCube(
    SizeType var_num, ///< [in] 変数の数
    Literal lit       ///< [in] リテラル
  ) : SopCube{var_num}
  {
    _check_lit(lit);
    _cube_set_literal(mChunk.begin(), lit);
  }

  /// @brief コンストラクタ
  SopCube(
    SizeType var_num,               ///< [in] 変数の数
    const vector<Literal>& lit_list ///< [in] キューブを表すリテラルのリスト
  ) : SopCube{var_num}
  {
    _check_lits(lit_list);
    _cube_set_literals(mChunk.begin(), lit_list);
  }

  /// @brief コンストラクタ
  SopCube(
    SizeType var_num,                   ///< [in] 変数の数
    initializer_list<Literal>& lit_list ///< [in] キューブを表すリテラル
                                        ///<      のリスト初期化子
  ) : SopCube{var_num}
  {
    _check_lits(lit_list);
    _cube_set_literals(mChunk.begin(), lit_list);
  }

  /// @brief コピーコンストラクタ
  SopCube(
    const SopCube& src ///< [in] コピー元のオブジェクト
  ) : SopBase{src.variable_num()},
    mChunk{src.mChunk}
  {
  }

  /// @brief コピー代入演算子
  /// @return 代入後の自身への参照を返す．
  SopCube&
  operator=(
    const SopCube& src ///< [in] コピー元のオブジェクト
  )
  {
    if ( this != &src ) {
      SopBase::operator=(src);
      mChunk = src.mChunk;
    }

    return *this;
  }

  /// @brief ムーブコンストラクタ
  SopCube(
    SopCube&& src ///< [in] ムーブ元のオブジェクト
  ) : SopBase{src.variable_num()},
      mChunk{std::move(src.mChunk)}
  {
  }

  /// @brief ムーブ代入演算子
  /// @return 代入後の自身への参照を返す．
  SopCube&
  operator=(
    SopCube&& src ///< [in] ムーブ元のオブジェクト
  )
  {
    SopBase::operator=(src);
    std::swap(mChunk, src.mChunk);

    return *this;
  }

  /// @brief 内容を指定するコンストラクタ
  SopCube(
    SizeType var_num, ///< [in] 変数の数
    Chunk&& chunk     ///< [in] キューブのパタンを表す本体
  ) : SopBase{var_num},
      mChunk{chunk}
  {
  }

  /// @brief 不正なキューブを作るクラスメソッド
  static
  SopCube
  invalid(
    SizeType var_num ///< [in] 変数の数
  )
  {
    auto cube = SopCube{var_num};
    cube._cube_make_invalid(cube._dst_cube());
    return cube;
  }

  /// @brief デストラクタ
  ~SopCube() = default;

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 内容を読み出す関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 適正なキューブの時 true を返す．
  bool
  is_valid() const
  {
    return !is_invalid();
  }

  /// @brief 不正なキューブの時 true を返す．
  bool
  is_invalid() const
  {
    if ( chunk().empty() ) {
      return true;
    }
    return _cube_is_invalid(_cube());
  }

  /// @brief 空のキューブの時 true を返す．
  bool
  is_tautology() const
  {
    return _cube_check_null(_cube());
  }

  /// @brief リテラル数を返す．
  SizeType
  literal_num() const
  {
    if ( is_invalid() ) {
      return 0;
    }
    return _literal_num(1, chunk());
  }

  /// @brief 指定した変数のパタンを読み出す．
  /// @retval SopPat::_X その変数は現れない．
  /// @retval SopPat::_1 その変数が肯定のリテラルとして現れる．
  /// @retval SopPat::_0 その変数が否定のリテラルとして現れる．
  SopPat
  get_pat(
    SizeType var ///< [in] 変数( 0 <= var_id.val() < variable_num() )
  ) const
  {
    _check_var(var);
    return _get_pat(chunk().begin(), var);
  }

  /// @brief 指定したリテラルを含んでいたら true を返す．
  bool
  check_literal(
    Literal lit ///< [in] 対象のリテラル
  ) const
  {
    _check_lit(lit);
    auto varid = lit.varid();
    auto inv = lit.is_negative();
    return _cube_check_literal(_cube(), varid, inv);
  }

  /// @brief 指定したリテラルを含んでいたら true を返す．
  bool
  check_literal(
    SizeType varid, ///< [in] 変数番号
    bool inv        ///< [in] 反転属性
  ) const
  {
    _check_var(varid);
    return _cube_check_literal(_cube(), varid, inv);
  }

  /// @brief 内容をリテラルのリストに変換する．
  vector<Literal>
  literal_list() const
  {
    return _cube_literal_list(_cube());
  }

  /// @brief Expr に変換する．
  Expr
  expr() const
  {
    return _to_expr(1, chunk());
  }

  /// @brief TvFunc に変換する．
  TvFunc
  tvfunc() const
  {
    return TvFunc::cube(variable_num(), literal_list());
  }

  /// @brief キューブのリスト（カバー）を TvFunc に変換する．
  ///
  /// - キューブの入力数は ni と等しくなければならない．
  /// - cube_list が空リストの場合は定数0関数が返される．
  static
  TvFunc
  tvfunc(
    SizeType ni,                     ///< [in] 変数の数
    const vector<SopCube>& cube_list ///< [in] キューブのリスト
  )
  {
    if ( cube_list.empty() ) {
      return TvFunc{ni};
    }
    for ( auto& cube: cube_list ) {
      if ( cube.variable_num() != ni ) {
	throw std::invalid_argument{"variable_num() mismatch"};
      }
    }
    vector<vector<Literal>> lits_list;
    lits_list.reserve(cube_list.size());
    for ( auto& cube: cube_list ) {
      lits_list.push_back(cube.literal_list());
    }
    return TvFunc::cover(ni, lits_list);
  }

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
  hash() const
  {
    return _hash(1, chunk());
  }

  /// @brief 内容をわかりやすい形で出力する．
  void
  print(
    ostream& s,                             ///< [in] 出力先のストリーム
    const vector<string>& varname_list = {} ///< [in] 変数名のリスト
  ) const
  {
    _print(s, chunk(), 0, 1, varname_list);
  }

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 演算を行う関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief キューブの論理積を計算する
  /// @return 論理積のキューブを返す．
  ///
  /// 相反するリテラルが含まれていたら不正なキューブとなる．
  SopCube
  operator&(
    const SopCube& right ///< [in] オペランド
  ) const
  {
    _check_size(right);
    auto dst_chunk = _new_chunk(1);
    auto dst_cube = SopBase::_dst_cube(dst_chunk);
    auto cube1 = _cube();
    auto cube2 = right._cube();
    if ( !_cube_product(dst_cube, cube1, cube2) ) {
      _cube_make_invalid(dst_cube);
    }
    return SopCube{variable_num(), std::move(dst_chunk)};
  }

  /// @brief 論理積を計算し自身に代入する．
  /// @return 演算後の自身の参照を返す．
  ///
  /// 相反するリテラルとの積があったら答は不正なキューブとなる．
  SopCube&
  operator&=(
    const SopCube& right ///< [in] オペランドのキューブ
  )
  {
    _check_size(right);
    auto dst_cube = _dst_cube();
    auto src_cube = right._cube();
    if ( !_cube_product_int(dst_cube, src_cube) ) {
      _cube_make_invalid(dst_cube);
    }
    return *this;
  }

  /// @brief キューブとリテラルの論理積を計算する
  /// @return 論理積のキューブを返す．
  ///
  /// 相反するリテラルが含まれていたら不正なキューブとなる．
  SopCube
  operator&(
    Literal right ///< [in] オペランドのリテラル
  ) const
  {
    _check_lit(right);
    auto dst_chunk = _new_chunk(1);
    auto dst_cube = SopBase::_dst_cube(dst_chunk);
    auto src_cube = _cube();
    if ( !_cube_product(dst_cube, src_cube, right) ) {
      _cube_make_invalid(dst_cube);
    }
    return SopCube{variable_num(), std::move(dst_chunk)};
  }

  /// @brief リテラルとの論理積を計算し自身に代入する．
  /// @return 演算後の自身の参照を返す．
  ///
  /// 相反するリテラルとの積があったら答は不正なキューブとなる．
  SopCube&
  operator&=(
    Literal right ///< [in] オペランドのリテラル
  )
  {
    _check_lit(right);
    auto dst_cube = _dst_cube();
    if ( !_cube_product_int(dst_cube, right) ) {
      _cube_make_invalid(dst_cube);
    }
    return *this;
  }

  /// @brief キューブによる商を計算する
  ///
  /// リテラル集合として考えると集合差を計算することになる<br>
  /// ただし，right のみに含まれるリテラルがあったら結果は不正となる．
  SopCube
  operator/(
    const SopCube& right ///< [in] オペランド
  ) const
  {
    _check_size(right);
    auto dst_chunk = _new_chunk(1);
    auto dst_cube = SopBase::_dst_cube(dst_chunk);
    auto cube1 = _cube();
    auto cube2 = right._cube();
    if ( !_cube_algdiv(dst_cube, cube1, cube2) ) {
      _cube_make_invalid(dst_cube);
    }
    return SopCube{variable_num(), std::move(dst_chunk)};
  }

  /// @brief キューブによる商を計算し自身に代入する．
  /// @return 演算後の自身の参照を返す．
  ///
  /// リテラル集合として考えると集合差を計算することになる<br>
  /// ただし，right のみに含まれるリテラルがあったら結果は不正となる．
  SopCube&
  operator/=(
    const SopCube& right ///< [in] オペランドのキューブ
  )
  {
    _check_size(right);
    auto dst_cube = _dst_cube();
    auto cube2 = right._cube();
    if ( !_cube_algdiv(dst_cube, dst_cube, cube2) ) {
      _cube_make_invalid(dst_cube);
    }
    return *this;
  }

  /// @brief リテラルによる商を計算する
  ///
  /// リテラル集合として考えると集合差を計算することになる<br>
  /// ただし，right のみに含まれるリテラルがあったら結果は不正となる．
  SopCube
  operator/(
    Literal right ///< [in] オペランドのリテラル
  ) const
  {
    _check_lit(right);
    auto dst_chunk = _new_chunk(1);
    auto dst_cube = SopBase::_dst_cube(dst_chunk);
    auto cube1 = _cube();
    if ( !_cube_algdiv(dst_cube, cube1, right) ) {
      _cube_make_invalid(dst_cube);
    }
    return SopCube{variable_num(), std::move(dst_chunk)};
  }

  /// @brief リテラルによる商を計算し自身に代入する．
  /// @return 演算後の自身の参照を返す．
  ///
  /// リテラル集合として考えると集合差を計算することになる<br>
  /// ただし，right のみに含まれるリテラルがあったら結果は不正となる．
  SopCube&
  operator/=(
    Literal right ///< [in] オペランドのリテラル
  )
  {
    _check_lit(right);
    auto dst_cube = _dst_cube();
    if ( !_cube_algdiv(dst_cube, dst_cube, right) ) {
      _cube_make_invalid(dst_cube);
    }
    return *this;
  }

  /// @brief オペランドのキューブに含まれていたら true を返す．
  ///
  /// * ここではキューブの表す論理関数の含意を考える．
  /// * だからリテラル集合としてはオペランドのキューブを含むことになる．
  bool
  check_containment(
    const SopCube& right ///< [in] オペランドのキューブ
  ) const
  {
    _check_size(right);
    auto cube1 = _cube();
    auto end1 = _cube_end(cube1);
    auto cube2 = right._cube();
    for ( ; cube1 != end1; ++ cube1, ++ cube2 ) {
      auto pat1 = *cube1;
      auto pat2 = *cube2;
      if ( (pat1 & pat2) != pat1 ) {
	return false;
      }
    }
    return true;
  }

  /// @brief SopCubeの比較演算子(EQ)
  /// @retval true  left == right
  /// @retval false left != right
  bool
  operator==(
    const SopCube& right ///< [in] 第2オペランド
  ) const
  {
    _check_size(right);
    return _compare(right) == 0;
  }

  /// @brief SopCubeの比較演算子(NE)
  /// @retval true  left != right
  /// @retval false left == right
  bool
  operator!=(
    const SopCube& right ///< [in] 第2オペランド
  ) const
  {
    _check_size(right);
    return _compare(right) != 0;
  }

  /// @brief SopCubeの比較演算子(LT)
  /// @retval true  left < right
  /// @retval false left >= right
  bool
  operator<(
    const SopCube& right ///< [in] 第2オペランド
  ) const
  {
    _check_size(right);
    return _compare(right) < 0;
  }

  /// @brief SopCubeの比較演算子(GT)
  /// @retval true  left > right
  /// @retval false left <= right
  bool
  operator>(
    const SopCube& right ///< [in] 第2オペランド
  ) const
  {
    _check_size(right);
    return _compare(right) > 0;
  }

  /// @brief SopCubeの比較演算子(LE)
  /// @retval true  left < right
  /// @retval false left >= right
  bool
  operator<=(
    const SopCube& right ///< [in] 第2オペランド
  ) const
  {
    _check_size(right);
    return _compare(right) <= 0;
  }

  /// @brief SopCubeの比較演算子(GE)
  /// @retval true  left < right
  /// @retval false left >= right
  bool
  operator>=(
    const SopCube& right ///< [in] 第2オペランド
  ) const
  {
    _check_size(right);
    return _compare(right) >= 0;
  }

  /// @relates SopCube
  /// @brief 比較関数(rich compare)
  static
  int
  compare(
    const SopCube& left, ///< [in] 第1オペランド
    const SopCube& right ///< [in] 第2オペランド
  )
  {
    left._check_size(right);
    return left._compare(right);
  }

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内部で用いられるキューブを得る．
  Cube
  _cube() const
  {
    return SopBase::_cube(chunk());
  }

  /// @brief 内部で用いられるキューブを得る．
  DstCube
  _dst_cube()
  {
    return SopBase::_dst_cube(chunk());
  }

  /// @brief 比較関数(rich compare)
  int
  _compare(
    const SopCube& right ///< [in] 第2オペランド
  ) const
  {
    auto cube1 = _cube();
    auto cube2 = right._cube();
    return _cube_compare(cube1, cube2);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 内容を表すビットベクタ
  Chunk mChunk;

};

/// @relates SopCube
/// @brief キューブの論理積を計算する
///
/// 相反するリテラルが含まれていたら空キューブとなる．
inline
SopCube
operator&(
  SopCube&& left,      ///< [in] 第1オペランド
  const SopCube& right ///< [in] 第2オペランド
)
{
  return SopCube{std::move(left)}.operator&=(right);
}

/// @relates SopCube
/// @brief キューブの論理積を計算する
///
/// 相反するリテラルが含まれていたら空キューブとなる．
inline
SopCube
operator&(
  const SopCube& left, ///< [in] 第1オペランド
  SopCube&& right      ///< [in] 第2オペランド
)
{
  return SopCube{std::move(right)}.operator&=(left);
}

/// @relates SopCube
/// @brief キューブの論理積を計算する
///
/// 相反するリテラルが含まれていたら空キューブとなる．
inline
SopCube
operator&(
  SopCube&& left, ///< [in] 第1オペランド
  SopCube&& right ///< [in] 第2オペランド
)
{
  return SopCube{std::move(left)}.operator&=(right);
}

/// @relates SopCube
/// @brief キューブとリテラルの論理積を計算する
///
/// 相反するリテラルが含まれていたら空キューブとなる．
inline
SopCube
operator&(
  SopCube&& left, ///< [in] 第1オペランド
  Literal right	  ///< [in] 第2オペランド
)
{
  return SopCube{std::move(left)}.operator&=(right);
}

/// @relates SopCube
/// @brief Literal と SopCube の論理積
/// @return 結果のキューブを返す．
inline
SopCube
operator&(
  Literal left,        ///< [in] 第1オペランド
  const SopCube& right ///< [in] 第2オペランド
)
{
  // 交換則を用いる．
  return right.operator&(left);
}

/// @relates SopCube
/// @brief Literal と SopCube の論理積
/// @return 結果のキューブを返す．
inline
SopCube
operator&(
  Literal left,    ///< [in] 第1オペランド
  SopCube&& right  ///< [in] 第2オペランド
)
{
  // 交換則を用いる．
  return SopCube{std::move(right)}.operator&=(left);
}

/// @relates SopCube
/// @brief キューブの除算を計算する
///
/// リテラル集合としてみると集合差となる<br>
inline
SopCube
operator/(
  SopCube&& left,      ///< [in] 第1オペランド(ムーブ参照)
  const SopCube& right ///< [in] 第2オペランド
)
{
  return SopCube{std::move(left)}.operator/=(right);
}

/// @relates SopCube
/// @brief キューブとリテラルの除算を計算する
///
/// リテラル集合としてみると集合差となる<br>
inline
SopCube
operator/(
  SopCube&& left, ///< [in] 第1オペランド(ムーブ参照)
  Literal right	  ///< [in] 第2オペランド
)
{
  return SopCube{std::move(left)}.operator/=(right);
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
