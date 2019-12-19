#ifndef YM_NPNMAP_H
#define YM_NPNMAP_H

/// @file ym/NpnMap.h
/// @brief NpnMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2016, 2017, 2019 Yusuke Matsunaga
/// All rights reserved.


#include "ym/logic.h"
#include "ym/VarId.h"
#include "ym/NpnVmap.h"
#include "ym/TvFunc.h"


BEGIN_NAMESPACE_YM_LOGIC

//////////////////////////////////////////////////////////////////////
/// @class NpnMap NpnMap.h "ym/NpnMap.h"
/// @ingroup LogicGroup
/// @brief NPN変換の情報を入れるクラス
///
/// 具体的には
/// - 入力変数の写像
/// - 入力変数の反転属性
/// - 出力変数の反転属性
/// の情報を持つ．入力変数の写像は N -> N の順列となる．
/// @sa NpnVmap
//////////////////////////////////////////////////////////////////////
class NpnMap
{
public:

  /// @brief 恒等変換を返すクラスメソッド
  /// @param[in] ni 入力数
  /// @param[in] oinv 出力を反転する時 true にするフラグ
  ///
  /// 当たり前だが定義域と値域のサイズ(入力数)は同じ
  static
  NpnMap
  identity(int ni,
	   bool oinv = false);

  /// @brief 空のコンストラクタ．
  ///
  /// 内容は不定
  NpnMap() = default;

  /// @brief 入力数を指定したコンストラクタ
  /// @param[in] ni 入力数
  ///
  /// 定義域と値域のサイズは同じになる．
  /// 恒等変換になる．
  explicit
  NpnMap(int ni);

