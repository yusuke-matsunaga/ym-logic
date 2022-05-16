#ifndef YM_TVFUNC_H
#define YM_TVFUNC_H

/// @file ym/TvFunc.h
/// @brief TvFunc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2016, 2017 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/VarId.h"
#include "ym/Literal.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class TvFunc TvFunc.h "ym/TvFunc.h"
/// @ingroup LogicGroup
/// @brief 真理値表形式で論理関数を表すクラス
///
/// 中身はビットパックされた本当の真理値表なので入力数の指数乗に比例した
/// メモリ容量を必要とする．
/// そのため通常の論理演算よりも代入付き論理演算のほうが効率がよい．
/// さらに，オペランドのどちらかが右辺値の場合には自動的にそのオブジェクト
/// からムーブ演算子で中身を移してそのオブジェクト乗で代入付き論理演算を
/// 行うようになっている．
/// 捨てて良いオブジェクトを用いる場合には move() で右辺値にしておいた
/// ほうが効率がよい．
//////////////////////////////////////////////////////////////////////
class TvFunc
{
  friend class TvFuncM;

public:
  using WordType = ymuint64;

public:

  /// @brief 不正値を作るコンストラクタ
  TvFunc();

  /// @brief 入力数のみ指定したコンストラクタ
  ///
  /// 中身は恒偽関数
  explicit
  TvFunc(
    SizeType ni ///< [in] 入力数
  );

  /// @brief 入力数と真理値を指定したコンストラクタ
  ///
  /// values のサイズは 2^ni に等しくなければならない．
  TvFunc(
    SizeType ni,              ///< [in] 入力数
    const vector<int>& values ///< [in] 真理値のベクタ
  );

  /// @brief コピーコンストラクタ
  TvFunc(
    const TvFunc& src ///< [in] コピー元のソースオブジェクト
  );

  /// @brief ムーブコンストラクタ
  TvFunc(
    TvFunc&& src ///< [in] ムーブ元のソースオブジェクト
  );

  /// @brief コピー代入演算子
  /// @return 自分自身への参照を返す．
  TvFunc&
  operator=(
    const TvFunc& src ///< [in] コピー元のソースオブジェクト
  );

  /// @brief ムーブ代入演算子
  /// @return 自分自身への参照を返す．
  TvFunc&
  operator=(
    TvFunc&& src ///< [in] ムーブ元のソースオブジェクト
  );

  /// @brief デストラクタ
  ~TvFunc();


public:
  //////////////////////////////////////////////////////////////////////
  // オブジェクト生成用のクラスメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief 不正な値を作る．
  static
  TvFunc
  make_invalid()
  {
    return TvFunc{};
  }

  /// @brief 恒偽関数を作る．
  /// @return 生成したオブジェクトを返す．
  static
  TvFunc
  make_zero(
    SizeType ni ///< [in] 入力数
  )
  {
    return TvFunc(ni);
  }

  /// @brief 恒真関数を作る．
  /// @return 生成したオブジェクトを返す．
  static
  TvFunc
  make_one(
    SizeType ni ///< [in] 入力数
  )
  {
    // 2番目の引数はダミー
    return TvFunc(ni, 0);
  }

  /// @brief リテラル関数を作る．
  /// @return 生成したオブジェクト
  static
  TvFunc
  make_literal(
    SizeType ni, ///< [in] 入力数
    VarId varid, ///< [in] リテラルの変数番号
    bool inv     ///< [in] 反転属性
                 ///<   - false: 反転なし (正極性)
                 ///<   - true:  反転あり (負極性)
  )
  {
    return TvFunc(ni, varid, inv);
  }

  /// @brief リテラル関数を作る．
  /// @return 生成したオブジェクト
  static
  TvFunc
  make_literal(
    SizeType ni, ///< [in] 入力数
    Literal lit  ///< [in] リテラル
  )
  {
    return TvFunc(ni, lit.varid(), lit.is_negative());
  }

  /// @brief 肯定のリテラル関数を作る．
  /// @return 生成したオブジェクトを返す．
  static
  TvFunc
  make_posi_literal(
    SizeType ni, ///< [in] 入力数
    VarId varid  ///< [in] リテラルの変数番号
  )
  {
    return TvFunc(ni, varid, false);
  }

