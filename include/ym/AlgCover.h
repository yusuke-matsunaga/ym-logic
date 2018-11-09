#ifndef YM_ALGCOVER_H
#define YM_ALGCOVER_H

/// @file ym/AlgCover.h
/// @brief AlgCover のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/Alg.h"
#include "ym/AlgCube.h"
#include "ym/AlgMgr.h"
#include "ym/HashFunc.h"


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
//////////////////////////////////////////////////////////////////////
class AlgCover
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  /// @param[in] cube_list キューブのリスト
  ///
  /// cube_list が空の時は空のカバーとなる．
  explicit
  AlgCover(AlgMgr& mgr,
	   const vector<AlgCube>& cube_list = vector<AlgCube>());

  /// @brief 特殊なコンストラクタ
  /// @param[in] mgr マネージャ
  /// @param[in] dummy ダミーの引数
  ///
  /// 空のキューブを1つ持つカバーとなる．
  /// dummy の値は無視される．
  AlgCover(AlgMgr& mgr,
	   int dummy);

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  /// @param[in] lit_list カバーを表すリテラルのリスト
  ///
  /// lit_list 中に Literal::X があった場合，キューブの区切りを表す
  /// とみなされる．
  AlgCover(AlgMgr& mgr,
	   const vector<Literal>& lit_list);

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  /// @param[in] str カバーを表す文字列
  AlgCover(AlgMgr& mgr,
	   const char* str);

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  /// @param[in] str カバーを表す文字列
  AlgCover(AlgMgr& mgr,
	   const string& str);

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  AlgCover(const AlgCover& src);

  /// @brief コピー代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 代入後の自身の参照を返す．
  AlgCover&
  operator=(const AlgCover& src);

  /// @brief キューブからの変換コンストラクタ
  /// @param[in] cube 対象のキューブ
  ///
  /// 指定されたキューブのみのカバーとなる．
  explicit
  AlgCover(const AlgCube& cube);

  /// @brief デストラクタ
  ///
  /// ここに属しているすべてのキューブは削除される．
  ~AlgCover();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief マネージャを返す．
  AlgMgr&
  mgr() const;

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

  /// @brief 内容を返す．
  /// @param[in] cube_id キューブ番号 ( 0 <= cube_id < cube_num() )
  /// @param[in] var_id 変数番号 ( 0 <= var_id.val() < variable_num() )
  AlgPol
  get_pol(int cube_id,
	  VarId var_id) const;

  /// @brief 論理和を計算する．
  /// @param[in] right オペランド
  /// @return 計算結果を返す．
  AlgCover
  operator+(const AlgCover& right) const;

  /// @brief 論理和を計算する(キューブ版)．
  /// @param[in] right オペランド
  /// @return 計算結果を返す．
  AlgCover
  operator+(const AlgCube& right) const;

  /// @brief 差分を計算する．
  /// @param[in] right オペランド
  /// @return 計算結果を返す．
  ///
  /// right のみに含まれる要素があっても無視される．
  AlgCover
  operator-(const AlgCover& right) const;

  /// @brief 差分を計算する(キューブ版)．
  /// @param[in] right オペランド
  /// @return 計算結果を返す．
  ///
  /// right のみに含まれる要素があっても無視される．
  AlgCover
  operator-(const AlgCube& right) const;

  /// @brief 論理積を計算する．
  /// @param[in] right オペランド
  /// @return 計算結果を返す．
  AlgCover
  operator*(const AlgCover& right) const;

  /// @brief 論理積を計算する(キューブ版)．
  /// @param[in] right オペランド
  /// @return 計算結果を返す．
  AlgCover
  operator*(const AlgCube& right) const;

  /// @brief 論理積を計算する(リテラル版)．
  /// @param[in] right オペランド
  /// @return 計算結果を返す．
  AlgCover
  operator*(Literal right) const;

  /// @brief algebraic division を計算する．
  /// @param[in] right オペランド
  /// @return 計算結果を返す．
  AlgCover
  operator/(const AlgCover& right) const;

  /// @brief キューブによる商を計算する．
  /// @param[in] cube 対象のキューブ
  /// @return 計算結果を返す．
  AlgCover
  operator/(const AlgCube& cube) const;

  /// @brief リテラルによる商を計算する．
  /// @param[in] lit 対象のリテラル
  /// @return 計算結果を返す．
  AlgCover
  operator/(Literal lit) const;

  /// @brief 論理和を計算して代入する．
  /// @param[in] right オペランド
  /// @return 演算後の自身への参照を返す．
  const AlgCover&
  operator+=(const AlgCover& right);

  /// @brief 論理和を計算して代入する(キューブ版)．
  /// @param[in] right オペランド
  /// @return 演算後の自身への参照を返す．
  const AlgCover&
  operator+=(const AlgCube& right);

  /// @brief 差分を計算して代入する．
  /// @param[in] right オペランド
  /// @return 演算後の自身への参照を返す．
  const AlgCover&
  operator-=(const AlgCover& right);

  /// @brief 差分を計算して代入する(キューブ版)．
  /// @param[in] right オペランド
  /// @return 演算後の自身への参照を返す．
  const AlgCover&
  operator-=(const AlgCube& right);

  /// @brief 論理積を計算して代入する．
  /// @param[in] right オペランド
  /// @return 演算後の自身への参照を返す．
  const AlgCover&
  operator*=(const AlgCover& right);

  /// @brief 論理積を計算して代入する(キューブ版)．
  /// @param[in] right オペランド
  /// @return 演算後の自身への参照を返す．
  const AlgCover&
  operator*=(const AlgCube& right);

  /// @brief 論理積を計算して代入する(リテラル版)．
  /// @param[in] right オペランド
  /// @return 演算後の自身への参照を返す．
  const AlgCover&
  operator*=(Literal right);

  /// @brief algebraic division を行って代入する．
  /// @param[in] right オペランド
  /// @return 演算後の自身への参照を返す．
  const AlgCover&
  operator/=(const AlgCover& right);

  /// @brief キューブによる商を計算して代入する．
  /// @param[in] cube 対象のキューブ
  /// @return 演算後の自身への参照を返す．
  AlgCover
  operator/=(const AlgCube& cube);

  /// @brief リテラルによる商を計算して代入する．
  /// @param[in] lit 対象のリテラル
  /// @return 演算後の自身への参照を返す．
  const AlgCover&
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
  void
  print(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を指定したコンストラクタ
  /// @param[in] mgr マネージャ
  /// @param[in] cube_num キューブ数
  /// @param[in] cube_cap キューブ容量
  /// @param[in] body 内容のパタンを表す本体
  ///
  /// この関数は危険なので普通は使わないこと
  AlgCover(AlgMgr& mgr,
	   int cube_num,
	   int cube_cap,
	   AlgBitVect* body);

  /// @brief キューブ容量を変更する．
  /// @param[in] req_cap 要求するキューブ容量
  ///
  /// 現在のキューブ容量が大きかれば変更しない．
  void
  resize(int req_cap);

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

  // キューブマネージャ
  AlgMgr* mMgr;

  // キューブ数
  int mCubeNum;

  // mBody の実際に確保されているキューブ容量
  int mCubeCap;

  // 内容を表すビットベクタ
  AlgBitVect* mBody;

};

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

