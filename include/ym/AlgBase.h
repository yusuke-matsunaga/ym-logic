#ifndef ALGBASE_H
#define ALGBASE_H

/// @file AlgBase.h
/// @brief AlgBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/Literal.h"
#include "ym/Expr.h"


BEGIN_NAMESPACE_YM_ALG

/// @brief AlgPat をパックしたワード型
using AlgPatWord = std::uint64_t;

/// @brief オール0
const AlgPatWord ALG_ALL0 = 0x0000000000000000ULL;

//////////////////////////////////////////////////////////////////////
/// @class AlgBase AlgBase.h "ym/AlgBase.h"
/// @brief AlgCube, AlgCover の共通な基底クラス
///
/// といっても実際の役割は入力数を覚えておくことと
/// 共通の関数群の提供
//////////////////////////////////////////////////////////////////////
class AlgBase
{
protected:

  /// @brief カバー/キューブの実体を表す型
  using Chunk = std::vector<AlgPatWord>;

  /// @brief 内部でキューブを表す型
  using Cube = Chunk::const_iterator;

  /// @brief 内部で代入先のキューブを表す型
  using DstCube = Chunk::iterator;


  //////////////////////////////////////////////////////////////////////
  /// @brief Cube の反復子を表すクラス
  //////////////////////////////////////////////////////////////////////
  class CubeIter
  {
  public:

    /// @brief コンストラクタ
    CubeIter(
      Cube cube_begin,   ///< [in] 先頭のキューブ
      SizeType cube_size ///< [in] キューブのサイズ
    ) : mCube{cube_begin},
	mCubeSize{cube_size}
    {
    }

    /// @brief デストラクタ
    ~CubeIter() = default;

    /// @brief キューブを取り出す．
    Cube
    operator*() const
    {
      return mCube;
    }

    /// @brief 次の要素に進める．
    CubeIter&
    operator++()
    {
      mCube += mCubeSize;
      return *this;
    }

    /// @brief 等価比較
    bool
    operator==(
      const CubeIter& right
    ) const
    {
      return mCube == right.mCube;
    }

    /// @brief 非等価比較
    bool
    operator!=(
      const CubeIter& right
    ) const
    {
      return !operator==(right);
    }


  private:
    //////////////////////////////////////////////////////////////////////
    // データメンバ
    //////////////////////////////////////////////////////////////////////

    // キューブ
    Cube mCube;

    // キューブサイズ
    SizeType mCubeSize;

  };


  //////////////////////////////////////////////////////////////////////
  /// @brief Cube のリストを表すクラス
  ///
  /// 内容は不変で読み出ししか行わない．
  //////////////////////////////////////////////////////////////////////
  class CubeList
  {
  public:

    /// @brief コンストラクタ
    CubeList(
      CubeIter begin, ///< [in] 開始位置
      CubeIter end    ///< [in] 終了位置
    ) : mBegin{begin},
	mEnd{end}
    {
    }

    /// @brief デストラクタ
    ~CubeList() = default;

    /// @brief 開始位置を返す．
    CubeIter
    begin() const
    {
      return mBegin;
    }

    /// @brief 終了位置を返す．
    CubeIter
    end() const
    {
      return mEnd;
    }


  private:
    //////////////////////////////////////////////////////////////////////
    // データメンバ
    //////////////////////////////////////////////////////////////////////

    // 開始位置
    CubeIter mBegin;

    // 終了位置
    CubeIter mEnd;

  };


  //////////////////////////////////////////////////////////////////////
  /// @brief DstCube のリストを表すクラス
  ///
  /// 末尾への追加しか行わない．
  //////////////////////////////////////////////////////////////////////
  class DstCubeList
  {
  public:

    /// @brief コンストラクタ
    DstCubeList(
      Chunk& chunk,       ///< [in] ビットベクタ本体
      SizeType cube_size, ///< [in] キューブサイズ
      SizeType offset = 0 ///< [in] オフセット
    ) : mChunk{chunk},
	mCubeSize{cube_size},
	mOffset{offset},
	mNum{0}
    {
    }

    /// @brief デストラクタ
    ~DstCubeList() = default;

    /// @brief 要素数を返す．
    SizeType
    num() const
    {
      return mNum;
    }

