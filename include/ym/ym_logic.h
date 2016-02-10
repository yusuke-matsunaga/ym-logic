#ifndef YM_YM_LOGIC_H
#define YM_YM_LOGIC_H

/// @file ym/ym_logic.h
/// @brief ym-logic パッケージの基本ヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup ExprGroup 論理式を扱うクラス
///
/// 論理式(ファクタードフォーム)を扱うためのクラスライブラリ
/// C++ の機能を活かして論理演算子のオーバーロードを使っている．


#include "ym_config.h"


/// @brief ym-logic 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_LOGIC \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsLogic)

/// @brief expr 用の名前空間の終了
#define END_NAMESPACE_YM_LOGIC \
END_NAMESPACE(nsLogic) \
END_NAMESPACE_YM

BEGIN_NAMESPACE_YM_LOGIC

//////////////////////////////////////////////////////////////////////
// クラスの先行宣言
//////////////////////////////////////////////////////////////////////

class Expr;
class ExprWriter;
class NpnMap;
class NpnMapM;
class NpnVmap;
class TvFunc;
class TvFuncM;

END_NAMESPACE_YM_LOGIC


BEGIN_NAMESPACE_YM

using nsLogic::Expr;
using nsLogic::ExprWriter;
using nsLogic::NpnMap;
using nsLogic::NpnMapM;
using nsLogic::NpnVmap;
using nsLogic::TvFunc;
using nsLogic::TvFuncM;

END_NAMESPACE_YM

#endif // YM_YM_LOGIC_H
