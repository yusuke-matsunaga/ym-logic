#ifndef POLCONF_H
#define POLCONF_H

/// @file PolConf.h
/// @brief PolConf のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "ym/ym_logic.h"


BEGIN_NAMESPACE_YM_LOGIC

//////////////////////////////////////////////////////////////////////
/// @class PolConf PolConf.h "PolConf.h"
/// @brief 極性の割当を表すクラス
//////////////////////////////////////////////////////////////////////
class PolConf
{
public:

  /// @brief 空のコンストラクタ
  ///
  /// 反転なしの割当を表す．
  PolConf();

  /// @brief 内容を指定したコンストラクタ
  /// @param[in] oinv 出力を反転させる時 true にするフラグ
  /// @param[in] iinv_bits 入力の反転フラグを表すビットベクタ
  PolConf(bool oinv,
	  ymuint iinv_bits);

  /// @brief デストラクタ
  ~PolConf();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力の反転フラグを返す．
  bool
  oinv() const;

  /// @brief 入力の反転ビットベクタを返す．
  ymuint
  iinv_bits() const;

  /// @brief 入力の反転フラグを返す．
  /// @param[in] pos
  bool
  iinv(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本体
  // 最下位ビットが oinv，残りが iinv_bits となる．
  ymuint mBits;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
//
// 反転なしの割当を表す．
inline
PolConf::PolConf()
{
  mBits = 0U;
}

// @brief 内容を指定したコンストラクタ
// @param[in] oinv 出力を反転させる時 true にするフラグ
// @param[in] iinv_bits 入力の反転フラグを表すビットベクタ
inline
PolConf::PolConf(bool oinv,
		 ymuint iinv_bits)
{
  mBits = static_cast<ymuint>(oinv) | (iinv_bits << 1);
}

// @brief デストラクタ
inline
PolConf::~PolConf()
{
}

// @brief 出力の反転フラグを返す．
inline
bool
PolConf::oinv() const
{
  return static_cast<bool>(mBits & 1U);
}

// @brief 入力の反転ビットベクタを返す．
inline
ymuint
PolConf::iinv_bits() const
{
  return mBits >> 1;
}

// @brief 入力の反転フラグを返す．
// @param[in] pos
inline
bool
PolConf::iinv(ymuint pos) const
{
  return static_cast<bool>((mBits >> (pos + 1)) & 1U);
}

END_NAMESPACE_YM_LOGIC


#endif // POLCONF_H
