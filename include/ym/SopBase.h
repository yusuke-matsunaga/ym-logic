#ifndef SOPBASE_H
#define SOPBASE_H

/// @file SopBase.h
/// @brief SopBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/Literal.h"
#include "ym/Expr.h"


BEGIN_NAMESPACE_YM_SOP

/// @brief SopPat をパックしたワード型
using SopPatWord = std::uint64_t;

/// @brief オール0
const SopPatWord SOP_ALL0 = 0x0000000000000000ULL;

/// @brief オール1
const SopPatWord SOP_ALL1 = 0xFFFFFFFFFFFFFFFFULL;

//////////////////////////////////////////////////////////////////////
/// @class SopBase SopBase.h "SopBase.h"
/// @brief SopCover/SopCube に共通な基底クラス
///
/// 入力数を持つだけのクラス．
/// 主に SopCover/SopCube の実装用の関数を提供する．
//////////////////////////////////////////////////////////////////////
class SopBase
{
protected:

  /// @brief カバー/キューブの実体を表す型
  using Chunk = std::vector<SopPatWord>;

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
  explicit
  SopBase(
    SizeType var_num = 0 ///< [in] 変数の数
  ) : mVarNum{var_num}
  {
  }

  /// @brief デストラクタ
  ~SopBase() = default;


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
  // 継承クラスで用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ビットベクタからワードを取り出す．
  SopPatWord
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
    SopPatWord word ///< [in] 書き込む値
  ) const
  {
    *(cube + blk) = word;
  }

  /// @brief ビットベクタからパタンを取り出す．
  SopPat
  _get_pat(
    Cube cube,   ///< [in] キューブを表す反復子
    SizeType var ///< [in] 変数 ( 0 <= var_id.val() < variable_num() )
  ) const
  {
    SizeType blk = _block_pos(var);
    SizeType sft = _shift_num(var);
    auto word = _get_word(cube, blk);
    return static_cast<SopPat>((word >> sft) & 3ULL);
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
  std::vector<std::vector<Literal>>
  _literal_list(
    SizeType cube_num, ///< [in] キューブ数
    const Chunk& chunk ///< [in] 本体のビットベクタ
  ) const
  {
    std::vector<std::vector<Literal>> ans_list;
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

  /// @brief カバーの内容を出力する．
  void
  _print(
    std::ostream& s,    ///< [in] 出力先のストリーム
    const Chunk& chunk, ///< [in] カバー/キューブを表すビットベクタ
    SizeType begin,     ///< [in] キューブの開始位置
    SizeType end,       ///< [in] キューブの終了位置(実際の末尾 + 1)
    const std::vector<std::string>& varname_list ///< [in] 変数名のリスト
    = {}
  ) const;

  /// @brief キューブの内容を出力する．
  void
  _print(
    std::ostream& s,                             ///< [in] 出力先のストリーム
    Cube cube,                                   ///< [in] キューブ
    const std::vector<std::string>& varname_list ///< [in] 変数名のリスト
    = {}
  ) const;

  /// @brief カバーの内容を blif のカバー形式で出力する．
  void
  _write_cover(
    std::ostream& s,
    const Chunk& chunk,
    SizeType begin,
    SizeType end
  ) const;

  /// @brief キューブの内容を blif のカバー形式で出力する．
  void
  _write_cube(
    std::ostream& s,
    Cube cube
  ) const;

  /// @brief 内容を出力する(デバッグ用)．
  void
  _debug_print(
    std::ostream& s,   ///< [in] 出力ストリーム
    SizeType cube_num, ///< [in] キューブ数
    const Chunk& chunk ///< [in] 本体のビットベクタ
  ) const;

  /// @brief 内容を出力する(デバッグ用)．
  void
  _debug_print(
    std::ostream& s, ///< [in] 出力ストリーム
    Cube cube        ///< [in] キューブ
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
    if ( (word | mask) == mask ) {
      return true;
    }
    return false;
  }

  /// @brief キューブの内容をリテラルのリストのリストに変換する．
  std::vector<Literal>
  _cube_literal_list(
    Cube cube ///< [in] キューブ
  ) const
  {
    std::vector<Literal> lit_list;
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
    *dst_p &= mask;
  }

  /// @brief Literal のリストからキューブ(を表すビットベクタ)のコピーを行う．
  ///
  /// * dst_bv には十分な容量があると仮定する．
  void
  _cube_set_literals(
    DstCube dst_cube,                    ///< [in] 対象のビットベクタ
    const std::vector<Literal>& lit_list ///< [in] キューブを表すリテラルのリスト
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
    DstCube dst_cube,                        ///< [in] 対象のビットベクタ
    std::initializer_list<Literal>& lit_list ///< [in] キューブを表すリテラルのリスト初期化子
  ) const
  {
    for ( auto lit: lit_list ) {
      _cube_set_literal(dst_cube, lit);
    }
  }

  /// @brief 不正なキューブにする．
  void
  _cube_make_invalid(
    DstCube dst_cube ///< [in] 対象のビットベクタ
  ) const
  {
    *dst_cube = SOP_ALL0;
  }

  /// @brief 不正なキューブかどうか調べる．
  bool
  _cube_is_invalid(
    Cube cube ///< [in] 対象のビットベクタ
  ) const
  {
    return ( *cube == SOP_ALL0 );
  }

  /// @brief 空キューブかどうか調べる．
  bool
  _cube_check_null(
    Cube cube ///< [in] 対象のビットベクタ
  ) const
  {
    auto cube_end = _cube_end(cube);
    for ( ; cube != cube_end; ++ cube ) {
      if ( *cube != SOP_ALL1 ) {
	return false;
      }
    }
    return true;
  }

  /// @brief 正常なキューブかどうか調べる．
  bool
  _cube_sanity_check(
    Cube cube ///< [in] 対象のビットベクタ
  ) const
  {
    auto cube_end = _cube_end(cube);
    for ( ; cube != cube_end; ++ cube ) {
      if ( _word_check_conflict(*cube) ) {
	// 不正なパタンがあった．
	std::cout << "conflict pattern: "
		  << std::hex << *cube << std::dec << std::endl;
	goto error;
      }
    }
    {
      // 範囲外の部分がすべて SopPat::_X であるか調べる．
      auto nv = variable_num();
      if ( nv % 32 > 0 ) {
	auto sft = _shift_num(nv);
	auto mask = SOP_ALL1 << sft;
	auto word = *(cube_end - 1);
	word |= mask;
	if ( word != SOP_ALL1 ) {
	  std::cout << "invalid pat in outbound: "
		    << std::hex << word << std::dec << std::endl;
	  goto error;
	}
      }
      return true;
    }
  error:
    {
      auto nv = _cube_size() * 32;
      for ( SizeType i = 0; i < nv; ++ i ) {
	if ( i == variable_num() ) {
	  std::cout << "|";
	}
	std::cout << _get_pat(cube, i);
      }
      std::cout << std::endl;
    }
    return false;
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
    for ( ; dst_cube != dst_end;
	  ++ dst_cube, ++ cube1, ++ cube2 ) {
      auto tmp = *cube1 & *cube2;
      if ( _word_check_conflict(tmp) ) {
	return false;
      }
      *dst_cube = tmp;
    }
    return true;
  }

  /// @brief 2つのキューブの積を計算する．
  /// @retval true 積が空でなかった．
  /// @retval false 積が空だった．
  bool
  _cube_product_int(
    DstCube dst_cube, ///< [in] コピー先のビットベクタ
    Cube cube2        ///< [in] 2つめのキューブを表すビットベクタ
  ) const
  {
    auto dst_end = _cube_end(dst_cube);
    for ( ; dst_cube != dst_end;
	  ++ dst_cube, ++ cube2 ) {
      *dst_cube &= *cube2;
      if ( _word_check_conflict(*dst_cube) ) {
	return false;
      }
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
    auto tmp = _get_word(src_cube, blk) & pat1;
    if ( (tmp & mask) == SOP_ALL0 ) {
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
    auto dst_p = dst_cube + blk;
    *dst_p &= pat1;
    auto tmp = *dst_p;
    if ( (tmp & mask) == SOP_ALL0 ) {
      // 相反するリテラルがあった．
      return false;
    }
    return true;
  }

  /// @brief キューブによるコファクターを求める．
  /// @return 正しく割ることができたら true を返す．
  ///
  /// cube1 と cube2 が相反するリテラルを持つ場合 false が返される．
  /// その場合の dst_cube の内容は不正となる．
  bool
  _cube_cofactor(
    DstCube dst_cube, ///< [in] コピー先のビットベクタ
    Cube cube1,       ///< [in] 被除数を表すビットベクタ
    Cube cube2        ///< [in] 除数を表すビットベクタ
  ) const
  {
    auto dst_end = _cube_end(dst_cube);
    for ( ; dst_cube != dst_end;
	  ++ dst_cube, ++ cube1, ++ cube2 ) {
      auto pat1 = *cube1;
      auto pat2 = *cube2;
      if ( _word_check_conflict(pat1 & pat2) ) {
	// この場合の dst の値は不定
	return false;
      }
      *dst_cube = pat1 | ~pat2;
    }
    return true;
  }

  /// @brief リテラルによるコファクターを求める．
  /// @return 正しく割ることができたら true を返す．
  ///
  /// cube1 が lit が相反するリテラルを持つ場合 false が返される．
  /// その場合の dst_cube の内容は不正となる．
  bool
  _cube_cofactor(
    DstCube dst_cube, ///< [in] コピー先のビットベクタ
    Cube src_cube,    ///< [in] 被除数を表すビットベクタ
    Literal lit       ///< [in] リテラル
  ) const
  {
    auto varid = lit.varid();
    auto inv = lit.is_negative();
    auto blk = _block_pos(varid);
    auto sft = _shift_num(varid);
    auto pat = static_cast<SopPatWord>(_bitpat(!inv)) << sft;
    auto mask = 3UL << sft;
    auto src_p = src_cube + blk;
    if ( (*src_p & mask) == pat ) {
      return false;
    }
    _cube_copy(dst_cube, src_cube);
    auto dst_p = dst_cube + blk;
    *dst_p |= mask;
    return true;
  }

  /// @brief キューブによる商を求める．
  /// @return 正しく割ることができたら true を返す．
  bool
  _cube_algdiv(
    DstCube dst_cube, ///< [in] コピー先のビットベクタ
    Cube cube1,       ///< [in] 被除数を表すビットベクタ
    Cube cube2        ///< [in] 除数を表すビットベクタ
  ) const
  {
    auto dst_end = _cube_end(dst_cube);
    for ( ; dst_cube != dst_end;
	  ++ dst_cube, ++ cube1, ++ cube2 ) {
      auto pat1 = *cube1;
      auto pat2 = *cube2;
      if ( (pat1 & ~pat2) != SOP_ALL0 ) {
	// この場合の dst の値は不定
	return false;
      }
      *dst_cube = pat1 | ~pat2;
    }
    return true;
  }

  /// @brief リテラルによる商を求める．
  /// @return 正しく割ることができたら true を返す．
  bool
  _cube_algdiv(
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
    auto npat1 = ~pat1;
    auto mask = 3UL << sft;
    auto nmask = ~mask;
    auto word = _get_word(src_cube, blk);
    if ( (word & npat1) != SOP_ALL0 ) {
      // この場合の dst の値は不定
      return false;
    }
    _cube_copy(dst_cube, src_cube);
    auto dst_p = dst_cube + blk;
    *dst_p |= npat1;
    return true;
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
      if ( pat1 > pat2 ) {
	return -1;
      }
      else if ( pat1 < pat2 ) {
	return 1;
      }
    }
    return 0;
  }


protected:
  //////////////////////////////////////////////////////////////////////
  // LitSet 用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief リテラルを追加する．
  void
  _litset_add_literal(
    DstCube dst_cube, ///< [in] 対象のキューブ(LitSet)
    Literal lit       ///< [in] 追加するリテラル
  )
  {
    auto varid = lit.varid();
    auto inv = lit.is_negative();
    auto blk = _block_pos(varid);
    auto mask = ~_get_mask(varid, inv);
    auto dst_p = dst_cube + blk;
    *dst_p |= mask;
  }

  /// @brief リテラルの集合を追加する．
  void
  _litset_add_literals(
    DstCube dst_cube,                    ///< [in] 対象のキューブ(LitSet)
    const std::vector<Literal>& lit_list ///< [in] 追加するリテラルのリスト
  )
  {
    for ( auto lit: lit_list ) {
      _litset_add_literal(dst_cube, lit);
    }
  }

  /// @brief リテラルの集合を追加する．
  void
  _litset_add_literals(
    DstCube dst_cube,                        ///< [in] 対象のキューブ(LitSet)
    std::initializer_list<Literal>& lit_list ///< [in] 追加するリテラルのリスト
  )
  {
    for ( auto lit: lit_list ) {
      _litset_add_literal(dst_cube, lit);
    }
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
    return Chunk(size, SOP_ALL1);
  }

  /// @brief 単純なコピーを行う．
  void
  _copy(
    DstCube dst_cube, ///< [in] コピー先のビットベクタ
    Cube src_cube,    ///< [in] コピー元のビットベクタ
    SizeType cube_num ///< [in] キューブ数
  ) const
  {
    auto src_end = src_cube + _cube_size() * cube_num;
    std::copy(src_cube, src_end, dst_cube);
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

  /// @brief ワード中にコンフリクトがあるか調べる．
  static
  bool
  _word_check_conflict(
    SopPatWord word ///< [in] 対象のワード
  )
  {
    const SopPatWord mask1 = 0x5555555555555555ULL;
    const SopPatWord mask2 = 0xAAAAAAAAAAAAAAAAULL;
    auto tmp1 = word & mask1;
    auto tmp2 = word & mask2;
    if ( (tmp1 | (tmp2 >> 1)) != mask1 ) {
      // 同じ変数の異なる極性のリテラルがあった．
      return true;
    }
    return false;
  }

  /// @brief ワード中のリテラルをリストに追加する．
  static
  void
  _word_literal_list(
    SopPatWord word,               ///< [in] 対象のワード
    SizeType base,                 ///< [in] 変数番号の開始位置
    std::vector<Literal>& lit_list ///< [out] リテラルを格納するリスト
  );

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
  SopPat
  _bitpat(
    bool inv ///< [in] 反転属性
  )
  {
    return static_cast<SopPat>(inv ? SopPat::_0 : SopPat::_1);
  }

  /// @brief リテラルからブロック番号を得る．
  static
  SizeType
  _block_pos(
    Literal lit ///< 対象のリテラル
  )
  {
    auto varid = lit.varid();
    return _block_pos(varid);
  }

  /// @brief リテラルからマスクを作る．
  static
  SopPatWord
  _get_mask(
    Literal lit ///< 対象のリテラル
  )
  {
    auto varid = lit.varid();
    auto inv = lit.is_negative();
    return _get_mask(varid, inv);
  }

  /// @brief リテラルからマスクを作る．
  static
  SopPatWord
  _get_mask(
    SizeType varid, ///< [in] 変数番号
    bool inv        ///< [in] 反転属性
  )
  {
    auto sft = _shift_num(varid);
    auto pat = _bitpat(!inv);
    auto mask = static_cast<SopPatWord>(pat) << sft;
    return ~mask;
  }

  /// @brief 入力数が同じかチェックする．
  void
  _check_size(
    const SopBase& right
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
    const std::vector<Literal>& lit_list
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

  // 変数の数
  SizeType mVarNum{0};

};

END_NAMESPACE_YM_SOP

#endif // SOPBASE_H