// @brief コンストラクタ
// @param[in] mgr マネージャ
// @param[in] cube_list キューブのリスト
inline
AlgCover::AlgCover(AlgMgr& mgr,
		   const vector<AlgCube>& cube_list) :
  mMgr(&mgr),
  mCubeNum(cube_list.size()),
  mCubeCap(0)
{
  resize(mCubeNum);
  AlgBitVect* dst = mBody;
  for ( auto& cube: cube_list ) {
    mMgr->cube_copy(dst, cube.mBody);
    dst += mgr.cube_size();
  }
  mMgr->sort(mCubeNum, mBody);
}

// @brief 特殊なコンストラクタ
// @param[in] mgr マネージャ
// @param[in] dummy ダミーの引数
//
// 空のキューブを1つ持つカバーとなる．
// dummy の値は無視される．
inline
AlgCover::AlgCover(AlgMgr& mgr,
		   int dummy) :
  mMgr(&mgr),
  mCubeNum(1),
  mCubeCap(0)
{
  resize(mCubeNum);
  mMgr->cube_clear(mBody);
}

// @brief コンストラクタ
// @param[in] mgr マネージャ
// @param[in] lit_list カバーを表すリテラルのリスト
//
// lit_list 中に Literal::X があった場合，キューブの区切り
// とみなされる．
inline
AlgCover::AlgCover(AlgMgr& mgr,
		   const vector<Literal>& lit_list) :
  mMgr(&mgr),
  mCubeCap(0)
{
  int n = 0;
  bool first = true;
  for ( auto lit: lit_list ) {
    if ( lit == Literal::X ) {
      first = true;
    }
    else if ( first ) {
      first = false;
      ++ n;
    }
  }
  mCubeNum = n;
  resize(mCubeNum);
  mMgr->set_literal(mBody, 0, lit_list);
  mMgr->sort(mCubeNum, mBody);
}

