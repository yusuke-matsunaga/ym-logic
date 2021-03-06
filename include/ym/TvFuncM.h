﻿#ifndef YM_TVFUNCM_H
#define YM_TVFUNCM_H

/// @file ym/TvFuncM.h
/// @brief TvFuncM のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2017, 2018 Yusuke Matsunaga
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
  /// @param[in] ni 入力数
  /// @param[in] no 出力数 ( > 0 )
  ///
  /// 中身は恒偽関数となる．
  explicit
  TvFuncM(int ni = 0,
	  int no = 1);

  /// @brief コピーコンストラクタ
  TvFuncM(const TvFuncM& src);

  /// @brief ムーブコンストラクタ
  TvFuncM(TvFuncM&& src);

  /// @brief TvFunc からの変換用コピーコンストラクタ
  explicit
  TvFuncM(const TvFunc& src);

  /// @brief TvFunc からの変換用ムーブコンストラクタ
  explicit
  TvFuncM(TvFunc&& src);

  /// @brief TvFunc のリストを用いたコンストラクタ
  /// @param[in] src_list 各出力の論理関数
  ///
  /// src_list の関数の入力数は等しくなければならない．
  TvFuncM(const vector<TvFunc>& src_list);

  /// @brief TvFunc のリストを用いたコンストラクタ
  /// @param[in] src_list 各出力の論理関数
  ///
  /// src_list の関数の入力数は等しくなければならない．
  TvFuncM(const list<TvFunc>& src_list);

  /// @brief コピー代入演算子
  TvFuncM&
  operator=(const TvFuncM& src);

  /// @brief ムーブ代入演算子
  TvFuncM&
  operator=(TvFuncM&& src);

  /// @brief デストラクタ
  ~TvFuncM();


public:
  //////////////////////////////////////////////////////////////////////
  // 論理演算
  //////////////////////////////////////////////////////////////////////

  /// @brief 否定した関数を返す．
  TvFuncM
  invert() const;

  /// @brief 論理積を返す．
  /// @param[in] right オペランド
  TvFuncM
  and_op(const TvFuncM& right) const;

  /// @brief 論理和を返す．
  /// @param[in] right オペランド
  TvFuncM
  or_op(const TvFuncM& right) const;

  /// @brief 排他的論理和を返す．
  /// @param[in] right オペランド
  TvFuncM
  xor_op(const TvFuncM& right) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 論理演算を伴った代入
  //////////////////////////////////////////////////////////////////////

  /// @brief 自分自身を否定する．
  /// @return 自身への参照を返す．
  TvFuncM&
  invert_int();

  /// @brief right との論理積を計算し自分に代入する．
  /// @param[in] right 論理対象のオブジェクト
  /// @return 自身への参照を返す．
  TvFuncM&
  and_int(const TvFuncM& right);

  /// @brief right との論理和を計算し自分に代入する．
  /// @param[in] right 論理対象のオブジェクト
  /// @return 自身への参照を返す．
  TvFuncM&
  or_int(const TvFuncM& right);

  /// @brief right との排他的論理和を計算し自分に代入する．
  /// @param[in] right 論理対象のオブジェクト
  /// @return 自身への参照を返す．
  TvFuncM&
  xor_int(const TvFuncM& right);

  /// @brief コファクターを計算し自分に代入する．
  /// @param[in] varid 変数番号
  /// @param[in] inv 極性
  ///                - false: 反転なし (正極性)
  ///                - true:  反転あり (負極性)
  /// @return 自身への参照を返す．
  TvFuncM&
  cofactor_int(VarId varid,
	       bool inv);


public:
  //////////////////////////////////////////////////////////////////////
  // intern 演算のオーバーロード定義
  //////////////////////////////////////////////////////////////////////

  /// @brief right との論理積を計算し自分に代入する．
  /// @param[in] right 論理対象のオブジェクト
  /// @return 自身への参照を返す．
  TvFuncM&
  operator&=(const TvFuncM& right);

  /// @brief right との論理和を計算し自分に代入する．
  /// @param[in] right 論理対象のオブジェクト
  /// @return 自身への参照を返す．
  TvFuncM&
  operator|=(const TvFuncM& right);

  /// @brief right との排他的論理和を計算し自分に代入する．
  /// @param[in] right 論理対象のオブジェクト
  /// @return 自身への参照を返す．
  TvFuncM&
  operator^=(const TvFuncM& right);


