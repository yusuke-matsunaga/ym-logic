#ifndef POLCONF_H
#define POLCONF_H

/// @file PolConf.h
/// @brief PolConf のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017 Yusuke Matsunaga
/// All rights reserved.


#include "ym/logic.h"


BEGIN_NAMESPACE_YM_LOGIC

//////////////////////////////////////////////////////////////////////
/// @class PolConf PolConf.h "PolConf.h"
/// @brief 極性の割当を表すクラス
///
/// 入力と出力の極性の情報をビットベクタの形で持つ．
/// 1の立っているビットは極性の反転を表す．
//////////////////////////////////////////////////////////////////////
class PolConf
{
public:

  /// @brief 空のコンストラクタ
  ///
  /// 反転なしの割当を表す．
  PolConf()
  {
    mBits = 0U;
  }

  /// @brief 内容を指定したコンストラクタ
  PolConf(
    bool oinv,              ///< [in] 出力を反転させる時 true にするフラグ
    std::uint32_t iinv_bits ///< [in] 入力の反転フラグを表すビットベクタ
  )
  {
    mBits = static_cast<std::uint32_t>(oinv) | (iinv_bits << 1);
  }

  /// @brief デストラクタ
  ~PolConf() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力の反転フラグを返す．
  bool
  oinv() const
  {
    return static_cast<bool>(mBits & 1U);
  }

  /// @brief 入力の反転ビットベクタを返す．
  std::uint32_t
  iinv_bits() const
  {
    return mBits >> 1;
  }

  /// @brief 入力の反転フラグを返す．
  bool
  iinv(
    SizeType pos ///< [in] 入力番号
  ) const
  {
    return static_cast<bool>((mBits >> (pos + 1)) & 1U);
  }

  /// @brief NpnMap に変換する．
  NpnMap
  to_npnmap(
    SizeType ni ///< [in] 入力数
  ) const;


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
  std::uint32_t mBits;

};

/// @relates PolConf
/// @brief PolConf の内容を出力する．
void
print_polconf(
  ostream& s,             ///< [in] 出力先のストリーム
  const PolConf& polconf, ///< [in] PolConf
  SizeType ni             ///< [in] 入力数
);

/// @relates PolConf
/// @brief PolConf リストのの内容を出力する．
void
print_polconf_list(
  ostream& s,                          ///< [in] 出力先のストリーム
  const vector<PolConf>& polconf_list, ///< [in] PolConf のリスト
  SizeType ni                          ///< [in] 入力数
);

END_NAMESPACE_YM_LOGIC


#endif // POLCONF_H
