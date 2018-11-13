#ifndef ALGMGR_H
#define ALGMGR_H

/// @file AlgMgr.h
/// @brief AlgMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/Alg.h"
#include "ym/VarId.h"


BEGIN_NAMESPACE_YM_LOGIC

//////////////////////////////////////////////////////////////////////
/// @class AlgMgr AlgMgr.h "ym/AlgMgr.h"
/// @brief AlgCube, AlgCover を管理するクラス
///
/// といっても実際の役割は入力数を覚えておくこと
//////////////////////////////////////////////////////////////////////
class AlgMgr
{
public:

  /// @brief コンストラクタ
  /// @param[in] variable_num 変数の数
  AlgMgr(int variable_num);

  /// @brief デストラクタ
  ~AlgMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数の数を返す．
  int
  variable_num() const;

  /// @brief キューブ/カバー用の領域を確保する．
  /// @param[in] cube_num キューブ数
  ///
  /// キューブの時は cube_num = 1 とする．
  AlgBitVect*
  new_body(int cube_num = 1);

  /// @brief キューブ/カバー用の領域を削除する．
  /// @param[in] p 領域を指すポインタ
  /// @param[in] cube_num キューブ数
  ///
  /// キューブの時は cube_num = 1 とする．
  void
  delete_body(AlgBitVect* p,
	      int cube_num = 1);


public:
  //////////////////////////////////////////////////////////////////////
  // AlgCube/AlgCover のための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ビットベクタからリテラルを取り出す．
  /// @param[in] bv ビットベクタ
  /// @param[in] cube_id キューブ番号
  /// @param[in] var_id 変数番号 ( 0 <= var_id.val() < variable_num() )
  AlgPol
  get_pol(const AlgBitVect* bv,
	  int cube_id,
	  VarId var_id);

  /// @brief ビットベクタ上のリテラル数を数える．
  /// @param[in] block 対象のブロック
  int
  literal_num(const AlgBlock& block);

  /// @brief ビットベクタ上の特定のリテラルの出現頻度を数える．
  /// @param[in] block 対象のブロック
  /// @param[in] lit 対象のリテラル
  int
  literal_num(const AlgBlock& block,
	      Literal lit);


public:
  //////////////////////////////////////////////////////////////////////
  // カバーを表すビットベクタに対する処理
  // 基本的に最初に結果を格納するビットベクタのポインタを引数にとる．
  // 結果のキューブ数は事前にはわからないので最大のキューブ数を計算し
  // 領域を確保しておく必要がある．
  // オペランドのカバーはビットベクタとキューブ数の組で表す．
  // 結果のキューブ数を関数の戻り値として返す．
  //////////////////////////////////////////////////////////////////////

  /// @brief カバー(を表すビットベクタ)に内容をセットする．
  /// @param[in] dst_bv 対象のビットベクタ
  /// @param[in] cube_list カバーを表すリテラルのリストのリスト
  ///
  /// * リテラルのリストでキューブを表す．
  /// * dst_bv には十分な容量が確保されていると仮定する．
  void
  cover_set(AlgBitVect* dst_bv,
	    const vector<vector<Literal>>& cube_list);

  /// @brief カバー(を表すビットベクタ)に内容をセットする．
  /// @param[in] dst_bv 対象のビットベクタ
  /// @param[in] cube_list カバーを表すリテラルのリストのリスト
  ///
  /// * リテラルのリストでキューブを表す．
  /// * dst_bv には十分な容量が確保されていると仮定する．
  void
  cover_set(AlgBitVect* dst_bv,
	    std::initializer_list<std::initializer_list<Literal>>& cube_list);

  /// @brief 2つのカバーの論理和を計算する．
  /// @param[in] dst_bv 結果を格納するビットベクタ
  /// @param[in] src1 1つめのブロック
  /// @param[in] src2 2つめのブロック
  /// @return 結果のキューブ数を返す．
  ///
  /// * dst_bv には十分な容量があると仮定する．
  /// * dst_bv != src1.bitvect() を仮定する．
  /// * dst_bv != src2.bitvect() を仮定する．
  int
  sum(AlgBitVect* dst_bv,
      const AlgBlock& src1,
      const AlgBlock& src2);

