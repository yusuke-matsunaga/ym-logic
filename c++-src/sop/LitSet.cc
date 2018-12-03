
/// @file LitSet.cc
/// @brief LitSet の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "LitSet.h"
#include "SopMgr.h"
#include "ym/SopCube.h"


BEGIN_NAMESPACE_YM_LOGIC

//////////////////////////////////////////////////////////////////////
// クラス LitSet
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] variable_num 変数の数
//
// * 空集合となる．
LitSet::LitSet(int variable_num) :
  mVariableNum(variable_num),
  mBody(nullptr)
{
  SopMgr mgr(mVariableNum);
  mBody = mgr.new_body(1);
  mgr.cube_clear(mBody);
}

// @brief コンストラクタ
// @param[in] variable_num 変数の数
// @param[in] lit リテラル
//
// 単一のリテラルからなるキューブを作る．
LitSet::LitSet(int variable_num,
		     Literal lit) :
  mVariableNum(variable_num),
  mBody(nullptr)
{
  SopMgr mgr(mVariableNum);
  mBody = mgr.new_body(1);
  mgr.cube_clear(mBody);
  mgr.cube_set(mBody, vector<Literal>({lit}));
}

// @brief コンストラクタ
// @param[in] variable_num 変数の数
// @param[in] lit_list キューブを表すリテラルのリスト
LitSet::LitSet(int variable_num,
		     const vector<Literal>& lit_list) :
  mVariableNum(variable_num),
  mBody(nullptr)
{
  SopMgr mgr(mVariableNum);
  mBody = mgr.new_body(1);
  mgr.cube_clear(mBody);
  mgr.cube_set(mBody, lit_list);
}

// @brief コンストラクタ
// @param[in] variable_num 変数の数
// @param[in] lit_list キューブを表すリテラルのリスト
LitSet::LitSet(int variable_num,
		     std::initializer_list<Literal>& lit_list) :
  mVariableNum(variable_num),
  mBody(nullptr)
{
  SopMgr mgr(mVariableNum);
  mBody = mgr.new_body(1);
  mgr.cube_clear(mBody);
  mgr.cube_set(mBody, lit_list);
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
LitSet::LitSet(const LitSet& src) :
  mVariableNum(src.mVariableNum),
  mBody(nullptr)
{
  SopMgr mgr(mVariableNum);
  mBody = mgr.new_body(1);
  mgr.cube_copy(mBody, src.mBody);
}

// @brief 代入演算子
// @param[in] src コピー元のオブジェクト
// @return 代入後の自身への参照を返す．
LitSet&
LitSet::operator=(const LitSet& src)
{
  if ( &src != this ) {
    SopMgr mgr(src.mVariableNum);
    if ( mVariableNum != src.mVariableNum ) {
      SopMgr old_mgr(mVariableNum);
      old_mgr.delete_body(mBody, 1);
      mVariableNum = src.mVariableNum;
      mBody = mgr.new_body(1);
    }
    mgr.cube_copy(mBody, src.mBody);
  }

  return *this;
}

// @brief ムーブコンストラクタ
// @param[in] src ムーブ元のオブジェクト
LitSet::LitSet(LitSet&& src) :
  mVariableNum{src.mVariableNum},
  mBody{src.mBody}
{
  src.mBody = nullptr;
}

// @brief ムーブ代入演算子
// @param[in] src ムーブ元のオブジェクト
// @return 代入後の自身への参照を返す．
LitSet&
LitSet::operator=(LitSet&& src)
{
  SopMgr mgr(mVariableNum);
  mgr.delete_body(mBody, 1);

  mVariableNum = src.mVariableNum;
  mBody = src.mBody;

  src.mBody = nullptr;

  return *this;
}

// @brief デストラクタ
LitSet::~LitSet()
{
  SopMgr mgr(mVariableNum);
  mgr.delete_body(mBody, 1);
}

// @brief 該当するリテラルが含まれているか調べる．
// @param[in] lit 対象のリテラル
// @retval true 含まれている．
// @retval false 含まれていない．
bool
LitSet::is_in(Literal lit) const
{
  SopMgr mgr(mVariableNum);
  return mgr.litset_check(mBody, lit);
}

// @brief ユニオン演算付き代入
// @param[in] right オペランド
// @return 自身への参照を返す．
LitSet&
LitSet::operator+=(const LitSet& right)
{
  SopMgr mgr(mVariableNum);
  mgr.litset_union(mBody, right.mBody);

  return *this;
}

// @brief 要素を足す．
// @param[in] lit 追加するリテラル
// @return 自身への参照を返す．
LitSet&
LitSet::operator+=(Literal lit)
{
  SopMgr mgr(mVariableNum);
  mgr.cube_set(mBody, vector<Literal>{lit});

  return *this;
}

// @brief 引数のキューブ中のリテラルをひとつでも含んでいたら true を返す．
// @param[in] right 対象のキューブ
bool
LitSet::check_intersect(const SopCube& right) const
{
  ASSERT_COND( mVariableNum == right.variable_num() );

  SopMgr mgr(mVariableNum);
  return mgr.cube_check_intersect(mBody, right);
}

END_NAMESPACE_YM_LOGIC
