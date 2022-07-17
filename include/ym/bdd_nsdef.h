#ifndef YM_BDD_NSDEF_H
#define YM_BDD_NSDEF_H

/// @file ym/bdd_nsdef.h
/// @brief BDD 用の名前空間の定義
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


/// @brief BDD 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_BDD \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsBdd)

/// @brief BDD 用の名前空間の終了
#define END_NAMESPACE_YM_BDD \
END_NAMESPACE(nsBdd) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// クラスの先行宣言
//////////////////////////////////////////////////////////////////////

class Bdd;
class BddMgr;

END_NAMESPACE_YM_BDD


BEGIN_NAMESPACE_YM

using nsBdd::Bdd;
using nsBdd::BddMgr;

END_NAMESPACE_YM

#endif // YM_BDD_NSDEF_H