  /// @brief 2つのカバーの差分を計算する．
  /// @param[in] dst_bv 結果を格納するビットベクタ
  /// @param[in] src1 1つめのブロック
  /// @param[in] src2 2つめのブロック
  /// @return 結果のキューブ数を返す．
  ///
  /// * dst_bv には十分な容量があると仮定する．
  /// * dst_bv != src2.bitvect() を仮定する．
  /// * dst_bv == src1.bitvect() でも正しく動く
  int
  diff(AlgBitVect* dst_bv,
       const AlgBlock& src1,
       const AlgBlock& src2);

  /// @brief 2つのカバーの論理積を計算する．
  /// @param[in] dst_bv 結果を格納するビットベクタ
  /// @param[in] src1 1つめのブロック
  /// @param[in] src2 2つめのブロック
  /// @return 結果のキューブ数を返す．
  ///
  /// * dst_bv には十分な容量があると仮定する．
  /// * dst_bv != src2.bitvect() を仮定する．
  /// * src2.cubenum() > 1 の時は dst_bv != src1.bitvect()
  ///   を仮定する．
  /// * src2.cubenum() == 1 の時は dst_bv == src1.bitvect()
  ///   でも正しく動く．
  int
  product(AlgBitVect* dst_bv,
	  const AlgBlock& src1,
	  const AlgBlock& src2);

  /// @brief カバーとリテラルとの論理積を計算する．
  /// @param[in] dst_bv 結果を格納するビットベクタ
  /// @param[in] src1 1つめのブロック
  /// @param[in] lit 対象のリテラル
  /// @return 結果のキューブ数を返す．
  ///
  /// * dst_bv には十分な容量があると仮定する．
  /// * dst_bv == src1.bitvect() でも正しく動く．
  int
  product(AlgBitVect* dst_bv,
	  const AlgBlock& src1,
	  Literal lit);

  /// @brief カバーの代数的除算を行う．
  /// @param[in] dst_bv 結果を格納するビットベクタ
  /// @param[in] src1 1つめのブロック
  /// @param[in] src2 2つめのブロック
  /// @return 結果のキューブ数を返す．
  ///
  /// * dst_bv には十分な容量があると仮定する．
  /// * dst_bv == src1.bitvect() でも正しく動く．
  /// * dst_bv == src2.bitvect() でも正しく動く．
  int
  quotient(AlgBitVect* dst_bv,
	   const AlgBlock& src1,
	   const AlgBlock& src2);

  /// @brief カバーをリテラルで割る．
  /// @param[in] dst_bv 結果を格納するビットベクタ
  /// @param[in] src1 1つめのブロック
  /// @param[in] lit 対象のリテラル
  /// @return 結果のキューブ数を返す．
  ///
  /// * dst_bv には十分な容量があると仮定する．
  /// * dst_bv == src1.bitvect() でも正しく動く．
  int
  quotient(AlgBitVect* dst_bv,
	   const AlgBlock& src1,
	   Literal lit);

  /// @brief カバー中のすべてのキューブの共通部分を求める．
  /// @param[in] dst_bv 結果を格納するビットベクタ
  /// @param[in] src1 1つめのブロック
  ///
  /// * 共通部分がないときは空のキューブとなる．
  /// * dst_bv には十分な容量があると仮定する．
  void
  common_cube(AlgBitVect* dst_bv,
	      const AlgBlock& src1);

  /// @brief カバー(を表すビットベクタ)のコピーを行う．
  /// @param[in] dst_bv コピー先のビットベクタ
  /// @param[in] src_bv ソースのビットベクタ
  /// @param[in] cube_num キューブ数
  ///
  /// * dst_bv には十分な容量があると仮定する．
  /// * dst_bv と src_bv の領域が重なっていたら正しく動かない．
  void
  _copy(AlgBitVect* dst_bv,
	const AlgBitVect* src_bv,
	int cube_num);

  /// @brief カバー(を表すビットベクタ)を整列する．
  /// @param[in] bv ビットベクタ
  /// @param[in] cube_num キューブ数
  void
  sort(AlgBitVect* bv,
       int cube_num);