    /// @brief 末尾のキューブを返す．
    DstCube
    back()
    {
      return mChunk.begin() + mOffset + mNum * mCubeSize;
    }

    /// @brief 要素数を増やす．
    void
    inc()
    {
      ++ mNum;
    }


  private:
    //////////////////////////////////////////////////////////////////////
    // データメンバ
    //////////////////////////////////////////////////////////////////////

    // ビットベクタ本体
    Chunk& mChunk;

    // キューブサイズ
    SizeType mCubeSize;

    // オフセット
    SizeType mOffset;

    // 現在の要素数
    SizeType mNum;

  };


public:

  /// @brief コンストラクタ
  AlgBase(
    SizeType var_num ///< [in] 変数の数
  ) : mVarNum{var_num}
  {
  }

  /// @brief デストラクタ
  ~AlgBase() = default;


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


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスで用いられる便利関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ビットベクタからワードを取り出す．
  AlgPatWord
  _get_word(
    Cube cube,   ///< [in] キューブを表す反復子
    SizeType blk ///< [in] ブロック番号
  ) const
  {
    return *(cube + blk);
  }

  /// @brief ビットベクタにワードを書き込む．
  void
  _set_word(
    DstCube cube,   ///< [in] キューブを表す反復子
    SizeType blk,   ///< [in] ブロック番号
    AlgPatWord word ///< [in] 書き込む値
  ) const
  {
    *(cube + blk) = word;
  }

  /// @brief ビットベクタからパタンを取り出す．
  AlgPat
  _get_pat(
    Cube cube,   ///< [in] キューブを表す反復子
    SizeType var ///< [in] 変数 ( 0 <= var_id.val() < variable_num() )
  ) const
  {
    _check_var(var);

    auto blk = _block_pos(var);
    auto sft = _shift_num(var);
    auto word = _get_word(cube, blk);
    return static_cast<AlgPat>((word >> sft) & 3ULL);
  }

  /// @brief リテラル数を数える．
  SizeType
  _literal_num(
    SizeType cube_num, ///< [in] キューブ数
    const Chunk& chunk ///< [in] 本体のビットベクタ
  ) const;

  /// @brief 指定されたリテラルの出現頻度を数える．
  SizeType
  _literal_num(
    SizeType cube_num,  ///< [in] キューブ数
    const Chunk& chunk, ///< [in] 本体のビットベクタ
    Literal lit         ///< [in] 対象のリテラル
  ) const
  {
    auto varid = lit.varid();
    auto inv = lit.is_negative();
    return _literal_num(cube_num, chunk, varid, inv);
  }

  /// @brief 指定されたリテラルの出現頻度を数える．
  SizeType
  _literal_num(
    SizeType cube_num,  ///< [in] キューブ数
    const Chunk& chunk, ///< [in] 本体のビットベクタ
    SizeType varid,     ///< [in] 変数番号
    bool inv            ///< [in] 反転属性
  ) const
  {
    auto blk = _block_pos(varid);
    auto mask = _get_mask(varid, inv);
    SizeType ans = 0;
    auto cube_list = _cube_list(chunk, 0, cube_num);
    for ( auto cube: cube_list ) {
      if ( _cube_check_literal(cube, varid, inv) ) {
	++ ans;
      }
    }
    return ans;
  }

  /// @brief 内容をリテラルのリストのリストに変換する．
  vector<vector<Literal>>
  _literal_list(
    SizeType cube_num, ///< [in] キューブ数
    const Chunk& chunk ///< [in] 本体のビットベクタ
  ) const
  {
    vector<vector<Literal>> ans_list;
    ans_list.reserve(cube_num);

    auto cube_list = _cube_list(chunk, 0, cube_num);
    for ( auto cube: cube_list ) {
      auto lit_list = _cube_literal_list(cube);
      ans_list.push_back(lit_list);
    }

    return ans_list;
  }

