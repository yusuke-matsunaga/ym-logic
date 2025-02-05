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
      Chunk& chunk,      ///< [in] ビットベクタ本体
      SizeType cube_size ///< [in] キューブサイズ
    ) : mChunk{chunk},
	mCubeSize{cube_size},
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
      return mChunk.begin() + mNum * mCubeSize;
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

  /// @brief ビットベクタからパタンを取り出す．
  SopPat
  _get_pat(
    Cube cube,   ///< [in] キューブを表す反復子
    SizeType var ///< [in] 変数 ( 0 <= var_id.val() < variable_num() )
  ) const
  {
    if ( var >= variable_num() ) {
      throw std::out_of_range{"var is out of range"};
    }

    SizeType blk = _block_pos(var);
    SizeType sft = _shift_num(var);
    auto p = cube + blk;
    return static_cast<SopPat>((*p >> sft) & 3ULL);
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
  ) const;

  /// @brief 内容をリテラルのリストのリストに変換する．
  vector<vector<Literal>>
  _literal_list(
    SizeType cube_num, ///< [in] キューブ数
    const Chunk& chunk ///< [in] 本体のビットベクタ
  ) const;

  /// @brief ビットベクタからハッシュ値を計算する．
  SizeType
  _hash(
    SizeType cube_num, ///< [in] キューブ数
    const Chunk& chunk ///< [in] 本体のビットベクタ
  ) const;

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


public:
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
      *dst_cube = SOP_ALL1;
    }
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
  ) const;

  /// @brief 2つのキューブの積を計算する．
  /// @retval true 積が空でなかった．
  /// @retval false 積が空だった．
  bool
  _cube_product(
    DstCube dst_cube, ///< [in] コピー先のビットベクタ
    Cube cube1,       ///< [in] 1つめのキューブを表すビットベクタ
    Cube cube2        ///< [in] 2つめのキューブを表すビットベクタ
  ) const;

  /// @brief キューブとリテラルの積を計算する．
  /// @retval true 積が空でなかった．
  /// @retval false 積が空だった．
  bool
  _cube_product(
    DstCube dst_cube,  ///< [in] コピー先のビットベクタ
    Cube cube1,        ///< [in] 1つめのキューブを表すビットベクタ
    Literal lit        ///< [in] リテラル
  ) const;

  /// @brief キューブとリテラルの積を計算する．
  /// @retval true 積が空でなかった．
  /// @retval false 積が空だった．
  bool
  _cube_product_int(
    DstCube dst_cube, ///< [in] コピー先のビットベクタ
    Literal lit       ///< [in] リテラル
  ) const;

  /// @brief キューブによる商を求める．
  /// @return 正しく割ることができたら true を返す．
  bool
  _cube_quotient(
    DstCube dst_cube, ///< [in] コピー先のビットベクタ
    Cube cube1,       ///< [in] 被除数を表すビットベクタ
    Cube cube2        ///< [in] 除数を表すビットベクタ
  ) const;

  /// @brief リテラルによる商を求める．
  /// @return 正しく割ることができたら true を返す．
  bool
  _cube_quotient(
    DstCube dst_cube, ///< [in] コピー先のビットベクタ
    Cube cube1,       ///< [in] 被除数を表すビットベクタ
    Literal lit       ///< [in] リテラル
  ) const;

  /// @brief キューブ(を表すビットベクタ)の比較を行う．
  /// @retval -1 bv1 <  bv2
  /// @retval  0 bv1 == bv2
  /// @retval  1 bv1 >  bv2
  int
  _cube_compare(
    Cube cube1, ///< [in] 1つめのキューブを表すビットベクタ
    Cube cube2  ///< [in] 2つめのキューブを表すビットベクタ
  ) const;


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
    Chunk& chunk ///< [in] ビットベクタの先頭
  ) const
  {
    return DstCubeList{chunk, _cube_size()};
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

#if 0
  /// @brief キューブ位置を計算する．
  DstCube
  _cube_begin(
    Chunk& chunk,    ///< [in] ビットベクタの先頭
    SizeType pos = 0 ///< [in] キューブ位置
  ) const
  {
    return chunk.begin() + pos * _cube_size();
  }

  /// @brief キューブ位置を計算する．
  Cube
  _cube_begin(
    const Chunk& chunk, ///< [in] ビットベクタの先頭
    SizeType pos = 0    ///< [in] キューブ位置
  ) const
  {
    return chunk.begin() + pos * _cube_size();
  }

  /// @brief キューブの末尾を計算する．
  DstCube
  _cube_end(
    DstCube begin,        ///< [in] キューブの先頭
    SizeType cube_num = 1 ///< [in] キューブ数
  ) const
  {
    return begin + _cube_size() * cube_num;
  }

  /// @brief キューブの末尾を計算する．
  Cube
  _cube_end(
    Cube begin,           ///< [in] キューブの先頭
    SizeType cube_num = 1 ///< [in] キューブ数
  ) const
  {
    return begin + _cube_size() * cube_num;
  }

  /// @brief 次のキューブに移動する．
  void
  _cube_next(
    Cube& cube ///< [out] キューブ
  ) const
  {
    cube += _cube_size();
  }

  /// @brief 次のキューブに移動する．
  void
  _cube_next(
    DstCube& cube ///< [out] キューブ
  ) const
  {
    cube += _cube_size();
  }
#endif

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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数の数
  SizeType mVarNum{0};

};

END_NAMESPACE_YM_SOP

#endif // SOPBASE_H
