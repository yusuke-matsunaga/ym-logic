#ifndef YM_TVFUNCM_H
#define YM_TVFUNCM_H

/// @file ym/TvFuncM.h
/// @brief TvFuncM のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/TvFunc.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class TvFuncM TvFunc.h "ym/TvFunc.h"
/// @brief 多出力論理関数を表すクラス
//////////////////////////////////////////////////////////////////////
class TvFuncM
{
public:
  using WordType = TvFunc::WordType;

public:
  /// @brief 入力数と出力数を指定したコンストラクタ
  ///
  /// 中身は恒偽関数となる．
  explicit
  TvFuncM(
    SizeType ni = 0, ///< [in] 入力数
    SizeType no = 1  ///< [in] 出力数 ( > 0 )
  );

  /// @brief コピーコンストラクタ
  TvFuncM(
    const TvFuncM& src
  );

  /// @brief ムーブコンストラクタ
  TvFuncM(
    TvFuncM&& src
  );

  /// @brief TvFunc からの変換用コピーコンストラクタ
  explicit
  TvFuncM(
    const TvFunc& src
  );

  /// @brief TvFunc からの変換用ムーブコンストラクタ
  explicit
  TvFuncM(
    TvFunc&& src
  );

  /// @brief TvFunc のリストを用いたコンストラクタ
  /// @param[in] src_list 各出力の論理関数
  ///
  /// src_list の関数の入力数は等しくなければならない．
  TvFuncM(
    const std::vector<TvFunc>& src_list
  );

  /// @brief コピー代入演算子
  TvFuncM&
  operator=(
    const TvFuncM& src
  );

  /// @brief ムーブ代入演算子
  TvFuncM&
  operator=(
    TvFuncM&& src
  );

  /// @brief デストラクタ
  ~TvFuncM();


public:
  //////////////////////////////////////////////////////////////////////
  // 論理演算
  //////////////////////////////////////////////////////////////////////

  /// @brief 否定した関数を返す．
  TvFuncM
  invert() const
  {
    return TvFuncM(*this).invert_int();
  }

  /// @brief 論理積を返す．
  TvFuncM
  and_op(
    const TvFuncM& right ///< [in] オペランド
  ) const
  {
    return TvFuncM(*this).and_int(right);
  }

  /// @brief 論理和を返す．
  TvFuncM
  or_op(
    const TvFuncM& right ///< [in] オペランド
  ) const
  {
    return TvFuncM(*this).or_int(right);
  }