  /// @brief ビットベクタからハッシュ値を計算する．
  SizeType
  _hash(
    SizeType cube_num, ///< [in] キューブ数
    const Chunk& chunk ///< [in] 本体のビットベクタ
  ) const
  {
    // キューブは常にソートされているので
    // 順番は考慮する必要はない．
    // ただおなじキューブの中のビットに関しては
    // 本当は区別しなければならないがどうしようもないので
    // 16ビットに区切って単純に XOR を取る．
    SizeType ans = 0;
    auto begin = _cube(chunk);
    auto end = _cube(chunk, cube_num);
    for ( auto p = begin; p != end; ++ p ) {
      auto pat = *p;
      ans ^= (pat & 0xFFFFULL); pat >>= 16;
      ans ^= (pat & 0xFFFFULL); pat >>= 16;
      ans ^= (pat & 0xFFFFULL); pat >>= 16;
      ans ^= pat;
    }
    return ans;
  }

  /// @brief Expr に変換する．
  Expr
  _to_expr(
    SizeType cube_num, ///< [in] キューブ数
    const Chunk& chunk ///< [in] 本体のビットベクタ
  ) const;

  /// @brief カバー/キューブの内容を出力する．
  void
  _print(
    ostream& s,         ///< [in] 出力先のストリーム
    const Chunk& chunk, ///< [in] カバー/キューブを表すビットベクタ
    SizeType begin,     ///< [in] キューブの開始位置
    SizeType end,       ///< [in] キューブの終了位置(実際の末尾 + 1)
    const vector<string>& varname_list ///< [in] 変数名のリスト
    = {}
  ) const;

  /// @brief キューブの内容を出力する．
  void
  _print(
    ostream& s,         ///< [in] 出力先のストリーム
    Cube cube,          ///< [in] キューブ
    const vector<string>& varname_list ///< [in] 変数名のリスト
    = {}
  ) const;

  /// @brief 内容を出力する(デバッグ用)．
  void
  _debug_print(
    ostream& s,        ///< [in] 出力ストリーム
    SizeType cube_num, ///< [in] キューブ数
    const Chunk& chunk ///< [in] 本体のビットベクタ
  ) const;

  /// @brief 内容を出力する(デバッグ用)．
  void
  _debug_print(
    ostream& s, ///< [in] 出力ストリーム
    Cube cube   ///< [in] キューブ
  ) const;

  /// @brief 内容を出力する(デバッグ用)．
  void
  _debug_print(
    ostream& s,  ///< [in] 出力ストリーム
    DstCube cube ///< [in] キューブ
  ) const;


protected:
  //////////////////////////////////////////////////////////////////////
  // キューブに対する処理
  // 基本的にはキューブはビットベクタの先頭アドレスで表す．
  // キューブのサイズ(変数の個数に依存)によってキューブの開始アドレスは変化する．
  //////////////////////////////////////////////////////////////////////

  /// @brief キューブ(を表すビットベクタ)をコピーする．
  void
  _cube_copy(
    DstCube dst_cube, ///< [in] 対象のビットベクタ
    Cube src_cube     ///< [in] コピー元のビットベクタ
  ) const
  {
    auto src_end = _cube_end(src_cube);
    std::copy(src_cube, src_end, dst_cube);
  }

  /// @brief キューブ(を表すビットベクタ)をクリアする．
  void
  _cube_clear(
    DstCube dst_cube ///< [in] 対象のビットベクタ
  ) const
  {
    auto end = _cube_end(dst_cube);
    for ( ; dst_cube != end; ++ dst_cube ) {
      *dst_cube = ALG_ALL0;
    }
  }

  /// @brief キューブのリテラル数を返す．
  SizeType
  _cube_literal_num(
    Cube src_cube ///< [in] キューブ
  ) const;

  /// @brief キューブが指定されたリテラルを含むか調べる．
  bool
  _cube_check_literal(
    Cube src_cube,  ///< [in] キューブ
    SizeType varid, ///< [in] 変数番号
    bool inv        ///< [in] 反転属性
  ) const
  {
    auto blk = _block_pos(varid);
    auto mask = _get_mask(varid, inv);
    auto word = _get_word(src_cube, blk);
    if ( (word & mask) == mask ) {
      return true;
    }
    return false;
  }

  /// @brief キューブの内容をリテラルのリストのリストに変換する．
  vector<Literal>
  _cube_literal_list(
    Cube cube ///< [in] キューブ
  ) const
  {
    vector<Literal> lit_list;
    lit_list.reserve(_cube_literal_num(cube));
    auto cube_end = _cube_end(cube);
    SizeType base = 0;
    for ( ; cube != cube_end; ++ cube, base += 32 ) {
      _word_literal_list(*cube, base, lit_list);
    }
    return lit_list;
  }

