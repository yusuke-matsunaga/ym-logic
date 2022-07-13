#ifndef SOPMGR_H
#define SOPMGR_H

/// @file SopMgr.h
/// @brief SopMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017, 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Sop.h"
#include "ym/SopCube.h"
#include "ym/VarId.h"


BEGIN_NAMESPACE_YM_LOGIC

//////////////////////////////////////////////////////////////////////
/// @class SopMgr SopMgr.h "ym/SopMgr.h"
/// @brief SopCube, SopCover を管理するクラス
///
/// といっても実際の役割は入力数を覚えておくこと
//////////////////////////////////////////////////////////////////////
class SopMgr
{
public:

  /// @brief コンストラクタ
  SopMgr(
    SizeType variable_num ///< [in] 変数の数
  );

  /// @brief デストラクタ
  ~SopMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数の数を返す．
  SizeType
  variable_num() const
  {
    return mVarNum;
  }

  /// @brief キューブ/カバー用の領域を確保する．
  ///
  /// キューブの時は cube_num = 1 とする．
  SopBitVect*
  new_body(
    SizeType cube_num = 1 ///< [in] キューブ数
  );

  /// @brief キューブ/カバー用の領域を削除する．
  ///
  /// キューブの時は cube_num = 1 とする．
  void
  delete_body(
    SopBitVect* p,        ///< [in] 領域を指すポインタ
    SizeType cube_num = 1 ///< [in] キューブ数
  );


public:
  //////////////////////////////////////////////////////////////////////
  // SopCube/SopCover のための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ビットベクタからパタンを取り出す．
  SopPat
  get_pat(
    const SopBitVect* bv, ///< [in] ビットベクタ
    SizeType cube_id,     ///< [in] キューブ番号
    VarId var             ///< [in] 変数 ( 0 <= var_id.val() < variable_num() )
  )
  {
    ASSERT_COND( var.val() >= 0 && var.val() < variable_num() );

    SizeType blk = _block_pos(var) + _cube_size() * cube_id;
    SizeType sft = _shift_num(var);
    return static_cast<SopPat>((bv[blk] >> sft) & 3ULL);
  }

  /// @brief ビットベクタ上のリテラル数を数える．
  SizeType
  literal_num(
    const SopBlock& block ///< [in] 対象のブロック
  );

  /// @brief ビットベクタ上の特定のリテラルの出現頻度を数える．
  SizeType
  literal_num(
    const SopBlock& block, ///< [in] 対象のブロック
    Literal lit            ///< [in] 対象のリテラル
  );

  /// @brief ビットベクタからハッシュ値を計算する．
  SizeType
  hash(
    const SopBlock& src ///< [in] 対象のブロック
  );

  /// @brief Expr に変換する．
  Expr
  to_expr(
    const SopBitVect* bv, ///< [in] カバー/キューブを表すビットベクタ
    SizeType cube_num     ///< [in] キューブ数
  );

  /// @brief カバー/キューブの内容を出力する．
  void
  print(
    ostream& s,           ///< [in] 出力先のストリーム
    const SopBitVect* bv, ///< [in] カバー/キューブを表すビットベクタ
    SizeType start,       ///< [in] キューブの開始位置
    SizeType end,         ///< [in] キューブの終了位置(実際の末尾 + 1)
    const vector<string>& varname_list = vector<string>() ///< [in] 変数名のリスト
  );


public:
  //////////////////////////////////////////////////////////////////////
  // カバーを表すビットベクタに対する処理
  // 基本的に最初に結果を格納するビットベクタのポインタを引数にとる．
  // 結果のキューブ数は事前にはわからないので最大のキューブ数を計算し
  // 領域を確保しておく必要がある．
  // オペランドのカバーはビットベクタとキューブ数の組で表す．
  // 結果のキューブ数を関数の戻り値として返す．
  //////////////////////////////////////////////////////////////////////

  /// @brief カバー(を表すビットベクタ)のコピーを行う．
  ///
  /// * dst_bv には十分な容量があると仮定する．
  /// * dst_bv と src_bv の領域が重なっていたら正しく動かない．
  void
  cover_copy(
    SopBitVect* dst_bv,       ///< [in] コピー先のビットベクタ
    const SopBitVect* src_bv, ///< [in] ソースのビットベクタ
    SizeType cube_num         ///< [in] キューブ数
  )
  {
    bcopy(src_bv, dst_bv, cube_num * _cube_size() * sizeof(SopBitVect));
  }

