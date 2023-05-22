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
  PolConf();

  /// @brief 内容を指定したコンストラクタ
  /// @param[in] oinv 出力を反転させる時 true にするフラグ
  /// @param[in] iinv_bits 入力の反転フラグを表すビットベクタ
  PolConf(bool oinv,
	  std::uint32_t iinv_bits);

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
  std::uint32_t
  iinv_bits() const;

  /// @brief 入力の反転フラグを返す．
  /// @param[in] pos
  bool
  iinv(int pos) const;

  /// @brief NpnMap に変換する．
  /// @param[in] ni 入力数
  NpnMap
  to_npnmap(int ni) const;


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
/// @param[in] s 出力先のストリーム
/// @param[in] polconf PolConf
/// @param[in] ni 入力数
void
print_polconf(ostream& s,
	      const PolConf& polconf,
	      int ni);

/// @relates PolConf
/// @brief PolConf リストのの内容を出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] polconf_list PolConf のリスト
/// @param[in] ni 入力数
void
print_polconf_list(ostream& s,
		   const vector<PolConf>& polconf_list,
		   int ni);


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
		 std::uint32_t iinv_bits)
{
  mBits = static_cast<std::uint32_t>(oinv) | (iinv_bits << 1);
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
std::uint32_t
PolConf::iinv_bits() const
{
  return mBits >> 1;
}

// @brief 入力の反転フラグを返す．
// @param[in] pos
inline
bool
PolConf::iinv(int pos) const
{
  return static_cast<bool>((mBits >> (pos + 1)) & 1U);
}

END_NAMESPACE_YM_LOGIC


#endif // POLCONF_H
