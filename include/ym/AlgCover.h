#ifndef YM_ALGCOVER_H
#define YM_ALGCOVER_H

/// @file ym/AlgCover.h
/// @brief AlgCover のヘッダファイル
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
///
/// 実装としてはパタンの実体は AlgBase::Chunk が持つ．
//////////////////////////////////////////////////////////////////////
class AlgCover:
  public AlgBase
{
public:

  /// @brief Literal のリストのリストの形の初期化子
  using initializer = std::initializer_list<std::initializer_list<Literal>>;

public:

  /// @brief コンストラクタ
  ///
  /// * 空のカバーとなる．
  explicit
  AlgCover(
    SizeType variable_num = 0 ///< [in] 変数の数
  );

  /// @brief コンストラクタ
  ///
  /// * cube_list 中の各キューブのサイズは variable_num
  ///   と等しくなければならない．
  /// * キューブの順番は変わる可能性がある．
  AlgCover(
    SizeType variable_num,           ///< [in] 変数の数
    const vector<AlgCube>& cube_list ///< [in] キューブのリスト
  );

  /// @brief コンストラクタ
  ///
  /// * キューブの順番は変わる可能性がある．
  AlgCover(
    SizeType variable_num,                   ///< [in] 変数の数
    const vector<vector<Literal>>& cube_list ///< [in] カバーを表すリテラルのリストのリスト
  );

  /// @brief コンストラクタ
  ///
  /// * キューブの順番は変わる可能性がある．
  AlgCover(
    SizeType variable_num, ///< [in] 変数の数
    initializer& cube_list ///< [in] カバーを表すリテラルのリストのリスト
  );

  /// @brief コピーコンストラクタ
  AlgCover(
    const AlgCover& src ///< [in] コピー元のオブジェクト
  );

  /// @brief コピー代入演算子
  /// @return 代入後の自身の参照を返す．
  AlgCover&
  operator=(
    const AlgCover& src ///< [in] コピー元のオブジェクト
  );

  /// @brief ムーブコンストラクタ
  AlgCover(
    AlgCover&& src ///< [in] ムーブ元のオブジェクト
  );

  /// @brief ムーブ代入演算子
  /// @return 代入後の自身の参照を返す．
  AlgCover&
  operator=(
    AlgCover&& src ///< [in] ムーブ元のオブジェクト
  );

  /// @brief キューブからのコピー変換コンストラクタ
  ///
  /// 指定されたキューブのみのカバーとなる．
  explicit
  AlgCover(
    const AlgCube& cube ///< [in] 対象のキューブ
  );

  /// @brief キューブからのムーブ変換コンストラクタ
  ///
  /// 指定されたキューブのみのカバーとなる．
  explicit
  AlgCover(
    AlgCube&& cube ///< [in] 対象のキューブ
  );

  /// @brief デストラクタ
  ///
  /// ここに属しているすべてのキューブは削除される．
  ~AlgCover() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

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

  /// @brief 指定されたリテラルの出現回数を返す．
  SizeType
  literal_num(
    SizeType varid, ///< [in] 変数番号
    bool inv        ///< [in] 反転属性
  ) const;

  /// @brief 内容をリテラルのリストのリストに変換する．
  vector<vector<Literal>>
  literal_list() const;

  /// @brief キューブを取り出す．
  AlgCube
  get_cube(
    SizeType cube_id ///< [in] キューブ番号 ( 0 <= cube_id < cube_num() )
  ) const;

  /// @brief パタンを返す．
  /// @retval AlgPat::_X その変数は現れない．
  /// @retval AlgPat::_1 その変数が肯定のリテラルとして現れる．
  /// @retval AlgPat::_0 その変数が否定のリテラルとして現れる．
  AlgPat
  get_pat(
    SizeType cube_id, ///< [in] キューブ番号 ( 0 <= cube_id < cube_num() )
    SizeType var_id   ///< [in] 変数( 0 <= var_id.val() < variable_num() )
  ) const;

  /// @brief 共通なキューブを返す．
  ///
  /// 共通なキューブがない場合には空のキューブを返す．
  AlgCube
  common_cube() const;

  /// @brief すべてのカーネルとコカーネルペアを列挙する．
  vector<pair<AlgCover, AlgCover>>
  all_kernels() const;

  /// @brief 最も価値の高いカーネルを求める．
  AlgCover
  best_kernel() const;

  /// @brief "quick factor" を行う．
  Expr
  quick_factor() const;

  /// @brief "good factor" を行う．
  Expr
  good_factor() const;

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

  /// @brief 内容をデバッグ用に出力する．
  void
  debug_print(
    ostream& s ///< [in] 出力先のストリーム
  ) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 演算子のオーバーロード
  //////////////////////////////////////////////////////////////////////
  /// @brief 論理和を計算する．
  /// @return 計算結果を返す．
  AlgCover
  operator+(
    const AlgCover& right ///< [in] オペランド
  ) const;

  /// @brief 論理和を計算して代入する．
  /// @return 演算後の自身への参照を返す．
  AlgCover&
  operator+=(
    const AlgCover& right ///< [in] オペランド
  );

  /// @brief 論理和を計算する(キューブ版)．
  /// @return 計算結果を返す．
  AlgCover
  operator+(
    const AlgCube& right ///< [in] オペランド
  ) const;

  /// @brief 論理和を計算して代入する(キューブ版)．
  /// @return 演算後の自身への参照を返す．
  AlgCover&
  operator+=(
    const AlgCube& right ///< [in] オペランド
  );

  /// @brief 差分を計算する．
  /// @return 計算結果を返す．
  ///
  /// right のみに含まれる要素があっても無視される．
  AlgCover
  operator-(
    const AlgCover& right ///< [in] オペランド
  ) const;

  /// @brief 差分を計算して代入する．
  /// @return 演算後の自身への参照を返す．
  AlgCover&
  operator-=(
    const AlgCover& right ///< [in] オペランド
  );

  /// @brief 差分を計算する(キューブ版)．
  /// @return 計算結果を返す．
  ///
  /// right のみに含まれる要素があっても無視される．
  AlgCover
  operator-(
    const AlgCube& right ///< [in] オペランド
  ) const;

  /// @brief 差分を計算して代入する(キューブ版)．
  /// @return 演算後の自身への参照を返す．
  AlgCover&
  operator-=(
    const AlgCube& right ///< [in] オペランド
  );

  /// @brief 論理積を計算する．
  /// @return 計算結果を返す．
  AlgCover
  operator*(
    const AlgCover& right ///< [in] オペランド
  ) const;

  /// @brief 論理積を計算して代入する．
  /// @return 演算後の自身への参照を返す．
  AlgCover&
  operator*=(
    const AlgCover& right ///< [in] オペランド
  );

  /// @brief 論理積を計算する(キューブ版)．
  /// @return 計算結果を返す．
  AlgCover
  operator*(
    const AlgCube& right ///< [in] オペランド
  ) const;

  /// @brief 論理積を計算して代入する(キューブ版)．
  /// @return 演算後の自身への参照を返す．
  AlgCover&
  operator*=(
    const AlgCube& right ///< [in] オペランド
  );

  /// @brief 論理積を計算する(リテラル版)．
  /// @return 計算結果を返す．
  AlgCover
  operator*(
    Literal right ///< [in] オペランド
  ) const;

  /// @brief 論理積を計算して代入する(リテラル版)．
  /// @return 演算後の自身への参照を返す．
  AlgCover&
  operator*=(
    Literal right ///< [in] オペランド
  );

  /// @brief algebraic division を計算する．
  /// @return 計算結果を返す．
  AlgCover
  operator/(
    const AlgCover& right ///< [in] オペランド
  ) const;

  /// @brief algebraic division を行って代入する．
  /// @return 演算後の自身への参照を返す．
  AlgCover&
  operator/=(
    const AlgCover& right ///< [in] オペランド
  );

  /// @brief キューブによる商を計算する．
  /// @return 計算結果を返す．
  AlgCover
  operator/(
    const AlgCube& cube ///< [in] 対象のキューブ
  ) const;

  /// @brief キューブによる商を計算して代入する．
  /// @return 演算後の自身への参照を返す．
  AlgCover&
  operator/=(
    const AlgCube& cube ///< [in] 対象のキューブ
  );

  /// @brief リテラルによる商を計算する．
  /// @return 計算結果を返す．
  AlgCover
  operator/(
    Literal lit ///< [in] 対象のリテラル
  ) const;

  /// @brief リテラルによる商を計算して代入する．
  /// @return 演算後の自身への参照を返す．
  AlgCover&
  operator/=(
    Literal lit ///< [in] 対象のリテラル
  );

  /// @brief 比較演算子 (EQ)
  /// @return 等しい時に true を返す．
  bool
  operator==(
    const AlgCover& right ///< [in] 第2オペランド
  ) const
  {
    return _compare(right) == 0;
  }

  /// @brief 比較演算子 (NE)
  /// @return 等しくない時に true を返す．
  bool
  operator!=(
    const AlgCover& right ///< [in] 第2オペランド
  ) const
  {
    return _compare(right) != 0;
  }

  /// @brief 比較演算子 (LT)
  /// @return left が right より小さい時に true を返す．
  bool
  operator<(
    const AlgCover& right ///< [in] 第2オペランド
  ) const
  {
    return _compare(right) < 0;
  }

  /// @brief 比較演算子 (GT)
  /// @return left が right より大きい時に true を返す．
  bool
  operator>(
    const AlgCover& right ///< [in] 第2オペランド
  ) const
  {
    return _compare(right) > 0;
  }

  /// @brief 比較演算子 (LE)
  /// @return left が right と等しいか小さい時に true を返す．
  bool
  operator<=(
    const AlgCover& right ///< [in] 第2オペランド
  ) const
  {
    return _compare(right) <= 0;
  }

  /// @brief 比較演算子 (GE)
  /// @return left が right と等しいか大きい時に true を返す．
  bool
  operator>=(
    const AlgCover& right ///< [in] 第2オペランド
  ) const
  {
    return _compare(right) >= 0;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を指定したコンストラクタ
  ///
  /// この関数は危険なので普通は使わないこと
  AlgCover(
    SizeType variable_num, ///< [in] 変数の数
    SizeType cube_num,     ///< [in] キューブ数
    Chunk&& chunk          ///< [in] 内容のパタンを表す本体
  );

  /// @brief 比較演算子(rich compare)
  /// @return 比較結果を返す．
  ///
  /// 比較方法はキューブごとの辞書式順序
  int
  _compare(
    const AlgCover& right ///< [in] 第2オペランド
  ) const;

  /// @brief ソートする．
  void
  _sort();

  /// @brief 連結演算の本体
  Chunk
  concate(
    SizeType num2,
    const Chunk& chunk2,
    SizeType& dst_num
  ) const;

  /// @brief 集合差演算の本体
  Chunk
  diff(
    SizeType num2,
    const Chunk& chunk2,
    SizeType& dst_num
  ) const;

  /// @brief 論理積演算の本体
  Chunk
  product(
    SizeType num2,
    const Chunk& chunk2,
    SizeType& dst_num
  ) const;

  /// @brief 論理積演算の本体
  Chunk
  product(
    Literal lit,
    SizeType& dst_num
  ) const;

  /// @brief 代数敵除算の本体
  Chunk
  algdiv(
    SizeType num2,
    const Chunk& chunk2,
    SizeType& dst_num
  ) const;

  /// @brief 代数敵除算の本体
  Chunk
  algdiv(
    const Chunk& chunk2,
    SizeType& dst_num
  ) const;

  /// @brief 代数敵除算の本体
  Chunk
  algdiv(
    Literal lit,
    SizeType& dst_num
  ) const;

  /// @brief AlgBlock の内容をセットする．
  void
  _set(
    SizeType cube_num,
    Chunk&& chunk
  )
  {
    mCubeNum = cube_num;
    std::swap(mChunk, chunk);
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

/// @relates AlgCover
/// @brief キューブとカバーの加算
/// @return 結果を返す．
inline
AlgCover
operator+(
  const AlgCube& left,  ///< [in] 第1オペランド
  const AlgCover& right ///< [in] 第2オペランド
)
{
  // 交換則を用いる．
  return right.operator+(left);
}

/// @relates AlgCover
/// @brief カバーの減算
/// @return 結果を返す．
inline
AlgCover
operator-(
  AlgCover&& left,      ///< [in] 第1オペランド(ムーブ参照)
  const AlgCover& right ///< [in] 第2オペランド
)
{
  return AlgCover{std::move(left)}.operator-=(right);
}

/// @relates AlgCover
/// @brief カバーとキューブの減算
/// @return 結果を返す．
inline
AlgCover
operator-(
  AlgCover&& left,     ///< [in] 第1オペランド(ムーブ参照)
  const AlgCube& right ///< [in] 第2オペランド
)
{
  return AlgCover{std::move(left)}.operator-=(right);
}

/// @relates AlgCover, AlgCube
/// @brief カバーとキューブの乗算
/// @return 結果を返す．
inline
AlgCover
operator*(
  AlgCover&& left,     ///< [in] 第1オペランド(ムーブ参照)
  const AlgCube& right ///< [in] 第2オペランド
)
{
  return AlgCover{std::move(left)}.operator*=(right);
}

/// @relates AlgCover, AlgCube
/// @brief キューブとカバーの乗算
/// @return 結果を返す．
inline
AlgCover
operator*(
  const AlgCube& left,  ///< [in] 第1オペランド
  const AlgCover& right ///< [in] 第2オペランド
)
{
  // 交換則を用いる．
  return right.operator*(left);
}

/// @relates AlgCover, AlgCube
/// @brief キューブとカバーの乗算
/// @return 結果を返す．
inline
AlgCover
operator*(
  const AlgCube& left, ///< [in] 第1オペランド
  AlgCover&& right     ///< [in] 第2オペランド(ムーブ参照)
)
{
  // 交換則を用いる．
  return AlgCover{std::move(right)}.operator*=(left);
}

/// @relates AlgCover, Literal
/// @brief カバーとリテラルの乗算
/// @return 結果を返す．
inline
AlgCover
operator*(
  AlgCover&& left, ///< [in] 第1オペランド(ムーブ参照)
  Literal right	   ///< [in] 第2オペランド
)
{
  return AlgCover{std::move(left)}.operator*=(right);
}

/// @relates AlgCover, Literal
/// @brief リテラルとカバーの乗算
/// @return 結果を返す．
inline
AlgCover
operator*(
  Literal left,         ///< [in] 第1オペランド
  const AlgCover& right ///< [in] 第2オペランド
)
{
  // 交換則を用いる．
  return right.operator*(left);
}

/// @relates AlgCover, Literal
/// @brief リテラルとカバーの乗算
/// @return 結果を返す．
inline
AlgCover
operator*(
  Literal left,    ///< [in] 第1オペランド
  AlgCover&& right ///< [in] 第2オペランド(ムーブ参照)
)
{
  // 交換則を用いる．
  return AlgCover{std::move(right)}.operator*=(left);
}

/// @relates AlgCover
/// @brief カバーの除算
/// @return 結果を返す．
inline
AlgCover
operator/(
  AlgCover&& left,      ///< [in] 第1オペランド(ムーブ参照)
  const AlgCover& right ///< [in] 第2オペランド
)
{
  return AlgCover{std::move(left)}.operator/=(right);
}

/// @relates AlgCover, AlgCube
/// @brief カバーとキューブの除算
/// @return 結果を返す．
inline
AlgCover
operator/(
  AlgCover&& left,     ///< [in] 第1オペランド(ムーブ参照)
  const AlgCube& right ///< [in] 第2オペランド
)
{
  return AlgCover{std::move(left)}.operator/=(right);
}

/// @relates AlgCover, Literal
/// @brief カバーとリテラルの除算
/// @return 結果を返す．
inline
AlgCover
operator/(
  AlgCover&& left, ///< [in] 第1オペランド(ムーブ参照)
  Literal right	   ///< [in] 第2オペランド
)
{
  return AlgCover{std::move(left)}.operator/=(right);
}

/// @relates AlgCover
/// @brief AlgCover の内容を出力する．
///
/// cover.print(s) と等価
inline
ostream&
operator<<(
  ostream& s,           ///< [in] 出力先のストリーム
  const AlgCover& cover ///< [in] 対象のカバー
)
{
  cover.print(s);
  return s;
}

END_NAMESPACE_YM_ALG

BEGIN_NAMESPACE_STD

/// @breif AlgCover をキーにしたハッシュ関数クラスの定義
template <>
struct hash<YM_NAMESPACE::AlgCover>
{
  SizeType
  operator()(const YM_NAMESPACE::AlgCover& cover) const
  {
    return cover.hash();
  }
};

END_NAMESPACE_STD

#endif // YM_ALGCOVER_H
