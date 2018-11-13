
/// @file AlgCover.cc
/// @brief AlgCover の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/AlgCover.h"
#include "ym/AlgCube.h"
#include "AlgBlock.h"
#include "AlgMgr.h"


BEGIN_NAMESPACE_YM_ALG

//////////////////////////////////////////////////////////////////////
// クラス AlgCover
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] variable_num 変数の数
// @param[in] cube_list キューブのリスト
//
// * cube_list が空の時は空のカバーとなる．
// * cube_list が空でない時は各キューブのサイズは variable_num
//   と等しくなければならない．
AlgCover::AlgCover(int variable_num,
		   const vector<AlgCube>& cube_list) :
  mVariableNum{variable_num},
  mCubeNum{static_cast<int>(cube_list.size())},
  mCubeCap{mCubeNum},
  mBody{nullptr}
{
  AlgMgr mgr(mVariableNum);
  mBody = mgr.new_body(mCubeCap);
  mgr.copy_from_cube_list(mBody, cube_list);
}

// @brief 特殊なコンストラクタ
// @param[in] variable_num 変数の数
// @param[in] dummy ダミーの引数
//
// 空のキューブを1つ持つカバーとなる．
// dummy の値は無視される．
AlgCover::AlgCover(int variable_num,
		   int dummy) :
  mVariableNum{variable_num},
  mCubeNum{1},
  mCubeCap{mCubeNum},
  mBody{nullptr}
{
  AlgMgr mgr(mVariableNum);
  mBody = mgr.new_body(mCubeCap);
  mgr.cube_clear(mBody);
}

BEGIN_NONAMESPACE

// lit_list をスキャンしてキューブ数を数える．
inline
int
count_cube_num(const vector<Literal>& lit_list)
{
  int n = 1;
  for ( auto lit: lit_list ) {
    if ( lit == Literal::x() ) {
      ++ n;
    }
  }
  return n;
}

END_NONAMESPACE

