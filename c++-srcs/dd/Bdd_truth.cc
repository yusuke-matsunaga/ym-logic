
/// @file Bdd_truth.cc
/// @brief Bdd::to_truth() の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "ym/BddVar.h"
#include "BddMgrImpl.h"
#include "DdEdge.h"
#include "DdNode.h"


BEGIN_NAMESPACE_YM_DD

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
/// @class TruStrOp TruStrOp.h "TruStrOp.h"
/// @brief 真理値表文字列を作る
///
/// 具体的には var_list を保持しているだけのクラス
//////////////////////////////////////////////////////////////////////
class TruStrOp
{
public:

  /// @brief コンストラクタ
  TruStrOp(
    const vector<BddVar>& var_list ///< [in] 変数のリスト
  ) : mVarList{var_list}
  {
  }

  /// @brief デストラクタ
  ~TruStrOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  string
  run(
    const Bdd& bdd
  ) const
  {
    return trustr_step(bdd, 0);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  string
  trustr_step(
    const Bdd& bdd,
    SizeType pos
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数のリスト
  vector<BddVar> mVarList;

};

string
TruStrOp::trustr_step(
  const Bdd& bdd,
  SizeType pos
) const
{
  if ( pos >= mVarList.size() ) {
    if ( bdd.is_zero() ) {
      return "0";
    }
    if ( bdd.is_one() ) {
      return "1";
    }
    throw std::invalid_argument("Bdd::to_truth(var_list): invalid var_list");
  }
  auto var = mVarList[pos];
  // bdd を var で分解する．
  auto bdd0 = bdd / ~var;
  auto bdd1 = bdd / var;
  auto ans0 = trustr_step(bdd0, pos + 1);
  auto ans1 = trustr_step(bdd1, pos + 1);
  auto ans = ans1 + ans0;
  return ans;
}

END_NONAMESPACE

// @brief 内容を真理値表の文字列に変換する．
string
Bdd::to_truth(
  const vector<BddVar> var_list
) const
{
  if ( is_invalid() ) {
    // 不正値の場合には空文字列を返す．
    return string{};
  }
  TruStrOp op{var_list};
  return op.run(*this);
}

END_NAMESPACE_YM_DD
