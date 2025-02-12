#ifndef YM_NPNMAP_H
#define YM_NPNMAP_H

/// @file ym/NpnMap.h
/// @brief NpnMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2016, 2017, 2019, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
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
  ///
  /// 当たり前だが定義域と値域のサイズ(入力数)は同じ
  static
  NpnMap
  identity(
    SizeType ni,      ///< [in] 入力数
    bool oinv = false ///< [in] 出力を反転する時 true にするフラグ
  )
  {
    NpnMap ans;
    ans.set_identity(ni, oinv);
    return ans;
  }

  /// @brief 空のコンストラクタ．
  ///
  /// 内容は不定
  NpnMap() = default;

  /// @brief 入力数を指定したコンストラクタ
  ///
  /// 定義域と値域のサイズは同じになる．
  /// 恒等変換になる．
  explicit
  NpnMap(
    SizeType ni ///< [in] 入力数
  );

  /// @brief 写像前と後の入力数を指定したコンストラクタ
  ///
  /// 内容は不定
  NpnMap(
    SizeType ni,  ///< [in] 写像前の入力数
    SizeType ni2  ///< [in] 写像後の入力数
  );

  /// @brief コピーコンストラクタ
  NpnMap(
    const NpnMap& src ///< [in] コピー元のオブジェクト
  );

  /// @brief 代入演算子
  /// @return 自分自身への定数参照を返す．
  NpnMap&
  operator=(
    const NpnMap& src ///< [in] コピー元のオブジェクト
  );

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
  ///
  /// 写像前と写像後の入力数が等しい場合．
  /// 以前の内容はクリアされる．
  void
  resize(
    SizeType ni ///< [in] 入力数
  );

  /// @brief 入力数を再設定する．
  ///
  /// 以前の内容はクリアされる．
  void
  resize(
    SizeType ni, ///< [in] 写像前の入力数
    SizeType ni2 ///< [in] 写像後の入力数
  );

  /// @brief 恒等変換を表すように設定する．
  void
  set_identity(
    SizeType ni,      ///< [in] 入力数
    bool oinv = false ///< [in] 出力を反転する時 true にするフラグ
  );

  /// @brief 入力の変換内容の設定
  void
  set(
    SizeType src_var, ///< [in] 入力変数
    SizeType dst_var, ///< [in] 変換先の入力変数
    bool inv          ///< [in] 極性
                      ///<  - false: 反転なし (正極性)
                      ///<  - true:  反転あり (負極性)
  )
  {
    set(src_var, NpnVmap(dst_var, inv));
  }

  /// @brief 入力の変換内容の設定
  /// @sa NpnVmap
  void
  set(
    SizeType src_var, ///< [in] 入力変数
    NpnVmap imap      ///< [in] 変換情報(変換先の入力番号と極性)
  );

  /// @brief 出力極性を設定する．
  void
  set_oinv(
    bool inv ///< [in] 出力極性
             ///< - false: 反転なし (正極性)
             ///< - true:  反転あり (負極性)
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 写像前の入力数を得る．
  /// @return 入力数
  SizeType
  input_num() const
  {
    return (mNiPol >> 1) & 0xffU;
  }

  /// @brief 写像後の入力数を得る．
  /// @return 入力数
  SizeType
  input_num2() const
  {
    return (mNiPol >> 9) & 0xffU;
  }

  /// @brief 入力の変換情報の取得
  /// @return pos 番目の入力の変換情報
  ///
  /// var に対応するマッピング情報がないときには不正な値を返す．
  /// @sa NpnVmap
  NpnVmap
  imap(
    SizeType var ///< [in] 入力変数
  ) const
  {
    if ( var < input_num() ) {
      return mImap[var];
    }
    else {
      return NpnVmap::invalid();
    }
  }

  /// @brief 出力極性を返す．
  /// @retval false 反転なし
  /// @retval true 反転あり
  bool
  oinv() const
  {
    return static_cast<bool>(mNiPol & 1U);
  }

public:
  //////////////////////////////////////////////////////////////////////
  // NpnMap に関連する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 逆写像を求める．
  ///
  /// 1対1写像でなければ答えは不定．
  NpnMap
  inverse() const;

  /// @brief inverse() の別名
  NpnMap
  operator~() const
  {
    return inverse();
  }

  /// @brief 合成を求める．
  ///
  /// this の値域とsrc2の定義域は一致していな
  /// ければならない．そうでなければ答えは不定．
  NpnMap
  operator*(
    const NpnMap& src2  ///< [in] 右側のオペランド
  ) const;

  /// @brief 内容が等しいか調べる．
  /// @return 自分自身と src が等しいときに true を返す．
  bool
  operator==(
    const NpnMap& src ///< [in] 比較対象のマップ
  ) const;

  /// @return src1 と src2 が等しくないときに true を返す．
  bool
  operator!=(
    const NpnMap& src2  ///< [in] 右側のオペランド
  ) const
  {
    return !operator==(src2);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // バイナリダンプ/リストア関数
  //////////////////////////////////////////////////////////////////////

  /// @brief バイナリストリームに出力する．
  void
  dump(
    ostream& s ///< [in] 出力先のストリーム
  ) const;

  /// @brief バイナリストリームから読み込む．
  void
  restore(
    istream& s ///< [in] 入力元のストリーム
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力数をセットする．
  ///
  /// 出力の極性は変化しない．
  void
  set_ni(
    SizeType ni, ///< [in] 写像前の入力数
    SizeType ni2 ///< [in] 写像後の入力数
  )
  {
    mNiPol &= 1U;
    mNiPol |= (ni << 1) | (ni2 << 9);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力数(8bit x 2) + 出力の極性
  std::uint32_t mNiPol{0};

  // 入力のマッピング情報
  NpnVmap mImap[TvFunc::kMaxNi];

};

/// @relates NpnMap
/// @brief 内容を表示する(主にデバッグ用)．
///
/// 改行はしない．
ostream&
operator<<(
  ostream& s,       ///< [in] 出力ストリーム
  const NpnMap& map ///< [in] 出力対象のマップ
);

END_NAMESPACE_YM_LOGIC

#endif // YM_NPNMAP_H
