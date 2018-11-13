
/// @file AlgCube.cc
/// @brief AlgCube の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/AlgCube.h"
#include "AlgBlock.h"
#include "AlgMgr.h"


BEGIN_NAMESPACE_YM_ALG

// @brief コンストラクタ
// @param[in] variable_num 変数の数
// @param[in] lit リテラル
//
// 単一のリテラルからなるキューブを作る．
AlgCube::AlgCube(int variable_num,
		 Literal lit) :
  mVariableNum{variable_num}
{
  AlgMgr mgr(mVariableNum);
  mBody = mgr.new_body(1);
  mgr.copy_from_lit_list(mBody, vector<Literal>{lit});
}

// @brief コンストラクタ
// @param[in] variable_num 変数の数
// @param[in] lit_list キューブを表すリテラルのリスト
//
// lit_list が省略された時には空のキューブを作る．
AlgCube::AlgCube(int variable_num,
		 const vector<Literal>& lit_list) :
  mVariableNum{variable_num}
{
  AlgMgr mgr(mVariableNum);
  mBody = mgr.new_body(1);
  mgr.copy_from_lit_list(mBody, lit_list);
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
AlgCube::AlgCube(const AlgCube& src) :
  mVariableNum{src.mVariableNum}
{
  AlgMgr mgr(mVariableNum);
  mBody = mgr.new_body(1);
  mgr.copy(mBody, src.mBody, 1);
}

// @brief コピー代入演算子
// @param[in] src コピー元のオブジェクト
// @return 代入後の自身への参照を返す．
AlgCube&
AlgCube::operator=(const AlgCube& src)
{
  if ( mVariableNum != src.mVariableNum ) {
    delete_body();
    mVariableNum = src.mVariableNum;
    AlgMgr mgr(mVariableNum);
    mBody = mgr.new_body(1);
  }
  AlgMgr mgr(mVariableNum);
  mgr.copy(mBody, src.mBody, 1);

  return *this;
}

// @brief ムーブコンストラクタ
// @param[in] src ムーブ元のオブジェクト
AlgCube::AlgCube(AlgCube&& src) :
  mVariableNum{src.mVariableNum},
  mBody{std::move(src.mBody)}
{
}

// @brief ムーブ代入演算子
// @param[in] src ムーブ元のオブジェクト
// @return 代入後の自身への参照を返す．
AlgCube&
AlgCube::operator=(AlgCube&& src)
{
  delete_body();

  mVariableNum = src.mVariableNum;
  mBody = std::move(src.mBody);

  return *this;
}

// @brief 内容を指定するコンストラクタ
// @param[in] variable_num 変数の数
// @param[in] body キューブのパタンを表す本体
//
// 危険なので普通は使わないように
AlgCube::AlgCube(int variable_num,
		 AlgBitVect* body) :
  mVariableNum{variable_num},
  mBody{body}
{
}

// @brief デストラクタ
AlgCube::~AlgCube()
{
  delete_body();
}

// @brief mBody を削除する．
void
AlgCube::delete_body()
{
  if ( mBody != nullptr ) {
    AlgMgr mgr(mVariableNum);
    mgr.delete_body(mBody, 1);
  }
}

// @brief 内容を読み出す．
// @param[in] varid 変数番号 ( 0 <= varid.val() < variable_num() )
// @retval AlgPol::X この位置にリテラルはない．
// @retval AlgPol::P 肯定のリテラルがある．
// @retval AlgPol::N 否定のリテラルがある．
AlgPol
AlgCube::get_pol(VarId varid) const
{
  AlgMgr mgr(mVariableNum);
  return mgr.get_pol(mBody, 0, varid);
}

// @brief リテラル数を返す．
int
AlgCube::literal_num() const
{
  AlgMgr mgr(mVariableNum);
  return mgr.literal_num(block());
}

// @brief 指定したリテラルを含んでいたら true を返す．
// @param[in] lit 対象のリテラル
bool
AlgCube::has_literal(Literal lit) const
{
  AlgMgr mgr(mVariableNum);
  return mgr.literal_num(block(), lit) > 0;
}

// @brief オペランドのキューブに含まれていたら true を返す．
// @param[in] right オペランドのキューブ
//
// ここではキューブの表す論理関数の含意を考える<br>
// だからリテラル集合としてはオペランドのキューブを含むことになる．
bool
AlgCube::check_containment(const AlgCube& right) const
{
  ASSERT_COND( mVariableNum == right.mVariableNum );

  AlgMgr mgr(mVariableNum);
  return mgr.cube_check_containment(mBody, right.mBody);
}

// @brief 2つのキューブに共通なリテラルがあれば true を返す．
// @param[in] right オペランドのキューブ
bool
AlgCube::check_intersect(const AlgCube& right) const
{
  ASSERT_COND( mVariableNum == right.mVariableNum );

  AlgMgr mgr(mVariableNum);
  return mgr.cube_check_intersect(mBody, right.mBody);
}

// @brief 引数のリテラルをひとつでも含んでいたら true を返す．
// @param[in] right 対象のリテラル集合
bool
AlgCube::contains(const AlgLitSet& right) const
{
}

// @brief キューブの論理積を計算する
// @param[in] right オペランド
//
// リテラル集合としてみると和集合となる<br>
// ただし，相反するリテラルが含まれていたら空キューブとなる．
AlgCube
AlgCube::operator*(const AlgCube& right) const
{
  ASSERT_COND( mVariableNum == right.mVariableNum );

  AlgMgr mgr(mVariableNum);
  AlgBitVect* body = mgr.new_body(1);
  bool stat = mgr.cube_product(body, mBody, right.mBody);
  if ( !stat ) {
    mgr.cube_clear(body);
  }

  return AlgCube(mVariableNum, body);
}

// @brief 論理積を計算し自身に代入する．
// @param[in] right オペランドのキューブ
// @return 演算後の自身の参照を返す．
//
// リテラル集合とみなすとユニオンを計算することになる<br>
// ただし，相反するリテラルとの積があったら答は空のキューブとなる．
AlgCube&
AlgCube::operator*=(const AlgCube& right)
{
  ASSERT_COND( mVariableNum == right.mVariableNum );

  AlgMgr mgr(mVariableNum);
  bool stat = mgr.cube_product(mBody, mBody, right.mBody);
  if ( !stat ) {
    mgr.cube_clear(mBody);
  }

  return *this;
}

// @brief キューブとリテラルの論理積を計算する
// @param[in] right オペランドのリテラル
//
// リテラル集合としてみると和集合となる<br>
// ただし，相反するリテラルが含まれていたら空キューブとなる．
AlgCube
AlgCube::operator*(Literal right) const
{
  AlgMgr mgr(mVariableNum);
  AlgBitVect* body = mgr.new_body(1);
  int nc = mgr.product(body, block(), right);
  if ( nc == 0 ) {
    mgr.cube_clear(body);
  }

  return AlgCube(mVariableNum, body);
}

// @brief リテラルとの論理積を計算し自身に代入する．
// @param[in] right オペランドのリテラル
// @return 演算後の自身の参照を返す．
//
// リテラル集合とみなすとユニオンを計算することになる<br>
// ただし，相反するリテラルとの積があったら答は空のキューブとなる．
AlgCube&
AlgCube::operator*=(Literal right)
{
  AlgMgr mgr(mVariableNum);
  int nc = mgr.product(mBody, block(), right);
  if ( nc == 0 ) {
    mgr.cube_clear(mBody);
  }

  return *this;
}

// @brief キューブによる商を計算する
// @param[in] right オペランド
AlgCube
AlgCube::operator/(const AlgCube& right) const
{
  ASSERT_COND( mVariableNum == right.mVariableNum );

  AlgMgr mgr(mVariableNum);
  AlgBitVect* body = mgr.new_body(1);
  bool stat = mgr.cube_division(body, mBody, right.mBody);
  if ( !stat ) {
    mgr.cube_clear(body);
  }

  return AlgCube(mVariableNum, body);
}

// @brief キューブによる商を計算し自身に代入する．
// @param[in] right オペランドのキューブ
// @return 演算後の自身の参照を返す．
//
// リテラル集合として考えると集合差を計算することになる<br>
// ただし，right のみに含まれるリテラルがあったら結果は空となる．
AlgCube&
AlgCube::operator/=(const AlgCube& right)
{
  ASSERT_COND( mVariableNum == right.mVariableNum );

  AlgMgr mgr(mVariableNum);
  bool stat = mgr.cube_division(mBody, mBody, right.mBody);
  if ( !stat ) {
    mgr.cube_clear(mBody);
  }
}

// @brief リテラルによる商を計算する
// @param[in] right オペランドのリテラル
AlgCube
AlgCube::operator/(Literal right) const
{
  AlgMgr mgr(mVariableNum);
  AlgBitVect* body = mgr.new_body(1);
  int nc = mgr.quotient(body, block(), right);
  if ( nc == 0 ) {
    mgr.cube_clear(body);
  }

  return AlgCube(mVariableNum, body);
}

// @brief リテラルによる商を計算し自身に代入する．
// @param[in] right オペランドのリテラル
// @return 演算後の自身の参照を返す．
//
// リテラル集合として考えると集合差を計算することになる<br>
// ただし，right のみに含まれるリテラルがあったら結果は空となる．
AlgCube&
AlgCube::operator/=(Literal right)
{
  AlgMgr mgr(mVariableNum);
  int nc = mgr.quotient(mBody, block(), right);
  if ( nc == 0 ) {
    mgr.cube_clear(mBody);
  }

  return *this;
}

// @breif AlgBlock を返す．
AlgBlock
AlgCube::block() const
{
  return AlgBlock(1, mBody);
}

// @brief ハッシュ値を返す．
SizeType
AlgCube::hash() const
{
  AlgMgr mgr(mVariableNum);
  return mgr.hash(block());
}

// @brief 内容をわかりやすい形で出力する．
// @param[in] s 出力先のストリーム
// @param[in] varname_list 変数名のリスト
void
AlgCube::print(ostream& s,
	       const vector<string>& varname_list) const
{
  AlgMgr mgr(mVariableNum);
  mgr.print(s, mBody, 0, 1, varname_list);
}

END_NAMESPACE_YM_ALG
