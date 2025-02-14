#ifndef YM_SOPCOVER_H
#define YM_SOPCOVER_H

/// @file ym/SopCover.h
/// @brief SopCover のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/SopBase.h"
#include "ym/SopCube.h"
#include "ym/Expr.h"
#include "ym/TvFunc.h"
#include "ym/Literal.h"


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
/// @class SopCover SopCover.h "ym/SopCover.h"
/// @brief カバー(積和形論理式)を表すクラス
///
/// 中身は positional cube matrix 表現を用いたビットベクタ<br>
/// ビットベクタの内容は一旦，設定されたら変わることはない<br>
/// キューブはワードレベルの順序で整列しているものと仮定している<br>
///
/// 実装としてはパタンの実体は SopBase::Chunk が持つ．
//////////////////////////////////////////////////////////////////////
class SopCover:
  public SopBase
{
public:

  /// @brief Literal のリストのリストの形の初期化子
  using initializer = std::initializer_list<std::initializer_list<Literal>>;

public:

  /// @brief コンストラクタ
  ///
  /// * 空のカバーとなる．
  explicit
  SopCover(
    SizeType var_num = 0 ///< [in] 変数の数
  ) : SopBase{var_num},
      mCubeNum{0}
  {
  }

  /// @brief コンストラクタ
  ///
  /// * cube_list 中の各キューブのサイズは var_num
  ///   と等しくなければならない．
  /// * キューブの順番は変わる可能性がある．
  SopCover(
    SizeType var_num,                ///< [in] 変数の数
    const vector<SopCube>& cube_list ///< [in] キューブのリスト
  ) : SopBase{var_num},
      mCubeNum{cube_list.size()},
      mChunk(_cube_size() * cube_num(), SOP_ALL1)
  {
    // cube_list のキューブのサイズが正しいかチェック
    for ( auto& cube: cube_list ) {
      _check_size(cube);
    }

    // mChunk に内容をコピーする．
    auto dst_list = _cube_list(chunk());
    for ( auto& cube: cube_list ) {
      auto src_cube = _cube(cube.chunk());
      auto dst_cube = dst_list.back();
      _cube_copy(dst_cube, src_cube);
      dst_list.inc();
    }
    _sort();
  }

  /// @brief コンストラクタ
  ///
  /// * キューブの順番は変わる可能性がある．
  SopCover(
    SizeType var_num,                        ///< [in] 変数の数
    const vector<vector<Literal>>& cube_list ///< [in] カバーを表すリテラル
                                             ///<      のリストのリスト
  ) : SopBase{var_num},
      mCubeNum{cube_list.size()},
      mChunk(_cube_size() * cube_num(), SOP_ALL1)
  {
    // cube_list の中のリテラル番号が var_num に収まるがチェック
    for ( auto& lits: cube_list ) {
      _check_lits(lits);
    }

    // mChunk に内容をコピーする．
    auto dst_list = _cube_list(chunk());
    for ( auto& lits: cube_list ) {
      auto dst_cube = dst_list.back();
      _cube_set_literals(dst_cube, lits);
      dst_list.inc();
    }
    _sort();
  }

  /// @brief コンストラクタ
  ///
  /// * キューブの順番は変わる可能性がある．
  SopCover(
    SizeType var_num,      ///< [in] 変数の数
    initializer& cube_list ///< [in] カバーを表すリテラルのリストのリスト
  ) : SopBase{var_num},
      mCubeNum{cube_list.size()},
      mChunk(_cube_size() * cube_list.size(), SOP_ALL1)
  {
    // cube_list の中のリテラル番号が var_num に収まるがチェック
    for ( auto& lits: cube_list ) {
      _check_lits(lits);
    }

    // mChunk に内容をコピーする．
    auto dst_list = _cube_list(chunk());
    for ( auto& lits: cube_list ) {
      auto dst_cube = dst_list.back();
      _cube_set_literals(dst_cube, lits);
      dst_list.inc();
    }
    _sort();
  }

  /// @brief コピーコンストラクタ
  SopCover(
    const SopCover& src ///< [in] コピー元のオブジェクト
  ) : SopBase{src.variable_num()},
      mCubeNum{src.mCubeNum},
      mChunk(_cube_size() * cube_num(), SOP_ALL1)
  {
    // mChunk に内容をコピーする．
    // mChunk の容量は余分に確保されている可能性があるので
    // _cube_size() に基づいて必要な分だけコピーする．
    auto src_begin = src.chunk().begin();
    auto src_end = src_begin + _cube_size() * cube_num();
    auto dst_begin = mChunk.begin();
    std::copy(src_begin, src_end, dst_begin);
  }

  /// @brief コピー代入演算子
  /// @return 代入後の自身の参照を返す．
  SopCover&
  operator=(
    const SopCover& src ///< [in] コピー元のオブジェクト
  )
  {
    if ( this != &src ) {
      SopBase::operator=(src);
      mCubeNum = src.mCubeNum;
      // mChunk に内容をコピーする．
      // mChunk の容量は余分に確保されている可能性があるので
      // _cube_size() に基づいて必要なぶんだけコピーする．
      SizeType size = _cube_size() * cube_num();
      auto src_begin = src.chunk().begin();
      auto src_end = src_begin + size;
      mChunk.resize(size, SOP_ALL1);
      auto dst_begin = mChunk.begin();
      std::copy(src_begin, src_end, dst_begin);
    }

    return *this;
  }

  /// @brief ムーブコンストラクタ
  SopCover(
    SopCover&& src ///< [in] ムーブ元のオブジェクト
  ) : SopBase{src},
      mCubeNum{src.mCubeNum},
      mChunk{std::move(src.mChunk)}
  {
    src.mCubeNum = 0;
  }

  /// @brief ムーブ代入演算子
  /// @return 代入後の自身の参照を返す．
  SopCover&
  operator=(
    SopCover&& src ///< [in] ムーブ元のオブジェクト
  )
  {
    SopBase::operator=(src);
    mCubeNum = src.mCubeNum;
    src.mCubeNum = 0;
    std::swap(mChunk, src.mChunk);

    return *this;
  }

  /// @brief キューブからのコピー変換コンストラクタ
  ///
  /// 指定されたキューブのみのカバーとなる．
  explicit
  SopCover(
    const SopCube& cube ///< [in] 対象のキューブ
  ) : SopBase{cube.variable_num()},
      mCubeNum{1},
      mChunk{cube.chunk()}
  {
  }

  /// @brief デストラクタ
  ///
  /// ここに属しているすべてのキューブは削除される．
  ~SopCover() = default;


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 内容を読み出す関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief キューブの数を返す．
  SizeType
  cube_num() const
  {
    return mCubeNum;
  }

  /// @brief リテラル数を返す．
  SizeType
  literal_num() const
  {
    return _literal_num(cube_num(), chunk());
  }

  /// @brief 指定されたリテラルの出現回数を返す．
  SizeType
  literal_num(
    Literal lit ///< [in] 対象のリテラル
  ) const
  {
    _check_lit(lit);
    return _literal_num(cube_num(), chunk(), lit);
  }

  /// @brief 指定されたリテラルの出現回数を返す．
  SizeType
  literal_num(
    SizeType varid, ///< [in] 変数番号
    bool inv        ///< [in] 反転属性
  ) const
  {
    _check_var(varid);
    return _literal_num(cube_num(), chunk(), varid, inv);
  }

  /// @brief 内容をリテラルのリストのリストに変換する．
  vector<vector<Literal>>
  literal_list() const
  {
    return _literal_list(cube_num(), chunk());
  }

  /// @brief キューブを取り出す．
  SopCube
  get_cube(
    SizeType cube_id ///< [in] キューブ番号 ( 0 <= cube_id < cube_num() )
  ) const
  {
    _check_cube_id(cube_id);
    auto dst_chunk = _new_chunk(1);
    auto dst_cube = SopBase::_dst_cube(dst_chunk);
    auto src_cube = _cube(chunk(), cube_id);
    _cube_copy(dst_cube, src_cube);
    return SopCube{variable_num(), std::move(dst_chunk)};
  }

  /// @brief 内容をキューブのリストに変換する
  vector<SopCube>
  cube_list() const
  {
    vector<SopCube> ans_list;
    ans_list.reserve(cube_num());
    for ( SizeType i = 0; i < cube_num(); ++ i ) {
      ans_list.push_back(get_cube(i));
    }
    return ans_list;
  }

  /// @brief パタンを返す．
  /// @retval SopPat::_X その変数は現れない．
  /// @retval SopPat::_1 その変数が肯定のリテラルとして現れる．
  /// @retval SopPat::_0 その変数が否定のリテラルとして現れる．
  SopPat
  get_pat(
    SizeType cube_id, ///< [in] キューブ番号 ( 0 <= cube_id < cube_num() )
    SizeType var_id   ///< [in] 変数( 0 <= var_id.val() < variable_num() )
  ) const
  {
    _check_cube_id(cube_id);
    _check_var(var_id);
    auto src_cube = _cube(chunk(), cube_id);
    return _get_pat(src_cube, var_id);
  }

  /// @brief Expr に変換する．
  Expr
  expr() const
  {
    return _to_expr(cube_num(), chunk());
  }

  /// @brief TvFunc に変換する．
  TvFunc
  tvfunc() const
  {
    return TvFunc::cover(variable_num(), literal_list());
  }

  /// @brief 内容を取り出す．
  Chunk&
  chunk()
  {
    return mChunk;
  }

  /// @brief 内容を取り出す．
  const Chunk&
  chunk() const
  {
    return mChunk;
  }

  /// @brief ハッシュ値を返す．
  SizeType
  hash() const
  {
    return _hash(cube_num(), chunk());
  }

  /// @brief 内容をわかりやすい形で出力する．
  void
  print(
    ostream& s,                             ///< [in] 出力先のストリーム
    const vector<string>& varname_list = {} ///< [in] 変数名のリスト
  ) const
  {
    _print(s, chunk(), 0, cube_num(), varname_list);
  }

  /// @brief 内容をデバッグ用に出力する．
  void
  debug_print(
    ostream& s ///< [in] 出力先のストリーム
  ) const
  {
    _debug_print(s, cube_num(), chunk());
  }

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 演算を行う関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 論理和を計算する．
  /// @return 計算結果を返す．
  SopCover
  operator|(
    const SopCover& right ///< [in] オペランド
  ) const;

  /// @brief 論理和を計算して代入する．
  /// @return 演算後の自身への参照を返す．
  SopCover&
  operator|=(
    const SopCover& right ///< [in] オペランド
  );

  /// @brief 論理和を計算する(キューブ版)．
  /// @return 計算結果を返す．
  SopCover
  operator|(
    const SopCube& right ///< [in] オペランド
  ) const;

  /// @brief 論理和を計算して代入する(キューブ版)．
  /// @return 演算後の自身への参照を返す．
  SopCover&
  operator|=(
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
  operator&(
    const SopCover& right ///< [in] オペランド
  ) const;

  /// @brief 論理積を計算して代入する．
  /// @return 演算後の自身への参照を返す．
  SopCover&
  operator&=(
    const SopCover& right ///< [in] オペランド
  );

  /// @brief 論理積を計算する(キューブ版)．
  /// @return 計算結果を返す．
  SopCover
  operator&(
    const SopCube& right ///< [in] オペランド
  ) const;

  /// @brief 論理積を計算して代入する(キューブ版)．
  /// @return 演算後の自身への参照を返す．
  SopCover&
  operator&=(
    const SopCube& right ///< [in] オペランド
  );

  /// @brief 論理積を計算する(リテラル版)．
  /// @return 計算結果を返す．
  SopCover
  operator&(
    Literal right ///< [in] オペランド
  ) const;

  /// @brief 論理積を計算して代入する(リテラル版)．
  /// @return 演算後の自身への参照を返す．
  SopCover&
  operator&=(
    Literal right ///< [in] オペランド
  );

  /// @brief algebraic division を計算する．
  /// @return 計算結果を返す．
  SopCover
  algdiv(
    const SopCover& right ///< [in] オペランド
  ) const;

  /// @brief algebraic division を行って代入する．
  /// @return 演算後の自身への参照を返す．
  SopCover&
  algdiv_int(
    const SopCover& right ///< [in] オペランド
  );

  /// @brief キューブによる商を計算する．
  /// @return 計算結果を返す．
  SopCover
  algdiv(
    const SopCube& cube ///< [in] 対象のキューブ
  ) const;

  /// @brief キューブによる商を計算して代入する．
  /// @return 演算後の自身への参照を返す．
  SopCover&
  algdiv_int(
    const SopCube& cube ///< [in] 対象のキューブ
  );

  /// @brief リテラルによる商を計算する．
  /// @return 計算結果を返す．
  SopCover
  algdiv(
    Literal lit ///< [in] 対象のリテラル
  ) const;

  /// @brief リテラルによる商を計算して代入する．
  /// @return 演算後の自身への参照を返す．
  SopCover&
  algdiv_int(
    Literal lit ///< [in] 対象のリテラル
  );

  /// @brief 共通なキューブを返す．
  ///
  /// 共通なキューブがない場合には空のキューブを返す．
  SopCube
  common_cube() const;

  /// @brief コファクターを計算する．
  /// @return 計算結果を返す．
  SopCover
  cofactor(
    const SopCube& right ///< [in] オペランド
  ) const;

  /// @brief コファクターを計算し自身に代入する．
  /// @return 自身への参照を返す．
  SopCover&
  cofactor_int(
    const SopCube& right ///< [in] オペランド
  );

  /// @brief コファクターを計算する．
  /// @return 計算結果を返す．
  SopCover
  cofactor(
    Literal lit ///< [in] オペランド
  ) const;

  /// @brief コファクターを計算し自身に代入する．
  /// @return 自身への参照を返す．
  SopCover&
  cofactor_int(
    Literal lit ///< [in] オペランド
  );

  /// @brief 比較演算子 (EQ)
  /// @return 等しい時に true を返す．
  bool
  operator==(
    const SopCover& right ///< [in] 第2オペランド
  ) const
  {
    _check_size(right);
    return _compare(right) == 0;
  }

  /// @brief 比較演算子 (NE)
  /// @return 等しくない時に true を返す．
  bool
  operator!=(
    const SopCover& right ///< [in] 第2オペランド
  )
  {
    _check_size(right);
    return _compare(right) != 0;
  }

  /// @brief 比較演算子 (LT)
  /// @return left が right より小さい時に true を返す．
  bool
  operator<(
    const SopCover& right ///< [in] 第2オペランド
  ) const
  {
    _check_size(right);
    return _compare(right) < 0;
  }

  /// @brief 比較演算子 (GT)
  /// @return left が right より大きい時に true を返す．
  bool
  operator>(
    const SopCover& right ///< [in] 第2オペランド
  ) const
  {
    _check_size(right);
    return _compare(right) > 0;
  }

  /// @brief 比較演算子 (LE)
  /// @return left が right と等しいか小さい時に true を返す．
  bool
  operator<=(
    const SopCover& right ///< [in] 第2オペランド
  ) const
  {
    _check_size(right);
    return _compare(right) <= 0;
  }

  /// @brief 比較演算子 (GE)
  /// @return left が right と等しいか大きい時に true を返す．
  bool
  operator>=(
    const SopCover& right ///< [in] 第2オペランド
  ) const
  {
    _check_size(right);
    return _compare(right) >= 0;
  }

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name factor 関係のメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief すべてのカーネルとコカーネルペアを列挙する．
  vector<pair<SopCover, vector<SopCube>>>
  all_kernels() const;

  /// @brief 最も価値の高いカーネルを求める．
  SopCover
  best_kernel() const;

  /// @brief 最も価値の高いカーネルを求める．
  SopCover
  best_kernel(
    std::function<int(const SopCover&,
		      const vector<SopCube>&)> eval_func ///< [in] 評価関数
  ) const;

  /// @brief "quick factor" を行う．
  Expr
  quick_factor() const;

  /// @brief "good factor" を行う．
  Expr
  good_factor() const;

  /// @brief "bool factor" を行う．
  Expr
  bool_factor() const;

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////


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
    Chunk&& chunk          ///< [in] 内容のパタンを表す本体
  ) : SopBase{variable_num},
      mCubeNum{cube_num},
      mChunk{std::move(chunk)}
  {
    _sort();
  }

  /// @brief ソートする．
  void
  _sort();

  /// @brief 比較演算子(rich compare)
  /// @return 比較結果を返す．
  ///
  /// 比較方法はキューブごとの辞書式順序
  int
  _compare(
    const SopCover& right ///< [in] 第2オペランド
  ) const;

  /// @brief concate の共通処理
  Chunk
  _concate(
    SizeType num2,       ///< [in] 第2オペランドのキューブ数
    const Chunk& chunk2, ///< [in] 第2オペランドキューブ本体
    SizeType& dst_num    ///< [out] 結果のキューブ数
  ) const;

  /// @brief 集合差演算の本体
  Chunk
  _diff(
    SizeType num2,
    const Chunk& chunk2,
    SizeType& dst_num
  ) const;

  /// @brief 論理積演算の本体
  Chunk
  _product(
    SizeType num2,
    const Chunk& chunk2,
    SizeType& dst_num
  ) const;

  /// @brief 論理積演算の本体
  Chunk
  _product(
    Literal lit,
    SizeType& dst_num
  ) const;

  /// @brief 代数的除算の本体
  Chunk
  _algdiv(
    SizeType num2,
    const Chunk& chunk2,
    SizeType& dst_num
  ) const;

  /// @brief 代数的除算の本体
  Chunk
  _algdiv(
    const Chunk& chunk2,
    SizeType& dst_num
  ) const;

  /// @brief 代数的除算の本体
  Chunk
  _algdiv(
    Literal lit,
    SizeType& dst_num
  ) const;

  /// @brief コファクターの本体
  Chunk
  _cofactor(
    const Chunk& chunk2,
    SizeType& dst_num
  ) const;

  /// @brief コファクターの本体
  Chunk
  _cofactor(
    Literal lit,
    SizeType& dst_num
  ) const;

  /// @brief 内容をセットする．
  void
  _set(
    SizeType num, ///< [in] キューブ数
    Chunk&& chunk ///< [in] 本体
  )
  {
    mCubeNum = num;
    std::swap(mChunk, chunk);
    _sort();
  }

  /// @brief キューブ番号が範囲内か調べる．
  void
  _check_cube_id(
    SizeType cube_id
  ) const
  {
    if ( cube_id >= cube_num() ) {
      throw std::out_of_range{"cube_id is out of range"};
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // キューブ数
  SizeType mCubeNum{0};

  // 内容を表すビットベクタ
  Chunk mChunk;

};

/// @relates SopCover
/// @brief キューブとカバーの論理和
/// @return 結果を返す．
inline
SopCover
operator|(
  const SopCube& left,  ///< [in] 第1オペランド
  const SopCover& right ///< [in] 第2オペランド
)
{
  // 交換則を用いる．
  return right.operator|(left);
}

/// @relates SopCover
/// @brief カバーの減算
/// @return 結果を返す．
inline
SopCover
operator-(
  SopCover&& left,      ///< [in] 第1オペランド(ムーブ参照)
  const SopCover& right ///< [in] 第2オペランド
)
{
  return SopCover{std::move(left)}.operator-=(right);
}

/// @relates SopCover
/// @brief カバーとキューブの減算
/// @return 結果を返す．
inline
SopCover
operator-(
  SopCover&& left,     ///< [in] 第1オペランド(ムーブ参照)
  const SopCube& right ///< [in] 第2オペランド
)
{
  return SopCover{std::move(left)}.operator-=(right);
}

/// @relates SopCover, SopCube
/// @brief カバーとキューブの乗算
/// @return 結果を返す．
inline
SopCover
operator&(
  SopCover&& left,     ///< [in] 第1オペランド(ムーブ参照)
  const SopCube& right ///< [in] 第2オペランド
)
{
  return SopCover{std::move(left)}.operator&=(right);
}

/// @relates SopCover, SopCube
/// @brief キューブとカバーの乗算
/// @return 結果を返す．
inline
SopCover
operator&(
  const SopCube& left,  ///< [in] 第1オペランド
  const SopCover& right ///< [in] 第2オペランド
)
{
  // 交換則を用いる．
  return right.operator&(left);
}

/// @relates SopCover, SopCube
/// @brief キューブとカバーの乗算
/// @return 結果を返す．
inline
SopCover
operator&(
  const SopCube& left, ///< [in] 第1オペランド
  SopCover&& right     ///< [in] 第2オペランド(ムーブ参照)
)
{
  // 交換則を用いる．
  return SopCover{std::move(right)}.operator&=(left);
}

/// @relates SopCover, Literal
/// @brief カバーとリテラルの乗算
/// @return 結果を返す．
inline
SopCover
operator&(
  SopCover&& left, ///< [in] 第1オペランド(ムーブ参照)
  Literal right	   ///< [in] 第2オペランド
)
{
  return SopCover{std::move(left)}.operator&=(right);
}

/// @relates SopCover, Literal
/// @brief リテラルとカバーの乗算
/// @return 結果を返す．
inline
SopCover
operator&(
  Literal left,         ///< [in] 第1オペランド
  const SopCover& right ///< [in] 第2オペランド
)
{
  // 交換則を用いる．
  return right.operator&(left);
}

/// @relates SopCover, Literal
/// @brief リテラルとカバーの乗算
/// @return 結果を返す．
inline
SopCover
operator&(
  Literal left,    ///< [in] 第1オペランド
  SopCover&& right ///< [in] 第2オペランド(ムーブ参照)
)
{
  // 交換則を用いる．
  return SopCover{std::move(right)}.operator&=(left);
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