  /// @brief 否定のリテラル関数を作る．
  /// @return 生成したオブジェクトを返す．
  static
  TvFunc
  make_nega_literal(
    SizeType ni, ///< [in] 入力数
    VarId varid  ///< [in] リテラルの変数番号
  )
  {
    return TvFunc(ni, varid, true);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 論理演算
  //////////////////////////////////////////////////////////////////////

  /// @brief 否定した関数を返す．
  TvFunc
  invert() const
  {
    return TvFunc(*this).invert_int();
  }

  /// @brief 論理積を返す．
  TvFunc
  and_op(
    const TvFunc& right ///< [in] オペランド
  ) const
  {
    return TvFunc(*this).and_int(right);
  }

  /// @brief 論理和を返す．
  TvFunc
  or_op(
    const TvFunc& right ///< [in] オペランド
  ) const
  {
    return TvFunc(*this).or_int(right);
  }

  /// @brief 排他的論理和を返す．
  TvFunc
  xor_op(
    const TvFunc& right ///< [in] オペランド
  ) const
  {
    return TvFunc(*this).xor_int(right);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 論理演算を伴った代入
  // 俗に言う intern 演算
  //////////////////////////////////////////////////////////////////////

  /// @brief 自分自身を否定する．
  /// @return 自身への参照を返す．
  TvFunc&
  invert_int();

  /// @brief src1 との論理積を計算し自分に代入する．
  /// @return 自身への参照を返す．
  TvFunc&
  and_int(
    const TvFunc& src1 ///< [in] オペランド
  );

  /// @brief src1 との論理和を計算し自分に代入する．
  /// @return 自身への参照を返す．
  TvFunc&
  or_int(
    const TvFunc& src1 ///< [in] オペランド
  );

  /// @brief src1 との排他的論理和を計算し自分に代入する．
  /// @return 自身への参照を返す．
  TvFunc&
  xor_int(
    const TvFunc& src1 ///< [in] オペランド
  );

  /// @brief コファクターを計算し自分に代入する．
  /// @return 自身への参照を返す．
  TvFunc&
  cofactor_int(
    VarId varid, ///< [in] 変数番号
    bool inv     ///< [in] 反転属性
                 ///<  - false: 反転なし (正極性)
                 ///<  - true:  反転あり (負極性)
  );


public:
  //////////////////////////////////////////////////////////////////////
  // intern 演算のオーバーロード定義
  //////////////////////////////////////////////////////////////////////

  /// @brief src1 との論理積を計算し自分に代入する．
  /// @return 自身への参照を返す．
  TvFunc&
  operator&=(
    const TvFunc& src1 ///< [in] オペランド
  )
  {
    return and_int(src1);
  }

  /// @brief src1 との論理和を計算し自分に代入する．
  /// @return 自身への参照を返す．
  TvFunc&
  operator|=(
    const TvFunc& src1 ///< [in] オペランド
  )
  {
    return or_int(src1);
  }

  /// @brief src1 との排他的論理和を計算し自分に代入する．
  /// @return 自身への参照を返す．
  TvFunc&
  operator^=(
    const TvFunc& src1 ///< [in] オペランド
  )
  {
    return xor_int(src1);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 適正な値を持っている時に true を返す．
  bool
  is_valid() const
  {
    return mBlockNum > 0;
  }

  /// @brief 不正値の時に true を返す．
  bool
  is_invalid() const
  {
    return !is_valid();
  }

  /// @brief 入力数を得る．
  SizeType
  input_num() const
  {
    return mInputNum;
  }

  /// @brief 入力値を2進数と見なしたときの pos 番目の値を得る．
  /// 答は 0 か 1 だが int 型
  int
  value(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < 2^(input_num()) )
  ) const
  {
    if ( is_invalid() ) {
      // 例外処理
      return 0;
    }
    return (mVector[block(pos)] >> shift(pos)) & 1;
  }

  /// @brief 0 の数を数える．
  SizeType
  count_zero() const;

  /// @brief 1 の数を数える．
  SizeType
  count_one() const;

  /// @brief 0次の Walsh 係数を求める．
  int
  walsh_0() const;

  /// @brief 1次の Walsh 係数を求める．
  int
  walsh_1(
    VarId varid ///< [in] 変数
  ) const;

  /// @brief 2次の Walsh 係数を求める．
  int
  walsh_2(
    VarId var1, ///< [in] 第1変数
    VarId var2  ///< [in] 第2変数
  ) const;

  /// @brief 0次と 1次の Walsh 係数を求める．
  /// @return 0次の Walsh 係数
  int
  walsh_01(
    int vec[] ///< [out] 1次の Walsh 係数を格納する配列
  ) const;

  /// @brief 0次と 1次と 2次の Walsh 係数を求める．
  /// @return 0次の Walsh 係数
  int
  walsh_012(
    int vec1[], ///< [out] 1次の Walsh 係数を格納する配列
    int vec2[]  ///< [out] 2次の Walsh 係数を格納する配列
  ) const;

  /// @brief 重み別の 0 次の Walsh 係数を求める．
  int
  walsh_w0(
    int w,     ///< [in] 重み
    bool oinv, ///< [in] 出力の反転属性
    int ibits  ///< [in] 入力の反転属性のビットベクタ
  ) const;

  /// @brief 重み別の 1 次の Walsh 係数を求める．
  int
  walsh_w1(
    VarId var, ///< [in] 変数
    int w,     ///< [in] 重み
    bool oinv, ///< [in] 出力の反転属性
    int ibits  ///< [in] 入力の反転属性のビットベクタ
  ) const;

  /// @brief var がサポートの時 true を返す．
  bool
  check_sup(
    VarId var ///< [in] 変数
  ) const;

  /// @brief var1 と var2 が対称のとき true を返す．
  bool
  check_sym(
    VarId var1,      ///< [in] 第1変数
    VarId var2,      ///< [in] 第2変数
    bool inv = false ///< [in] 反転属性
  ) const;

  /// @brief ハッシュ値を返す．
  SizeType
  hash() const;


public:
  //////////////////////////////////////////////////////////////////////
  // デバッグ用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容の出力
  void
  print(
    ostream& s,  ///< [in] 出力先のストリーム
    int mode = 2 ///< [in] 出力モード
                 ///< -  2: 2進モード
                 ///< - 16: 16進モード
  ) const;


public:
  //////////////////////////////////////////////////////////////////////
  // バイナリ入出力用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief バイナリファイルの書き出し
  void
  dump(
    BinEnc& s ///< [in] 出力先のストリーム
  ) const;

  /// @brief バイナリファイルの読み込み
  void
  restore(
    BinDec& s ///< [in] 入力元のストリーム
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 演算
  // 多くは演算子のオーバーロードになっているのでここは少ない．
  //////////////////////////////////////////////////////////////////////

  /// @brief コファクターを返す．
  TvFunc
  cofactor(
    VarId varid, ///< [in] 変数番号
    bool inv     ///< [in] 極性
                 ///<  - false: 反転なし (正極性)
                 ///<  - true:  反転あり (負極性)
  ) const
  {
    return TvFunc(*this).cofactor_int(varid, inv);
  }

  /// @brief npnmap に従った変換を行う．
  /// @return 変換した関数を返す．
  TvFunc
  xform(
    const NpnMap& npnmap ///< [in] 変換マップ
  ) const;

  /// @brief 独立な変数を取り除く変換を返す．
  NpnMap
  shrink_map() const;

  /// @brief npn 変換の正規変換を求める．
  ///
  /// 正規変換とは，変換後の関数が以下の性質を持つもの．
  /// - Walsh の0次の係数が非負である．
  /// - Walsh の1次の係数が非負である．
  /// - 他のNPN同値の関数と比較して(ある基準で)最大となる．
  NpnMap
  npn_cannonical_map() const;

  /// @brief npn 変換の正規変換をすべて求める．
  /// @return 変換を格納するリストを返す．
  vector<NpnMap>
  npn_cannonical_all_map() const;


public:
  //////////////////////////////////////////////////////////////////////
  // その他の実装依存の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ブロック数を得る．
  SizeType
  nblk() const
  {
    return mBlockNum;
  }

  /// @brief 生のデータを得る．
  WordType
  raw_data(
    SizeType blk ///< [in] ブロック番号
  ) const
  {
    return mVector[blk];
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 定数
  //////////////////////////////////////////////////////////////////////

  /// @brief 最大の入力数
  /// 特に根拠はないが，これなら Walsh 係数が 32 ビット整数で収まる．
  /// あと真理値表ベースの手法ではこれくらいが限度
  static
  constexpr SizeType kMaxNi = 20;


public:
  //////////////////////////////////////////////////////////////////////
  // フレンド関数の定義
  // ここの public に意味はない
  //////////////////////////////////////////////////////////////////////

  friend
  int
  compare(
    const TvFunc& left,
    const TvFunc& right
  );

  friend
  bool
  operator&&(
    const TvFunc& left,
    const TvFunc& right
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で使われる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 恒真関数を作るコンストラクタ
  ///
  /// 2番目の引数はダミー
  TvFunc(
    SizeType ni, ///< [in] 入力数
    int dummy    ///< [in] ダミー
  );

  /// @brief リテラル関数を作るコンストラクタ
  /// @param[in] ni
  /// @param[in] varid 変数番号
  /// @param[in] inv 極性
  TvFunc(
    SizeType ni, ///< [in] 入力数
    VarId varid, ///< [in] 変数
    bool inv     ///< [in] 反転属性
                 ///< - false: 反転なし (正極性)
                 ///< - true:  反転あり (負極性)
  );

  /// @brief コファクターマスクを得る．
  static
  WordType
  c_mask(
    SizeType pos ///< [in] 入力番号 ( 0 <= pos <= 5 )
  );

  /// @brief リテラル関数のビットパタンを得る
  static
  WordType
  lit_pat(
    SizeType var_id,  ///< [in] 変数番号
    SizeType block_id ///< [in] ブロック番号
  );

  /// @brief 入力数 ni のベクタを納めるのに必要なブロック数を計算する．
  static
  SizeType
  nblock(
    SizeType ni ///< [in] 入力数
  )
  {
    const int wsize = sizeof(WordType) * 8;
    return ((1 << ni) + wsize - 1) / wsize;
  }

  /// @brief pos 番目の要素のブロック位置を計算する．
  static
  SizeType
  block(
    SizeType pos ///< [in] 位置番号
  )
  {
    const int wsize = sizeof(WordType) * 8;
    return pos / wsize;
  }

  /// @brief pos 番目の要素のシフト量を計算する．
  static
  SizeType
  shift(
    SizeType pos ///< [in] 位置番号
  )
  {
    const int wsize = sizeof(WordType) * 8;
    return pos % wsize;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力数
  SizeType mInputNum;

  // ブロック数
  SizeType mBlockNum;

  // パックされた真理値ベクトル
  WordType* mVector;

};


//////////////////////////////////////////////////////////////////////
// TvFunc の演算子
//////////////////////////////////////////////////////////////////////

/// @relates TvFunc
/// @brief 否定を求める．
inline
TvFunc
operator~(
  const TvFunc& src ///< [in] オペランド
)
{
  return TvFunc(src).invert_int();
}

/// @relates TvFunc
/// @brief 否定を求める．
inline
TvFunc
operator~(
  const TvFunc&& src ///< [in] オペランド
)
{
  return TvFunc(src).invert_int();
}

/// @relates TvFunc
/// @brief 論理積を求める．
inline
TvFunc
operator&(
  const TvFunc& left, ///< [in] 第1オペランド
  const TvFunc& right ///< [in] 第2オペランド
)
{
  return TvFunc(left).and_int(right);
}

/// @relates TvFunc
/// @brief 論理積を求める．
inline
TvFunc
operator&(
  const TvFunc&& left, ///< [in] 第1オペランド
  const TvFunc& right  ///< [in] 第2オペランド
)
{
  return TvFunc(left).and_int(right);
}

/// @relates TvFunc
/// @brief 論理積を求める．
inline
TvFunc
operator&(
  const TvFunc& left,  ///< [in] 第1オペランド
  const TvFunc&& right ///< [in] 第2オペランド
)
{
  return TvFunc(right).and_int(left);
}

/// @relates TvFunc
/// @brief 論理積を求める．
inline
TvFunc
operator&(
  const TvFunc&& left, ///< [in] 第1オペランド
  const TvFunc&& right ///< [in] 第2オペランド
)
{
  return TvFunc(left).and_int(right);
}

/// @relates TvFunc
/// @brief 論理和を求める．
inline
TvFunc
operator|(
  const TvFunc& left, ///< [in] 第1オペランド
  const TvFunc& right ///< [in] 第2オペランド
)
{
  return TvFunc(left).or_int(right);
}

/// @relates TvFunc
/// @brief 論理和を求める．
inline
TvFunc
operator|(
  const TvFunc&& left, ///< [in] 第1オペランド
  const TvFunc& right  ///< [in] 第2オペランド
)
{
  return TvFunc(left).or_int(right);
}

/// @relates TvFunc
/// @brief 論理和を求める．
inline
TvFunc
operator|(
  const TvFunc& left,  ///< [in] 第1オペランド
  const TvFunc&& right ///< [in] 第2オペランド
)
{
  return TvFunc(right).or_int(left);
}

/// @relates TvFunc
/// @brief 論理和を求める．
inline
TvFunc
operator|(
  const TvFunc&& left, ///< [in] 第1オペランド
  const TvFunc&& right ///< [in] 第2オペランド
)
{
  return TvFunc(left).or_int(right);
}

/// @relates TvFunc
/// @brief 排他的論理和を求める．
inline
TvFunc
operator^(
  const TvFunc& left, ///< [in] 第1オペランド
  const TvFunc& right ///< [in] 第2オペランド
)
{
  return TvFunc(left).xor_int(right);
}

/// @relates TvFunc
/// @brief 排他的論理和を求める．
inline
TvFunc
operator^(
  const TvFunc&& left, ///< [in] 第1オペランド
  const TvFunc& right  ///< [in] 第2オペランド
)
{
  return TvFunc(left).xor_int(right);
}

/// @relates TvFunc
/// @brief 排他的論理和を求める．
inline
TvFunc
operator^(
  const TvFunc& left,  ///< [in] 第1オペランド
  const TvFunc&& right ///< [in] 第2オペランド
)
{
  return TvFunc(right).xor_int(left);
}

/// @relates TvFunc
/// @brief 排他的論理和を求める．
inline
TvFunc
operator^(
  const TvFunc&& left, ///< [in] 第1オペランド
  const TvFunc&& right ///< [in] 第2オペランド
)
{
  return TvFunc(left).xor_int(right);
}

/// @relates TvFunc
/// @brief 比較関数
/// @retval -1 left < right
/// @retval  0 left = right
/// @retval  1 left > right
///
/// 入力数の異なる関数間の比較はまず入力数で比較する．
int
compare(
  const TvFunc& left, ///< [in] 第1オペランド
  const TvFunc& right ///< [in] 第2オペランド
);

/// @relates TvFunc
/// @brief 等価比較
///
/// 入力数の異なる関数間の比較は false となる．
inline
bool
operator==(
  const TvFunc& left, ///< [in] 第1オペランド
  const TvFunc& right ///< [in] 第2オペランド
)
{
  return compare(left, right) == 0;
}

/// @relates TvFunc
/// @brief 非等価比較
///
/// 入力数の異なる関数間の比較は true となる．
inline
bool
operator!=(
  const TvFunc& left, ///< [in] 第1オペランド
  const TvFunc& right ///< [in] 第2オペランド
)
{
  return !operator==(left, right);
}

/// @relates TvFunc
/// @brief 大小比較(小なり)
///
/// 入力数の異なる関数間の比較はまず入力数で比較する．
inline
bool
operator<(
  const TvFunc& left, ///< [in] 第1オペランド
  const TvFunc& right ///< [in] 第2オペランド
)
{
  return compare(left, right) == -1;
}

/// @relates TvFunc
/// @brief 大小比較(大なり)
///
/// 入力数の異なる関数間の比較はまず入力数で比較する．
inline
bool
operator>(
  const TvFunc& left, ///< [in] 第1オペランド
  const TvFunc& right ///< [in] 第2オペランド
)
{
  return operator<(right, left);
}

/// @relates TvFunc
/// @brief 大小比較(小なりイコール)
///
/// 入力数の異なる関数間の比較はまず入力数で比較する．
inline
bool
operator<=(
  const TvFunc& left, ///< [in] 第1オペランド
  const TvFunc& right ///< [in] 第2オペランド
)
{
  return !operator<(right, left);
}

/// @relates TvFunc
/// @brief 大小比較(大なりイコール)
///
/// 入力数の異なる関数間の比較はまず入力数で比較する．
inline
bool
operator>=(
  const TvFunc& left, ///< [in] 第1オペランド
  const TvFunc& right ///< [in] 第2オペランド
)
{
  return !operator<(left, right);
}

/// @relates TvFunc
/// @brief 交差チェック
///
/// 入力数の異なる関数間の比較は false を返す．
bool
operator&&(
  const TvFunc& left, ///< [in] 第1オペランド
  const TvFunc& right ///< [in] 第2オペランド
);

/// @relates TvFunc
/// @brief ストリームに対する出力
inline
ostream&
operator<<(
  ostream& s,        ///< [in] 出力先のストリーム
  const TvFunc& func ///< [in] 対象の関数
)
{
  func.print(s, 2);
  return s;
}

END_NAMESPACE_YM

BEGIN_NAMESPACE_STD

// TvFunc をキーにしたハッシュ関数クラスの定義
template <>
struct hash<YM_NAMESPACE::TvFunc>
{
  SizeType
  operator()(
    const YM_NAMESPACE::TvFunc& f
  ) const
  {
    return f.hash();
  }
};

END_NAMESPACE_STD

#endif // YM_TVFUNC_H