  /// @brief キューブにリテラルをセットする．
  void
  _cube_set_literal(
    DstCube dst_cube, ///< [in] 対象のビットベクタ
    Literal lit       ///< [in] リテラル
  ) const
  {
    auto varid = lit.varid();
    auto inv = lit.is_negative();
    _cube_set_literal(dst_cube, varid, inv);
  }

  /// @brief キューブにリテラルをセットする．
  void
  _cube_set_literal(
    DstCube dst_cube, ///< [in] 対象のビットベクタ
    SizeType varid,   ///< [in] 変数番号
    bool inv          ///< [in] 反転属性
  ) const
  {
    auto blk = _block_pos(varid);
    auto mask = _get_mask(varid, inv);
    auto dst_p = dst_cube + blk;
    *dst_p |= mask;
  }

  /// @brief Literal のリストからキューブ(を表すビットベクタ)のコピーを行う．
  ///
  /// * dst_bv には十分な容量があると仮定する．
  void
  _cube_set_literals(
    DstCube dst_cube,               ///< [in] 対象のビットベクタ
    const vector<Literal>& lit_list ///< [in] キューブを表すリテラルのリスト
  ) const
  {
    for ( auto lit: lit_list ) {
      _cube_set_literal(dst_cube, lit);
    }
  }

  /// @brief Literal のリストからキューブ(を表すビットベクタ)のコピーを行う．
  ///
  /// * dst_cube には十分な容量があると仮定する．
  void
  _cube_set_literals(
    DstCube dst_cube,                   ///< [in] 対象のビットベクタ
    initializer_list<Literal>& lit_list ///< [in] キューブを表すリテラルのリスト初期化子
  ) const
  {
    for ( auto lit: lit_list ) {
      _cube_set_literal(dst_cube, lit);
    }
  }

  /// @brief 空キューブかどうか調べる．
  bool
  _cube_check_null(
    Cube cube ///< [in] 対象のビットベクタ
  ) const
  {
    auto cube_end = _cube_end(cube);
    for ( ; cube != cube_end; ++ cube ) {
      if ( *cube != ALG_ALL0 ) {
	return false;
      }
    }
    return true;
  }

  /// @brief 2つのキューブの積を計算する．
  /// @retval true 積が空でなかった．
  /// @retval false 積が空だった．
  bool
  _cube_product(
    DstCube dst_cube, ///< [in] コピー先のビットベクタ
    Cube cube1,       ///< [in] 1つめのキューブを表すビットベクタ
    Cube cube2        ///< [in] 2つめのキューブを表すビットベクタ
  ) const
  {
    auto dst_end = _cube_end(dst_cube);
    for ( ; dst_cube != dst_end; ++ dst_cube, ++ cube1, ++ cube2 ) {
      auto tmp = *cube1 | *cube2;
      if ( _word_check_conflict(tmp) ) {
	return false;
      }
      *dst_cube = tmp;
    }
    return true;
  }

  /// @brief キューブとリテラルの積を計算する．
  /// @retval true 積が空でなかった．
  /// @retval false 積が空だった．
  bool
  _cube_product(
    DstCube dst_cube,  ///< [in] コピー先のビットベクタ
    Cube src_cube,     ///< [in] 1つめのキューブを表すビットベクタ
    Literal lit        ///< [in] リテラル
  ) const
  {
    auto varid = lit.varid();
    auto inv = lit.is_negative();
    auto blk = _block_pos(varid);
    auto sft = _shift_num(varid);
    auto pat1 = _get_mask(varid, inv);
    auto mask = 3UL << sft;

    // 単純には答の積項数は2つの積項数の積だが
    // 相反するリテラルを含む積は数えない．
    auto tmp = _get_word(src_cube, blk) | pat1;
    if ( (tmp & mask) == mask ) {
      // 相反するリテラルがあった．
      return false;
    }
    _cube_copy(dst_cube, src_cube);
    _set_word(dst_cube, blk, tmp);
    return true;
  }

