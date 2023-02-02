#ifndef SOPMGR_H
#define SOPMGR_H

/// @file SopMgr.h
/// @brief SopMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/sop_nsdef.h"
#include "ym/SopCube.h"
#include "ym/SopCover.h"
#include "ym/Literal.h"
#include "ym/Expr.h"
#include "SopBlock.h"


BEGIN_NAMESPACE_YM_SOP

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

  /// @brief SopCube を作る．
  SopCube
  make_cube(
    SopBitVect* bv ///< [in] 本体のビットベクタ
  )
  {
    return SopCube{variable_num(), bv};
  }

  /// @brief SopCover を作る．
  SopCover
  make_cover(
    const SopBlock& block ///< [in] 本体のブロック
  )
  {
    SizeType nc = block.cube_num;
    auto body = block.body;
    return SopCover{variable_num(), nc, nc, body};
  }


public:
  //////////////////////////////////////////////////////////////////////
  // SopCube/SopCover のための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ビットベクタからパタンを取り出す．
  SopPat
  get_pat(
    const SopBitVect* bv, ///< [in] ビットベクタ
    SizeType cube_id,     ///< [in] キューブ番号
    SizeType var          ///< [in] 変数 ( 0 <= var_id.val() < variable_num() )
  )
  {
    ASSERT_COND( var >= 0 && var < variable_num() );

    SizeType blk = _block_pos(var) + _cube_size() * cube_id;
    SizeType sft = _shift_num(var);
    return static_cast<SopPat>((bv[blk] >> sft) & 3ULL);
  }

  /// @brief ビットベクタ上のリテラル数を数える．
  SizeType
  literal_num(
    const SopBlock& block ///< [in] 対象のブロック
  )
  {
    return literal_num(block.body, block.cube_num);
  }

  /// @brief ビットベクタ上のリテラル数を数える．
  SizeType
  literal_num(
    const SopBitVect* bv, ///< [in] 対象のブロック
    SizeType cube_num     ///< [in] キューブ数
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
    const SopBlock& block ///< [in] 対象のブロック
  );

  /// @brief カバー/キューブの内容を出力する．
  void
  print(
    ostream& s,           ///< [in] 出力先のストリーム
    const SopBitVect* bv, ///< [in] カバー/キューブを表すビットベクタ
    SizeType start,       ///< [in] キューブの開始位置
    SizeType end,         ///< [in] キューブの終了位置(実際の末尾 + 1)
    const vector<string>& varname_list = {} ///< [in] 変数名のリスト
  );

  /// @brief 内容を出力する(デバッグ用)．
  void
  debug_print(
    ostream& s,
    const SopBitVect* bv,
    SizeType num
  );