// @brief コンストラクタ
// @param[in] mgr マネージャ
// @param[in] str カバーを表す文字列
inline
AlgCover::AlgCover(AlgMgr& mgr,
		   const char* str) :
  mMgr(&mgr),
  mCubeCap(0)
{
  vector<AlgLiteral> lit_list;
  mCubeNum = mMgr->parse(str, lit_list);
  resize(mCubeNum);
  mMgr->set_literal(mBody, 0, lit_list);
  mMgr->sort(mCubeNum, mBody);
}

// @brief コンストラクタ
// @param[in] mgr マネージャ
// @param[in] str カバーを表す文字列
inline
AlgCover::AlgCover(AlgMgr& mgr,
		   const string& str) :
  mMgr(&mgr),
  mCubeCap(0)
{
  vector<AlgLiteral> lit_list;
  mCubeNum = mMgr->parse(str.c_str(), lit_list);
  resize(mCubeNum);
  mMgr->set_literal(mBody, 0, lit_list);
  mMgr->sort(mCubeNum, mBody);
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
inline
AlgCover::AlgCover(const AlgCover& src) :
  mMgr(src.mMgr),
  mCubeNum(src.mCubeNum),
  mCubeCap(0)
{
  resize(mCubeNum);
  mMgr->copy(mCubeNum, mBody, 0, src.mBody, 0);
}

// @brief 代入演算子
// @param[in] src コピー元のオブジェクト
// @return 代入後の自身の参照を返す．
inline
AlgCover&
AlgCover::operator=(const AlgCover& src)
{
  if ( &src != this ) {
    int old_cap = mCubeCap;
    AlgBitVect* old_body = mBody;
    mCubeNum = src.mCubeNum;
    resize(mCubeNum);
    if ( old_body != mBody ) {
      mgr().delete_body(mBody, old_cap);
    }
    mMgr->copy(mCubeNum, mBody, 0, src.mBody, 0);
  }

  return *this;
}

// @brief キューブからの変換コンストラクタ
// @param[in] cube 対象のキューブ
//
// 指定されたキューブのみのカバーとなる．
inline
AlgCover::AlgCover(const AlgCube& cube) :
  mMgr(&cube.mgr()),
  mCubeNum(1),
  mCubeCap(0)
{
  resize(mCubeNum);
  mMgr->cube_copy(mBody, cube.mBody);
}

// @brief デストラクタ
//
// ここに属しているすべてのキューブは削除される．
inline
AlgCover::~AlgCover()
{
  mMgr->delete_body(mBody, mCubeCap);
}

// @brief 内容を指定したコンストラクタ
// @param[in] mgr マネージャ
// @param[in] cube_num キューブ数
// @param[in] cube_cap キューブ容量
// @param[in] body 内容のパタンを表す本体
//
// この関数は危険なので普通は使わないこと
inline
AlgCover::AlgCover(AlgMgr& mgr,
		   int cube_num,
		   int cube_cap,
		   AlgBitVect* body) :
  mMgr(&mgr),
  mCubeNum(cube_num),
  mCubeCap(cube_cap),
  mBody(body)
{
}

// @brief マネージャを返す．
inline
AlgMgr&
AlgCover::mgr() const
{
  return *mMgr;
}

// @brief 変数の数を返す．
inline
int
AlgCover::variable_num() const
{
  return mMgr->variable_num();
}

// @brief キューブの数を返す．
inline
int
AlgCover::cube_num() const
{
  return mCubeNum;
}

// @brief リテラル数を返す．
inline
int
AlgCover::literal_num() const
{
  return mgr().literal_num(cube_num(), mBody);
}

// @brief 指定されたリテラルの出現回数を返す．
// @param[in] lit 対象のリテラル
inline
int
AlgCover::literal_num(Literal lit) const
{
  return mgr().literal_num(cube_num(), mBody, lit);
}

// @brief 内容を返す．
// @param[in] cube_id キューブ番号 ( 0 <= cube_id < cube_num() )
// @param[in] var_id 変数番号 ( 0 <= var_id < variable_num() )
inline
AlgPol
AlgCover::literal(int cube_id,
		  VarId var_id) const
{
  ASSERT_COND( cube_id >= 0 && cube_id < cube_num() );

  return mgr().literal(mBody, cube_id, var_id);
}

// @brief 論理和を計算する．
// @param[in] right オペランド
// @return 計算結果を返す．
inline
AlgCover
AlgCover::operator+(const AlgCover& right) const
{
  ASSERT_COND( variable_num() == right.variable_num() );

  int nc1 = cube_num();
  int nc2 = right.cube_num();
  int cap = get_capacity(nc1 + nc2);
  AlgBitVect* body = mgr().new_body(cap);
  int nc = mgr().sum(body, block(), right.block());

  return AlgCover(mgr(), nc, cap, body);
}

// @brief 論理和を計算する(キューブ版)．
// @param[in] right オペランド
// @return 計算結果を返す．
inline
AlgCover
AlgCover::operator+(const AlgCube& right) const
{
  ASSERT_COND( variable_num() == right.variable_num() );

  int nc1 = cube_num();
  int nc2 = 1;
  int cap = get_capacity(nc1 + nc2);
  AlgBitVect* body = mgr().new_body(cap);
  int nc = mgr().sum(body, block(), AlgMgr::Block{1, right.mBody});

  return AlgCover(mgr(), nc, cap, body);
}

// @brief 論理和を計算して代入する．
// @param[in] right オペランド
// @return 演算後の自身への参照を返す．
inline
const AlgCover&
AlgCover::operator+=(const AlgCover& right)
{
  ASSERT_COND( variable_num() == right.variable_num() );

  int nc1 = cube_num();
  int nc2 = right.cube_num();
  int old_cap = mCubeCap;
  AlgBitVect* old_body = mBody;
  resize(nc1 + nc2);
  mCubeNum = mgr().sum(mBody, AlgMgr::Block{nc1, old_body}, right.block());
  if ( old_body != mBody ) {
    mgr().delete_body(old_body, old_cap);
  }

  return *this;
}

// @brief 論理和を計算して代入する(キューブ版)．
// @param[in] right オペランド
// @return 演算後の自身への参照を返す．
inline
const AlgCover&
AlgCover::operator+=(const AlgCube& right)
{
  ASSERT_COND( variable_num() == right.variable_num() );

  int nc1 = cube_num();
  int nc2 = 1;
  int old_cap = mCubeCap;
  AlgBitVect* old_body = mBody;
  resize(nc1 + nc2);
  mCubeNum = mgr().sum(mBody, AlgMgr::Block{nc1, old_body}, AlgMgr::Block{1, right.mBody});
  if ( old_body != mBody ) {
    mgr().delete_body(old_body, old_cap);
  }

  return *this;
}

// @brief 差分を計算する．
// @param[in] right オペランド
// @return 計算結果を返す．
inline
AlgCover
AlgCover::operator-(const AlgCover& right) const
{
  ASSERT_COND( variable_num() == right.variable_num() );

  int nc1 = cube_num();
  int cap = get_capacity(nc1);
  AlgBitVect* body = mgr().new_body(cap);
  int nc = mgr().diff(body, block(), right.block());
n
  return AlgCover(mgr(), nc, cap, body);
}

// @brief 差分を計算する(キューブ版)．
// @param[in] right オペランド
// @return 計算結果を返す．
inline
AlgCover
AlgCover::operator-(const AlgCube& right) const
{
  ASSERT_COND( variable_num() == right.variable_num() );

  int nc1 = cube_num();
  int cap = get_capacity(nc1);
  AlgBitVect* body = mgr().new_body(cap);
  int nc = mgr().diff(body, block(), AlgMgr::Block{1, right.mBody});

  return AlgCover(mgr(), nc, cap, body);
}

// @brief 差分を計算して代入する．
// @param[in] right オペランド
// @return 演算後の自身への参照を返す．
inline
const AlgCover&
AlgCover::operator-=(const AlgCover& right)
{
  ASSERT_COND( variable_num() == right.variable_num() );

  // 結果のキューブ数は減るだけなのでキューブ容量の変更はしない．
  mCubeNum = mgr().diff(mBody, block(), right.block());

  return *this;
}

// @brief 差分を計算して代入する(キューブ版)．
// @param[in] right オペランド
// @return 演算後の自身への参照を返す．
inline
const AlgCover&
AlgCover::operator-=(const AlgCube& right)
{
  ASSERT_COND( variable_num() == right.variable_num() );

  // 結果のキューブ数は減るだけなのでキューブ容量の変更はしない．
  mCubeNum = mgr().diff(mBody, block(), AlgMgr::Block{1, right.mBody});

  return *this;
}

// @brief 論理積を計算する．
// @param[in] right オペランド
// @return 計算結果を返す．
inline
AlgCover
AlgCover::operator*(const AlgCover& right) const
{
  ASSERT_COND( variable_num() == right.variable_num() );

  int nc1 = cube_num();
  int nc2 = right.cube_num();
  int cap = get_capacity(nc1 * nc2);
  AlgBitVect* body = mgr().new_body(cap);
  int nc = mgr().product(body, block(), right.block());

  return AlgCover(mgr(), nc, cap, body);
}

// @brief 論理積を計算する(キューブ版)．
// @param[in] right オペランド
// @return 計算結果を返す．
inline
AlgCover
AlgCover::operator*(const AlgCube& right) const
{
  ASSERT_COND( variable_num() == right.variable_num() );

  int nc1 = cube_num();
  int cap = get_capacity(nc1);
  AlgBitVect* body = mgr().new_body(cap);
  int nc = mgr().product(body, block(), AlgMgr::Block{1, right.mBody});

  return AlgCover(mgr(), nc, cap, body);
}

// @brief 論理積を計算する(リテラル版)．
// @param[in] right オペランド
// @return 計算結果を返す．
inline
AlgCover
AlgCover::operator*(Literal right) const
{
  int nc1 = cube_num();
  int cap = get_capacity(nc1);
  AlgBitVect* body = mgr().new_body(cap);
  int nc = mgr().product(body, block(), right);

  return AlgCover(mgr(), nc, cap, body);
}

// @brief 論理積を計算して代入する．
// @param[in] right オペランド
// @return 演算後の自身への参照を返す．
inline
const AlgCover&
AlgCover::operator*=(const AlgCover& right)
{
  ASSERT_COND( variable_num() == right.variable_num() );

  int nc1 = cube_num();
  int nc2 = right.cube_num();
  int old_cap = mCubeCap;
  AlgBitVect* old_body = mBody;
  resize(nc1 * nc2);
  mCubeNum = mgr().product(mBody, AlgMgr::Block{nc1, old_body}, right.block());
  if ( old_body != mBody ) {
    mgr().delete_body(old_body, old_cap);
  }

  return *this;
}

// @brief 論理積を計算して代入する(キューブ版)．
// @param[in] right オペランド
// @return 演算後の自身への参照を返す．
inline
const AlgCover&
AlgCover::operator*=(const AlgCube& right)
{
  ASSERT_COND( variable_num() == right.variable_num() );

  // キューブ数が増えることはない
  mCubeNum = mgr().product(mBody, block(), right.block());

  return *this;
}

// @brief 論理積を計算して代入する(リテラル版)．
// @param[in] right オペランド
// @return 演算後の自身への参照を返す．
inline
const AlgCover&
AlgCover::operator*=(Literal right)
{
  mCubeNum = mgr().product(mBody, mCubeNum, mBody, right);

  return *this;
}

// @brief algebraic division を計算する．
// @param[in] right オペランド
// @return 計算結果を返す．
inline
AlgCover
AlgCover::operator/(const AlgCover& right) const
{
  ASSERT_COND( variable_num() == right.variable_num() );

  int nc1 = cube_num();
  int nc2 = right.cube_num();
  int cap = get_capacity(nc1 / nc2);
  AlgBitVect* body = mgr().new_body(cap);
  int nc = mgr().division(body, block(), right.block());

  return AlgCover(mgr(), nc, cap, body);
}

// @brief algebraic division を行って代入する．
// @param[in] right オペランド
// @return 演算後の自身への参照を返す．
inline
const AlgCover&
AlgCover::operator/=(const AlgCover& right)
{
  ASSERT_COND( variable_num() == right.variable_num() );

  // 結果のキューブ数は減るだけなのでキューブ容量は変更しない．
  mCubeNum = mgr().division(mBody, block(), right.block());

  return *this;
}

// @brief キューブによる商を計算する．
// @param[in] cube 対象のキューブ
// @return 計算結果を返す．
inline
AlgCover
AlgCover::operator/(const AlgCube& cube) const
{
  ASSERT_COND( variable_num() == cube.variable_num() );

  int nc1 = cube_num();
  int cap = get_capacity(nc1);
  AlgBitVect* body = mgr().new_body(cap);
  int nc = mgr().division(body, block(), cube.block());

  return AlgCover(mgr(), nc, cap, body);
}

// @brief キューブによる商を計算して代入する．
// @param[in] cube 対象のキューブ
// @return 演算後の自身への参照を返す．
inline
AlgCover
AlgCover::operator/=(const AlgCube& cube)
{
  ASSERT_COND( variable_num() == cube.variable_num() );

  // 結果のキューブ数は減るだけなのでキューブ容量は変更しない．
  mCubeNum = mgr().division(mBody, block(), cube.block());

  return *this;
}

// @brief リテラルによる商を計算する．
// @param[in] lit 対象のリテラル
// @return 計算結果を返す．
inline
AlgCover
AlgCover::operator/(Literal lit) const
{
  int nc1 = cube_num();
  int cap = get_capacity(nc1);
  AlgBitVect* body = mgr().new_body(cap);
  int nc = mgr().division(body, block(), lit);

  return AlgCover(mgr(), nc, cap, body);
}

// @brief リテラルによる商を計算して代入する．
// @param[in] lit 対象のリテラル
// @return 演算後の自身への参照を返す．
inline
const AlgCover&
AlgCover::operator/=(Literal lit)
{
  // 結果のキューブ数は減るだけなのでキューブ容量は変更しない．
  mCubeNum = mgr().division(mBody, block(), lit);

  return *this;
}

// @brief 共通なキューブを返す．
//
// 共通なキューブがない場合には空のキューブを返す．
inline
AlgCube
AlgCover::common_cube() const
{
  AlgBitVect* body = mgr().new_body();
  mgr().common_cube(body, block());

  return AlgCube(mgr(), body);
}

// @brief ハッシュ値を返す．
inline
SizeType
AlgCover::hash() const
{
  return mgr().hash(mCubeNum, mBody);
}

// @brief 内容をわかりやすい形で出力する．
// @param[in] s 出力先のストリーム
inline
void
AlgCover::print(ostream& s) const
{
  mgr().print(s, mBody, 0, mCubeNum);
}

// @relates AlgCover
// @brief 比較演算子(rich compare)
// @param[in] left, right オペランド
// @return 比較結果を返す．
//
// 比較方法はキューブごとの辞書式順序
inline
int
compare(const AlgCover& left,
	const AlgCover& right)
{
  ASSERT_COND( &left.mgr() == &right.mgr() );

  return left.mgr().compare(left.block(), right.block());
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

// @brief キューブ容量を変更する．
// @param[in] req_cap 要求するキューブ容量
//
// 現在のキューブ容量が大きかれば変更しない．
inline
void
AlgCover::resize(int req_cap)
{
  int new_cap = get_capacity(req_cap);
  if ( new_cap > mCubeCap ) {
    mCubeCap = new_cap;
    mBody = mgr().new_body(mCubeCap);
  }
}

// @brief キューブ容量を計算する．
inline
int
AlgCover::get_capacity(int cube_num)
{
  // 初期値を16としてcube_numを下回らない
  // ２のべき乗の数を求める．
  int ans = 16;
  while ( ans < cube_num ) {
    ans *= 2;
  }
  return ans;
}

END_NAMESPACE_YM_ALG

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