  /// @brief 排他的論理和を返す．
  TvFuncM
  xor_op(
    const TvFuncM& right ///< [in] オペランド
  ) const
  {
    return TvFuncM(*this).xor_int(right);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 論理演算を伴った代入
  //////////////////////////////////////////////////////////////////////

  /// @brief 自分自身を否定する．
  /// @return 自身への参照を返す．
  TvFuncM&
  invert_int();

  /// @brief right との論理積を計算し自分に代入する．
  /// @return 自身への参照を返す．
  TvFuncM&
  and_int(
    const TvFuncM& right ///< [in] 対象のオブジェクト
  );

  /// @brief right との論理和を計算し自分に代入する．
  /// @return 自身への参照を返す．
  TvFuncM&
  or_int(
    const TvFuncM& right ///< [in] 対象のオブジェクト
  );

  /// @brief right との排他的論理和を計算し自分に代入する．
  /// @return 自身への参照を返す．
  TvFuncM&
  xor_int(
    const TvFuncM& right ///< [in] 対象のオブジェクト
  );

  /// @brief コファクターを計算し自分に代入する．
  /// @return 自身への参照を返す．
  TvFuncM&
  cofactor_int(
    SizeType varid, ///< [in] 変数番号
    bool inv        ///< [in] 極性
                    ///<  - false: 反転なし (正極性)
                    ///<  - true:  反転あり (負極性)
  );


public:
  //////////////////////////////////////////////////////////////////////
  // intern 演算のオーバーロード定義
  //////////////////////////////////////////////////////////////////////

  /// @brief right との論理積を計算し自分に代入する．
  /// @return 自身への参照を返す．
  TvFuncM&
  operator&=(
    const TvFuncM& right ///< [in] 対象のオブジェクト
  )
  {
    return and_int(right);
  }

  /// @brief right との論理和を計算し自分に代入する．
  /// @return 自身への参照を返す．
  TvFuncM&
  operator|=(
    const TvFuncM& right ///< [in] 対象のオブジェクト
  )
  {
    return or_int(right);
  }

  /// @brief right との排他的論理和を計算し自分に代入する．
  /// @return 自身への参照を返す．
  TvFuncM&
  operator^=(
    const TvFuncM& right ///< [in] 対象のオブジェクト
  )
  {
    return xor_int(right);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 一つに出力に対する論理演算を伴った代入
  // 対象の出力を ovar で指定する．
  //////////////////////////////////////////////////////////////////////

  /// @brief 自分自身を否定する．
  /// @return 自身への参照を返す．
  TvFuncM&
  invert_int(
    SizeType ovar ///< [in] 出力番号
  );

  /// @brief right との論理積を計算し自分に代入する．
  /// @return 自身への参照を返す．
  TvFuncM&
  and_int(
    SizeType ovar,      ///< [in] 出力番号
    const TvFunc& right ///< [in] 対象のオブジェクト
  );

  /// @brief right との論理和を計算し自分に代入する．
  /// @return 自身への参照を返す．
  TvFuncM&
  or_int(
    SizeType ovar,      ///< [in] 出力番号
    const TvFunc& right ///< [in] 対象のオブジェクト
  );

  /// @brief right との排他的論理和を計算し自分に代入する．
  /// @return 自身への参照を返す．
  TvFuncM&
  xor_int(
    SizeType ovar,      ///< [in] 出力番号
    const TvFunc& right ///< [in] 対象のオブジェクト
  );

  /// @brief コファクターを計算し自分に代入する．
  /// @return 自身への参照を返す．
  TvFuncM&
  cofactor_int(
    SizeType ovar,  ///< [in] 出力番号
    SizeType varid, ///< [in] 変数番号
    bool inv        ///< [in] 極性
		    ///<   - false: 反転なし (正極性)
		    ///<   - true:  反転あり (負極性)
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力数を得る．
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

  /// @brief 1出力の論理関数を切り出す．
  TvFunc
  slice(
    SizeType ovar ///< [in] 出力番号
  ) const;

  /// @brief 入力値を2進数と見なしたときの pos 番目の値を得る．
  /// 答は 0 か 1 だが int 型
  int
  value(
    SizeType ovar, ///< [in] 出力番号
    int pos        ///< [in] 位置番号 ( 0 <= pos < 2^(input_num()) )
  ) const
  {
    SizeType opos = ovar;
    return (mVector[block(pos) + opos * mBlockNum1] >> shift(pos)) & 1;
  }

  /// @brief varid 番目の変数がサポートの時 true を返す．
  bool
  check_sup(
    SizeType varid ///< [in] 変数番号
  ) const;

  /// @brief pos1 番目と pos2 番目の変数が対称のとき true を返す．
  /// @param[in] pos1, pos2 変数番号
  /// @param[in] inv
  bool
  check_sym(
    SizeType var1,   ///< [in] 変数1
    SizeType var2,   ///< [in] 変数2
    bool inv = false ///< [in] 極性
                     ///<  - false: 反転なし (正極性)
                     ///<  - true:  反転あり (負極性)
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
    std::ostream& s, ///< [in] 出力先のストリーム
    int mode = 2     ///< [in] 出力モード
                     ///<  -  2: 2進表記
                     ///<  - 16: 16進表記
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
  // 論理演算
  // 多くは演算子のオーバーロードになっているのでここは少ない．
  //////////////////////////////////////////////////////////////////////

  /// @brief コファクターを返す．
  TvFuncM
  cofactor(
    SizeType varid, ///< [in] 変数番号
    bool inv        ///< [in] 極性
                    ///<  - false: 反転なし (正極性)
                    ///<  - true:  反転あり (負極性)
  ) const
  {
    return TvFuncM(*this).cofactor_int(varid, inv);
  }

  /// @brief npnmap に従った変換を行う．
  /// @return 変換した関数を返す．
  TvFuncM
  xform(
    const NpnMapM& npnmap ///< [in] 変換マップ
  ) const;


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
    const TvFuncM& left,
    const TvFuncM& right
  );

  friend
  bool
  operator&&(
    const TvFuncM& left,
    const TvFuncM& right
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で使われる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力数 ni のベクタを納めるのに必要なブロック数を計算する．
  static
  SizeType
  nblock(
    SizeType ni
  )
  {
    const SizeType wsize = sizeof(WordType) * 8;
    return ((1 << ni) + wsize - 1) / wsize;
  }

  /// @brief ipos 番目の要素のブロック位置を計算する．
  static
  SizeType
  block(
    SizeType pos
  )
  {
    const SizeType wsize = sizeof(WordType) * 8;
    return pos / wsize;
  }

  /// @brief ipos 番目の要素のシフト量を計算する．
  static
  SizeType
  shift(
    SizeType pos
  )
  {
    const SizeType wsize = sizeof(WordType) * 8;
    return pos % wsize;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力数
  SizeType mInputNum;

  // 出力数
  SizeType mOutputNum;

  // 1出力分のブロック数
  SizeType mBlockNum1;

  // ブロック数
  SizeType mBlockNum;

  // パックされた真理値ベクトル
  std::vector<WordType> mVector;

};


//////////////////////////////////////////////////////////////////////
// TvFuncM の演算子
//////////////////////////////////////////////////////////////////////

/// @relates TvFuncM
/// @brief 否定を求める．
inline
TvFuncM
operator~(
  const TvFuncM& src ///< [in] ソースの関数
)
{
  return TvFuncM(src).invert_int();
}

/// @relates TvFuncM
/// @brief 否定を求める．
inline
TvFuncM
operator~(
  const TvFuncM&& src ///< [in] ソースの関数
)
{
  return TvFuncM(src).invert_int();
}

/// @relates TvFuncM
/// @brief 論理積を求める．
inline
TvFuncM
operator&(
  const TvFuncM& left, ///< [in] オペランド1
  const TvFuncM& right ///< [in] オペランド2
)
{
  return TvFuncM(left).and_int(right);
}

/// @relates TvFuncM
/// @brief 論理積を求める．
inline
TvFuncM
operator&(
  const TvFuncM&& left, ///< [in] オペランド1
  const TvFuncM& right  ///< [in] オペランド2
)
{
  return TvFuncM(left).and_int(right);
}

/// @relates TvFuncM
/// @brief 論理積を求める．
inline
TvFuncM
operator&(
  const TvFuncM& left,  ///< [in] オペランド1
  const TvFuncM&& right ///< [in] オペランド2t
)
{
  return TvFuncM(right).and_int(left);
}

/// @relates TvFuncM
/// @brief 論理積を求める．
inline
TvFuncM
operator&(
  const TvFuncM&& left, ///< [in] オペランド1
  const TvFuncM&& right ///< [in] オペランド2
)
{
  return TvFuncM(left).and_int(right);
}

/// @relates TvFuncM
/// @brief 論理和を求める．
inline
TvFuncM
operator|(
  const TvFuncM& left, ///< [in] オペランド1
  const TvFuncM& right ///< [in] オペランド2
)
{
  return TvFuncM(left).or_int(right);
}

/// @relates TvFuncM
/// @brief 論理和を求める．
inline
TvFuncM
operator|(
  const TvFuncM&& left, ///< [in] オペランド1
  const TvFuncM& right  ///< [in] オペランド2
)
{
  return TvFuncM(left).or_int(right);
}

/// @relates TvFuncM
/// @brief 論理和を求める．
inline
TvFuncM
operator|(
  const TvFuncM& left,  ///< [in] オペランド1
  const TvFuncM&& right ///< [in] オペランド2t
)
{
  return TvFuncM(right).or_int(left);
}

/// @relates TvFuncM
/// @brief 論理和を求める．
inline
TvFuncM
operator|(
  const TvFuncM&& left, ///< [in] オペランド1
  const TvFuncM&& right ///< [in] オペランド2
)
{
  return TvFuncM(left).or_int(right);
}

/// @relates TvFuncM
/// @brief 排他的論理和を求める．
inline
TvFuncM
operator^(
  const TvFuncM& left, ///< [in] オペランド1
  const TvFuncM& right ///< [in] オペランド2
)
{
  return TvFuncM(left).xor_int(right);
}

/// @relates TvFuncM
/// @brief 排他的論理和を求める．
inline
TvFuncM
operator^(
  const TvFuncM&& left, ///< [in] オペランド1
  const TvFuncM& right  ///< [in] オペランド2
)
{
  return TvFuncM(left).xor_int(right);
}

/// @relates TvFuncM
/// @brief 排他的論理和を求める．
inline
TvFuncM
operator^(
  const TvFuncM& left,  ///< [in] オペランド1
  const TvFuncM&& right ///< [in] オペランド2
)
{
  return TvFuncM(right).xor_int(left);
}

/// @relates TvFuncM
/// @brief 排他的論理和を求める．
inline
TvFuncM
operator^(
  const TvFuncM&& left, ///< [in] オペランド1
  const TvFuncM&& right ///< [in] オペランド2
)
{
  return TvFuncM(left).xor_int(right);
}

/// @relates TvFunc
/// @brief 比較関数
/// @retval -1 left < right
/// @retval  0 left = right
/// @retval  1 left > right
///
/// 入力数/出力数の異なる関数間の比較はまず入力数つぎに出力数で比較する．
int
compare(
  const TvFuncM& left, ///< [in] オペランド1
  const TvFuncM& right ///< [in] オペランド2
);

/// @relates TvFuncM
/// @brief 等価比較
///
/// 入力数/出力数の異なる関数間の比較は false を返す．
inline
bool
operator==(
  const TvFuncM& left, ///< [in] オペランド1
  const TvFuncM& right ///< [in] オペランド2
)
{
  return compare(left, right) == 0;
}

/// @relates TvFuncM
/// @brief 非等価比較
///
/// 入力数/出力数の異なる関数間の比較は true を返す．
inline
bool
operator!=(
  const TvFuncM& left, ///< [in] オペランド1
  const TvFuncM& right ///< [in] オペランド2
)
{
  return !operator==(left, right);
}

/// @relates TvFuncM
/// @brief 大小比較(小なり)
///
/// 入力数/出力数の異なる関数間の比較はまず入力数つぎに出力数で比較する．
inline
bool
operator<(
  const TvFuncM& left, ///< [in] オペランド1
  const TvFuncM& right ///< [in] オペランド2
)
{
  return compare(left, right) == -1;
}

/// @relates TvFuncM
/// @brief 大小比較(大なり)
///
/// 入力数/出力数の異なる関数間の比較はまず入力数つぎに出力数で比較する．
inline
bool
operator>(
  const TvFuncM& left, ///< [in] オペランド1
  const TvFuncM& right ///< [in] オペランド2
)
{
  return operator<(right, left);
}

/// @relates TvFuncM
/// @brief 大小比較(小なりイコール)
///
/// 入力数/出力数の異なる関数間の比較はまず入力数つぎに出力数で比較する．
inline
bool
operator<=(
  const TvFuncM& left, ///< [in] オペランド1
  const TvFuncM& right ///< [in] オペランド2
)
{
  return !operator<(right, left);
}

/// @relates TvFuncM
/// @brief 大小比較(大なりイコール)
///
/// 入力数/出力数の異なる関数間の比較はまず入力数つぎに出力数で比較する．
inline
bool
operator>=(
  const TvFuncM& left, ///< [in] オペランド1
  const TvFuncM& right ///< [in] オペランド2
)
{
  return !operator<(left, right);
}

/// @relates TvFuncM
/// @brief 交差チェック
///
/// 入力数/出力数の異なる関数間の比較は false を返す．
bool
operator&&(
  const TvFuncM& left, ///< [in] オペランド1
  const TvFuncM& right ///< [in] オペランド2
);

/// @relates TvFuncM
/// @brief ストリームに対する出力
inline
std::ostream&
operator<<(
  std::ostream& s,    ///< [in] 出力先のストリーム
  const TvFuncM& func ///< [in] 対象の関数
)
{
  func.print(s, 2);
  return s;
}

END_NAMESPACE_YM

BEGIN_NAMESPACE_STD

// TvFuncM の等価比較関数クラスの定義
template <>
struct equal_to<YM_NAMESPACE::TvFuncM>
{
  SizeType
  operator()(const YM_NAMESPACE::TvFuncM& f1,
	     const YM_NAMESPACE::TvFuncM& f2) const
  {
    return f1 == f2;
  }
};

// TvFuncM をキーにしたハッシュ関数クラスの定義
template <>
struct hash<YM_NAMESPACE::TvFuncM>
{
  SizeType
  operator()(const YM_NAMESPACE::TvFuncM& f) const
  {
    return f.hash();
  }
};

END_NAMESPACE_STD

#endif // YM_TVFUNCM_H
