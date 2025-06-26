#ifndef YM_AIG_H
#define YM_AIG_H

#error "don't include me"

/// @file ym/aig.h
/// @brief aig のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.


//////////////////////////////////////////////////////////////////////
// ym-common のコンフィギュレーションファイル
//////////////////////////////////////////////////////////////////////

#include "ym_config.h"


//////////////////////////////////////////////////////////////////////
// aig 用の名前空間の定義用マクロ
//////////////////////////////////////////////////////////////////////

/// @brief aig 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_AIG \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsAig)

/// @brief aig 用の名前空間の終了
#define END_NAMESPACE_YM_AIG \
END_NAMESPACE(nsAig) \
END_NAMESPACE_YM

/// @namespace nsYm::nsAig
/// @brief AIG 関係のクラスが属する名前空間
///
/// ただし，主要なものは nsYm にインポートされている．
/// 実装用のローカルな要素だけがこの名前空間に存在する．

BEGIN_NAMESPACE_YM_AIG

class AigHandle;
class AigMgr;
class AigMgrImpl;

END_NAMESPACE_YM_AIG

BEGIN_NAMESPACE_YM

using nsAig::AigHandle;
using nsAig::AigMgr;

// 論理シミュレーション用の型
using AigBitVect = std::uint64_t;

END_NAMESPACE_YM

#endif // YM_AIG_H
