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
  /// @brief 入力数のみ指定したコンストラクタ
  /// @param[in] ni 入力数
  ///
  /// 中身は恒偽関数
  explicit
  TvFunc(int ni = 0);

  /// @brief 入力数と真理値を指定したコンストラクタ
  /// @param[in] ni 入力数
  /// @param[in] values 真理値のベクタ
  ///
  /// values のサイズは 2^ni に等しくなければならない．
  TvFunc(int ni,
	 const vector<int>& values);

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のソースオブジェクト
  TvFunc(const TvFunc& src);

  /// @brief ムーブコンストラクタ
  /// @param[in] src ムーブ元のソースオブジェクト
  TvFunc(TvFunc&& src);

  /// @brief コピー代入演算子
  /// @param[in] src コピー元のソースオブジェクト
  /// @return 自分自身への参照を返す．
  TvFunc&
  operator=(const TvFunc& src);

  /// @brief ムーブ代入演算子
  /// @param[in] src コピー元のソースオブジェクト
  /// @return 自分自身への参照を返す．
  TvFunc&
  operator=(TvFunc&& src);

  /// @brief デストラクタ
  ~TvFunc();


public:
  //////////////////////////////////////////////////////////////////////
  // オブジェクト生成用のクラスメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief 恒偽関数を作る．
  /// @param[in] ni 入力数
  /// @return 生成したオブジェクトを返す．
  static
  TvFunc
  make_zero(int ni);

  /// @brief 恒真関数を作る．
  /// @param[in] ni 入力数
  /// @return 生成したオブジェクトを返す．
  static
  TvFunc
  make_one(int ni);

  /// @brief リテラル関数を作る．
  /// @param[in] ni 入力数
  /// @param[in] varid リテラルの変数番号
  /// @param[in] inv 極性
  ///                - false: 反転なし (正極性)
  ///                - true:  反転あり (負極性)
  /// @return 生成したオブジェクト
  static
  TvFunc
  make_literal(int ni,
	       VarId varid,
	       bool inv);

  /// @brief リテラル関数を作る．
  /// @param[in] ni 入力数
  /// @param[in] lit リテラル
  /// @return 生成したオブジェクト
  static
  TvFunc
  make_literal(int ni,
	       Literal lit);

  /// @brief 肯定のリテラル関数を作る．
  /// @param[in] ni 入力数
  /// @param[in] varid リテラルの変数番号
  /// @return 生成したオブジェクトを返す．
  static
  TvFunc
  make_posi_literal(int ni,
		    VarId varid);

  /// @brief 否定のリテラル関数を作る．
  /// @param[in] ni 入力数
  /// @param[in] varid リテラルの変数番号
  /// @return 生成したオブジェクトを返す．
  static
  TvFunc
  make_nega_literal(int ni,
		    VarId varid);


public:
  //////////////////////////////////////////////////////////////////////
  // 論理演算
  //////////////////////////////////////////////////////////////////////

  /// @brief 否定した関数を返す．
  TvFunc
  invert() const;

  /// @brief 論理積を返す．
  /// @param[in] right オペランド
  TvFunc
  and_op(const TvFunc& right) const;

  /// @brief 論理和を返す．
  /// @param[in] right オペランド
  TvFunc
  or_op(const TvFunc& right) const;

  /// @brief 排他的論理和を返す．
  /// @param[in] right オペランド
  TvFunc
  xor_op(const TvFunc& right) const;


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
  /// @param[in] src1 演算対象のオブジェクト
  /// @return 自身への参照を返す．
  TvFunc&
  and_int(const TvFunc& src1);

  /// @brief src1 との論理和を計算し自分に代入する．
  /// @param[in] src1 演算対象のオブジェクト
  /// @return 自身への参照を返す．
  TvFunc&
  or_int(const TvFunc& src1);

  /// @brief src1 との排他的論理和を計算し自分に代入する．
  /// @param[in] src1 演算対象のオブジェクト
  /// @return 自身への参照を返す．
  TvFunc&
  xor_int(const TvFunc& src1);

  /// @brief コファクターを計算し自分に代入する．
  /// @param[in] varid 変数番号
  /// @param[in] inv 極性
  ///                - false: 反転なし (正極性)
  ///                - true:  反転あり (負極性)
  /// @return 自身への参照を返す．
  TvFunc&
  cofactor_int(VarId varid,
	       bool inv);