public:
  //////////////////////////////////////////////////////////////////////
  // 一つに出力に対する論理演算を伴った代入
  // 対象の出力を ovar で指定する．
  //////////////////////////////////////////////////////////////////////

  /// @brief 自分自身を否定する．
  /// @param[in] ovar 出力番号
  /// @return 自身への参照を返す．
  TvFuncM&
  invert_int(VarId ovar);

  /// @brief right との論理積を計算し自分に代入する．
  /// @param[in] ovar 出力番号
  /// @param[in] right 論理対象のオブジェクト
  /// @return 自身への参照を返す．
  TvFuncM&
  and_int(VarId ovar,
	  const TvFunc& right);

  /// @brief right との論理和を計算し自分に代入する．
  /// @param[in] ovar 出力番号
  /// @param[in] right 論理対象のオブジェクト
  /// @return 自身への参照を返す．
  TvFuncM&
  or_int(VarId ovar,
	 const TvFunc& right);

  /// @brief right との排他的論理和を計算し自分に代入する．
  /// @param[in] ovar 出力番号
  /// @param[in] right 論理対象のオブジェクト
  /// @return 自身への参照を返す．
  TvFuncM&
  xor_int(VarId ovar,
	  const TvFunc& right);

  /// @brief コファクターを計算し自分に代入する．
  /// @param[in] ovar 出力番号
  /// @param[in] varid 変数番号
  /// @param[in] inv 極性
  ///                - false: 反転なし (正極性)
  ///                - true:  反転あり (負極性)
  /// @return 自身への参照を返す．
  TvFuncM&
  cofactor_int(VarId ovar,
	       VarId varid,
	       bool inv);


public:
  //////////////////////////////////////////////////////////////////////
  // 情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力数を得る．
  int
  input_num() const;

  /// @brief 出力数を得る．
  int
  output_num() const;

  /// @brief 1出力の論理関数を切り出す．
  /// @param[in] ovar 出力番号
  TvFunc
  slice(VarId ovar) const;

  /// @brief 入力値を2進数と見なしたときの pos 番目の値を得る．
  /// @param[in] ovar 出力番号
  /// @param[in] pos 位置番号 ( 0 <= pos < 2^(input_num()) )
  /// 答は 0 か 1 だが int 型
  int
  value(VarId ovar,
	int pos) const;

  /// @brief varid 番目の変数がサポートの時 true を返す．
  /// @param[in] varid 変数番号
  bool
  check_sup(VarId varid) const;

  /// @brief pos1 番目と pos2 番目の変数が対称のとき true を返す．
  /// @param[in] pos1, pos2 変数番号
  /// @param[in] inv 極性
  ///                - false: 反転なし (正極性)
  ///                - true:  反転あり (負極性)
  bool
  check_sym(VarId pos1,
	    VarId pos2,
	    bool inv = false) const;

  /// @brief ハッシュ値を返す．
  SizeType
  hash() const;


public:
  //////////////////////////////////////////////////////////////////////
  // デバッグ用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容の出力
  /// @param[in] s 出力先のストリーム
  /// @param[in] mode 出力モード
  /// @note mode は 2 か 16
  void
  print(ostream& s,
	int mode = 2) const;


public:
  //////////////////////////////////////////////////////////////////////
  // バイナリ入出力用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief バイナリファイルの書き出し
  /// @param[in] s 出力先のストリーム
  void
  dump(ostream& s) const;

  /// @brief バイナリファイルの読み込み
  /// @param[in] s 入力元のストリーム
  void
  restore(istream& s);


public:
  //////////////////////////////////////////////////////////////////////
  // 論理演算
  // 多くは演算子のオーバーロードになっているのでここは少ない．
  //////////////////////////////////////////////////////////////////////

  /// @brief コファクターを返す．
  /// @param[in] varid 変数番号
  /// @param[in] inv 極性
  ///                - false: 反転なし (正極性)
  ///                - true:  反転あり (負極性)
  TvFuncM
  cofactor(VarId varid,
	   bool inv) const;

  /// @brief npnmap に従った変換を行う．
  /// @param[in] npnmap 変換マップ
  /// @return 変換した関数を返す．
  TvFuncM
  xform(const NpnMapM& npnmap) const;


public:
  //////////////////////////////////////////////////////////////////////
  // その他の実装依存の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ブロック数を得る．
  int
  nblk() const;

  /// @brief 生のデータを得る．
  WordType
  raw_data(int blk) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 定数
  //////////////////////////////////////////////////////////////////////

  /// @brief 最大の入力数
  /// 特に根拠はないが，これなら Walsh 係数が 32 ビット整数で収まる．
  /// あと真理値表ベースの手法ではこれくらいが限度
  static
  constexpr int kMaxNi = 20;


