#ifndef YM_NPNMAPM_H
#define YM_NPNMAPM_H

/// @file ym/NpnMapM.h
/// @brief NpnMapM のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2016, 2017, 2018, 2019, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/NpnVmap.h"


BEGIN_NAMESPACE_YM_LOGIC

//////////////////////////////////////////////////////////////////////
/// @class NpnMapM NpnMapM.h "ym/NpnMapM.h"
/// @ingroup LogicGroup
/// @brief 他出力のNPN変換の情報を入れるクラス
///
/// @sa NpnVmap
//////////////////////////////////////////////////////////////////////
class NpnMapM
{
public:

  /// @brief 空のコンストラクタ．
  ///
  /// 内容は不定
  NpnMapM() = default;

  /// @brief 入力数と出力数を指定したコンストラクタ
  ///
  /// 各変数の変換内容は不正な値になっている．
  NpnMapM(
    SizeType ni, ///< [in] 入力数
    SizeType no  ///< [in] 出力数
  );

  /// @brief コピーコンストラクタ
  NpnMapM(
    const NpnMapM& src ///< [in] コピー元のオブジェクト
  );

  /// @brief 代入演算子
  /// @return 自分自身への定数参照を返す．
  NpnMapM&
  operator=(
    const NpnMapM& src ///< [in] コピー元のオブジェクト
  );

  /// @brief NpnMap からの変換コンストラクタ
  ///
  /// 出力数が1となる．
  explicit
  NpnMapM(
    const NpnMap& src ///< [in] コピー元のオブジェクト
  );

  /// @brief デストラクタ
  ~NpnMapM();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をクリアする．
  ///
  /// 各変数の変換内容は不正な値になる．
  void
  clear();

  /// @brief 入力数と出力数を再設定する．
  ///
  /// 以前の内容はクリアされる．
  void
  resize(
    SizeType ni, ///< [in] 入力数
    SizeType no  ///< [in] 出力数
  );

  /// @brief 恒等変換を表すように設定する．
  void
  set_identity(
    SizeType ni, ///< [in] 入力数
    SizeType no  ///< [in] 出力数
  );

  /// @brief 入力の変換内容の設定
  void
  set_imap(
    SizeType src_var, ///< [in] 入力番号
    SizeType dst_var, ///< [in] 変換先の入力番号
    bool inv          ///< [in] 極性
                      ///< - false: 反転なし (正極性)
                      ///< - true:  反転あり (負極性)
  )
  {
    set_imap(src_var, NpnVmap(dst_var, inv));
  }

  /// @brief 入力の変換内容の設定
  /// @sa NpnVmap
  void
  set_imap(
    SizeType var, ///< [in] 入力番号
    NpnVmap imap  ///< [in] 変換情報(変換先の入力番号と極性)
  );

  /// @brief 出力の変換内容の設定
  void
  set_omap(
    SizeType src_var, ///< [in] 出力番号
    SizeType dst_var, ///< [in] 変換先の出力番号
    bool inv          ///< [in] 極性
                      ///< - false: 反転なし (正極性)
                      ///< - true:  反転あり (負極性)
  )
  {
    set_omap(src_var, NpnVmap(dst_var, inv));
  }

  /// @brief 出力の変換内容の設定
  /// @sa NpnVmap
  void
  set_omap(
    SizeType var, ///< [in] 出力番号
    NpnVmap omap  ///< [in] 変換情報(変換先の出力番号と極性)
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力数を得る．
  /// @return 入力数
  SizeType
  input_num() const
  {
    return mInputNum;
  }

  /// @brief 出力数を得る．
  SizeType
  output_num() const
  {
    return mOutputNum;
  }

  /// @brief 入力の変換情報の取得
  /// @return var の変換情報
  ///
  /// var に対応するマッピング情報がないときには不正な値を返す．
  /// @sa NpnVmap
  NpnVmap
  imap(
    SizeType var ///< [in] 入力番号
  ) const
  {
    if ( var < input_num() ) {
      return mMapArray[var];
    }
    return NpnVmap::invalid();
  }

  /// @brief 出力の変換情報の取得
  /// @return var の変換情報
  ///
  /// var に対応するマッピング情報がないときには不正な値を返す．
  /// @sa NpnVmap
  NpnVmap
  omap(
    SizeType var ///< [in] 出力番号
  ) const
  {
    if ( var < output_num() ) {
      return mMapArray[var + input_num()];
    }
    return NpnVmap::invalid();
  }

  /// @brief 内容が等しいか調べる．
  /// @return 自分自身と src が等しいときに true を返す．
  bool
  operator==(
    const NpnMapM& src ///< [in] 比較対象のマップ
  ) const;


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
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief コピーする．
  void
  copy(
    const NpnMapM& src ///< [in] コピー元のオブジェクト
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力数
  SizeType mInputNum{0};

  // 出力数
  SizeType mOutputNum{0};

  // 入力と出力のマッピング情報
  NpnVmap* mMapArray{nullptr};

};


//////////////////////////////////////////////////////////////////////
// NpnMapM に関連した関数
//////////////////////////////////////////////////////////////////////

/// @relates NpnMapM
/// @brief 逆写像を求める．
/// @return src の逆写像
///
/// 1対1写像でなければ答えは不定．
NpnMapM
inverse(
  const NpnMapM& src ///< [in] 入力となるマップ
);

/// @relates NpnMapM
/// @brief 合成を求める．
/// @return src1 と src2 を合成したもの
///
/// src1の値域とsrc2の定義域は一致していなければならない．
/// そうでなければ答えは不定．
NpnMapM
operator*(
  const NpnMapM& src1, ///< [in] 左側のオペランド
  const NpnMapM& src2  ///< [in] 右側のオペランド
);

/// @relates NpnMapM
/// @brief 内容を表示する(主にデバッグ用)．
///
/// 改行はしない．
ostream&
operator<<(
  ostream& s,        ///< [in] 出力ストリーム
  const NpnMapM& map ///< [in] 出力対象のマップ
);

END_NAMESPACE_YM_LOGIC

#endif // YM_NPNMAPM_H
