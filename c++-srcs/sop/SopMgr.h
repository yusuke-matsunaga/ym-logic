#ifndef SOPMGR_H
#define SOPMGR_H

/// @file SopMgr.h
/// @brief SopMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
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
class SopMgr
{
public:

  /// @brief コンストラクタ
  SopMgr(
    SizeType variable_num, ///< [in] 変数の数
    SizeType buff_size = 0 ///< [in] バッファサイズ
  ) : mVarNum{variable_num}
  {
    if ( buff_size > 0 ) {
      _resize_buff(buff_size);
    }
  }

  /// @brief デストラクタ
  ~SopMgr()
  {
    delete [] mTmpBuff;
  }


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
    SizeType cap = block.capacity;
    auto body = block.body;
    return SopCover{variable_num(), nc, cap, body};
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
    ASSERT_COND( 0 <= var && var < variable_num() );

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
  )
  {
    auto varid = lit.varid();
    auto inv = lit.is_negative();
    return literal_num(block, varid, inv);
  }

  /// @brief ビットベクタ上の特定のリテラルの出現頻度を数える．
  SizeType
  literal_num(
    const SopBlock& block, ///< [in] 対象のブロック
    SizeType varid,        ///< [in] 変数番号
    bool inv               ///< [in] 反転属性
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
  // SopBlock の生成と開放
  //////////////////////////////////////////////////////////////////////

  /// @brief カバーを確保する．
  SopBlock
  new_block(
    SizeType cube_num ///< [in] キューブ数
  )
  {
    auto bv = _new_body(cube_num);
    return SopBlock{0, cube_num, bv};
  }

  /// @brief カバーのコピーを行う．
  SopBlock
  new_block(
    const SopBlock& src ///< [in] ソースのカバー
  )
  {
    SizeType nc = src.cube_num;
    auto dst = new_block(nc);
    bcopy(src.body, dst.body, nc * _cube_size() * sizeof(SopBitVect));
    dst.cube_num = nc;
    return dst;
  }

  /// @brief キューブのリストからカバーを生成する．
  SopBlock
  new_block(
    const vector<SopCube>& cube_list ///< [in] キューブのリスト
  )
  {
    SizeType nc = cube_list.size();
    auto dst = new_block(nc);
    auto dst_bv = dst.body;
    for ( auto& cube: cube_list ) {
      auto bv = cube.mBody;
      cube_copy(dst_bv, bv);
      dst_bv += _cube_size();
    }
    dst.cube_num = nc;
    sort(dst);
    return dst;
  }

  /// @brief ビットベクタのリストをカバーに変換する．
  SopBlock
  new_block(
    const vector<SopBitVect*>& bv_list ///< [in] キューブを表すビットベクタのリスト
  )
  {
    SizeType nc = bv_list.size();
    auto dst = new_block(nc);
    auto dst_bv = dst.body;
    for ( auto bv: bv_list ) {
      cube_copy(dst_bv, bv);
      dst_bv += _cube_size();
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
    auto dst_bv = dst.body;
    for ( auto& lit_list: cube_list ) {
      cube_set_literals(dst_bv, lit_list);
      dst_bv += _cube_size();
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
    auto dst_bv = dst.body;
    for ( auto& lit_list: cube_list ) {
      cube_set_literals(dst_bv, lit_list);
      dst_bv += _cube_size();
    }
    dst.cube_num = nc;
    sort(dst);
    return dst;
  }

  /// @brief カバーの領域を開放する．
  static
  void
  delete_block(
    const SopBlock& cover
  )
  {
    _delete_body(cover.body);
  }

  /// @brief カバーの内容をコピーする．
  void
  copy_block(
    SopBlock& dst,      ///< [in] コピー先のブロック
    const SopBlock& src ///< [in] コピー元のブロック
  )
  {
    ASSERT_COND( dst.cube_num >= src.cube_num );
    _copy(dst.body, src.body, src.cube_num);
    dst.cube_num = src.cube_num;
  }

  /// @brief 指定されたキューブの先頭を求める．
  SopBitVect*
  cube_top(
    const SopBlock& block, ///< [in] カバーの内容
    SizeType cube_id       ///< [in] キューブ番号
  ) const
  {
    if ( cube_id >= block.cube_num ) {
      throw std::out_of_range{"cube_id is out of range"};
    }
    return _calc_offset(block.body, cube_id);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // キューブに対する処理
  // 基本的にはキューブはビットベクタの先頭アドレスで表す．
  // キューブのサイズ(変数の個数に依存)によってキューブの開始アドレスは変化する．
  //////////////////////////////////////////////////////////////////////

  /// @brief キューブを確保する．
  SopBitVect*
  new_bv()
  {
    return _new_body(1);
  }

  /// @brief コピーしたキューブを作る．
  SopBitVect*
  new_bv(
    const SopBitVect* src_bv ///< [in] コピー元のビットベクタ
  )
  {
    auto dst_bv = new_bv();
    cube_copy(dst_bv, src_bv);
    return dst_bv;
  }

  /// @brief Literal のリストからキューブを作る．
  SopBitVect*
  new_bv(
    const vector<Literal>& lit_list ///< [in] キューブを表すリテラルのリスト
  )
  {
    auto dst_bv = new_bv();
    for ( auto lit: lit_list ) {
      cube_set_literal(dst_bv, lit);
    }
    return dst_bv;
  }

  /// @brief キューブの領域を開放する．
  static
  void
  delete_bv(
    SopBitVect* bv
  )
  {
    _delete_body(bv);
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

  /// @brief キューブ(を表すビットベクタ)をクリアする．
  void
  cube_clear(
    SopBitVect* dst_bv ///< [in] 対象のビットベクタ
  )
  {
    SizeType size = _cube_size() * sizeof(SopBitVect);
    bzero(dst_bv, size);
  }

  /// @brief キューブにリテラルをセットする．
  void
  cube_set_literal(
    SopBitVect* dst_bv, ///< [in] 対象のビットベクタ
    Literal lit         ///< [in] リテラル
  )
  {
    auto varid = lit.varid();
    auto inv = lit.is_negative();
    cube_set_literal(dst_bv, varid, inv);
  }

  /// @brief キューブにリテラルをセットする．
  void
  cube_set_literal(
    SopBitVect* dst_bv, ///< [in] 対象のビットベクタ
    SizeType varid,     ///< [in] 変数番号
    bool inv            ///< [in] 反転属性
  )
  {
    auto blk = _block_pos(varid);
    auto sft = _shift_num(varid);
    auto pat = bitvect(inv);
    auto mask = pat << sft;
    dst_bv[blk] |= mask;
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
    SopBitVect* dst_bv,                 ///< [in] 対象のビットベクタ
    initializer_list<Literal>& lit_list ///< [in] キューブを表すリテラルのリスト初期化子
  )
  {
    for ( auto lit: lit_list ) {
      cube_set_literal(dst_bv, lit);
    }
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

  /// @brief キューブ(を表すビットベクタ)の比較を行う．
  /// @retval -1 bv1 <  bv2
  /// @retval  0 bv1 == bv2
  /// @retval  1 bv1 >  bv2
  int
  cube_compare(
    const SopBitVect* bv1, ///< [in] 1つめのキューブを表すビットベクタ
    const SopBitVect* bv2  ///< [in] 2つめのキューブを表すビットベクタ
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
    SopBitVect* bv1, ///< [in] 1つめのキューブを表すビットベクタ
    SopBitVect* bv2  ///< [in] 2つめのキューブを表すビットベクタ
  )
  {
    _cube_save(bv1);
    cube_copy(bv1, bv2);
    _cube_restore(bv2);
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
    _cube_save(bv1);
    cube_copy(bv1, bv2);
    cube_copy(bv2, bv3);
    _cube_restore(bv3);
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
    _cube_save(bv1);
    cube_copy(bv1, bv2);
    cube_copy(bv2, bv3);
    cube_copy(bv3, bv4);
    _cube_restore(bv4);
  }


protected:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

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

  /// @brief マージソートを行う下請け関数
  ///
  /// bv[start] - bv[end - 1] の領域をソートする．
  void
  _sort(
    SopBitVect* bv, ///< [in] 対象のビットベクタ
    SizeType start, ///< [in] 開始位置
    SizeType end    ///< [in] 終了位置
  );

  /// @brief mTmpBuff に必要な領域を確保する．
  void
  _resize_buff(
    SizeType req_size ///< [in] 要求するキューブ数
  );

  /// @brief キューブ(を表すビットベクタ)をバッファにコピーする．
  void
  _cube_save(
    const SopBitVect* src_bv ///< [in] ソースのビットベクタ
  )
  {
    cube_copy(mTmpBuff, src_bv);
  }

  /// @brief キューブ(を表すビットベクタ)をバッファからコピーする．
  void
  _cube_restore(
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
  ) const
  {
    return bv + cube_num * _cube_size();
  }

  /// @brief ビットベクタの末尾を計算する．
  const SopBitVect*
  _calc_offset(
    const SopBitVect* bv, ///< [in] ビットベクタの先頭
    SizeType cube_num     ///< [in] キューブ数
  ) const
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
  bitvect(
    bool inv ///< [in] 反転属性
  )
  {
    return static_cast<SopBitVect>(inv ? SopPat::_0 : SopPat::_1);
  }


protected:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数の数
  SizeType mVarNum;

  // 作業用に用いられるビットベクタ
  SopBitVect* mTmpBuff{nullptr};

  // mTmpBuff のキューブ数
  SizeType mTmpBuffSize{0};

};

END_NAMESPACE_YM_SOP

#endif // SOPMGR_H