public:
  //////////////////////////////////////////////////////////////////////
  // フレンド関数の定義
  // ここの public に意味はない
  //////////////////////////////////////////////////////////////////////

  friend
  int
  compare(const TvFuncM& left,
	  const TvFuncM& right);

  friend
  bool
  operator&&(const TvFuncM& left,
	     const TvFuncM& right);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で使われる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力数 ni のベクタを納めるのに必要なブロック数を計算する．
  static
  int
  nblock(int ni);

  /// @brief ipos 番目の要素のブロック位置を計算する．
  static
  int
  block(int pos);

  /// @brief ipos 番目の要素のシフト量を計算する．
  static
  int
  shift(int pos);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力数
  int mInputNum;

  // 出力数
  int mOutputNum;

  // 1出力分のブロック数
  int mBlockNum1;

  // ブロック数
  int mBlockNum;

  // パックされた真理値ベクトル
  WordType* mVector;

};


//////////////////////////////////////////////////////////////////////
// TvFuncM の演算子
//////////////////////////////////////////////////////////////////////

/// @relates TvFuncM
/// @brief 否定を求める．
/// @param[in] src ソースの関数
TvFuncM
operator~(const TvFuncM& src);

/// @relates TvFuncM
/// @brief 否定を求める．
/// @param[in] src ソースの関数
TvFuncM
operator~(const TvFuncM&& src);

/// @relates TvFuncM
/// @brief 論理積を求める．
/// @param[in] left, right オペランド
TvFuncM
operator&(const TvFuncM& left,
	  const TvFuncM& right);

/// @relates TvFuncM
/// @brief 論理積を求める．
/// @param[in] left, right オペランド
TvFuncM
operator&(const TvFuncM&& left,
	  const TvFuncM& right);

/// @relates TvFuncM
/// @brief 論理積を求める．
/// @param[in] left, right オペランド
TvFuncM
operator&(const TvFuncM& left,
	  const TvFuncM&& right);

/// @relates TvFuncM
/// @brief 論理積を求める．
/// @param[in] left, right オペランド
TvFuncM
operator&(const TvFuncM&& left,
	  const TvFuncM&& right);

/// @relates TvFuncM
/// @brief 論理和を求める．
/// @param[in] left, right オペランド
TvFuncM
operator|(const TvFuncM& left,
	  const TvFuncM& right);

/// @relates TvFuncM
/// @brief 論理和を求める．
/// @param[in] left, right オペランド
TvFuncM
operator|(const TvFuncM&& left,
	  const TvFuncM& right);

/// @relates TvFuncM
/// @brief 論理和を求める．
/// @param[in] left, right オペランド
TvFuncM
operator|(const TvFuncM& left,
	  const TvFuncM&& right);

/// @relates TvFuncM
/// @brief 論理和を求める．
/// @param[in] left, right オペランド
TvFuncM
operator|(const TvFuncM&& left,
	  const TvFuncM&& right);

/// @relates TvFuncM
/// @brief 排他的論理和を求める．
/// @param[in] left, right オペランド
TvFuncM
operator^(const TvFuncM& left,
	  const TvFuncM& right);

/// @relates TvFuncM
/// @brief 排他的論理和を求める．
/// @param[in] left, right オペランド
TvFuncM
operator^(const TvFuncM&& left,
	  const TvFuncM& right);

/// @relates TvFuncM
/// @brief 排他的論理和を求める．
/// @param[in] left, right オペランド
TvFuncM
operator^(const TvFuncM& left,
	  const TvFuncM&& right);

/// @relates TvFuncM
/// @brief 排他的論理和を求める．
/// @param[in] left, right オペランド
TvFuncM
operator^(const TvFuncM&& left,
	  const TvFuncM&& right);

/// @relates TvFunc
/// @brief 比較関数
/// @param[in] left, right オペランド
/// @retval -1 left < right
/// @retval  0 left = right
/// @retval  1 left > right
/// @note 入力数/出力数の異なる関数間の比較はまず入力数つぎに出力数で比較する．
int
compare(const TvFuncM& left,
	const TvFuncM& right);

/// @relates TvFuncM
/// @brief 等価比較
/// @param[in] left, right オペランド
/// @note 入力数/出力数の異なる関数間の比較は false を返す．
bool
operator==(const TvFuncM& left,
	   const TvFuncM& right);

