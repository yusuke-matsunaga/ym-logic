
/// @file AigMgrPtr.cc
/// @brief AigMgrPtr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/AigMgrPtr.h"
#include "AigMgrImpl.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
// クラス AigMgrPtr
//////////////////////////////////////////////////////////////////////

// @brief 値を指定したコンストラクタ
AigMgrPtr::AigMgrPtr(
  AigMgrImpl* ptr
) : mPtr{ptr}
{
  if ( mPtr != nullptr ) {
    mPtr->inc();
  }
}


// @brief コピーコンストラクタ
AigMgrPtr::AigMgrPtr(
  const AigMgrPtr& src
) : AigMgrPtr{src.mPtr}
{
}

// @brief デストラクタ
AigMgrPtr::~AigMgrPtr()
{
  if ( mPtr != nullptr ) {
    mPtr->dec();
  }
}

END_NAMESPACE_YM_AIG