  /// @brief カバー(を表すビットベクタ)の比較を行う．
  /// @param[in] src1 1つめのブロック
  /// @param[in] src2 2つめのブロック
  /// @retval -1 bv1 <  bv2
  /// @retval  0 bv1 == bv2
  /// @retval  1 bv1 >  bv2
  ///
  /// 比較はキューブごとの辞書式順序で行う．
  int
  compare(const AlgBlock& src1,
	  const AlgBlock& src2);

  /// @brief ビットベクタからハッシュ値を計算する．
  /// @param[in] src 対象のブロック
  SizeType
  hash(const AlgBlock& src);


public:
  //////////////////////////////////////////////////////////////////////
  // キューブに対する処理
  // 基本的にはキューブはビットベクタの先頭アドレスで表す．
  // キューブのサイズ(変数の個数に依存)によってキューブの開始アドレスは変化する．
  //////////////////////////////////////////////////////////////////////

  /// @brief キューブ(を表すビットベクタ)をクリアする．
  /// @param[in] dst_bv 対象のビットベクタ
  void
  cube_clear(AlgBitVect* dst_bv);

  /// @brief キューブ(を表すビットベクタ)をクリアする．
  /// @param[in] dst_bv 対象のビットベクタ
  /// @param[in] dst_offset オフセット
  void
  cube_clear(AlgBitVect* dst_bv,
	     int dst_offset);

  /// @brief キューブ(を表すビットベクタ)をコピーする．
  /// @param[in] dst_bv 対象のビットベクタ
  /// @param[in] src_bv コピー元のビットベクタ
  void
  cube_copy(AlgBitVect* dst_bv,
	    const AlgBitVect* src_bv);

  /// @brief キューブ(を表すビットベクタ)をコピーする．
  /// @param[in] dst_bv 対象のビットベクタ
  /// @param[in] dst_offset dst_bv のオフセット
  /// @param[in] src_bv コピー元のビットベクタ
  /// @param[in] src_offset src_bv のオフセット
  void
  cube_copy(AlgBitVect* dst_bv,
	     int dst_offset,
	     const AlgBitVect* src_bv,
	     int src_offset);

  /// @brief Literal のリストからキューブ(を表すビットベクタ)のコピーを行う．
  /// @param[in] dst_bv 対象のビットベクタ
  /// @param[in] lit_list キューブを表すリテラルのリスト
  ///
  /// * dst_bv には十分な容量があると仮定する．
  void
  cube_set(AlgBitVect* dst_bv,
	   const vector<Literal>& lit_list);

  /// @brief Literal のリストからキューブ(を表すビットベクタ)のコピーを行う．
  /// @param[in] dst_bv 対象のビットベクタ
  /// @param[in] dst_offset オフセット
  /// @param[in] lit_list キューブを表すリテラルのリスト
  ///
  /// * dst_bv には十分な容量があると仮定する．
  void
  cube_set(AlgBitVect* dst_bv,
	   int dst_offset,
	   const vector<Literal>& lit_list);

  /// @brief Literal のリストからキューブ(を表すビットベクタ)のコピーを行う．
  /// @param[in] dst_bv 対象のビットベクタ
  /// @param[in] lit_list キューブを表すリテラルのリスト初期化子
  ///
  /// * dst_bv には十分な容量があると仮定する．
  void
  cube_set(AlgBitVect* dst_bv,
	   std::initializer_list<Literal> lit_list);

  /// @brief Literal のリストからキューブ(を表すビットベクタ)のコピーを行う．
  /// @param[in] dst_bv 対象のビットベクタ
  /// @param[in] dst_offset オフセット
  /// @param[in] lit_list キューブを表すリテラルのリスト初期化子
  ///
  /// * dst_bv には十分な容量があると仮定する．
  void
  cube_set(AlgBitVect* dst_bv,
	   int dst_offset,
	   std::initializer_list<Literal> lit_list);