// @brief コンストラクタ
// @param[in] variable_num 変数の数
// @param[in] lit_list カバーを表すリテラルのリスト
//
// lit_list 中に Literal::x() があった場合，キューブの区切りを表す．
AlgCover::AlgCover(int variable_num,
		   const vector<Literal>& lit_list) :
  mVariableNum{variable_num},
  mCubeNum{count_cube_num(lit_list)},
  mCubeCap{mCubeNum},
  mBody{nullptr}
{
  AlgMgr mgr(mVariableNum);
  mBody = mgr.new_body(mCubeCap);
  mgr.copy_from_lit_list(mBody, lit_list);
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
AlgCover::AlgCover(const AlgCover& src) :
  mVariableNum{src.mVariableNum},
  mCubeNum{src.mCubeNum},
  mCubeCap{mCubeNum},
  mBody{nullptr}
{
  AlgMgr mgr(mVariableNum);
  mBody = mgr.new_body(mCubeCap);
  mgr.copy(mBody, src.mBody, mCubeNum);
}

// @brief コピー代入演算子
// @param[in] src コピー元のオブジェクト
// @return 代入後の自身の参照を返す．
AlgCover&
AlgCover::operator=(const AlgCover& src)
{
  if ( this != &src ) {
    int n = src.cube_num();
    if ( mCubeCap < n ) {
      AlgMgr mgr(mVariableNum);
      mgr.delete_body(mBody, mCubeCap);
    }
    mVariableNum = src.mVariableNum;
    mCubeNum = src.mCubeNum;
    mCubeCap = mCubeNum;;
    AlgMgr mgr(mVariableNum);
    mBody = mgr.new_body(mCubeCap);
    mgr.copy(mBody, src.mBody, mCubeNum);
  }

  return *this;
}

// @brief ムーブコンストラクタ
// @param[in] src ムーブ元のオブジェクト
AlgCover::AlgCover(AlgCover&& src) :
  mVariableNum{src.mVariableNum},
  mCubeNum{src.mCubeNum},
  mCubeCap{src.mCubeCap},
  mBody{std::move(src.mBody)}
{
  src.mBody = nullptr;
}

// @brief ムーブ代入演算子
// @param[in] src ムーブ元のオブジェクト
// @return 代入後の自身の参照を返す．
AlgCover&
AlgCover::operator=(AlgCover&& src)
{
  AlgMgr mgr(mVariableNum);
  mgr.delete_body(mBody, 1);

  mVariableNum = src.mVariableNum;
  mCubeNum = src.mCubeNum;
  mCubeCap = src.mCubeCap;
  mBody = std::move(src.mBody);
  src.mBody = nullptr;

  return *this;
}

// @brief キューブからのコピー変換コンストラクタ
// @param[in] cube 対象のキューブ
//
// 指定されたキューブのみのカバーとなる．
AlgCover::AlgCover(const AlgCube& cube) :
  mVariableNum{cube.variable_num()},
  mCubeNum{1},
  mCubeCap{mCubeNum},
  mBody{0}
{
  AlgMgr mgr(mVariableNum);
  mBody = mgr.new_body(mCubeCap);
  mgr.copy(mBody, cube.mBody, 1);
}

// @brief キューブからのムーブ変換コンストラクタ
// @param[in] cube 対象のキューブ
//
// 指定されたキューブのみのカバーとなる．
AlgCover::AlgCover(AlgCube&& cube) :
  mVariableNum{cube.variable_num()},
  mCubeNum{1},
  mCubeCap{mCubeNum},
  mBody{std::move(cube.mBody)}
{
  cube.mBody = nullptr;
}

// @brief 内容を指定したコンストラクタ
// @param[in] variable_num 変数の数
// @param[in] cube_num キューブ数
// @param[in] cube_cap キューブ容量
// @param[in] body 内容のパタンを表す本体
//
// この関数は危険なので普通は使わないこと
AlgCover::AlgCover(int variable_num,
		   int cube_num,
		   int cube_cap,
		   AlgBitVect* body) :
  mVariableNum{variable_num},
  mCubeNum{cube_num},
  mCubeCap{cube_cap},
  mBody{body}
{
}

// @brief デストラクタ
//
// ここに属しているすべてのキューブは削除される．
AlgCover::~AlgCover()
{
  AlgMgr mgr(mVariableNum);
  mgr.delete_body(mBody, 1);
}

// @brief リテラル数を返す．
int
AlgCover::literal_num() const
{
  AlgMgr mgr(mVariableNum);
  return mgr.literal_num(block());
}

// @brief 指定されたリテラルの出現回数を返す．
// @param[in] lit 対象のリテラル
int
AlgCover::literal_num(Literal lit) const
{
  AlgMgr mgr(mVariableNum);
  return mgr.literal_num(block(), lit);
}

// @brief 内容を返す．
// @param[in] cube_id キューブ番号 ( 0 <= cube_id < cube_num() )
// @param[in] var_id 変数の位置番号 ( 0 <= var_id < variable_num() )
AlgPol
AlgCover::get_pol(int cube_id,
		  VarId var_id) const
{
  AlgMgr mgr(mVariableNum);
  return mgr.get_pol(mBody, cube_id, var_id);
}

// @brief 内容を表す AlgBlock を返す．
inline
AlgBlock
AlgCover::block() const
{
  return AlgBlock{mCubeNum, mBody};
}

// @brief 論理和を計算する．
// @param[in] right オペランド
// @return 計算結果を返す．
AlgCover
AlgCover::operator+(const AlgCover& right) const
{
  ASSERT_COND( mVariableNum == right.mVariableNum );

  AlgMgr mgr(mVariableNum);
  int cap = mCubeNum + right.mCubeNum;
  AlgBitVect* body = mgr.new_body(cap);
  int nc = mgr.sum(body, block(), right.block());

  return AlgCover(mVariableNum, nc, cap, body);
}

// @brief 論理和を計算して代入する．
// @param[in] right オペランド
// @return 演算後の自身への参照を返す．
AlgCover&
AlgCover::operator+=(const AlgCover& right)
{
  ASSERT_COND( mVariableNum == right.mVariableNum );

  AlgMgr mgr(mVariableNum);
  int cap = mCubeNum + right.mCubeNum;
  // 新しいブロックを作る．
  AlgBitVect* body = mgr.new_body(cap);
  int nc = mgr.sum(body, block(), right.block());

  mgr.delete_body(mBody, mCubeNum);

  mCubeNum = nc;
  mCubeCap = cap;
  mBody = body;

  return *this;
}

// @brief 論理和を計算する(キューブ版)．
// @param[in] right オペランド
// @return 計算結果を返す．
AlgCover
AlgCover::operator+(const AlgCube& right) const
{
  ASSERT_COND( mVariableNum == right.mVariableNum );

  AlgMgr mgr(mVariableNum);
  int cap = mCubeNum + 1;
  AlgBitVect* body = mgr.new_body(cap);
  int nc = mgr.sum(body, block(), right.block());
  return AlgCover(mVariableNum, nc, cap, body);
}

// @brief 論理和を計算して代入する(キューブ版)．
// @param[in] right オペランド
// @return 演算後の自身への参照を返す．
AlgCover&
AlgCover::operator+=(const AlgCube& right)
{
  ASSERT_COND( mVariableNum == right.mVariableNum );

  AlgMgr mgr(mVariableNum);
  int cap = mCubeNum + 1;
  AlgBitVect* body = mgr.new_body(cap);
  int nc = mgr.sum(body, block(), right.block());

  mgr.delete_body(mBody, mCubeNum);

  mCubeNum = nc;
  mCubeCap = cap;
  mBody = body;

  return *this;
}

// @brief 差分を計算する．
// @param[in] right オペランド
// @return 計算結果を返す．
//
// right のみに含まれる要素があっても無視される．
AlgCover
AlgCover::operator-(const AlgCover& right) const
{
  ASSERT_COND( mVariableNum == right.mVariableNum );

  AlgMgr mgr(mVariableNum);
  int cap = mCubeNum;
  AlgBitVect* body = mgr.new_body(cap);
  int nc = mgr.diff(body, block(), right.block());

  return AlgCover(mVariableNum, nc, cap, body);
}

// @brief 差分を計算して代入する．
// @param[in] right オペランド
// @return 演算後の自身への参照を返す．
AlgCover&
AlgCover::operator-=(const AlgCover& right)
{
  ASSERT_COND( mVariableNum == right.mVariableNum );

  // キューブ数は増えないのでブロックはそのまま
  AlgMgr mgr(mVariableNum);
  mCubeNum = mgr.diff(mBody, block(), right.block());

  return *this;
}

// @brief 差分を計算する(キューブ版)．
// @param[in] right オペランド
// @return 計算結果を返す．
//
// right のみに含まれる要素があっても無視される．
AlgCover
AlgCover::operator-(const AlgCube& right) const
{
  ASSERT_COND( mVariableNum == right.mVariableNum );

  AlgMgr mgr(mVariableNum);
  int cap = mCubeNum;
  AlgBitVect* body = mgr.new_body(cap);
  int nc = mgr.diff(body, block(), right.block());

  return AlgCover(mVariableNum, nc, cap, body);
}

// @brief 差分を計算して代入する(キューブ版)．
// @param[in] right オペランド
// @return 演算後の自身への参照を返す．
AlgCover&
AlgCover::operator-=(const AlgCube& right)
{
  ASSERT_COND( mVariableNum == right.mVariableNum );

  AlgMgr mgr(mVariableNum);
  mCubeNum = mgr.diff(mBody, block(), right.block());

  return *this;
}

// @brief 論理積を計算する．
// @param[in] right オペランド
// @return 計算結果を返す．
AlgCover
AlgCover::operator*(const AlgCover& right) const
{
  ASSERT_COND( mVariableNum == right.mVariableNum );

  AlgMgr mgr(mVariableNum);
  int cap = mCubeNum * right.mCubeNum;
  AlgBitVect* body = mgr.new_body(cap);
  int nc = mgr.product(body, block(), right.block());

  return AlgCover(mVariableNum, nc, cap, body);
}

// @brief 論理積を計算して代入する．
// @param[in] right オペランド
// @return 演算後の自身への参照を返す．
AlgCover&
AlgCover::operator*=(const AlgCover& right)
{
  ASSERT_COND( mVariableNum == right.mVariableNum );

  AlgMgr mgr(mVariableNum);
  int cap = mCubeNum * right.mCubeNum;
  AlgBitVect* body = mgr.new_body(cap);
  int nc = mgr.product(body, block(), right.block());

  mgr.delete_body(mBody, mCubeNum);

  mCubeNum = nc;
  mCubeCap = cap;
  mBody = body;

  return *this;
}

// @brief 論理積を計算する(キューブ版)．
// @param[in] right オペランド
// @return 計算結果を返す．
AlgCover
AlgCover::operator*(const AlgCube& right) const
{
  ASSERT_COND( mVariableNum == right.mVariableNum );

  AlgMgr mgr(mVariableNum);
  int cap = mCubeNum;
  AlgBitVect* body = mgr.new_body(cap);
  int nc = mgr.product(body, block(), right.block());

  return AlgCover(mVariableNum, nc, cap, body);
}

// @brief 論理積を計算して代入する(キューブ版)．
// @param[in] right オペランド
// @return 演算後の自身への参照を返す．
AlgCover&
AlgCover::operator*=(const AlgCube& right)
{
  ASSERT_COND( mVariableNum == right.mVariableNum );

  AlgMgr mgr(mVariableNum);
  mCubeNum = mgr.product(mBody, block(), right.block());

  return *this;
}

// @brief 論理積を計算する(リテラル版)．
// @param[in] right オペランド
// @return 計算結果を返す．
AlgCover
AlgCover::operator*(Literal right) const
{
  AlgMgr mgr(mVariableNum);
  int cap = mCubeNum;
  AlgBitVect* body = mgr.new_body(cap);
  int nc = mgr.product(body, block(), right);

  return AlgCover(mVariableNum, nc, cap, body);
}

// @brief 論理積を計算して代入する(リテラル版)．
// @param[in] right オペランド
// @return 演算後の自身への参照を返す．
AlgCover&
AlgCover::operator*=(Literal right)
{
  AlgMgr mgr(mVariableNum);
  mCubeNum = mgr.product(mBody, block(), right);

  return *this;
}

// @brief algebraic division を計算する．
// @param[in] right オペランド
// @return 計算結果を返す．
AlgCover
AlgCover::operator/(const AlgCover& right) const
{
  ASSERT_COND( mVariableNum == right.mVariableNum );

  AlgMgr mgr(mVariableNum);
  int cap = mCubeNum;
  AlgBitVect* body = mgr.new_body(cap);
  int nc = mgr.quotient(body, block(), right.block());

  return AlgCover(mVariableNum, nc, cap, body);
}

// @brief algebraic division を行って代入する．
// @param[in] right オペランド
// @return 演算後の自身への参照を返す．
AlgCover&
AlgCover::operator/=(const AlgCover& right)
{
  ASSERT_COND( mVariableNum == right.mVariableNum );

  AlgMgr mgr(mVariableNum);
  mCubeNum = mgr.quotient(mBody, block(), right.block());

  return *this;
}

// @brief キューブによる商を計算する．
// @param[in] right 対象のキューブ
// @return 計算結果を返す．
AlgCover
AlgCover::operator/(const AlgCube& right) const
{
  ASSERT_COND( mVariableNum == right.mVariableNum );

  AlgMgr mgr(mVariableNum);
  int cap = mCubeNum;
  AlgBitVect* body = mgr.new_body(cap);
  int nc = mgr.quotient(body, block(), right.block());

  return AlgCover(mVariableNum, nc, cap, body);
}

// @brief キューブによる商を計算して代入する．
// @param[in] right 対象のキューブ
// @return 演算後の自身への参照を返す．
AlgCover&
AlgCover::operator/=(const AlgCube& right)
{
  ASSERT_COND( mVariableNum == right.mVariableNum );

  AlgMgr mgr(mVariableNum);
  mCubeNum = mgr.quotient(mBody, block(), right.block());

  return *this;
}

// @brief リテラルによる商を計算する．
// @param[in] lit 対象のリテラル
// @return 計算結果を返す．
AlgCover
AlgCover::operator/(Literal lit) const
{
  AlgMgr mgr(mVariableNum);
  int cap = mCubeNum;
  AlgBitVect* body = mgr.new_body(cap);
  int nc = mgr.quotient(body, block(), lit);

  return AlgCover(mVariableNum, nc, cap, body);
}

// @brief リテラルによる商を計算して代入する．
// @param[in] lit 対象のリテラル
// @return 演算後の自身への参照を返す．
AlgCover&
AlgCover::operator/=(Literal lit)
{
  AlgMgr mgr(mVariableNum);
  mCubeNum = mgr.quotient(mBody, block(), lit);

  return *this;
}

// @brief 共通なキューブを返す．
//
// 共通なキューブがない場合には空のキューブを返す．
AlgCube
AlgCover::common_cube() const
{
  AlgMgr mgr(mVariableNum);
  AlgBitVect* body = mgr.new_body(1);
  mgr.common_cube(body, block());

  return AlgCube(mVariableNum, body);
}

// @brief ハッシュ値を返す．
SizeType
AlgCover::hash() const
{
  AlgMgr mgr(mVariableNum);
  return mgr.hash(block());
}

// @brief 内容をわかりやすい形で出力する．
// @param[in] s 出力先のストリーム
// @param[in] varname_list 変数名のリスト
void
AlgCover::print(ostream& s,
		const vector<string>& varname_list) const
{
  AlgMgr mgr(mVariableNum);
  mgr.print(s, mBody, 0, mCubeNum, varname_list);
}

END_NAMESPACE_YM_ALG