public:
  //////////////////////////////////////////////////////////////////////
  // カバーを表すビットベクタに対する処理
  // カバーは入力，出力ともに SopBlock で表す．
  //////////////////////////////////////////////////////////////////////

  /// @brief カバーを確保する．
  SopBlock
  new_cover(
    SizeType cube_num ///< [in] キューブ数
  )
  {
    auto bv = _new_body(cube_num);
    return SopBlock{cube_num, bv};
  }

  /// @brief カバーのコピーを行う．
  SopBlock
  new_cover(
    const SopBlock& src ///< [in] ソースのカバー
  )
  {
    SizeType nc = src.cube_num;
    auto dst_block = new_cover(nc);
    auto dst_body = dst_block.body;
    bcopy(src.body, dst_body, nc * _cube_size() * sizeof(SopBitVect));
    return dst_block;
  }

  /// @brief キューブのリストからカバーを生成する．
  SopBlock
  new_cover(
    const vector<SopCube>& cube_list ///< [in] キューブのリスト
  )
  {
    SizeType nc = cube_list.size();
    auto dst_block = new_cover(nc);
    auto dst_body = dst_block.body;
    for ( SizeType pos = 0; pos < nc; ++ pos ) {
      auto& cube = cube_list[pos];
      auto bv = cube.mBody;
      cube_copy(_calc_offset(dst_body, pos), bv);
    }
    _sort(dst_body, 0, nc);
    return dst_block;
  }

  /// @brief ビットベクタのリストをカバーに変換する．
  SopBlock
  new_cover(
    const vector<SopBitVect*>& bv_list ///< [in] キューブを表すビットベクタのリスト
  )
  {
    SizeType nc = bv_list.size();
    auto dst_block = new_cover(nc);
    auto dst_body = dst_block.body;
    for ( SizeType pos = 0; pos < nc; ++ pos ) {
      auto bv = bv_list[pos];
      cube_copy(_calc_offset(dst_body, pos), bv);
    }
    _sort(dst_body, 0, nc);
    return dst_block;
  }

  /// @brief リテラルのリストのリストをカバーに変換する．
  SopBlock
  new_cover(
    const vector<vector<Literal>>& cube_list ///< [in] カバーを表すリテラルのリストのリスト
  )
  {
    SizeType nc = cube_list.size();
    auto dst_block = new_cover(nc);
    auto dst_body = dst_block.body;
    for ( SizeType pos = 0; pos < nc; ++ pos ) {
      auto& lit_list = cube_list[pos];
      cube_set_literals(_calc_offset(dst_body, pos), lit_list);
    }
    _sort(dst_body, 0, nc);
    return dst_block;
  }

  /// @brief リテラルのリストのリストをカバーに変換する．
  SopBlock
  new_cover(
    initializer_list<initializer_list<Literal>>& cube_list ///< [in] カバーを表すリテラルのリストのリスト
  )
  {
    SizeType nc = cube_list.size();
    auto dst_block = new_cover(nc);
    auto dst_body = dst_block.body;
    SizeType pos = 0;
    for ( auto& lit_list: cube_list ) {
      cube_set_literals(_calc_offset(dst_body, pos), lit_list);
      ++ pos;
    }
    _sort(dst_body, 0, nc);
    return dst_block;
  }

  /// @brief カバーの領域を開放する．
  static
  void
  delete_cover(
    const SopBlock& cover
  )
  {
    _delete_body(cover.body);
  }

  /// @brief カバー中のキューブをクリアする．
  void
  cover_clear_cube(
    SopBitVect* body, ///< [in] 先頭
    SizeType cube_pos ///< [in] キューブ番号
  )
  {
    cube_clear(_calc_offset(body, cube_pos));
  }

  /// @brief カバーの内容をコピーする．
  void
  cover_copy(
    SopBlock& dst_block, ///< [in] コピー先のブロック
    const SopBlock& src  ///< [in] コピー元のブロック
  )
  {
    ASSERT_COND( dst_block.cube_num >= src.cube_num );
    _copy(dst_block.body, src.body, src.cube_num);
    dst_block.cube_num = src.cube_num;
  }

  /// @brief カバー中のキューブにリテラルをセットする．
  void
  cover_set_literal(
    SopBitVect* body,  ///< [in] 先頭
    SizeType cube_pos, ///< [in] キューブ番号
    Literal lit        ///< [in] 設定するリテラル
  )
  {
    cube_set_literal(_calc_offset(body, cube_pos), lit);
  }

  /// @brief 否定のカバーを求める．
  SopBlock
  cover_complement(
    const SopBlock& src ///< [in] 元のカバーを表すブロック
  );

  /// @brief 2つのカバーの論理和を計算する．
  /// @return 結果のカバーを返す．
  ///
  /// カバーをキューブの集合とみなして和集合を計算する．
  SopBlock
  cover_sum(
    const SopBlock& src1, ///< [in] 1つめのカバー
    const SopBlock& src2  ///< [in] 2つめのカバー
  );

  /// @brief 2つのカバーの差分を計算する．
  /// @return 結果のカバーを返す．
  ///
  /// カバーをキューブの集合とみなして集合差を計算する．
  SopBlock
  cover_diff(
    const SopBlock& src1, ///< [in] 1つめのカバー
    const SopBlock& src2  ///< [in] 2つめのカバー
  )
  {
    auto dst = new_cover(src1.cube_num);
    _cover_diff_sub(dst, src1, src2);
    return dst;
  }

  /// @brief 2つのカバーの差分を計算する．
  ///
  /// カバーをキューブの集合とみなして集合差を計算する．
  /// 結果は src1 に格納される．
  void
  cover_diff_int(
    SopBlock& src1,      ///< [in] 1つめのカバー
    const SopBlock& src2 ///< [in] 2つめのカバー
  )
  {
    _cover_diff_sub(src1, src1, src2);
  }

  /// @brief 2つのカバーの差分を計算する．
  ///
  /// カバーをキューブの集合とみなして集合差を計算する．
  /// 結果は src1 に格納される．
  void
  _cover_diff_sub(
    SopBlock& dst,        ///< [in] 結果を格納するブロック
    const SopBlock& src1, ///< [in] 1つめのカバー
    const SopBlock& src2  ///< [in] 2つめのカバー
  );

  /// @brief 2つのカバーの論理積を計算する．
  /// @return 結果のカバーを返す．
  SopBlock
  cover_product(
    const SopBlock& src1, ///< [in] 1つめのカバー
    const SopBlock& src2  ///< [in] 2つめのカバー
  );

  /// @brief カバーとキューブの論理積を計算する．
  /// @return 結果のカバーを返す．
  SopBlock
  cover_product(
    const SopBlock& src1,  ///< [in] 1つめのカバー
    const SopBitVect* src2 ///< [in] 2つめのキューブ
  )
  {
    auto dst = new_cover(src1.cube_num);
    _cover_product_sub(dst, src1, src2);
    return dst;
  }

  /// @brief カバーとキューブの論理積を計算する．
  void
  cover_product_int(
    SopBlock& src1,        ///< [in] 1つめのカバー
    const SopBitVect* src2 ///< [in] 2つめのキューブ
  )
  {
    _cover_product_sub(src1, src1, src2);
  }

  /// @brief カバーとキューブの論理積を計算する．
  /// @return 結果のカバーを返す．
  void
  _cover_product_sub(
    SopBlock& dst,         ///< [in] 結果を格納するブロック
    const SopBlock& src1,  ///< [in] 1つめのカバー
    const SopBitVect* src2 ///< [in] 2つめのキューブ
  );

  /// @brief カバーとリテラルとの論理積を計算する．
  /// @return 結果のカバーを返す．
  SopBlock
  cover_product(
    const SopBlock& src1, ///< [in] 1つめのカバー
    Literal lit           ///< [in] 対象のリテラル
  )
  {
    auto dst = new_cover(src1.cube_num);
    _cover_product_sub(dst, src1, lit);
    return dst;
  }

  /// @brief カバーとリテラルとの論理積を計算する．
  void
  cover_product_int(
    SopBlock& src1, ///< [in] 1つめのカバー
    Literal lit     ///< [in] 対象のリテラル
  )
  {
    _cover_product_sub(src1, src1, lit);
  }

  /// @brief カバーとリテラルとの論理積を計算する．
  void
  _cover_product_sub(
    SopBlock& dst,        ///< [in] 結果を格納するブロック
    const SopBlock& src1, ///< [in] 1つめのカバー
    Literal lit           ///< [in] 対象のリテラル
  );

  /// @brief カバーの代数的除算を行う．
  /// @return 結果のカバーを返す．
  SopBlock
  cover_quotient(
    const SopBlock& src1, ///< [in] 被除数
    const SopBlock& src2  ///< [in] 除数
  );

  /// @brief カバーをキューブで割る．
  /// @return 結果のカバーを返す．
  SopBlock
  cover_quotient(
    const SopBlock& src1,  ///< [in] 1つめのカバー
    const SopBitVect* src2 ///< [in] 対象のキューブ
  )
  {
    auto dst_block = new_cover(src1.cube_num);
    _cover_quotient_sub(dst_block, src1, src2);
    return dst_block;
  }

  /// @brief カバーをキューブで割る．
  void
  cover_quotient_int(
    SopBlock& src1,        ///< [in] 1つめのカバー
    const SopBitVect* src2 ///< [in] 対象のキューブ
  )
  {
    _cover_quotient_sub(src1, src1, src2);
  }

  /// @brief カバーをキューブで割る．
  void
  _cover_quotient_sub(
    SopBlock& dst,         ///< [in] 1つめのカバー
    const SopBlock& src1,  ///< [in] 1つめのカバー
    const SopBitVect* src2 ///< [in] 対象のキューブ
  );

  /// @brief カバーをリテラルで割る．
  /// @return 結果のカバーを返す．
  SopBlock
  cover_quotient(
    const SopBlock& src1, ///< [in] 1つめのカバー
    Literal lit           ///< [in] 対象のリテラル
  )
  {
    auto dst_block = new_cover(src1.cube_num);
    _cover_quotient_sub(dst_block, src1, lit);
    return dst_block;
  }

  /// @brief カバーをリテラルで割る．
  void
  cover_quotient_int(
    SopBlock& src1, ///< [in] 1つめのカバー
    Literal lit     ///< [in] 対象のリテラル
  )
  {
    _cover_quotient_sub(src1, src1, lit);
  }

  /// @brief カバーをリテラルで割る．
  void
  _cover_quotient_sub(
    SopBlock& dst,        ///< [in] 結果を格納するブロック
    const SopBlock& src1, ///< [in] 1つめのカバー
    Literal lit           ///< [in] 対象のリテラル
  );

  /// @brief コファクターを求める．
  SopBlock
  cover_cofactor(
    const SopBlock& src1, ///< [in] 対象のカバー
    Literal lit           ///< [in] リテラル
  );

  /// @brief カバー中のすべてのキューブの共通部分を求める．
  ///
  /// * 共通部分がないときは空のキューブとなる．
  SopBitVect*
  common_cube(
    const SopBlock& src1 ///< [in] 対象のカバー
  );

  /// @brief カバーの比較を行う．
  /// @retval -1 src1 <  src2
  /// @retval  0 src1 == src2
  /// @retval  1 src1 >  src2
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

  /// @brief キューブを確保する．
  SopBitVect*
  new_cube()
  {
    return _new_body(1);
  }

  /// @brief コピーしたキューブを作る．
  SopBitVect*
  new_cube(
    const SopBitVect* src_bv ///< [in] コピー元のビットベクタ
  )
  {
    auto dst_body = new_cube();
    cube_copy(dst_body, src_bv);
    return dst_body;
  }

  /// @brief Literal のリストからキューブを作る．
  SopBitVect*
  new_cube(
    const vector<Literal>& lit_list ///< [in] キューブを表すリテラルのリスト
  )
  {
    auto dst_body = new_cube();
    for ( auto lit: lit_list ) {
      cube_set_literal(dst_body, lit);
    }
    return dst_body;
  }

  /// @brief キューブ(を表すビットベクタ)をコピーする．
  void
  cube_copy(
    SopBitVect* dst_bv,      ///< [in] 対象のビットベクタ
    const SopBitVect* src_bv ///< [in] コピー元のビットベクタ
  )
  {
    _copy(dst_bv, src_bv, 1);
  }

  /// @brief キューブの領域を開放する．
  static
  void
  delete_cube(
    SopBitVect* cube
  )
  {
    _delete_body(cube);
  }

  /// @brief キューブ(を表すビットベクタ)をクリアする．
  void
  cube_clear(
    SopBitVect* dst_bv ///< [in] 対象のビットベクタ
  )
  {
    SizeType size = _cube_size() * sizeof(SopBitVect);
    bzero(dst_bv, size);
  }

  /// @brief キューブ(を表すビットベクタ)をクリアする．
  void
  cube_clear(
    SopBitVect* dst_bv, ///< [in] 対象のビットベクタ
    SizeType dst_offset ///< [in] オフセット
  )
  {
    cube_clear(_calc_offset(dst_bv, dst_offset));
  }

  /// @brief キューブにリテラルをセットする．
  void
  cube_set_literal(
    SopBitVect* dst_bv, ///< [in] 対象のビットベクタ
    Literal lit         ///< [in] リテラル
  )
  {
    auto var_id = lit.varid();
    auto blk = _block_pos(var_id);
    auto sft = _shift_num(var_id);
    auto pat = lit2bv(lit);
    auto mask = pat << sft;
    dst_bv[blk] |= mask;
  }

  /// @brief キューブにリテラルをセットする．
  void
  cube_set_literal(
    SopBitVect* dst_bv,  ///< [in] 対象のビットベクタ
    SizeType dst_offset, ///< [in] オフセット
    Literal lit          ///< [in] リテラル
  )
  {
    cube_set_literal(_calc_offset(dst_bv, dst_offset), lit);
  }

  /// @brief Literal のリストからキューブ(を表すビットベクタ)のコピーを行う．
  ///
  /// * dst_bv には十分な容量があると仮定する．
  void
  cube_set_literals(
    SopBitVect* dst_bv,             ///< [in] 対象のビットベクタ
    const vector<Literal>& lit_list ///< [in] キューブを表すリテラルのリスト
  )
  {
    for ( auto lit: lit_list ) {
      cube_set_literal(dst_bv, lit);
    }
  }

  /// @brief Literal のリストからキューブ(を表すビットベクタ)のコピーを行う．
  ///
  /// * dst_bv には十分な容量があると仮定する．
  void
  cube_set_literals(
    SopBitVect* dst_bv,             ///< [in] 対象のビットベクタ
    SizeType dst_offset,            ///< [in] オフセット
    const vector<Literal>& lit_list ///< [in] キューブを表すリテラルのリスト
  )
  {
    cube_set_literals(_calc_offset(dst_bv, dst_offset), lit_list);
  }

  /// @brief Literal のリストからキューブ(を表すビットベクタ)のコピーを行う．
  ///
  /// * dst_bv には十分な容量があると仮定する．
  void
  cube_set_literals(
    SopBitVect* dst_bv,                 ///< [in] 対象のビットベクタ
    initializer_list<Literal>& lit_list ///< [in] キューブを表すリテラルのリスト初期化子
  )
  {
    for ( auto lit: lit_list ) {
      cube_set_literal(dst_bv, lit);
    }
  }

  /// @brief Literal のリストからキューブ(を表すビットベクタ)のコピーを行う．
  ///
  /// * dst_bv には十分な容量があると仮定する．
  void
  cube_set_literals(
    SopBitVect* dst_bv,                 ///< [in] 対象のビットベクタ
    SizeType dst_offset,                ///< [in] オフセット
    initializer_list<Literal>& lit_list ///< [in] キューブを表すリテラルのリスト初期化子
  )
  {
    cube_set_literals(_calc_offset(dst_bv, dst_offset), lit_list);
  }

  /// @brief キューブの内容を取り出す．
  ///
  /// 以後キューブは使用不可
  static
  SopBitVect*
  cube_extract(
    SopCube& cube
  )
  {
    auto ans = cube.mBody;
    cube.mBody = nullptr;
    return ans;
  }

  /// @brief 空キューブかどうか調べる．
  bool
  cube_check_null(
    const SopBitVect* bv ///< [in] 対象のビットベクタ
  );

  /// @brief 空キューブかどうか調べる．
  bool
  cube_check_null(
    const SopBitVect* bv,   ///< [in] 対象のビットベクタ
    SizeType cube_pos ///< [in] キューブ位置
  )
  {
    return cube_check_null(_calc_offset(bv, cube_pos));
  }

  /// @brief キューブを否定する．
  SopBlock
  cube_complement(
    const SopBitVect* bv
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

  /// @brief キューブとリテラルの積を計算する．
  /// @retval true 積が空でなかった．
  /// @retval false 積が空だった．
  bool
  cube_product(
    SopBitVect* dst_bv,    ///< [in] コピー先のビットベクタ
    const SopBitVect* bv1, ///< [in] 1つめのキューブを表すビットベクタ
    Literal lit            ///< [in] リテラル
  );

  /// @brief キューブとリテラルの積を計算する．
  /// @retval true 積が空でなかった．
  /// @retval false 積が空だった．
  bool
  cube_product_int(
    SopBitVect* dst_bv,    ///< [in] コピー先のビットベクタ
    Literal lit            ///< [in] リテラル
  );

  /// @brief キューブによる商を求める．
  /// @return 正しく割ることができたら true を返す．
  bool
  cube_quotient(
    SopBitVect* dst_bv,    ///< [in] コピー先のビットベクタ
    const SopBitVect* bv1, ///< [in] 1つめのキューブを表すビットベクタ
    const SopBitVect* bv2  ///< [in] 2つめのキューブを表すビットベクタ
  );

  /// @brief リテラルによる商を求める．
  /// @return 正しく割ることができたら true を返す．
  bool
  cube_quotient(
    SopBitVect* dst_bv,    ///< [in] コピー先のビットベクタ
    const SopBitVect* bv1, ///< [in] 1つめのキューブを表すビットベクタ
    Literal lit            ///< [in] リテラル
  );

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

  /// @brief MOST_BINATE variable を選ぶ．
  SizeType
  binate_select(
    const SopBlock& src
  );

  /// @brief MERGE_WITH_IDENTITY を行う．
  SopBlock
  merge_with_identity(
    SizeType var,
    const SopBlock& f0,
    const SopBlock& f1
  );

  /// @brief キューブ/カバー用の領域を確保する．
  SopBitVect*
  _new_body(
    SizeType cube_num = 1 ///< [in] キューブ数
  )
  {
    auto size = _cube_size() * cube_num;
    auto body = new SopBitVect[size];
    bzero(body, size * sizeof(SopBitVect));
    return body;
  }

  /// @brief キューブ/カバー用の領域を削除する．
  static
  void
  _delete_body(
    SopBitVect* p ///< [in] 領域を指すポインタ
  )
  {
    delete [] p;
  }

  /// @brief マージソートを行う下請け関数
  ///
  /// bv[start] - bv[end - 1] の領域をソートする．
  void
  _sort(
    SopBitVect* bv, ///< [in] 対象のビットベクタ
    SizeType start, ///< [in] 開始位置
    SizeType end    ///< [in] 終了位置
  );

  /// @brief 単純なコピーを行う．
  void
  _copy(
    SopBitVect* dst_bv,       ///< [in] コピー先のアドレス
    const SopBitVect* src_bv, ///< [in] コピー元のアドレス
    SizeType cube_num         ///< [in] キューブ数
  )
  {
    SizeType size = _cube_size() * cube_num * sizeof(SopBitVect);
    bcopy(src_bv, dst_bv, size);
  }

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
    SizeType var_id ///< [in] 変数番号
  )
  {
    return var_id / 32;
  }

  /// @brief シフト量を計算する．
  static
  SizeType
  _shift_num(
    SizeType var_id ///< [in] 変数番号
  )
  {
    // ソートしたときの見栄えの問題で左(MSB)から始める．
    return (31 - (var_id % 32)) * 2;
  }

  /// @brief キューブ1つ分のブロックサイズを計算する．
  SizeType
  _cube_size() const
  {
    // SopBitVect は64 ビットだが1つの変数に2ビット使っている．
    return (variable_num() + 31) / 32;
  }

  static
  SopBitVect
  lit2bv(
    Literal lit
  )
  {
    return static_cast<SopBitVect>(lit.is_positive() ? SopPat::_1 : SopPat::_0);
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

END_NAMESPACE_YM_SOP

#endif // SOPMGR_H