public:
  //////////////////////////////////////////////////////////////////////
  // intern 演算のオーバーロード定義
  //////////////////////////////////////////////////////////////////////

  /// @brief src1 との論理積を計算し自分に代入する．
  /// @param[in] src1 演算対象のオブジェクト
  /// @return 自身への参照を返す．
  TvFunc&
  operator&=(const TvFunc& src1);

  /// @brief src1 との論理和を計算し自分に代入する．
  /// @param[in] src1 演算対象のオブジェクト
  /// @return 自身への参照を返す．
  TvFunc&
  operator|=(const TvFunc& src1);

  /// @brief src1 との排他的論理和を計算し自分に代入する．
  /// @param[in] src1 演算対象のオブジェクト
  /// @return 自身への参照を返す．
  TvFunc&
  operator^=(const TvFunc& src1);


public:
  //////////////////////////////////////////////////////////////////////
  // 情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力数を得る．
  int
  input_num() const;

  /// @brief 入力値を2進数と見なしたときの pos 番目の値を得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < 2^(input_num()) )
  /// 答は 0 か 1 だが int 型
  int
  value(int pos) const;

  /// @brief 0 の数を数える．
  int
  count_zero() const;

  /// @brief 1 の数を数える．
  int
  count_one() const;

  /// @brief 0次の Walsh 係数を求める．
  int
  walsh_0() const;

  /// @brief 1次の Walsh 係数を求める．
  /// @param[in] varid 変数番号
  int
  walsh_1(VarId varid) const;

  /// @brief 2次の Walsh 係数を求める．
  /// @param[in] var1, var2 変数番号
  int
  walsh_2(VarId var1,
	  VarId var2) const;

  /// @brief 0次と 1次の Walsh 係数を求める．
  /// @param[out] vec 値を格納する配列
  int
  walsh_01(int vec[]) const;

  /// @brief 0次と 1次と 2次の Walsh 係数を求める．
  /// @param[out] vec1, vec2 値を格納する配列
  int
  walsh_012(int vec1[],
	    int vec2[]) const;

  /// @brief 重み別の 0 次の Walsh 係数を求める．
  /// @param[in] w
  /// @param[in] oinv
  /// @param[in] ibits
  int
  walsh_w0(int w,
	   bool oinv,
	   int ibits) const;

  /// @brief 重み別の 1 次の Walsh 係数を求める．
  int
  walsh_w1(VarId i,
	   int w,
	   bool oinv,
	   int ibits) const;

  /// @brief pos 番目の変数がサポートの時 true を返す．
  /// @param[in] pos 変数番号
  bool
  check_sup(VarId pos) const;

  /// @brief pos1 番目と pos2 番目の変数が対称のとき true を返す．
  /// @param[in] pos1, pos2 変数番号
  /// @param[in] inv 極性
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
  // 演算
  // 多くは演算子のオーバーロードになっているのでここは少ない．
  //////////////////////////////////////////////////////////////////////

  /// @brief コファクターを返す．
  /// @param[in] varid 変数番号
  /// @param[in] inv 極性
  ///                - false: 反転なし (正極性)
  ///                - true:  反転あり (負極性)
  TvFunc
  cofactor(VarId varid,
	   bool inv) const;

  /// @brief npnmap に従った変換を行う．
  /// @param[in] npnmap 変換マップ
  /// @return 変換した関数を返す．
  TvFunc
  xform(const NpnMap& npnmap) const;

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
  /// @param[out] map_list 変換を格納するリスト
  void
  npn_cannonical_all_map(vector<NpnMap>& map_list) const;


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
  compare(const TvFunc& left,
	  const TvFunc& right);

  friend
  bool
  operator&&(const TvFunc& left,
	     const TvFunc& right);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で使われる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 恒真関数を作るコンストラクタ
  /// @param[in] ni 入力数
  /// @param[in] dummy ダミー
  /// 2番目の引数はダミー
  TvFunc(int ni,
	 int dummy);

  /// @brief リテラル関数を作るコンストラクタ
  /// @param[in] ni 入力数
  /// @param[in] varid 変数番号
  /// @param[in] inv 極性
  ///                - false: 反転なし (正極性)
  ///                - true:  反転あり (負極性)
  TvFunc(int ni,
	 VarId varid,
	 bool inv);

  /// @brief コファクターマスクを得る．
  /// @param[in] pos 入力番号 ( 0 <= pos <= 5 )
  static
  WordType
  c_mask(int pos);

  /// @brief リテラル関数のビットパタンを得る
  /// @param[in] var_id 変数番号
  /// @param[in] block_id ブロック番号
  static
  WordType
  lit_pat(int var_id,
	  int block_id);

  /// @brief 入力数 ni のベクタを納めるのに必要なブロック数を計算する．
  /// @param[in] ni 入力数
  static
  int
  nblock(int ni);

  /// @brief pos 番目の要素のブロック位置を計算する．
  /// @param[in] pos 位置番号
  static
  int
  block(int pos);

  /// @brief pos 番目の要素のシフト量を計算する．
  /// @param[in] pos 位置番号
  static
  int
  shift(int pos);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力数
  int mInputNum;

  // ブロック数
  int mBlockNum;

  // パックされた真理値ベクトル
  WordType* mVector;

};


