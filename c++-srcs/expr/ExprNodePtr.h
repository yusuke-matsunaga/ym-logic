#ifndef EXPRNODEPTR_H
#define EXPRNODEPTR_H

/// @file ExprNodePtr.h
/// @brief ExprNodePtr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/Expr.h"


BEGIN_NAMESPACE_YM_LOGIC

//////////////////////////////////////////////////////////////////////
/// @class ExprNodePtr ExprNode.h "ExprNode.h*
/// @brief ExprNode 用のスマートポインタ
//////////////////////////////////////////////////////////////////////
class ExprNodePtr
{
  friend class ExprNodePtrList;

public:

  /// @brief コンストラクタ
  ExprNodePtr(
    const ExprNode* ptr = nullptr ///< [in] Cポインタ
  ) : mPtr{ptr}
  {
    if ( mPtr ) {
      mPtr->inc_ref();
    }
  }

  /// @brief コピーコンストラクタ
  ExprNodePtr(
    const ExprNodePtr& src ///< [in] コピー元のオブジェクト
  ) : mPtr{src.mPtr}
  {
    if ( mPtr ) {
      mPtr->inc_ref();
    }
  }

  /// @brief 代入演算子
  ExprNodePtr&
  operator=(
    const ExprNodePtr& src ///< [in] 代入元のオブジェクト
  )
  {
    if ( src.mPtr ) {
      src.mPtr->inc_ref();
    }
    if ( mPtr ) {
      mPtr->dec_ref();
    }
    mPtr = src.mPtr;
    return *this;
  }

  /// @brief デストラクタ
  ~ExprNodePtr()
  {
    if ( mPtr ) {
      mPtr->dec_ref();
    }
  }


public:

  /// @brief 中身を取り出す演算子
  operator const ExprNode*() const
  {
    return mPtr;
  }

  /// @brief 中身を取り出す演算子
  const ExprNode*
  operator->() const
  {
    return mPtr;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際のポインタ
  const ExprNode* mPtr;

};

typedef vector<ExprNodePtr> ExprNodeList;

END_NAMESPACE_YM_LOGIC

#endif // EXPRNODEPTR_H