  /// @brief 写像前と後の入力数を指定したコンストラクタ
  /// @param[in] ni 写像前の入力数
  /// @param[in] ni2 写像後の入力数
  ///
  /// 内容は不定
  NpnMap(int ni,
	 int ni2);

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  NpnMap(const NpnMap& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身への定数参照を返す．
  const NpnMap&
  operator=(const NpnMap& src);

  /// @brief デストラクタ
  ~NpnMap() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をクリアする．
  ///
  /// 入力の変換内容は不正な値になる．
  /// 出力の極性は反転なし
  void
  clear();

  /// @brief 入力数を再設定する．
  /// @param[in] ni 入力数
  ///
  /// 写像前と写像後の入力数が等しい場合．
  /// 以前の内容はクリアされる．
  void
  resize(int ni);

  /// @brief 入力数を再設定する．
  /// @param[in] ni 写像前の入力数
  /// @param[in] ni2 写像後の入力数
  ///
  /// 以前の内容はクリアされる．
  void
  resize(int ni,
	 int ni2);

  /// @brief 恒等変換を表すように設定する．
  /// @param[in] ni 入力数
  /// @param[in] oinv 出力を反転する時 true にするフラグ
  void
  set_identity(int ni,
	       bool oinv = false);

  /// @brief 入力の変換内容の設定
  /// @param[in] src_var 入力変数
  /// @param[in] dst_var 変換先の入力変数
  /// @param[in] inv 極性
  ///                - false: 反転なし (正極性)
  ///                - true:  反転あり (負極性)
  void
  set(VarId src_var,
      VarId dst_var,
      bool inv);

  /// @brief 入力の変換内容の設定
  /// @param[in] src_var 入力変数
  /// @param[in] imap 変換情報(変換先の入力番号と極性)
  /// @sa NpnVmap
  void
  set(VarId src_var,
      NpnVmap imap);

  /// @brief 出力極性を設定する．
  /// @param[in] inv 出力極性
  ///                - false: 反転なし (正極性)
  ///                - true:  反転あり (負極性)
  void
  set_oinv(bool inv);


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 写像前の入力数を得る．
  /// @return 入力数
  int
  input_num() const;

  /// @brief 写像後の入力数を得る．
  /// @return 入力数
  int
  input_num2() const;

  /// @brief 入力の変換情報の取得
  /// @param[in] var 入力変数
  /// @return pos 番目の入力の変換情報
  /// @note var に対応するマッピング情報がないときには不正な値を返す．
  /// @sa NpnVmap
  NpnVmap
  imap(VarId var) const;

  /// @brief 出力極性を返す．
  /// @retval false 反転なし
  /// @retval true 反転あり
  bool
  oinv() const;

  /// @brief 内容が等しいか調べる．
  /// @param[in] src 比較対象のマップ
  /// @return 自分自身と src が等しいときに true を返す．
  bool
  operator==(const NpnMap& src) const;


public:
  //////////////////////////////////////////////////////////////////////
  // バイナリダンプ/リストア関数
  //////////////////////////////////////////////////////////////////////

  /// @brief バイナリストリームに出力する．
  void
  dump(ostream& s) const;

  /// @brief バイナリストリームから読み込む．
  void
  restore(istream& s);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力数をセットする．
  /// @param[in] ni 写像前の入力数
  /// @param[in] ni2 写像後の入力数
  ///
  /// 出力の極性は変化しない．
  void
  set_ni(int ni,
	 int ni2);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力数(8bit x 2) + 出力の極性
  ymuint mNiPol{0};

  // 入力のマッピング情報
  NpnVmap mImap[TvFunc::kMaxNi];

};


//////////////////////////////////////////////////////////////////////
// NpnMap に関連する関数
//////////////////////////////////////////////////////////////////////

/// @relates NpnMap
/// @brief 逆写像を求める．
/// @param[in] src 入力となるマップ
/// @return src の逆写像
///
/// 1対1写像でなければ答えは不定．
NpnMap
inverse(const NpnMap& src);

/// @relates NpnMap
/// @brief 合成を求める．
/// @param[in] src1,src2 入力となるマップ
/// @return src1 と src2 を合成したもの
///
/// src1の値域とsrc2の定義域は一致していな
/// ければならない．そうでなければ答えは不定．
NpnMap
operator*(const NpnMap& src1,
	  const NpnMap& src2);

/// @brief 非等価演算子
/// @param[in] src1, src2 オペランド
/// @return src1 と src2 が等しくないときに true を返す．
bool
operator!=(const NpnMap& src1,
	   const NpnMap& src2);

/// @relates NpnMap
/// @brief 内容を表示する(主にデバッグ用)．
/// @param[in] s 出力ストリーム
/// @param[in] map 出力対象のマップ
///
/// 改行はしない．
ostream&
operator<<(ostream& s,
	   const NpnMap& map);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 恒等変換を返すクラスメソッド
// @param[in] ni 入力数
// @param[in] oinv 出力を反転する時 true にするフラグ
//
// 当たり前だが定義域と値域のサイズ(入力数)は同じ
inline
NpnMap
NpnMap::identity(int ni,
		 bool oinv)
{
  NpnMap ans;
  ans.set_identity(ni, oinv);
  return ans;
}

// 入力数を得る．
inline
int
NpnMap::input_num() const
{
  return (mNiPol >> 1) & 0xffU;
}

// @brief 写像後の入力数を得る．
// @return 入力数
inline
int
NpnMap::input_num2() const
{
  return (mNiPol >> 9) & 0xffU;
}

// var に対応するマッピング情報を得る．
inline
NpnVmap
NpnMap::imap(VarId var) const
{
  int idx = var.val();
  if ( idx < input_num() ) {
    return mImap[idx];
  }
  else {
    return NpnVmap::invalid();
  }
}

// 出力極性を返す．
inline
bool
NpnMap::oinv() const
{
  return static_cast<bool>(mNiPol & 1U);
}

// @brief 入力の変換内容の設定
// @param[in] src_var 入力変数
// @param[in] dst_var 変換先の入力変数
// @param[in] inv 極性
inline
void
NpnMap::set(VarId src_var,
	    VarId dst_var,
	    bool inv)
{
  set(src_var, NpnVmap(dst_var, inv));
}

// @brief 入力数をセットする．
// @param[in] ni 写像前の入力数
// @param[in] ni2 写像後の入力数
//
// 出力の極性は変化しない．
inline
void
NpnMap::set_ni(int ni,
	       int ni2)
{
  mNiPol &= 1U;
  mNiPol |= (ni << 1) | (ni2 << 9);
}

// @brief 非等価演算子
// @param[in] src1, src2 オペランド
// @return src1 と src2 が等しくないときに true を返す．
inline
bool
operator!=(const NpnMap& src1,
	   const NpnMap& src2)
{
  return !src1.operator==(src2);
}

END_NAMESPACE_YM_LOGIC

#endif // YM_NPNMAP_H