  /// @brief キューブ(を表すビットベクタ)の比較を行う．
  /// @param[in] bv1 1つめのキューブを表すビットベクタ
  /// @param[in] bv2 2つめのキューブを表すビットベクタ
  /// @retval -1 bv1 <  bv2
  /// @retval  0 bv1 == bv2
  /// @retval  1 bv1 >  bv2
  int
  cube_compare(const AlgBitVect* bv1,
	       const AlgBitVect* bv2);

  /// @brief キューブ(を表すビットベクタ)の比較を行う．
  /// @param[in] bv1 1つめのキューブを表すビットベクタ
  /// @param[in] bv1_offset bv1 のオフセット
  /// @param[in] bv2 2つめのキューブを表すビットベクタ
  /// @param[in] bv2_offset bv2 のオフセット
  /// @retval -1 bv1 <  bv2
  /// @retval  0 bv1 == bv2
  /// @retval  1 bv1 >  bv2
  int
  cube_compare(const AlgBitVect* bv1,
	       int bv1_offset,
	       const AlgBitVect* bv2,
	       int bv2_offset);

  /// @brief 2つのキューブが矛盾していないか調べる．
  /// @param[in] bv1 1つめのキューブを表すビットベクタ
  /// @param[in] bv2 2つめのキューブを表すビットベクタ
  /// @retval true 同じ変数の相反するリテラルがある．
  /// @retval false 衝突していない．
  bool
  cube_check_conflict(const AlgBitVect* bv1,
		      const AlgBitVect* bv2);

  /// @brief 2つのキューブが矛盾していないか調べる．
  /// @param[in] bv1 1つめのキューブを表すビットベクタ
  /// @param[in] bv1_offset bv1 のオフセット
  /// @param[in] bv2 2つめのキューブを表すビットベクタ
  /// @param[in] bv2_offset bv2 のオフセット
  /// @retval true 同じ変数の相反するリテラルがある．
  /// @retval false 衝突していない．
  bool
  cube_check_conflict(const AlgBitVect* bv1,
		      int bv1_offset,
		      const AlgBitVect* bv2,
		      int bv2_offset);

  /// @brief 一方のキューブが他方のキューブに含まれているか調べる．
  /// @param[in] bv1 1つめのキューブを表すビットベクタ
  /// @param[in] bv2 2つめのキューブを表すビットベクタ
  /// @return 1つめのキューブが2つめのキューブ に含まれていたら true を返す．
  bool
  cube_check_containment(const AlgBitVect* bv1,
			 const AlgBitVect* bv2);

  /// @brief 一方のキューブが他方のキューブに含まれているか調べる．
  /// @param[in] bv1 1つめのキューブを表すビットベクタ
  /// @param[in] bv1_offset bv1 のオフセット
  /// @param[in] bv2 2つめのキューブを表すビットベクタ
  /// @param[in] bv2_offset bv2 のオフセット
  /// @return 1つめのキューブが2つめのキューブ に含まれていたら true を返す．
  bool
  cube_check_containment(const AlgBitVect* bv1,
			 int bv1_offset,
			 const AlgBitVect* bv2,
			 int bv2_offset);

  /// @brief ２つのキューブに共通なリテラルがあれば true を返す．
  /// @param[in] bv1 1つめのキューブを表すビットベクタ
  /// @param[in] bv2 2つめのキューブを表すビットベクタ
  /// @return ２つのキューブに共通なリテラルがあれば true を返す．
  bool
  cube_check_intersect(const AlgBitVect* bv1,
		       const AlgBitVect* bv2);

  /// @brief ２つのキューブに共通なリテラルがあれば true を返す．
  /// @param[in] bv1 1つめのキューブを表すビットベクタ
  /// @param[in] bv1_offset bv1 のオフセット
  /// @param[in] bv2 2つめのキューブを表すビットベクタ
  /// @param[in] bv2_offset bv2 のオフセット
  /// @return ２つのキューブに共通なリテラルがあれば true を返す．
  bool
  cube_check_intersect(const AlgBitVect* bv1,
		       int bv1_offset,
		       const AlgBitVect* bv2,
		       int bv2_offset);

