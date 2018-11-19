#ifndef DIVIDE_H
#define DIVIDE_H

/// @file Divide.h
/// @brief Divide のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Alg.h"


BEGIN_NAMESPACE_YM_LOGIC

//////////////////////////////////////////////////////////////////////
/// @class Divide Divide.h "Divide.h"
/// @brief 除算を行う純粋仮想基底クラス
//////////////////////////////////////////////////////////////////////
class Divide
{
public:

  /// @brief デストラクタ
  virtual
  ~Divide() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 除算を行う．
  /// @param[in] f 被除数
  /// @param[in] d 除数
  /// @return 商q と余りr の pair を返す．
  virtual
  pair<AlgCover, AlgCover>
  operator()(const AlgCover& f,
	     const AlgCover& d) const = 0;

};

END_NAMESPACE_YM_LOGIC

#endif // DIVIDE_H