//////////////////////////////////////////////////////////////////////
// TvFunc の演算子
//////////////////////////////////////////////////////////////////////

/// @relates TvFunc
/// @brief 否定を求める．
/// @param[in] src オペランド
TvFunc
operator~(const TvFunc& src);

/// @relates TvFunc
/// @brief 否定を求める．
/// @param[in] src オペランド
TvFunc
operator~(const TvFunc&& src);

/// @relates TvFunc
/// @brief 論理積を求める．
/// @param[in] left, right オペランド
TvFunc
operator&(const TvFunc& left,
	  const TvFunc& right);

/// @relates TvFunc
/// @brief 論理積を求める．
/// @param[in] left, right オペランド
TvFunc
operator&(const TvFunc&& left,
	  const TvFunc& right);

/// @relates TvFunc
/// @brief 論理積を求める．
/// @param[in] left, right オペランド
TvFunc
operator&(const TvFunc& left,
	  const TvFunc&& right);

/// @relates TvFunc
/// @brief 論理積を求める．
/// @param[in] left, right オペランド
TvFunc
operator&(const TvFunc&& left,
	  const TvFunc&& right);

/// @relates TvFunc
/// @brief 論理和を求める．
/// @param[in] left, right オペランド
TvFunc
operator|(const TvFunc& left,
	  const TvFunc& right);

/// @relates TvFunc
/// @brief 論理和を求める．
/// @param[in] left, right オペランド
TvFunc
operator|(const TvFunc&& left,
	  const TvFunc& right);

/// @relates TvFunc
/// @brief 論理和を求める．
/// @param[in] left, right オペランド
TvFunc
operator|(const TvFunc& left,
	  const TvFunc&& right);

/// @relates TvFunc
/// @brief 論理和を求める．
/// @param[in] left, right オペランド
TvFunc
operator|(const TvFunc&& left,
	  const TvFunc&& right);

/// @relates TvFunc
/// @brief 排他的論理和を求める．
/// @param[in] left, right オペランド
TvFunc
operator^(const TvFunc& left,
	  const TvFunc& right);

/// @relates TvFunc
/// @brief 排他的論理和を求める．
/// @param[in] left, right オペランド
TvFunc
operator^(const TvFunc&& left,
	  const TvFunc& right);

/// @relates TvFunc
/// @brief 排他的論理和を求める．
/// @param[in] left, right オペランド
TvFunc
operator^(const TvFunc& left,
	  const TvFunc&& right);

/// @relates TvFunc
/// @brief 排他的論理和を求める．
/// @param[in] left, right オペランド
TvFunc
operator^(const TvFunc&& left,
	  const TvFunc&& right);

/// @relates TvFunc
/// @brief 比較関数
/// @param[in] left, right オペランド
/// @retval -1 left < right
/// @retval  0 left = right
/// @retval  1 left > right
/// @note 入力数の異なる関数間の比較はまず入力数で比較する．
int
compare(const TvFunc& left,
	const TvFunc& right);

/// @relates TvFunc
/// @brief 等価比較
/// @param[in] left, right オペランド
/// @note 入力数の異なる関数間の比較は false となる．
bool
operator==(const TvFunc& left,
	   const TvFunc& right);

/// @relates TvFunc
/// @brief 非等価比較
/// @param[in] left, right オペランド
/// @note 入力数の異なる関数間の比較は true となる．
bool
operator!=(const TvFunc& left,
	   const TvFunc& right);

/// @relates TvFunc
/// @brief 大小比較(小なり)
/// @param[in] left, right オペランド
/// @note 入力数の異なる関数間の比較はまず入力数で比較する．
bool
operator<(const TvFunc& left,
	  const TvFunc& right);