  /// @brief 2つのキューブの積を計算する．
  /// @param[in] dst_bv コピー先のビットベクタ
  /// @param[in] bv1 1つめのキューブを表すビットベクタ
  /// @param[in] bv2 2つめのキューブを表すビットベクタ
  /// @retval true 積が空でなかった．
  /// @retval false 積が空だった．
  bool
  cube_product(AlgBitVect* dst_bv,
	       const AlgBitVect* bv1,
	       const AlgBitVect* bv2);

  /// @brief キューブによる商を求める．
  /// @param[in] dst_bv コピー先のビットベクタ
  /// @param[in] bv1 1つめのキューブを表すビットベクタ
  /// @param[in] bv2 2つめのキューブを表すビットベクタ
  /// @return 正しく割ることができたら true を返す．
  bool
  cube_quotient(AlgBitVect* dst_bv,
		const AlgBitVect* bv1,
		const AlgBitVect* bv2);

  /// @brief 2つのキューブ(を表すビットベクタ)を入れ替える．
  /// @param[in] bv1 1つめのキューブを表すビットベクタ
  /// @param[in] bv2 2つめのキューブを表すビットベクタ
  void
  cube_swap(AlgBitVect* bv1,
	    AlgBitVect* bv2);

  /// @brief 3つのキューブ(を表すビットベクタ)を入れ替える．
  /// @param[in] bv1 1つめのキューブを表すビットベクタ
  /// @param[in] bv2 2つめのキューブを表すビットベクタ
  /// @param[in] bv3 3つめのキューブを表すビットベクタ
  ///
  /// bv1 <- bv2, bv2 <- bv3, bv3 <- bv1 となる．
  void
  cube_rotate3(AlgBitVect* bv1,
	       AlgBitVect* bv2,
	       AlgBitVect* bv3);

  /// @brief 4つのキューブ(を表すビットベクタ)を入れ替える．
  /// @param[in] bv1 1つめのキューブを表すビットベクタ
  /// @param[in] bv2 2つめのキューブを表すビットベクタ
  /// @param[in] bv3 3つめのキューブを表すビットベクタ
  /// @param[in] bv4 4つめのキューブを表すビットベクタ
  ///
  /// bv1 <- bv2, bv2 <- bv3, bv3 <- bv4, bv4 <- bv1 となる．
  void
  cube_rotate4(AlgBitVect* bv1,
	       AlgBitVect* bv2,
	       AlgBitVect* bv3,
	       AlgBitVect* bv4);

  /// @brief カバー/キューブの内容を出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] bv カバー/キューブを表すビットベクタ
  /// @param[in] start キューブの開始位置
  /// @param[in] end キューブの終了位置
  /// @param[in] varname_list 変数名のリスト
  ///
  /// end は実際の末尾 + 1 を指す．
  void
  print(ostream& s,
	const AlgBitVect* bv,
	int start,
	int end,
	const vector<string>& varname_list = vector<string>());


public:
  //////////////////////////////////////////////////////////////////////
  // リテラル集合に対する演算
  // 基本キューブと同じだが相反するリテラルがあってもOK
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素のチェック
  /// @param[in] bv ビットベクタ
  /// @param[in] lit 対象のリテラル
  /// @return lit を含んでいたら true を返す．
  bool
  is_in(AlgBitVect* bv,
	Literal lit);

  /// @brief 要素の追加
  /// @param[in] bv ビットベクタ
  /// @param[in] lit 対象のリテラル
  void
  add_lit(AlgBitVect* bv,
	  Literal lit);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief マージソートを行う下請け関数
  /// @param[in] bv 対象のビットベクタ
  /// @param[in] start 開始位置
  /// @param[in] end 終了位置
  ///
  /// bv[start] - bv[end - 1] の領域をソートする．
  void
  _sort(AlgBitVect* bv,
	int start,
	int end);

  /// @brief mTmpBuff を初期化する．
  void
  _init_buff();

  /// @brief mTmpBuff に必要な領域を確保する．
  /// @param[in] req_size 要求するキューブ数
  void
  _resize_buff(int req_size);

  /// @brief キューブ(を表すビットベクタ)をバッファにコピーする．
  /// @param[in] src_bv ソースのビットベクタ
  void
  cube_save(const AlgBitVect* src_bv);