  /// @brief カバー(を表すキューブのリスト)に内容をセットする．
  ///
  /// * dst_bv には十分な容量が確保されていると仮定する．
  void
  cover_set(
    SopBitVect* dst_bv,              ///< [in] 対象のビットベクタ
    const vector<SopCube>& cube_list ///< [in] キューブのリスト
  )
  {
    SopBitVect* dst_bv0 = dst_bv;
    for ( const auto& cube: cube_list ) {
      cube_copy(dst_bv, cube.mBody);
      dst_bv += _cube_size();
    }
  }

  /// @brief カバー(を表すビットベクタ)に内容をセットする．
  ///
  /// * リテラルのリストでキューブを表す．
  /// * dst_bv には十分な容量が確保されていると仮定する．
  void
  cover_set(
    SopBitVect* dst_bv,                ///< [in] 対象のビットベクタ
    const vector<SopBitVect*>& bv_list ///< [in] キューブを表すビットベクタのリスト
  )
  {
    SopBitVect* dst_bv0 = dst_bv;
    for ( auto bv: bv_list ) {
      cube_copy(dst_bv, bv);
      dst_bv += _cube_size();
    }
    _sort(dst_bv0, 0, bv_list.size());
  }

  /// @brief カバー(を表すビットベクタ)に内容をセットする．
  ///
  /// * リテラルのリストでキューブを表す．
  /// * dst_bv には十分な容量が確保されていると仮定する．
  void
  cover_set(
    SopBitVect* dst_bv,                      ///< [in] 対象のビットベクタ
    const vector<vector<Literal>>& cube_list ///< [in] カバーを表すリテラルのリストのリスト
  )
  {
    SopBitVect* dst_bv0 = dst_bv;
    for ( auto& lit_list: cube_list ) {
      cube_set(dst_bv, lit_list);
      dst_bv += _cube_size();
    }
    _sort(dst_bv0, 0, cube_list.size());
  }

  /// @brief カバー(を表すビットベクタ)に内容をセットする．
  ///
  /// * リテラルのリストでキューブを表す．
  /// * dst_bv には十分な容量が確保されていると仮定する．
  void
  cover_set(
    SopBitVect* dst_bv, ///< [in] 対象のビットベクタ
    initializer_list<initializer_list<Literal>>& cube_list ///< [in] カバーを表すリテラルのリストのリスト
  )
  {
    SopBitVect* dst_bv0 = dst_bv;
    for ( auto lit_list: cube_list ) {
      cube_set(dst_bv, lit_list);
      dst_bv += _cube_size();
    }
    _sort(dst_bv0, 0, cube_list.size());
  }

  /// @brief 2つのカバーの論理和を計算する．
  /// @return 結果のキューブ数を返す．
  ///
  /// * dst_bv には十分な容量があると仮定する．
  /// * dst_bv != src1.bitvect() を仮定する．
  /// * dst_bv != src2.bitvect() を仮定する．
  SizeType
  cover_sum(
    SopBitVect* dst_bv,   ///< [in] 結果を格納するビットベクタ
    const SopBlock& src1, ///< [in] 1つめのブロック
    const SopBlock& src2  ///< [in] 2つめのブロック
  );

  /// @brief 2つのカバーの差分を計算する．
  /// @return 結果のキューブ数を返す．
  ///
  /// * dst_bv には十分な容量があると仮定する．
  /// * dst_bv != src2.bitvect() を仮定する．
  /// * dst_bv == src1.bitvect() でも正しく動く
  SizeType
  cover_diff(
    SopBitVect* dst_bv,   ///< [in] 結果を格納するビットベクタ
    const SopBlock& src1, ///< [in] 1つめのブロック
    const SopBlock& src2  ///< [in] 2つめのブロック
  );

  /// @brief 2つのカバーの論理積を計算する．
  /// @return 結果のキューブ数を返す．
  ///
  /// * dst_bv には十分な容量があると仮定する．
  /// * dst_bv != src2.bitvect() を仮定する．
  /// * src2.cubenum() > 1 の時は dst_bv != src1.bitvect()
  ///   を仮定する．
  /// * src2.cubenum() == 1 の時は dst_bv == src1.bitvect()
  ///   でも正しく動く．
  SizeType
  cover_product(
    SopBitVect* dst_bv,   ///< [in] 結果を格納するビットベクタ
    const SopBlock& src1, ///< [in] 1つめのブロック
    const SopBlock& src2  ///< [in] 2つめのブロック
  );

