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


BEGIN_NAMESPACE_YM_SOP

//////////////////////////////////////////////////////////////////////
/// @class SopBase SopBase.h "SopBase.h"
/// @brief SopCover/SopCube に共通な基底クラス
///
/// 入力数を持つ．
//////////////////////////////////////////////////////////////////////
class SopBase
{
public:

  /// @brief コンストラクタ
  explicit
  SopBase(
    SizeType variable_num = 0 ///< [in] 変数の数
  ) : mVarNum{variable_num}
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
  SopBitVectConstIter
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
    return (variable_num() + 31) / 32;
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
  static
  SizeType
  block_pos(
    Literal lit ///< 対象のリテラル
  )
  {
    auto varid = lit.varid();
    return _block_pos(varid);
  }

  /// @brief リテラルからマスクを作る．
  static
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
  static
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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数の数
  SizeType mVarNum{0};

};

END_NAMESPACE_YM_SOP

#endif // SOPBASE_H