/// @relates TvFunc
/// @brief 大小比較(大なり)
/// @param[in] left, right オペランド
/// @note 入力数の異なる関数間の比較はまず入力数で比較する．
bool
operator>(const TvFunc& left,
	  const TvFunc& right);

/// @relates TvFunc
/// @brief 大小比較(小なりイコール)
/// @param[in] left, right オペランド
/// @note 入力数の異なる関数間の比較はまず入力数で比較する．
bool
operator<=(const TvFunc& left,
	   const TvFunc& right);

/// @relates TvFunc
/// @brief 大小比較(大なりイコール)
/// @param[in] left, right オペランド
/// @note 入力数の異なる関数間の比較はまず入力数で比較する．
bool
operator>=(const TvFunc& left,
	   const TvFunc& right);

/// @relates TvFunc
/// @brief 交差チェック
/// @param[in] left, right オペランド
/// @note 入力数の異なる関数間の比較は false を返す．
bool
operator&&(const TvFunc& left,
	   const TvFunc& right);

/// @relates TvFunc
/// @brief ストリームに対する出力
/// @param[in] s 出力先のストリーム
/// @param[in] func 対象の関数
ostream&
operator<<(ostream& s,
	   const TvFunc& func);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// 恒偽関数を作る．
inline
TvFunc
TvFunc::make_zero(int ni)
{
  return TvFunc(ni);
}

// 恒真関数を作る．
inline
TvFunc
TvFunc::make_one(int ni)
{
  return TvFunc(ni, 0);
}

// @brief リテラル関数を作る．
// @param[in] ni 入力数
// @param[in] varid リテラルの変数番号
// @param[in] inv 極性
//                - false: 反転なし (正極性)
//                - true:  反転あり (負極性)
// @return 生成したオブジェクト
inline
TvFunc
TvFunc::make_literal(int ni,
		     VarId varid,
		     bool inv)
{
  return TvFunc(ni, varid, inv);
}

// @brief リテラル関数を作る．
// @param[in] ni 入力数
// @param[in] lit リテラル
// @return 生成したオブジェクト
inline
TvFunc
TvFunc::make_literal(int ni,
		     Literal lit)
{
  return TvFunc(ni, lit.varid(), lit.is_negative());
}

// 肯定のリテラル関数を作る．
inline
TvFunc
TvFunc::make_posi_literal(int ni,
			  VarId varid)
{
  return TvFunc(ni, varid, false);
}

// 否定のリテラル関数を作る．
inline
TvFunc
TvFunc::make_nega_literal(int ni,
			  VarId varid)
{
  return TvFunc(ni, varid, true);
}

// 入力数を得る．
inline
int
TvFunc::input_num() const
{
  return mInputNum;
}

// 入力値を2進数と見なしたときの pos 番目の値を得る．
// 答は 0 か 1 だが int 型
inline
int
TvFunc::value(int pos) const
{
  return (mVector[block(pos)] >> shift(pos)) & 1;
}

// ブロック数を得る．
inline
int
TvFunc::nblk() const
{
  return mBlockNum;
}

// 生のデータを得る．
inline
TvFunc::WordType
TvFunc::raw_data(int blk) const
{
  return mVector[blk];
}

// 入力数 ni のベクタを納めるのに必要なブロック数を計算する．
inline
int
TvFunc::nblock(int ni)
{
  const int wsize = sizeof(WordType) * 8;
  return ((1 << ni) + wsize - 1) / wsize;
}

// pos 番目の要素のブロック位置を計算する．
inline
int
TvFunc::block(int pos)
{
  const int wsize = sizeof(WordType) * 8;
  return pos / wsize;
}

// pos 番目の要素のシフト量を計算する．
inline
int
TvFunc::shift(int pos)
{
  const int wsize = sizeof(WordType) * 8;
  return pos % wsize;
}

// @brief 否定した関数を返す．
inline
TvFunc
TvFunc::invert() const
{
  return TvFunc(*this).invert_int();
}

// @brief 論理積を返す．
// @param[in] right オペランド
inline
TvFunc
TvFunc::and_op(const TvFunc& right) const
{
  return TvFunc(*this).and_int(right);
}

// @brief 論理和を返す．
// @param[in] right オペランド
inline
TvFunc
TvFunc::or_op(const TvFunc& right) const
{
  return TvFunc(*this).or_int(right);
}

// @brief 排他的論理和を返す．
// @param[in] right オペランド
inline
TvFunc
TvFunc::xor_op(const TvFunc& right) const
{
  return TvFunc(*this).xor_int(right);
}