  /// @brief カバーとリテラルとの論理積を計算する．
  /// @return 結果のキューブ数を返す．
  ///
  /// * dst_bv には十分な容量があると仮定する．
  /// * dst_bv == src1.bitvect() でも正しく動く．
  SizeType
  cover_product(
    SopBitVect* dst_bv,   ///< [in] 結果を格納するビットベクタ
    const SopBlock& src1, ///< [in] 1つめのブロック
    Literal lit           ///< [in] 対象のリテラル
  );

  /// @brief カバーの代数的除算を行う．
  /// @return 結果のキューブ数を返す．
  ///
  /// * dst_bv には十分な容量があると仮定する．
  /// * dst_bv == src1.bitvect() でも正しく動く．
  /// * dst_bv == src2.bitvect() でも正しく動く．
  SizeType
  cover_quotient(
    SopBitVect* dst_bv,   ///< [in] 結果を格納するビットベクタ
    const SopBlock& src1, ///< [in] 1つめのブロック
    const SopBlock& src2  ///< [in] 2つめのブロック
  );

  /// @brief カバーをリテラルで割る．
  /// @return 結果のキューブ数を返す．
  ///
  /// * dst_bv には十分な容量があると仮定する．
  /// * dst_bv == src1.bitvect() でも正しく動く．
  SizeType
  cover_quotient(
    SopBitVect* dst_bv,   ///< [in] 結果を格納するビットベクタ
    const SopBlock& src1, ///< [in] 1つめのブロック
    Literal lit           ///< [in] 対象のリテラル
  );

  /// @brief カバー中のすべてのキューブの共通部分を求める．
  ///
  /// * 共通部分がないときは空のキューブとなる．
  SopCube
  common_cube(
    const SopBlock& src1 ///< [in] 1つめのブロック
  );

  /// @brief カバー(を表すビットベクタ)の比較を行う．
  /// @retval -1 bv1 <  bv2
  /// @retval  0 bv1 == bv2
  /// @retval  1 bv1 >  bv2
  ///
  /// 比較はキューブごとの辞書式順序で行う．
  int
  cover_compare(
    const SopBlock& src1, ///< [in] 1つめのブロック
    const SopBlock& src2  ///< [in] 2つめのブロック
  );


public:
  //////////////////////////////////////////////////////////////////////
  // キューブに対する処理
  // 基本的にはキューブはビットベクタの先頭アドレスで表す．
  // キューブのサイズ(変数の個数に依存)によってキューブの開始アドレスは変化する．
  //////////////////////////////////////////////////////////////////////

  /// @brief キューブ(を表すビットベクタ)をクリアする．
  void
  cube_clear(
    SopBitVect* dst_bv ///< [in] 対象のビットベクタ
  );

  /// @brief キューブ(を表すビットベクタ)をクリアする．
  void
  cube_clear(
    SopBitVect* dst_bv, ///< [in] 対象のビットベクタ
    SizeType dst_offset ///< [in] オフセット
  )
  {
    cube_clear(_calc_offset(dst_bv, dst_offset));
  }

  /// @brief キューブをコピーする．
  void
  cube_copy(
    SopBitVect* dst_bv,     ///< [in] 対象のビットベクタ
    const SopCube& src_cube ///< [in] コピー元のキューブ
  )
  {
    cover_copy(dst_bv, src_cube.mBody, 1);
  }

  /// @brief キューブ(を表すビットベクタ)をコピーする．
  void
  cube_copy(
    SopBitVect* dst_bv,      ///< [in] 対象のビットベクタ
    const SopBitVect* src_bv ///< [in] コピー元のビットベクタ
  )
  {
    cover_copy(dst_bv, src_bv, 1);
  }

  /// @brief キューブ(を表すビットベクタ)をコピーする．
  void
  cube_copy(
    SopBitVect* dst_bv,       ///< [in] 対象のビットベクタ
    SizeType dst_offset,      ///< [in] dst_bv のオフセット
    const SopBitVect* src_bv, ///< [in] コピー元のビットベクタ
    SizeType src_offset       ///< [in] src_bv のオフセット
  )
  {
    cube_copy(_calc_offset(dst_bv, dst_offset),
	      _calc_offset(src_bv, src_offset));
  }