  /// @brief キューブ(を表すビットベクタ)をバッファからコピーする．
  /// @param[in] dst_bv コピー先のビットベクタ
  void
  cube_restore(AlgBitVect* dst_bv);

  /// @brief ブロック位置を計算する．
  /// @param[in] var_id 変数番号
  static
  int
  _block_pos(VarId var_id);

  /// @brief シフト量を計算する．
  /// @param[in] var_id 変数番号
  static
  int
  _shift_num(VarId var_id);

  /// @brief キューブ1つ分のブロックサイズを計算する．
  int
  _cube_size() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数の数
  int mVarNum;

  // 作業用に用いられるビットベクタ
  AlgBitVect* mTmpBuff;

  // mTmpBuff のキューブ数
  int mTmpBuffSize;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 変数の数を返す．
inline
int
AlgMgr::variable_num() const
{
  return mVarNum;
}

// @brief ビットベクタからリテラルを取り出す．
// @param[in] bv ビットベクタ
// @param[in] cube_id キューブ番号
// @param[in] var_id 変数番号 ( 0 <= var_id.val() < variable_num() )
inline
AlgPol
AlgMgr::get_pol(const AlgBitVect* bv,
		int cube_id,
		VarId var_id)
{
  ASSERT_COND( var_id.val() >= 0 && var_id.val() < variable_num() );

  int blk = _block_pos(var_id) + _cube_size() * cube_id;
  int sft = _shift_num(var_id);
  return static_cast<AlgPol>((bv[blk] >> sft) & 3ULL);
}

// @brief カバー(を表すビットベクタ)に内容をセットする．
// @param[in] dst_bv 対象のビットベクタ
// @param[in] cube_list カバーを表すリテラルのリストのリスト
//
// * リテラルのリストでキューブを表す．
// * dst_bv には十分な容量が確保されていると仮定する．
inline
void
AlgMgr::cover_set(AlgBitVect* dst_bv,
		  const vector<vector<Literal>>& cube_list)
{
  AlgBitVect* dst_bv0 = dst_bv;
  for ( auto& lit_list: cube_list ) {
    cube_set(dst_bv, lit_list);
    dst_bv += _cube_size();
  }
  sort(dst_bv0, cube_list.size());
}

// @brief カバー(を表すビットベクタ)に内容をセットする．
// @param[in] dst_bv 対象のビットベクタ
// @param[in] cube_list カバーを表すリテラルのリストのリスト
//
// * リテラルのリストでキューブを表す．
// * dst_bv には十分な容量が確保されていると仮定する．
inline
void
AlgMgr::cover_set(AlgBitVect* dst_bv,
		  std::initializer_list<std::initializer_list<Literal>>& cube_list)
{
  AlgBitVect* dst_bv0 = dst_bv;
  for ( auto lit_list: cube_list ) {
    cube_set(dst_bv, lit_list);
    dst_bv += _cube_size();
  }
  sort(dst_bv0, cube_list.size());
}

// @brief カバー(を表すビットベクタ)を整列する．
// @param[in] cube_num キューブ数
// @param[in] bv ビットベクタ
inline
void
AlgMgr::sort(AlgBitVect* bv,
	     int cube_num)
{
  // 下請け関数を呼ぶだけ
  _sort(bv, 0, cube_num);
}

// @brief キューブ(を表すビットベクタ)をクリアする．
// @param[in] dst_bv 対象のビットベクタ
// @param[in] dst_offset オフセット
inline
void
AlgMgr::cube_clear(AlgBitVect* dst_bv,
		   int dst_offset)
{
  cube_clear(dst_bv + dst_offset * _cube_size());
}

// @brief キューブ(を表すビットベクタ)をコピーする．
// @param[in] dst_bv 対象のビットベクタ
// @param[in] src_bv コピー元のビットベクタ
inline
void
AlgMgr::cube_copy(AlgBitVect* dst_bv,
		  const AlgBitVect* src_bv)
{
  _copy(dst_bv, src_bv, 1);
}

// @brief キューブ(を表すビットベクタ)をコピーする．
// @param[in] dst_bv 対象のビットベクタ
// @param[in] dst_offset dst_bv のオフセット
// @param[in] src_bv コピー元のビットベクタ
// @param[in] src_offset src_bv のオフセット
inline
void
AlgMgr::cube_copy(AlgBitVect* dst_bv,
		  int dst_offset,
		  const AlgBitVect* src_bv,
		  int src_offset)
{
  cube_copy(dst_bv + dst_offset * _cube_size(),
	    src_bv + src_offset * _cube_size());
}

// @brief Literal のリストからキューブ(を表すビットベクタ)のコピーを行う．
// @param[in] dst_bv 対象のビットベクタ
// @param[in] dst_offset オフセット
// @param[in] lit_list キューブを表すリテラルのリスト
//
/// * dst_bv には十分な容量があると仮定する．
inline
void
AlgMgr::cube_set(AlgBitVect* dst_bv,
		 int dst_offset,
		 const vector<Literal>& lit_list)
{
  cube_set(dst_bv + dst_offset * _cube_size(), lit_list);
}

// @brief Literal のリストからキューブ(を表すビットベクタ)のコピーを行う．
// @param[in] dst_bv 対象のビットベクタ
// @param[in] dst_offset オフセット
// @param[in] lit_list キューブを表すリテラルのリスト初期化子
//
// * dst_bv には十分な容量があると仮定する．
inline
void
AlgMgr::cube_set(AlgBitVect* dst_bv,
		 int dst_offset,
		 std::initializer_list<Literal> lit_list)
{
  cube_set(dst_bv + dst_offset * _cube_size(), lit_list);
}

// @brief キューブ(を表すビットベクタ)の比較を行う．
// @param[in] bv1 1つめのキューブを表すビットベクタ
// @param[in] bv1_offset bv1 のオフセット
// @param[in] bv2 2つめのキューブを表すビットベクタ
// @param[in] bv2_offset bv2 のオフセット
// @retval -1 bv1 <  bv2
// @retval  0 bv1 == bv2
// @retval  1 bv1 >  bv2
inline
int
AlgMgr::cube_compare(const AlgBitVect* bv1,
		     int bv1_offset,
		     const AlgBitVect* bv2,
		     int bv2_offset)
{
  return cube_compare(bv1 + bv1_offset * _cube_size(),
		      bv2 + bv2_offset * _cube_size());
}

// @brief 2つのキューブが矛盾していないか調べる．
// @param[in] bv1 1つめのキューブを表すビットベクタ
// @param[in] bv1_offset bv1 のオフセット
// @param[in] bv2 2つめのキューブを表すビットベクタ
// @param[in] bv2_offset bv2 のオフセット
// @retval true 同じ変数の相反するリテラルがある．
// @retval false 衝突していない．
inline
bool
AlgMgr::cube_check_conflict(const AlgBitVect* bv1,
			    int bv1_offset,
			    const AlgBitVect* bv2,
			    int bv2_offset)
{
  return cube_check_conflict(bv1 + bv1_offset * _cube_size(),
			     bv2 + bv2_offset * _cube_size());
}

// @brief 一方のキューブが他方のキューブに含まれているか調べる．
// @param[in] bv1 1つめのキューブを表すビットベクタ
// @param[in] bv1_offset bv1 のオフセット
// @param[in] bv2 2つめのキューブを表すビットベクタ
// @param[in] bv2_offset bv2 のオフセット
// @return 1つめのキューブが2つめのキューブ に含まれていたら true を返す．
inline
bool
AlgMgr::cube_check_containment(const AlgBitVect* bv1,
			       int bv1_offset,
			       const AlgBitVect* bv2,
			       int bv2_offset)
{
  return cube_check_containment(bv1 + bv1_offset * _cube_size(),
				bv2 + bv2_offset * _cube_size());
}

// @brief ２つのキューブに共通なリテラルがあれば true を返す．
// @param[in] bv1 1つめのキューブを表すビットベクタ
// @param[in] bv1_offset bv1 のオフセット
// @param[in] bv2 2つめのキューブを表すビットベクタ
// @param[in] bv2_offset bv2 のオフセット
// @return ２つのキューブに共通なリテラルがあれば true を返す．
inline
bool
AlgMgr::cube_check_intersect(const AlgBitVect* bv1,
			     int bv1_offset,
			     const AlgBitVect* bv2,
			     int bv2_offset)
{
  return cube_check_intersect(bv1 + bv1_offset * _cube_size(),
			      bv2 + bv2_offset * _cube_size());
}

// @brief カバー(を表すビットベクタ)のコピーを行う．
// @param[in] dst_bv コピー先のビットベクタ
// @param[in] src_bv ソースのビットベクタ
// @param[in] cube_num キューブ数
inline
void
AlgMgr::_copy(AlgBitVect* dst_bv,
	      const AlgBitVect* src_bv,
	      int cube_num)
{
  const AlgBitVect* src_end = src_bv + cube_num * _cube_size();
  for ( ; src_bv != src_end; ++ src_bv, ++ dst_bv ) {
    *dst_bv = *src_bv;
  }
}

// @brief 2つのキューブ(を表すビットベクタ)を入れ替える．
// @param[in] bv1 1つめのカバーを表すビットベクタ
// @param[in] bv2 2つめのカバーを表すビットベクタ
inline
void
AlgMgr::cube_swap(AlgBitVect* bv1,
		  AlgBitVect* bv2)
{
  cube_save(bv1);
  cube_copy(bv1, bv2);
  cube_restore(bv2);
}

// @brief 3つのキューブ(を表すビットベクタ)を入れ替える．
// @param[in] bv1 1つめのカバーを表すビットベクタ
// @param[in] bv2 2つめのカバーを表すビットベクタ
// @param[in] bv3 3つめのカバーを表すビットベクタ
//
// bv1 <- bv2, bv2 <- bv3, bv3 <- bv1 となる．
inline
void
AlgMgr::cube_rotate3(AlgBitVect* bv1,
		     AlgBitVect* bv2,
		     AlgBitVect* bv3)
{
  cube_save(bv1);
  cube_copy(bv1, bv2);
  cube_copy(bv2, bv3);
  cube_restore(bv3);
}

// @brief 4つのキューブ(を表すビットベクタ)を入れ替える．
// @param[in] bv1 1つめのカバーを表すビットベクタ
// @param[in] bv2 2つめのカバーを表すビットベクタ
// @param[in] bv3 3つめのカバーを表すビットベクタ
// @param[in] bv4 4つめのカバーを表すビットベクタ
//
// bv1 <- bv2, bv2 <- bv3, bv3 <- bv4, bv4 <- bv1 となる．
inline
void
AlgMgr::cube_rotate4(AlgBitVect* bv1,
		     AlgBitVect* bv2,
		     AlgBitVect* bv3,
		     AlgBitVect* bv4)
{
  cube_save(bv1);
  cube_copy(bv1, bv2);
  cube_copy(bv2, bv3);
  cube_copy(bv3, bv4);
  cube_restore(bv4);
}

// @brief キューブ(を表すビットベクタ)をバッファにコピーする．
// @param[in] src_bv ソースのビットベクタ
inline
void
AlgMgr::cube_save(const AlgBitVect* src_bv)
{
  cube_copy(mTmpBuff, src_bv);
}

// @brief キューブ(を表すビットベクタ)をバッファからコピーする．
// @param[in] dst_bv コピー先のビットベクタ
inline
void
AlgMgr::cube_restore(AlgBitVect* dst_bv)
{
  cube_copy(dst_bv, mTmpBuff);
}

// @brief ブロック位置を計算する．
// @param[in] var_id 変数番号
inline
int
AlgMgr::_block_pos(VarId var_id)
{
  return var_id.val() / 32;
}

// @brief シフト量を計算する．
// @param[in] var_id 変数番号
inline
int
AlgMgr::_shift_num(VarId var_id)
{
  // ソートしたときの見栄えの問題で左(MSB)から始める．
  return (31 - (var_id.val() % 32)) * 2;
}

// @brief キューブ1つ分のブロックサイズを計算する．
inline
int
AlgMgr::_cube_size() const
{
  return (variable_num() + 31) / 32;
}

END_NAMESPACE_YM_LOGIC

#endif // ALGMGR_H