/// @relates TvFuncM
/// @brief 非等価比較
/// @param[in] left, right オペランド
/// @note 入力数/出力数の異なる関数間の比較は true を返す．
bool
operator!=(const TvFuncM& left,
	   const TvFuncM& right);

/// @relates TvFuncM
/// @brief 大小比較(小なり)
/// @param[in] left, right オペランド
/// @note 入力数/出力数の異なる関数間の比較はまず入力数つぎに出力数で比較する．
bool
operator<(const TvFuncM& left,
	  const TvFuncM& right);

/// @relates TvFuncM
/// @brief 大小比較(大なり)
/// @param[in] left, right オペランド
/// @note 入力数/出力数の異なる関数間の比較はまず入力数つぎに出力数で比較する．
bool
operator>(const TvFuncM& left,
	  const TvFuncM& right);

/// @relates TvFuncM
/// @brief 大小比較(小なりイコール)
/// @param[in] left, right オペランド
/// @note 入力数/出力数の異なる関数間の比較はまず入力数つぎに出力数で比較する．
bool
operator<=(const TvFuncM& left,
	   const TvFuncM& right);

/// @relates TvFuncM
/// @brief 大小比較(大なりイコール)
/// @param[in] left, right オペランド
/// @note 入力数/出力数の異なる関数間の比較はまず入力数つぎに出力数で比較する．
bool
operator>=(const TvFuncM& left,
	   const TvFuncM& right);

/// @relates TvFuncM
/// @brief 交差チェック
/// @param[in] left, right オペランド
/// @note 入力数/出力数の異なる関数間の比較は false を返す．
bool
operator&&(const TvFuncM& left,
	   const TvFuncM& right);

/// @relates TvFuncM
/// @brief ストリームに対する出力
/// @param[in] s 出力先のストリーム
/// @param[in] func 対象の関数
ostream&
operator<<(ostream& s,
	   const TvFuncM& func);



//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// 入力数を得る．
inline
int
TvFuncM::input_num() const
{
  return mInputNum;
}

// @brief 出力数を得る．
inline
int
TvFuncM::output_num() const
{
  return mOutputNum;
}

// 入力値を2進数と見なしたときの (ovar, pos) 番目の値を得る．
// 答は 0 か 1 だが int 型
inline
int
TvFuncM::value(VarId ovar,
	       int pos) const
{
  int opos = ovar.val();
  return (mVector[block(pos) + opos * mBlockNum1] >> shift(pos)) & 1;
}

// ブロック数を得る．
inline
int
TvFuncM::nblk() const
{
  return mBlockNum;
}

// 生のデータを得る．
inline
TvFuncM::WordType
TvFuncM::raw_data(int blk) const
{
  return mVector[blk];
}

// 入力数 ni, 出力数 no のベクタを納めるのに必要なブロック数を計算する．
inline
int
TvFuncM::nblock(int ni)
{
  const int wsize = sizeof(WordType) * 8;
  return ((1 << ni) + wsize - 1) / wsize;
}

// pos 番目の要素のブロック位置を計算する．
inline
int
TvFuncM::block(int pos)
{
  const int wsize = sizeof(WordType) * 8;
  return pos / wsize;
}

// pos 番目の要素のシフト量を計算する．
inline
int
TvFuncM::shift(int pos)
{
  const int wsize = sizeof(WordType) * 8;
  return pos % wsize;
}

// @brief 否定した関数を返す．
inline
TvFuncM
TvFuncM::invert() const
{
  return TvFuncM(*this).invert_int();
}

// @brief 論理積を返す．
// @param[in] right オペランド
inline
TvFuncM
TvFuncM::and_op(const TvFuncM& right) const
{
  return TvFuncM(*this).and_int(right);
}

// @brief 論理和を返す．
// @param[in] right オペランド
inline
TvFuncM
TvFuncM::or_op(const TvFuncM& right) const
{
  return TvFuncM(*this).or_int(right);
}

// @brief 排他的論理和を返す．
// @param[in] right オペランド
inline
TvFuncM
TvFuncM::xor_op(const TvFuncM& right) const
{
  return TvFuncM(*this).xor_int(right);
}

// @brief right との論理積を計算し自分に代入する．
// @param[in] right 論理対象のオブジェクト
// @return 自身への参照を返す．
inline
TvFuncM&
TvFuncM::operator&=(const TvFuncM& right)
{
  return and_int(right);
}