  /// @brief キューブをムーブする．
  ///
  /// 実際には SopCube のメンバにアクセスするだけ．
  SopBitVect*
  cube_move(
    SopCube& src_cube ///< [in] ムーブ元のキューブ
  )
  {
    SopBitVect* dst_bv = src_cube.mBody;
    src_cube.mBody = nullptr;
    return dst_bv;
  }

  /// @brief Literal のリストからキューブ(を表すビットベクタ)のコピーを行う．
  ///
  /// * dst_bv には十分な容量があると仮定する．
  void
  cube_set(
    SopBitVect* dst_bv,             ///< [in] 対象のビットベクタ
    const vector<Literal>& lit_list ///< [in] キューブを表すリテラルのリスト
  );

  /// @brief Literal のリストからキューブ(を表すビットベクタ)のコピーを行う．
  ///
  /// * dst_bv には十分な容量があると仮定する．
  void
  cube_set(
    SopBitVect* dst_bv,             ///< [in] 対象のビットベクタ
    SizeType dst_offset,            ///< [in] オフセット
    const vector<Literal>& lit_list ///< [in] キューブを表すリテラルのリスト
  )
  {
    cube_set(_calc_offset(dst_bv, dst_offset), lit_list);
  }

  /// @brief Literal のリストからキューブ(を表すビットベクタ)のコピーを行う．
  ///
  /// * dst_bv には十分な容量があると仮定する．
  void
  cube_set(
    SopBitVect* dst_bv,                 ///< [in] 対象のビットベクタ
    initializer_list<Literal>& lit_list ///< [in] キューブを表すリテラルのリスト初期化子
  );

  /// @brief Literal のリストからキューブ(を表すビットベクタ)のコピーを行う．
  ///
  /// * dst_bv には十分な容量があると仮定する．
  void
  cube_set(
    SopBitVect* dst_bv,                 ///< [in] 対象のビットベクタ
    SizeType dst_offset,                ///< [in] オフセット
    initializer_list<Literal>& lit_list ///< [in] キューブを表すリテラルのリスト初期化子
  );

  /// @brief キューブ(を表すビットベクタ)の比較を行う．
  /// @retval -1 bv1 <  bv2
  /// @retval  0 bv1 == bv2
  /// @retval  1 bv1 >  bv2
  int
  cube_compare(
    const SopBitVect* bv1, ///< [in] 1つめのキューブを表すビットベクタ
    const SopBitVect* bv2  ///< [in] 2つめのキューブを表すビットベクタ
  );

  /// @brief キューブ(を表すビットベクタ)の比較を行う．
  /// @retval -1 bv1 <  bv2
  /// @retval  0 bv1 == bv2
  /// @retval  1 bv1 >  bv2
  int
  cube_compare(
    const SopBitVect* bv1, ///< [in] 1つめのキューブを表すビットベクタ
    SizeType bv1_offset,   ///< [in] bv1 のオフセット
    const SopBitVect* bv2, ///< [in] 2つめのキューブを表すビットベクタ
    SizeType bv2_offset    ///< [in] bv2 のオフセット
  )
  {
    return cube_compare(_calc_offset(bv1, bv1_offset),
			_calc_offset(bv2, bv2_offset));
  }

  /// @brief 2つのキューブが矛盾していないか調べる．
  /// @retval true 同じ変数の相反するリテラルがある．
  /// @retval false 衝突していない．
  bool
  cube_check_conflict(
    const SopBitVect* bv1, ///< [in] 1つめのキューブを表すビットベクタ
    const SopBitVect* bv2  ///< [in] 2つめのキューブを表すビットベクタ
  );

  /// @brief 2つのキューブが矛盾していないか調べる．
  /// @retval true 同じ変数の相反するリテラルがある．
  /// @retval false 衝突していない．
  bool
  cube_check_conflict(
    const SopBitVect* bv1, ///< [in] 1つめのキューブを表すビットベクタ
    SizeType bv1_offset,   ///< [in] bv1 のオフセット
    const SopBitVect* bv2, ///< [in] 2つめのキューブを表すビットベクタ
    SizeType bv2_offset	   ///< [in] bv2 のオフセット
  )
  {
    return cube_check_conflict(_calc_offset(bv1, bv1_offset),
			       _calc_offset(bv2, bv2_offset));
  }

  /// @brief 一方のキューブが他方のキューブに含まれているか調べる．
  /// @return 1つめのキューブが2つめのキューブ に含まれていたら true を返す．
  bool
  cube_check_containment(
    const SopBitVect* bv1, ///< [in] 1つめのキューブを表すビットベクタ
    const SopBitVect* bv2  ///< [in] 2つめのキューブを表すビットベクタ
  );

