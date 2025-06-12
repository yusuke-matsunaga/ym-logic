#ifndef YM_FRAIG_H
#define YM_FRAIG_H

/// @file ym/fraig.h
/// @brief fraig のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.


//////////////////////////////////////////////////////////////////////
// ym-common のコンフィギュレーションファイル
//////////////////////////////////////////////////////////////////////

#include "ym_config.h"


//////////////////////////////////////////////////////////////////////
// fraig 用の名前空間の定義用マクロ
//////////////////////////////////////////////////////////////////////

/// @brief fraig 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_FRAIG \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsFraig)

/// @brief fraig 用の名前空間の終了
#define END_NAMESPACE_YM_FRAIG \
END_NAMESPACE(nsFraig) \
END_NAMESPACE_YM

/// @namespace nsYm::nsFraig
/// @brief FRAIG 関係のクラスが属する名前空間
///
/// ただし，主要なものは nsYm にインポートされている．
/// 実装用のローカルな要素だけがこの名前空間に存在する．

BEGIN_NAMESPACE_YM_FRAIG

class FraigHandle;
class FraigMgr;

END_NAMESPACE_YM_FRAIG

BEGIN_NAMESPACE_YM

using nsFraig::FraigHandle;
using nsFraig::FraigMgr;

END_NAMESPACE_YM

#endif // YM_FRAIG_H
