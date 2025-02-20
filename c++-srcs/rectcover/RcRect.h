#ifndef RCRECT_H
#define RCRECT_H

/// @file RcRect.h
/// @brief RcRect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"


BEGIN_NAMESPACE_YM_RC

//////////////////////////////////////////////////////////////////////
/// @class RcRect RcRect.h "RcRect.h"
/// @brief 矩形を表すクラス
///
/// 具体的には 行番号の集合と列番号の集合を持つ．
//////////////////////////////////////////////////////////////////////
class RcRect
{
public:

  /// @brief 空のコンストラクタ
  RcRect() = default;

  /// @brief コンストラクタ
  RcRect(
    const vector<SizeType>& row_list, ///< [in] 行番号のリスト
    const vector<SizeType>& col_list  ///< [in] 列番号のリスト
  ) : mRowList{row_list},
      mColList{col_list}
  {
  }

  /// @brief デストラクタ
  ~RcRect() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 行番号のリストを返す．
  const vector<SizeType>&
  row_list() const
  {
    return mRowList;
  }

  /// @brief 列番号のリストを返す．
  const vector<SizeType>&
  col_list() const
  {
    return mColList;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 行番号のリスト
  vector<SizeType> mRowList;

  // 列番号のリスト
  vector<SizeType> mColList;

};

END_NAMESPACE_YM_RC

#endif // RCRECT_H