  /// @brief 一方のキューブが他方のキューブに含まれているか調べる．
  /// @return 1つめのキューブが2つめのキューブ に含まれていたら true を返す．
  bool
  cube_check_containment(
    const SopBitVect* bv1, ///< [in] 1つめのキューブを表すビットベクタ
    SizeType bv1_offset,   ///< [in] bv1 のオフセット
    const SopBitVect* bv2, ///< [in] 2つめのキューブを表すビットベクタ
    SizeType bv2_offset	   ///< [in] bv2 のオフセット
  )
  {
    return cube_check_containment(_calc_offset(bv1, bv1_offset),
				  _calc_offset(bv2, bv2_offset));
  }

  /// @brief ２つのキューブに共通なリテラルがあれば true を返す．
  /// @return ２つのキューブに共通なリテラルがあれば true を返す．
  bool
  cube_check_intersect(
    const SopBitVect* bv1, ///< [in] 1つめのキューブを表すビットベクタ
    const SopCube& cube2   ///< [in] 2つめのキューブ
  )
  {
    return cube_check_intersect(bv1, cube2.mBody);
  }

  /// @brief ２つのキューブに共通なリテラルがあれば true を返す．
  /// @return ２つのキューブに共通なリテラルがあれば true を返す．
  bool
  cube_check_intersect(
    const SopBitVect* bv1, ///< [in] 1つめのキューブを表すビットベクタ
    const SopBitVect* bv2  ///< [in] 2つめのキューブを表すビットベクタ
  );

  /// @brief ２つのキューブに共通なリテラルがあれば true を返す．
  /// @return ２つのキューブに共通なリテラルがあれば true を返す．
  bool
  cube_check_intersect(
    const SopBitVect* bv1, ///< [in] 1つめのキューブを表すビットベクタ
    SizeType bv1_offset,   ///< [in] bv1 のオフセット
    const SopBitVect* bv2, ///< [in] 2つめのキューブを表すビットベクタ
    SizeType bv2_offset	   ///< [in] bv2 のオフセット
  )
  {
    return cube_check_intersect(_calc_offset(bv1, bv1_offset),
				_calc_offset(bv2, bv2_offset));
}

  /// @brief 2つのキューブの積を計算する．
  /// @retval true 積が空でなかった．
  /// @retval false 積が空だった．
  bool
  cube_product(
    SopBitVect* dst_bv,    ///< [in] コピー先のビットベクタ
    const SopBitVect* bv1, ///< [in] 1つめのキューブを表すビットベクタ
    const SopBitVect* bv2  ///< [in] 2つめのキューブを表すビットベクタ
  );

  /// @brief キューブによる商を求める．
  /// @return 正しく割ることができたら true を返す．
  bool
  cube_quotient(
    SopBitVect* dst_bv,    ///< [in] コピー先のビットベクタ
    const SopBitVect* bv1, ///< [in] 1つめのキューブを表すビットベクタ
    const SopBitVect* bv2  ///< [in] 2つめのキューブを表すビットベクタ
  );

  /// @brief キューブによる商を求める．
  /// @return 正しく割ることができたら true を返す．
  bool
  cube_quotient(
    SopBitVect* dst_bv,    ///< [in] コピー先のビットベクタ
    SizeType dst_offset,   ///< [in] オフセット
    const SopBitVect* bv1, ///< [in] 1つめのキューブを表すビットベクタ
    SizeType bv1_offset,   ///< [in] bv1 のオフセット
    const SopBitVect* bv2, ///< [in] 2つめのキューブを表すビットベクタ
    SizeType bv2_offset	   ///< [in] bv2 のオフセット
  )
  {
    return cube_quotient(_calc_offset(dst_bv, dst_offset),
			 _calc_offset(bv1, bv1_offset),
			 _calc_offset(bv2, bv2_offset));
  }

  /// @brief 2つのキューブ(を表すビットベクタ)を入れ替える．
  void
  cube_swap(
    SopBitVect* bv1, ///< [in] 1つめのキューブを表すビットベクタ
    SopBitVect* bv2  ///< [in] 2つめのキューブを表すビットベクタ
  )
  {
    cube_save(bv1);
    cube_copy(bv1, bv2);
    cube_restore(bv2);
  }

