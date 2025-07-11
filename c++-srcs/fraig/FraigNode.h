﻿#ifndef FRAIGNODE_H
#define FRAIGNODE_H

/// @file FraigNode.h
/// @brief FraigNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/fraig.h"
#include "ym/FraigHandle.h"


BEGIN_NAMESPACE_YM_FRAIG

//////////////////////////////////////////////////////////////////////
/// @class FraigNode FraigNode.h "FraigNode.h"
/// @brief Fraig のノードを表すクラス
//////////////////////////////////////////////////////////////////////
class FraigNode
{
public:

  /// @brief 入力用のコンストラクタ
  FraigNode(
    SizeType id,                               ///< [in] ノード番号
    SizeType input_id,                         ///< [in] 入力番号
    const std::vector<std::uint64_t>& init_pat ///< [in] 初期パタン
  );

  /// @brief AND用のコンストラクタ
  FraigNode(
    SizeType id,       ///< [in] ノード番号
    FraigNode* fanin0, ///< [in] ファンイン0のノード
    bool inv0,         ///< [in] ファンイン0の反転属性
    FraigNode* fanin1, ///< [in] ファンイン1のノード
    bool inv1	       ///< [in] ファンイン1の反転属性
  );

  /// @brief デストラクタ
  ~FraigNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 基本情報
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  SizeType
  id() const
  {
    return mId;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 入力ノードの時のアクセス関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ノードの時に true を返す．
  bool
  is_input() const
  {
    return mFlags[BIT_I];
  }

  /// @brief 入力番号を返す．
  SizeType
  input_id() const
  {
    return reinterpret_cast<SizeType>(mFanins[0]);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // AND ノードの時のアクセス関数
  //////////////////////////////////////////////////////////////////////

  /// @brief AND の時に true を返す．
  bool
  is_and() const
  {
    return !is_input();
  }

  /// @brief ファンインを得る．
  FraigNode*
  fanin(
    SizeType pos ///< [in] 位置 ( 0 or 1 )
  ) const
  {
    // 安全のため pos の値を補正しておく．
    pos &= 1;
    return mFanins[pos];
  }

  /// @brief 1番めのファンインを得る．
  FraigNode*
  fanin0() const
  {
    return mFanins[0];
  }

  /// @brief 2番めのファンインを得る．
  FraigNode*
  fanin1() const
  {
    return mFanins[1];
  }

  /// @brief ファンインの極性を得る．
  bool
  fanin_inv(
    SizeType pos ///< [in] 位置 ( 0 or 1 )
  ) const
  {
    // 安全のため pos の値を補正しておく．
    pos &= 1;
    return mFlags[BIT_INV0 + pos];
  }

  /// @brief 1番めのファンインの極性を得る．
  bool
  fanin0_inv() const
  {
    return mFlags[BIT_INV0];
  }

  /// @brief 2番めのファンインの極性を得る．
  bool
  fanin1_inv() const
  {
    return mFlags[BIT_INV1];
  }

  /// @brief 1番目のファンインのシグネチャを得る．
  SizeType
  fanin0_sig() const
  {
    return fanin0()->id() * 2 + static_cast<SizeType>(fanin0_inv());
  }

  /// @brief 2番目のファンインのシグネチャを得る．
  SizeType
  fanin1_sig() const
  {
    return fanin1()->id() * 2 + static_cast<SizeType>(fanin1_inv());
  }



public:
  //////////////////////////////////////////////////////////////////////
  // シミュレーション・パタンに関するアクセス関数
  //////////////////////////////////////////////////////////////////////

  /// @brief パタンを初期化する．
  void
  resize_pat(
    SizeType size ///< [in] サイズ
  );

  /// @brief パタンを追加する．
  void
  add_pat(
    std::uint64_t pat ///< [in] 追加するパタン
  );

  /// @brief パタンを計算する．
  ///
  /// ANDノード用
  void
  calc_pat(
    SizeType start, ///< [in] 開始位置
    SizeType end    ///< [in] 終了位置
  );

  /// @brief パタンの先頭を返す．
  const std::uint64_t*
  pat() const
  {
    return mPat;
  }

  /// @brief パタンの末尾を返す．
  const std::uint64_t*
  pat_end() const
  {
    return mPat + mPatUsed;
  }

  /// @brief 0 の値を取るとき true を返す．
  bool
  check_0mark() const
  {
    return mFlags[BIT_0];
  }

  /// @brief 1 の値を取るとき true を返す．
  bool
  check_1mark() const
  {
    return mFlags[BIT_1];
  }

  /// @brief パタンのハッシュ値を返す．
  SizeType
  pat_hash() const
  {
    return mHash;
  }

  /// @brief ハッシュ値の極性を返す．
  bool
  pat_hash_inv() const
  {
    return mFlags[BIT_H];
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ハッシュ値を計算する．
  void
  calc_hash(
    SizeType start, ///< [in] 開始位置
    SizeType end    ///< [in] 終了位置
  );

  /// @brief 0 の値を取ったことを記録する．
  void
  set_0mark()
  {
    mFlags[BIT_0] = true;
  }

  /// @brief 1 の値を取ったことを記録する．
  void
  set_1mark()
  {
    mFlags[BIT_1] = true;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  SizeType mId;

  // ファンインのノード
  FraigNode* mFanins[2]{nullptr, nullptr};

  // 0/1マーク，極性などの情報をパックしたもの
  std::bitset<7> mFlags{0};

  // シミュレーションパタン
  std::uint64_t* mPat{nullptr};

  // mPat のハッシュ値
  SizeType mHash{0UL};


private:
  //////////////////////////////////////////////////////////////////////
  // クラスメンバ
  //////////////////////////////////////////////////////////////////////

  // ハッシュ用の素数配列
  static
  std::uint64_t mPrimes[];


public:
  //////////////////////////////////////////////////////////////////////
  // クラスメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際のパタンサイズ
  static
  SizeType mPatSize;

  // 使用中のパタンサイズ
  static
  SizeType mPatUsed;


private:
  //////////////////////////////////////////////////////////////////////
  // mFlags で用いるシフト定数
  //////////////////////////////////////////////////////////////////////

  // 入力フラグ
  static
  const int BIT_I  = 0;

  // ファンイン0 の極性
  static
  const int BIT_INV0 = 1;

  // ファンイン1 の極性
  static
  const int BIT_INV1 = 2;

  // 0 になったことがあるかどうか
  static
  const int BIT_0  = 3;

  // 1 になったことがあるかどうか
  static
  const int BIT_1  = 4;

  // ハッシュパタンの極性
  static
  const int BIT_H  = 5;

};

END_NAMESPACE_YM_FRAIG

#endif // FRAIGNODE_H