// @brief src1 との論理積を計算し自分に代入する．
// @param[in] src1 演算対象のオブジェクト
// @return 自身への参照を返す．
inline
TvFunc&
TvFunc::operator&=(const TvFunc& src1)
{
  return and_int(src1);
}

// @brief src1 との論理和を計算し自分に代入する．
// @param[in] src1 演算対象のオブジェクト
// @return 自身への参照を返す．
inline
TvFunc&
TvFunc::operator|=(const TvFunc& src1)
{
  return or_int(src1);
}

// @brief src1 との排他的論理和を計算し自分に代入する．
// @param[in] src1 演算対象のオブジェクト
// @return 自身への参照を返す．
inline
TvFunc&
TvFunc::operator^=(const TvFunc& src1)
{
  return xor_int(src1);
}

// 否定を求める．
inline
TvFunc
operator~(const TvFunc& src)
{
  return TvFunc(src).invert_int();
}

// 否定を求める．
inline
TvFunc
operator~(const TvFunc&& src)
{
  return TvFunc(src).invert_int();
}

// 論理積を求める．
inline
TvFunc
operator&(const TvFunc& left,
	  const TvFunc& right)
{
  return TvFunc(left).and_int(right);
}

// 論理積を求める．
inline
TvFunc
operator&(const TvFunc&& left,
	  const TvFunc& right)
{
  return TvFunc(left).and_int(right);
}

// 論理積を求める．
inline
TvFunc
operator&(const TvFunc& left,
	  const TvFunc&& right)
{
  return TvFunc(right).and_int(left);
}

// 論理積を求める．
inline
TvFunc
operator&(const TvFunc&& left,
	  const TvFunc&& right)
{
  return TvFunc(left).and_int(right);
}

// 論理和を求める．
inline
TvFunc
operator|(const TvFunc& left,
	  const TvFunc& right)
{
  return TvFunc(left).or_int(right);
}

// 論理和を求める．
inline
TvFunc
operator|(const TvFunc&& left,
	  const TvFunc& right)
{
  return TvFunc(left).or_int(right);
}

// 論理和を求める．
inline
TvFunc
operator|(const TvFunc& left,
	  const TvFunc&& right)
{
  return TvFunc(right).or_int(left);
}

// 論理和を求める．
inline
TvFunc
operator|(const TvFunc&& left,
	  const TvFunc&& right)
{
  return TvFunc(left).or_int(right);
}

// 排他的論理和を求める．
inline
TvFunc
operator^(const TvFunc& left,
	  const TvFunc& right)
{
  return TvFunc(left).xor_int(right);
}

// 排他的論理和を求める．
inline
TvFunc
operator^(const TvFunc&& left,
	  const TvFunc& right)
{
  return TvFunc(left).xor_int(right);
}

// 排他的論理和を求める．
inline
TvFunc
operator^(const TvFunc& left,
	  const TvFunc&& right)
{
  return TvFunc(right).xor_int(left);
}

// 排他的論理和を求める．
inline
TvFunc
operator^(const TvFunc&& left,
	  const TvFunc&& right)
{
  return TvFunc(left).xor_int(right);
}

// @brief コファクターを返す．
// @param[in] varid 変数番号
// @param[in] pol 極性
inline
TvFunc
TvFunc::cofactor(VarId varid,
		 bool inv) const
{
  return TvFunc(*this).cofactor_int(varid, inv);
}

/// @brief 等価比較
inline
bool
operator==(const TvFunc& left,
	   const TvFunc& right)
{
  return compare(left, right) == 0;
}

// 等価比較
inline
bool
operator!=(const TvFunc& left,
	   const TvFunc& right)
{
  return !operator==(left, right);
}

// 大小比較のバリエーション
inline
bool
operator<(const TvFunc& left,
	  const TvFunc& right)
{
  return compare(left, right) == -1;
}

inline
bool
operator>(const TvFunc& left,
	  const TvFunc& right)
{
  return operator<(right, left);
}

inline
bool
operator<=(const TvFunc& left,
	   const TvFunc& right)
{
  return !operator<(right, left);
}

inline
bool
operator>=(const TvFunc& left,
	   const TvFunc& right)
{
  return !operator<(left, right);
}

// ストリームに対する出力
inline
ostream&
operator<<(ostream& s,
	   const TvFunc& func)
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
  operator()(const YM_NAMESPACE::TvFunc& f) const
  {
    return f.hash();
  }
};

END_NAMESPACE_STD

#endif // YM_TVFUNC_H
