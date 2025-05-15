#ifndef RWT_H
#define RWT_H

/// @file rwt.h
/// @brief rwt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "magus.h"


#define RWT_NSNAME nsRwt

// 名前空間の定義
#define BEGIN_NAMESPACE_RWT \
BEGIN_NAMESPACE_MAGUS \
BEGIN_NAMESPACE(RWT_NSNAME)

#define END_NAMESPACE_RWT \
END_NAMESPACE(RWT_NSNAME) \
END_NAMESPACE_MAGUS


#endif // RWT_H