// @brief right との論理和を計算し自分に代入する．
// @param[in] right 論理対象のオブジェクト
// @return 自身への参照を返す．
inline
TvFuncM&
TvFuncM::operator|=(const TvFuncM& right)
{
  return or_int(right);
}

// @brief right との排他的論理和を計算し自分に代入する．
// @param[in] right 論理対象のオブジェクト
// @return 自身への参照を返す．
inline
TvFuncM&
TvFuncM::operator^=(const TvFuncM& right)
{
  return xor_int(right);
}

// 否定を求める．
inline
TvFuncM
operator~(const TvFuncM& src)
{
  return TvFuncM(src).invert_int();
}

// 否定を求める．
inline
TvFuncM
operator~(const TvFuncM&& src)
{
  return TvFuncM(src).invert_int();
}

// 論理積を求める．
inline
TvFuncM
operator&(const TvFuncM& left,
	  const TvFuncM& right)
{
  return TvFuncM(left).and_int(right);
}

// 論理積を求める．
inline
TvFuncM
operator&(const TvFuncM&& left,
	  const TvFuncM& right)
{
  return TvFuncM(left).and_int(right);
}

// 論理積を求める．
inline
TvFuncM
operator&(const TvFuncM& left,
	  const TvFuncM&& right)
{
  return TvFuncM(right).and_int(left);
}

// 論理積を求める．
inline
TvFuncM
operator&(const TvFuncM&& left,
	  const TvFuncM&& right)
{
  return TvFuncM(left).and_int(right);
}

// 論理和を求める．
inline
TvFuncM
operator|(const TvFuncM& left,
	  const TvFuncM& right)
{
  return TvFuncM(left).or_int(right);
}

// 論理和を求める．
inline
TvFuncM
operator|(const TvFuncM&& left,
	  const TvFuncM& right)
{
  return TvFuncM(left).or_int(right);
}

// 論理和を求める．
inline
TvFuncM
operator|(const TvFuncM& left,
	  const TvFuncM&& right)
{
  return TvFuncM(right).or_int(left);
}

// 論理和を求める．
inline
TvFuncM
operator|(const TvFuncM&& left,
	  const TvFuncM&& right)
{
  return TvFuncM(left).or_int(right);
}

// 排他的論理和を求める．
inline
TvFuncM
operator^(const TvFuncM& left,
	  const TvFuncM& right)
{
  return TvFuncM(left).xor_int(right);
}

// 排他的論理和を求める．
inline
TvFuncM
operator^(const TvFuncM&& left,
	  const TvFuncM& right)
{
  return TvFuncM(left).xor_int(right);
}

// 排他的論理和を求める．
inline
TvFuncM
operator^(const TvFuncM& left,
	  const TvFuncM&& right)
{
  return TvFuncM(right).xor_int(left);
}

// 排他的論理和を求める．
inline
TvFuncM
operator^(const TvFuncM&& left,
	  const TvFuncM&& right)
{
  return TvFuncM(left).xor_int(right);
}

// @brief コファクターを返す．
// @param[in] varid 変数番号
// @param[in] inv 極性
//                - false: 反転なし (正極性)
//                - true:  反転あり (負極性)
inline
TvFuncM
TvFuncM::cofactor(VarId varid,
		  bool inv) const
{
  return TvFuncM(*this).cofactor_int(varid, inv);
}

// @relates TvFuncM
// @brief 等価比較
// @param[in] left, right オペランド
// @note 入力数/出力数の異なる関数間の比較は false を返す．
inline
bool
operator==(const TvFuncM& left,
	   const TvFuncM& right)
{
  return compare(left, right) == 0;
}

// 等価比較
inline
bool
operator!=(const TvFuncM& left,
	   const TvFuncM& right)
{
  return !operator==(left, right);
}

// @relates TvFuncM
// @brief 大小比較(小なり)
// @param[in] left, right オペランド
// @note 入力数/出力数の異なる関数間の比較はまず入力数つぎに出力数で比較する．
inline
bool
operator<(const TvFuncM& left,
	  const TvFuncM& right)
{
  return compare(left, right) == -1;
}

inline
bool
operator>(const TvFuncM& left,
	  const TvFuncM& right)
{
  return operator<(right, left);
}

inline
bool
operator<=(const TvFuncM& left,
	   const TvFuncM& right)
{
  return !operator<(right, left);
}

inline
bool
operator>=(const TvFuncM& left,
	   const TvFuncM& right)
{
  return !operator<(left, right);
}

// ストリームに対する出力
inline
ostream&
operator<<(ostream& s,
	   const TvFuncM& func)
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