  /// @brief キューブとリテラルの積を計算する．
  /// @retval true 積が空でなかった．
  /// @retval false 積が空だった．
  bool
  _cube_product_int(
    DstCube dst_cube, ///< [in] コピー先のビットベクタ
    Literal lit       ///< [in] リテラル
  ) const
  {
    auto varid = lit.varid();
    auto inv = lit.is_negative();
    auto blk = _block_pos(varid);
    auto sft = _shift_num(varid);
    auto pat1 = _get_mask(varid, inv);
    auto mask = 3UL << sft;

    // 単純には答の積項数は2つの積項数の積だが
    // 相反するリテラルを含む積は数えない．
    auto tmp = _get_word(dst_cube, blk) | pat1;
    if ( (tmp & mask) == mask ) {
      // 相反するリテラルがあった．
      return false;
    }
    _set_word(dst_cube, blk, tmp);
    return true;
  }

  /// @brief キューブによる商を求める．
  /// @return 正しく割ることができたら true を返す．
  bool
  _cube_quotient(
    DstCube dst_cube, ///< [in] コピー先のビットベクタ
    Cube cube1,       ///< [in] 被除数を表すビットベクタ
    Cube cube2        ///< [in] 除数を表すビットベクタ
  ) const
  {
    auto dst_end = _cube_end(dst_cube);
    for ( ; dst_cube != dst_end; ++ dst_cube, ++ cube1, ++ cube2 ) {
      auto pat1 = *cube1;
      auto pat2 = *cube2;
      if ( (~pat1 & pat2) != ALG_ALL0 ) {
	// この場合の dst の値は不定
	return false;
      }
      *dst_cube = pat1 & ~pat2;
    }
    return true;
  }

  /// @brief リテラルによる商を求める．
  /// @return 正しく割ることができたら true を返す．
  bool
  _cube_quotient(
    DstCube dst_cube, ///< [in] コピー先のビットベクタ
    Cube src_cube,    ///< [in] 被除数を表すビットベクタ
    Literal lit       ///< [in] リテラル
  ) const
  {
    auto varid = lit.varid();
    auto inv = lit.is_negative();
    auto blk = _block_pos(varid);
    auto sft = _shift_num(varid);
    auto pat1 = _get_mask(varid, inv);
    auto mask = 3UL << sft;
    auto nmask = ~mask;
    auto src_p = src_cube + blk;
    if ( (*src_p & mask) == pat1 ) {
      _cube_copy(dst_cube, src_cube);
      auto dst_p = dst_cube + blk;
      *dst_p &= nmask;
      return true;
    }
    return false;
  }

  /// @brief キューブ(を表すビットベクタ)の比較を行う．
  /// @retval -1 bv1 <  bv2
  /// @retval  0 bv1 == bv2
  /// @retval  1 bv1 >  bv2
  int
  _cube_compare(
    Cube cube1, ///< [in] 1つめのキューブを表すビットベクタ
    Cube cube2  ///< [in] 2つめのキューブを表すビットベクタ
  ) const
  {
    auto src1_end = _cube_end(cube1);
    for ( ; cube1 != src1_end; ++ cube1, ++ cube2 ) {
      auto pat1 = *cube1;
      auto pat2 = *cube2;
      if ( pat1 < pat2 ) {
	return -1;
      }
      else if ( pat1 > pat2 ) {
	return 1;
      }
    }
    return 0;
  }


protected:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief キューブ/カバー用の領域を確保する．
  Chunk
  _new_chunk(
    SizeType cube_num = 1 ///< [in] キューブ数
  ) const
  {
    auto size = _cube_size() * cube_num;
    return Chunk(size, ALG_ALL0);
  }

  /// @brief キューブのリストを返す．
  CubeList
  _cube_list(
    const Chunk& chunk, ///< [in] ビットベクタの先頭
    SizeType begin_pos, ///< [in] 開始位置
    SizeType end_pos    ///< [in] 終了位置
  ) const
  {
    auto begin = CubeIter{_cube(chunk, begin_pos), _cube_size()};
    auto end = CubeIter{_cube(chunk, end_pos), _cube_size()};
    return CubeList{begin, end};
  }

  /// @brief キューブを取り出す．
  Cube
  _cube(
    const Chunk& chunk, ///< [in] ビットベクタの先頭
    SizeType pos = 0    ///< [in] キューブ位置
  ) const
  {
    return chunk.begin() + pos * _cube_size();
  }