  /// @brief 3つのキューブ(を表すビットベクタ)を入れ替える．
  ///
  /// bv1 <- bv2, bv2 <- bv3, bv3 <- bv1 となる．
  void
  cube_rotate3(
    SopBitVect* bv1, ///< [in] 1つめのキューブを表すビットベクタ
    SopBitVect* bv2, ///< [in] 2つめのキューブを表すビットベクタ
    SopBitVect* bv3  ///< [in] 3つめのキューブを表すビットベクタ
  )
  {
    cube_save(bv1);
    cube_copy(bv1, bv2);
    cube_copy(bv2, bv3);
    cube_restore(bv3);
  }

  /// @brief 4つのキューブ(を表すビットベクタ)を入れ替える．
  ///
  /// bv1 <- bv2, bv2 <- bv3, bv3 <- bv4, bv4 <- bv1 となる．
  void
  cube_rotate4(
    SopBitVect* bv1, ///< [in] 1つめのキューブを表すビットベクタ
    SopBitVect* bv2, ///< [in] 2つめのキューブを表すビットベクタ
    SopBitVect* bv3, ///< [in] 3つめのキューブを表すビットベクタ
    SopBitVect* bv4  ///< [in] 4つめのキューブを表すビットベクタ
  )
  {
    cube_save(bv1);
    cube_copy(bv1, bv2);
    cube_copy(bv2, bv3);
    cube_copy(bv3, bv4);
    cube_restore(bv4);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // リテラル集合に対する演算
  // 基本キューブと同じだが相反するリテラルがあってもOK
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素のチェック
  /// @return lit を含んでいたら true を返す．
  bool
  litset_check(
    SopBitVect* bv, ///< [in] ビットベクタ
    Literal lit     ///< [in] 対象のリテラル
  );

  /// @brief ユニオン演算
  void
  litset_union(
    SopBitVect* dst_bv,      ///< [in] 対象のビットベクタ
    const SopBitVect* src_bv ///< [in] 加えるビットベクタ
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief マージソートを行う下請け関数
  ///
  /// bv[start] - bv[end - 1] の領域をソートする．
  void
  _sort(
    SopBitVect* bv, ///< [in] 対象のビットベクタ
    SizeType start, ///< [in] 開始位置
    SizeType end    ///< [in] 終了位置
  );

  /// @brief mTmpBuff を初期化する．
  void
  _init_buff();

  /// @brief mTmpBuff に必要な領域を確保する．
  void
  _resize_buff(
    SizeType req_size ///< [in] 要求するキューブ数
  );

  /// @brief キューブ(を表すビットベクタ)をバッファにコピーする．
  void
  cube_save(
    const SopBitVect* src_bv ///< [in] ソースのビットベクタ
  )
  {
    cube_copy(mTmpBuff, src_bv);
  }

  /// @brief キューブ(を表すビットベクタ)をバッファからコピーする．
  void
  cube_restore(
    SopBitVect* dst_bv ///< [in] コピー先のビットベクタ
  )
  {
    cube_copy(dst_bv, mTmpBuff);
  }

  /// @brief ビットベクタの末尾を計算する．
  SopBitVect*
  _calc_offset(
    SopBitVect* bv,   ///< [in] ビットベクタの先頭
    SizeType cube_num ///< [in] キューブ数
  )
  {
    return bv + cube_num * _cube_size();
  }

  /// @brief ビットベクタの末尾を計算する．
  const SopBitVect*
  _calc_offset(
    const SopBitVect* bv, ///< [in] ビットベクタの先頭
    SizeType cube_num     ///< [in] キューブ数
  )
  {
    return bv + cube_num * _cube_size();
  }

  /// @brief ブロック位置を計算する．
  static
  SizeType
  _block_pos(
    VarId var_id ///< [in] 変数番号
  )
  {
    return var_id.val() / 32;
  }

  /// @brief シフト量を計算する．
  static
  SizeType
  _shift_num(
    VarId var_id ///< [in] 変数番号
  )
  {
    // ソートしたときの見栄えの問題で左(MSB)から始める．
    return (31 - (var_id.val() % 32)) * 2;
  }

  /// @brief キューブ1つ分のブロックサイズを計算する．
  SizeType
  _cube_size() const
  {
    return (variable_num() + 31) / 32;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数の数
  SizeType mVarNum;

  // 作業用に用いられるビットベクタ
  SopBitVect* mTmpBuff;

  // mTmpBuff のキューブ数
  SizeType mTmpBuffSize;

};

END_NAMESPACE_YM_LOGIC

#endif // SOPMGR_H
