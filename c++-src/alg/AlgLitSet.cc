
/// @file AlgLitSet.cc
/// @brief AlgLitSet の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "AlgLitSet.h"
#include "AlgMgr.h"


BEGIN_NAMESPACE_YM_LOGIC

//////////////////////////////////////////////////////////////////////
// クラス AlgLitSet
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] variable_num 変数の数
//
// * 空集合となる．
AlgLitSet::AlgLitSet(int variable_num) :
  mVariableNum(variable_num),
  mBody(nullptr)
{
  AlgMgr mgr(mVariableNum);
  mBody = mgr.new_body(1);
  mgr.cube_clear(mBody);
}

// @brief コンストラクタ
// @param[in] variable_num 変数の数
// @param[in] lit リテラル
//
// 単一のリテラルからなるキューブを作る．
AlgLitSet::AlgLitSet(int variable_num,
		     Literal lit) :
  mVariableNum(variable_num),
  mBody(nullptr)
{
  AlgMgr mgr(mVariableNum);
  mBody = mgr.new_body(1);
  mgr.cube_clear(mBody);
  mgr.cube_set(mBody, vector<Literal>({lit}));
}

// @brief コンストラクタ
// @param[in] variable_num 変数の数
// @param[in] lit_list キューブを表すリテラルのリスト
AlgLitSet::AlgLitSet(int variable_num,
		     const vector<Literal>& lit_list) :
  mVariableNum(variable_num),
  mBody(nullptr)
{
  AlgMgr mgr(mVariableNum);
  mBody = mgr.new_body(1);
  mgr.cube_clear(mBody);
  mgr.cube_set(mBody, lit_list);
}

// @brief コンストラクタ
// @param[in] variable_num 変数の数
// @param[in] lit_list キューブを表すリテラルのリスト
AlgLitSet::AlgLitSet(int variable_num,
		     std::initializer_list<Literal>& lit_list) :
  mVariableNum(variable_num),
  mBody(nullptr)
{
  AlgMgr mgr(mVariableNum);
  mBody = mgr.new_body(1);
  mgr.cube_clear(mBody);
  mgr.cube_set(mBody, lit_list);
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
AlgLitSet::AlgLitSet(const AlgLitSet& src) :
  mVariableNum(src.mVariableNum),
  mBody(nullptr)
{
  AlgMgr mgr(mVariableNum);
  mBody = mgr.new_body(1);
  mgr.cube_copy(mBody, src.mBody);
}

// @brief 代入演算子
// @param[in] src コピー元のオブジェクト
// @return 代入後の自身への参照を返す．
AlgLitSet&
AlgLitSet::operator=(const AlgLitSet& src)
{
  if ( &src != this ) {
    AlgMgr mgr(src.mVariableNum);
    if ( mVariableNum != src.mVariableNum ) {
      AlgMgr old_mgr(mVariableNum);
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
AlgLitSet::AlgLitSet(AlgLitSet&& src) :
  mVariableNum{src.mVariableNum},
  mBody{src.mBody}
{
  src.mBody = nullptr;
}

// @brief ムーブ代入演算子
// @param[in] src ムーブ元のオブジェクト
// @return 代入後の自身への参照を返す．
AlgLitSet&
AlgLitSet::operator=(AlgLitSet&& src)
{
  AlgMgr mgr(mVariableNum);
  mgr.delete_body(mBody, 1);

  mVariableNum = src.mVariableNum;
  mBody = src.mBody;

  src.mBody = nullptr;

  return *this;
}

// @brief デストラクタ
AlgLitSet::~AlgLitSet()
{
  AlgMgr mgr(mVariableNum);
  mgr.delete_body(mBody, 1);
}

// @brief 該当するリテラルが含まれているか調べる．
// @param[in] lit 対象のリテラル
// @retval true 含まれている．
// @retval false 含まれていない．
bool
AlgLitSet::is_in(Literal lit) const
{
  AlgMgr mgr(mVariableNum);
  return mgr.litset_check(mBody, lit);
}

// @brief ユニオン演算付き代入
// @param[in] right オペランド
// @return 自身への参照を返す．
AlgLitSet&
AlgLitSet::operator+=(const AlgLitSet& right)
{
  AlgMgr mgr(mVariableNum);
  mgr.litset_union(mBody, right.mBody);

  return *this;
}

// @brief 要素を足す．
// @param[in] lit 追加するリテラル
// @return 自身への参照を返す．
AlgLitSet&
AlgLitSet::operator+=(Literal lit)
{
  AlgMgr mgr(mVariableNum);
  mgr.cube_set(mBody, vector<Literal>{lit});

  return *this;
}

END_NAMESPACE_YM_LOGIC