  /// @brief キューブの末尾を計算する．
  Cube
  _cube_end(
    Cube cube ///< [in] キューブの先頭
  ) const
  {
    return cube + _cube_size();
  }

  /// @brief キューブのリストを返す．
  DstCubeList
  _cube_list(
    Chunk& chunk,      ///< [in] ビットベクタの先頭
    SizeType begin = 0 ///< [in] 開始位置
  ) const
  {
    return DstCubeList{chunk, _cube_size(), begin};
  }

  /// @brief キューブを取り出す．
  DstCube
  _dst_cube(
    Chunk& chunk,    ///< [in] ビットベクタの先頭
    SizeType pos = 0 ///< [in] キューブ位置
  ) const
  {
    return chunk.begin() + pos * _cube_size();
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
    return (variable_num() + 31) / 32;
  }

  /// @brief パタンを作る．
  static
  AlgPat
  _bitpat(
    bool inv ///< [in] 反転属性
  )
  {
    return static_cast<AlgPat>(inv ? AlgPat::_0 : AlgPat::_1);
  }

  /// @brief ワード中にコンフリクトがあるか調べる．
  static
  bool
  _word_check_conflict(
    AlgPatWord word ///< [in] 対象のワード
  )
  {
    const AlgPatWord mask1 = 0x5555555555555555ULL;
    const AlgPatWord mask2 = 0xAAAAAAAAAAAAAAAAULL;
    auto tmp1 = word & mask1;
    auto tmp2 = word & mask2;
    if ( (tmp1 & (tmp2 >> 1)) != ALG_ALL0 ) {
      // 同じ変数の異なる極性のリテラルがあった．
      return true;
    }
    return false;
  }

  /// @brief ワード中のリテラルをリストに追加する．
  static
  void
  _word_literal_list(
    AlgPatWord word,          ///< [in] 対象のワード
    SizeType base,            ///< [in] 変数番号の開始位置
    vector<Literal>& lit_list ///< [out] リテラルを格納するリスト
  );

  /// @brief リテラルからブロック番号を得る．
  SizeType
  _block_pos(
    Literal lit ///< 対象のリテラル
  ) const
  {
    auto varid = lit.varid();
    return _block_pos(varid);
  }

  /// @brief リテラルからマスクを作る．
  AlgPatWord
  _get_mask(
    Literal lit ///< 対象のリテラル
  ) const
  {
    auto varid = lit.varid();
    auto inv = lit.is_negative();
    return _get_mask(varid, inv);
  }

  /// @brief リテラルからマスクを作る．
  AlgPatWord
  _get_mask(
    SizeType varid, ///< [in] 変数番号
    bool inv        ///< [in] 反転属性
  ) const
  {
    auto sft = _shift_num(varid);
    auto pat = _bitpat(inv);
    auto mask = static_cast<AlgPatWord>(pat) << sft;
    return mask;
  }

  /// @brief 入力数が同じかチェックする．
  void
  _check_size(
    const AlgBase& right
  ) const
  {
    if ( variable_num() != right.variable_num() ) {
      throw std::invalid_argument("variable_num() is different from each other");
    }
  }

  /// @brief リテラルが範囲内かチェックする．
  void
  _check_lit(
    Literal lit
  ) const
  {
    if ( lit.varid() >= variable_num() ) {
      throw std::out_of_range{"literal is out of range"};
    }
  }

  /// @brief リテラルが範囲内かチェックする．
  void
  _check_lits(
    const vector<Literal>& lit_list
  ) const
  {
    for ( auto lit: lit_list ) {
      _check_lit(lit);
    }
  }

  /// @brief リテラルが範囲内かチェックする．
  void
  _check_lits(
    std::initializer_list<Literal>& lit_list
  ) const
  {
    for ( auto lit: lit_list ) {
      _check_lit(lit);
    }
  }

  /// @brief 変数が範囲内かチェックする．
  void
  _check_var(
    SizeType var
  ) const
  {
    if ( var >= variable_num() ) {
      throw std::out_of_range{"var is out of range"};
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力数
  SizeType mVarNum;

};

END_NAMESPACE_YM_ALG

#endif // ALGBASE_H
