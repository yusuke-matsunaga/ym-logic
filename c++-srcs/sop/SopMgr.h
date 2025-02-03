#ifndef SOPMGR_H
#define SOPMGR_H

/// @file SopMgr.h
/// @brief SopMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/SopBase.h"
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
/// といっても実際の役割は入力数を覚えておくことと
/// 関数群の提供
//////////////////////////////////////////////////////////////////////
class SopMgr:
  public SopBase
{
public:

  /// @brief コンストラクタ
  SopMgr(
    SizeType variable_num, ///< [in] 変数の数
    SizeType buff_size = 0 ///< [in] バッファサイズ
  ) : SopBase{variable_num}
  {
    if ( buff_size > 0 ) {
      _resize_buff(buff_size);
    }
  }

  /// @brief デストラクタ
  ~SopMgr() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief SopCube を作る．
  SopCube
  make_cube(
    SopBitVect&& bv ///< [in] 本体のビットベクタ
  )
  {
    return SopCube{variable_num(), std::move(bv)};
  }

  /// @brief SopCover を作る．
  SopCover
  make_cover(
    SopBlock&& block ///< [in] 本体のブロック
  )
  {
    return SopCover{variable_num(),
		    block.cube_num,
		    std::move(block.body)};
  }


public:
  //////////////////////////////////////////////////////////////////////
  // SopCube/SopCover のための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ビットベクタからパタンを取り出す．
  SopPat
  get_pat(
    SopBitVectConstIter iter, ///< [in] ビットベクタ
    SizeType var              ///< [in] 変数 ( 0 <= var_id.val() < variable_num() )
  ) const
  {
    if ( var >= variable_num() ) {
      throw std::out_of_range{"var is out of range"};
    }

    SizeType blk = _block_pos(var);
    SizeType sft = _shift_num(var);
    auto p = iter + blk;
    return static_cast<SopPat>((*p >> sft) & 3ULL);
  }

  /// @brief ビットベクタ上のリテラル数を数える．
  SizeType
  literal_num(
    const SopBlockRef& block ///< [in] 対象のブロック
  ) const;

  /// @brief ビットベクタ上の特定のリテラルの出現頻度を数える．
  SizeType
  literal_num(
    const SopBlockRef& block, ///< [in] 対象のブロック
    Literal lit               ///< [in] 対象のリテラル
  ) const
  {
    auto varid = lit.varid();
    auto inv = lit.is_negative();
    return literal_num(block, varid, inv);
  }

  /// @brief ビットベクタ上の特定のリテラルの出現頻度を数える．
  SizeType
  literal_num(
    const SopBlockRef& block, ///< [in] 対象のブロック
    SizeType varid,           ///< [in] 変数番号
    bool inv                  ///< [in] 反転属性
  ) const;

  /// @brief 内容をリテラルのリストのリストに変換する．
  vector<vector<Literal>>
  literal_list(
    const SopBlockRef& block ///< [in] 対象のブロック
  ) const;

  /// @brief カバーからキューブを取り出す．
  SopCube
  get_cube(
    const SopBlockRef& block, ///< [in] 対象のブロック
    SizeType cube_id          ///< [in] キューブ番号 ( 0 <= cube_id < cube_num() )
  ) const;

  /// @brief ビットベクタからハッシュ値を計算する．
  SizeType
  hash(
    const SopBlockRef& block ///< [in] 対象のブロック
  );

  /// @brief Expr に変換する．
  Expr
  to_expr(
    const SopBlockRef& block ///< [in] 対象のブロック
  );

  /// @brief カバー/キューブの内容を出力する．
  void
  print(
    ostream& s,           ///< [in] 出力先のストリーム
    const SopBitVect& bv, ///< [in] カバー/キューブを表すビットベクタ
    SizeType start,       ///< [in] キューブの開始位置
    SizeType end,         ///< [in] キューブの終了位置(実際の末尾 + 1)
    const vector<string>& varname_list ///< [in] 変数名のリスト
    = {}
  );

  /// @brief 内容を出力する(デバッグ用)．
  void
  debug_print(
    ostream& s,
    const SopBlockRef& block
  );


public:
  //////////////////////////////////////////////////////////////////////
  // SopBlock の生成と開放
  //////////////////////////////////////////////////////////////////////

  /// @brief カバーを確保する．
  SopBlock
  new_block(
    SizeType cube_num ///< [in] キューブ数
  )
  {
    return SopBlock{cube_num, cube_num * _cube_size()};
  }

  /// @brief カバーのコピーを行う．
  SopBlock
  copy_block(
    const SopBlockRef& src ///< [in] コピー元
  )
  {
    return SopBlock{src};
  }

  /// @brief キューブのリストからカバーを生成する．
  SopBlock
  new_block(
    const vector<SopCube>& cube_list ///< [in] キューブのリスト
  )
  {
    SizeType nc = cube_list.size();
    auto dst = new_block(nc);
    auto dst_cube = _cube_begin(dst.body);
    for ( auto& cube: cube_list ) {
      auto src_cube= _cube_begin(cube.body());
      cube_copy(dst_cube, src_cube);
      dst_cube += _cube_size();
    }
    dst.cube_num = nc;
    sort(dst);
    return dst;
  }

  /// @brief ビットベクタのリストをカバーに変換する．
  SopBlock
  new_block(
    const vector<SopBitVect>& bv_list ///< [in] キューブを表すビットベクタのリスト
  )
  {
    SizeType nc = bv_list.size();
    auto dst = new_block(nc);
    auto dst_cube = _cube_begin(dst.body);
    for ( auto bv: bv_list ) {
      auto src_cube = _cube_begin(bv);
      cube_copy(dst_cube, src_cube);
      dst_cube += _cube_size();
    }
    dst.cube_num = nc;
    sort(dst);
    return dst;
  }

  /// @brief リテラルのリストのリストをカバーに変換する．
  SopBlock
  new_block(
    const vector<vector<Literal>>& cube_list ///< [in] カバーを表すリテラルのリストのリスト
  )
  {
    SizeType nc = cube_list.size();
    auto dst = new_block(nc);
    auto dst_cube = _cube_begin(dst.body);
    for ( auto& lit_list: cube_list ) {
      cube_set_literals(dst_cube, lit_list);
      dst_cube += _cube_size();
    }
    dst.cube_num = nc;
    sort(dst);
    return dst;
  }

  /// @brief リテラルのリストのリストをカバーに変換する．
  SopBlock
  new_block(
    initializer_list<initializer_list<Literal>>& cube_list ///< [in] カバーを表すリテラルのリストのリスト
  )
  {
    SizeType nc = cube_list.size();
    auto dst = new_block(nc);
    auto dst_cube = _cube_begin(dst.body);
    for ( auto& lit_list: cube_list ) {
      cube_set_literals(dst_cube, lit_list);
      dst_cube += _cube_size();
    }
    dst.cube_num = nc;
    sort(dst);
    return dst;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // キューブに対する処理
  // 基本的にはキューブはビットベクタの先頭アドレスで表す．
  // キューブのサイズ(変数の個数に依存)によってキューブの開始アドレスは変化する．
  //////////////////////////////////////////////////////////////////////

  /// @brief キューブを確保する．
  SopBitVect
  new_cube()
  {
    return SopBitVect(_cube_size());
  }

  /// @brief コピーしたキューブを作る．
  SopBitVect
  copy_cube(
    const SopBitVect& src_bv ///< [in] コピー元のビットベクタ
  )
  {
    return SopBitVect{src_bv};
  }

  /// @brief Literal のリストからキューブを作る．
  SopBitVect
  new_cube(
    const vector<Literal>& lit_list ///< [in] キューブを表すリテラルのリスト
  )
  {
    auto dst_bv = new_cube();
    auto dst_cube = _cube_begin(dst_bv);
    for ( auto lit: lit_list ) {
      cube_set_literal(dst_cube, lit);
    }
    return dst_bv;
  }

  /// @brief キューブ(を表すビットベクタ)をコピーする．
  void
  cube_copy(
    SopBitVectIter dst_cube,     ///< [in] 対象のビットベクタ
    SopBitVectConstIter src_cube ///< [in] コピー元のビットベクタ
  )
  {
    _copy(dst_cube, src_cube, 1);
  }

  /// @brief キューブ(を表すビットベクタ)をクリアする．
  void
  cube_clear(
    SopBitVectIter dst_cube ///< [in] 対象のビットベクタ
  )
  {
    auto end = _cube_end(dst_cube);
    for ( ; dst_cube != end; ++ dst_cube ) {
      *dst_cube = 0UL;
    }
  }

  /// @brief キューブにリテラルをセットする．
  void
  cube_set_literal(
    SopBitVectIter dst_cube, ///< [in] 対象のビットベクタ
    Literal lit              ///< [in] リテラル
  )
  {
    auto varid = lit.varid();
    auto inv = lit.is_negative();
    cube_set_literal(dst_cube, varid, inv);
  }

  /// @brief キューブにリテラルをセットする．
  void
  cube_set_literal(
    SopBitVectIter dst_cube, ///< [in] 対象のビットベクタ
    SizeType varid,          ///< [in] 変数番号
    bool inv                 ///< [in] 反転属性
  )
  {
    auto blk = _block_pos(varid);
    auto mask = get_mask(varid, inv);
    auto dst_p = dst_cube + blk;
    *dst_p |= mask;
  }

  /// @brief Literal のリストからキューブ(を表すビットベクタ)のコピーを行う．
  ///
  /// * dst_bv には十分な容量があると仮定する．
  void
  cube_set_literals(
    SopBitVectIter dst_cube,        ///< [in] 対象のビットベクタ
    const vector<Literal>& lit_list ///< [in] キューブを表すリテラルのリスト
  )
  {
    for ( auto lit: lit_list ) {
      cube_set_literal(dst_cube, lit);
    }
  }

  /// @brief Literal のリストからキューブ(を表すビットベクタ)のコピーを行う．
  ///
  /// * dst_cube には十分な容量があると仮定する．
  void
  cube_set_literals(
    SopBitVectIter dst_cube,            ///< [in] 対象のビットベクタ
    initializer_list<Literal>& lit_list ///< [in] キューブを表すリテラルのリスト初期化子
  )
  {
    for ( auto lit: lit_list ) {
      cube_set_literal(dst_cube, lit);
    }
  }

  /// @brief 空キューブかどうか調べる．
  bool
  cube_check_null(
    SopBitVectConstIter cube ///< [in] 対象のビットベクタ
  );

  /// @brief 2つのキューブの積を計算する．
  /// @retval true 積が空でなかった．
  /// @retval false 積が空だった．
  bool
  cube_product(
    SopBitVectIter dst_cube,       ///< [in] コピー先のビットベクタ
    SopBitVectConstIter src1_cube, ///< [in] 1つめのキューブを表すビットベクタ
    SopBitVectConstIter src2_cube  ///< [in] 2つめのキューブを表すビットベクタ
  );

  /// @brief キューブとリテラルの積を計算する．
  /// @retval true 積が空でなかった．
  /// @retval false 積が空だった．
  bool
  cube_product(
    SopBitVectIter dst_cube,       ///< [in] コピー先のビットベクタ
    SopBitVectConstIter src1_cube, ///< [in] 1つめのキューブを表すビットベクタ
    Literal lit                    ///< [in] リテラル
  );

  /// @brief キューブとリテラルの積を計算する．
  /// @retval true 積が空でなかった．
  /// @retval false 積が空だった．
  bool
  cube_product_int(
    SopBitVectIter dst_cube,             ///< [in] コピー先のビットベクタ
    Literal lit                          ///< [in] リテラル
  );

  /// @brief キューブによる商を求める．
  /// @return 正しく割ることができたら true を返す．
  bool
  cube_quotient(
    SopBitVectIter dst_cube,       ///< [in] コピー先のビットベクタ
    SopBitVectConstIter src1_cube, ///< [in] 被除数を表すビットベクタ
    SopBitVectConstIter src2_cube  ///< [in] 除数を表すビットベクタ
  );

  /// @brief リテラルによる商を求める．
  /// @return 正しく割ることができたら true を返す．
  bool
  cube_quotient(
    SopBitVectIter dst_cube,       ///< [in] コピー先のビットベクタ
    SopBitVectConstIter src1_cube, ///< [in] 被除数を表すビットベクタ
    Literal lit                    ///< [in] リテラル
  );

  /// @brief キューブ(を表すビットベクタ)の比較を行う．
  /// @retval -1 bv1 <  bv2
  /// @retval  0 bv1 == bv2
  /// @retval  1 bv1 >  bv2
  int
  cube_compare(
    SopBitVectConstIter src1_cube, ///< [in] 1つめのキューブを表すビットベクタ
    SopBitVectConstIter src2_cube  ///< [in] 2つめのキューブを表すビットベクタ
  );

  /// @brief キューブをソートする．
  void
  sort(
    SopBlock& block
  )
  {
    _sort(block.body, 0, block.cube_num);
  }

  /// @brief 2つのキューブ(を表すビットベクタ)を入れ替える．
  void
  cube_swap(
    SopBitVectIter cube1, ///< [in] 1つめのキューブの位置
    SopBitVectIter cube2  ///< [in] 2つめのキューブの位置
  )
  {
    _cube_save(cube1);
    cube_copy(cube1, cube2);
    _cube_restore(cube2);
  }

  /// @brief 3つのキューブ(を表すビットベクタ)を入れ替える．
  ///
  /// 1 <- 2, 2 <- 3, 3 <- 1 となる．
  void
  cube_rotate3(
    SopBitVectIter cube1, ///< [in] 1つめのキューブの位置
    SopBitVectIter cube2, ///< [in] 2つめのキューブの位置
    SopBitVectIter cube3  ///< [in] 3つめのキューブの位置
  )
  {
    _cube_save(cube1);
    cube_copy(cube1, cube2);
    cube_copy(cube2, cube3);
    _cube_restore(cube3);
  }

  /// @brief 4つのキューブ(を表すビットベクタ)を入れ替える．
  ///
  /// 1 <- 2, 2 <- 3, 3 <- 4, 4 <- 1 となる．
  void
  cube_rotate4(
    SopBitVectIter cube1, ///< [in] 1つめのキューブの位置
    SopBitVectIter cube2, ///< [in] 2つめのキューブの位置
    SopBitVectIter cube3, ///< [in] 3つめのキューブの位置
    SopBitVectIter cube4  ///< [in] 4つめのキューブの位置
  )
  {
    _cube_save(cube1);
    cube_copy(cube1, cube2);
    cube_copy(cube2, cube3);
    cube_copy(cube3, cube4);
    _cube_restore(cube4);
  }


protected:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief キューブ/カバー用の領域を確保する．
  SopBitVect
  _new_body(
    SizeType cube_num = 1 ///< [in] キューブ数
  )
  {
    auto size = _cube_size() * cube_num;
    return SopBitVect(size, 0UL);
  }

  /// @brief 単純なコピーを行う．
  void
  _copy(
    SopBitVectIter dst_cube,      ///< [in] コピー先のビットベクタ
    SopBitVectConstIter src_cube, ///< [in] コピー元のビットベクタ
    SizeType cube_num             ///< [in] キューブ数
  )
  {
    SizeType size = _cube_size() * cube_num;
    auto src_end = src_cube + _cube_size() * cube_num;
    std::copy(src_cube, src_end, dst_cube);
  }

  /// @brief マージソートを行う下請け関数
  ///
  /// bv[start] - bv[end - 1] の領域をソートする．
  void
  _sort(
    SopBitVect& bv, ///< [in] 対象のビットベクタ
    SizeType start, ///< [in] 開始位置
    SizeType end    ///< [in] 終了位置
  );

  /// @brief mTmpBlock に必要な領域を確保する．
  void
  _resize_buff(
    SizeType req_size ///< [in] 要求するキューブ数
  );

  /// @brief キューブ(を表すビットベクタ)をバッファにコピーする．
  void
  _cube_save(
    SopBitVectConstIter src_cube ///< [in] ソースのビットベクタ
  )
  {
    auto dst_cube = _cube_begin(mTmpBlock.body);
    cube_copy(dst_cube, src_cube);
  }

  /// @brief キューブ(を表すビットベクタ)をバッファからコピーする．
  void
  _cube_restore(
    SopBitVectIter dst_cube ///< [in] コピー先のビットベクタ
  )
  {
    auto src_cube = _cube_begin(mTmpBlock.body);
    cube_copy(dst_cube, src_cube);
  }

#if 0
  /// @brief キューブ位置を計算する．
  SopBitVectIter
  _cube_begin(
    SopBitVect& bv,  ///< [in] ビットベクタの先頭
    SizeType pos = 0 ///< [in] キューブ位置
  ) const
  {
    return bv.begin() + pos * _cube_size();
  }

  /// @brief キューブ位置を計算する．
  const SopBitVectConstIter
  _cube_begin(
    const SopBitVect& bv, ///< [in] ビットベクタの先頭
    SizeType pos = 0      ///< [in] キューブ位置
  ) const
  {
    return bv.begin() + pos * _cube_size();
  }

  /// @brief キューブの末尾を計算する．
  SopBitVectIter
  _cube_end(
    SopBitVectIter begin, ///< [in] キューブの先頭
    SizeType cube_num = 1 ///< [in] キューブ数
  ) const
  {
    return begin + _cube_size() * cube_num;
  }

  /// @brief キューブの末尾を計算する．
  SopBitVectConstIter
  _cube_end(
    SopBitVectConstIter begin, ///< [in] キューブの先頭
    SizeType cube_num = 1      ///< [in] キューブ数
  ) const
  {
    return begin + _cube_size() * cube_num;
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
    return cube_size(variable_num());
  }

  /// @brief パタンを作る．
  static
  SopPat
  bitpat(
    bool inv ///< [in] 反転属性
  )
  {
    return static_cast<SopPat>(inv ? SopPat::_0 : SopPat::_1);
  }

  /// @brief リテラルからブロック番号を得る．
  SizeType
  block_pos(
    Literal lit ///< 対象のリテラル
  )
  {
    auto varid = lit.varid();
    return _block_pos(varid);
  }

  /// @brief リテラルからマスクを作る．
  SopPatWord
  get_mask(
    Literal lit ///< 対象のリテラル
  )
  {
    auto varid = lit.varid();
    auto inv = lit.is_negative();
    return get_mask(varid, inv);
  }

  /// @brief リテラルからマスクを作る．
  SopPatWord
  get_mask(
    SizeType varid, ///< [in] 変数番号
    bool inv        ///< [in] 反転属性
  )
  {
    auto sft = _shift_num(varid);
    auto pat = bitpat(inv);
    auto mask = static_cast<SopPatWord>(pat) << sft;
    return mask;
  }
#endif


protected:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 作業用に用いられるビットベクタ
  SopBlock mTmpBlock;

};

END_NAMESPACE_YM_SOP

#endif // SOPMGR_H
