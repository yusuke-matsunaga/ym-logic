#ifndef FRAIGHANDLE_H
#define FRAIGHANDLE_H

/// @file FraigHandle.h
/// @brief FraigHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/fraig.h"


BEGIN_NAMESPACE_YM_FRAIG

//////////////////////////////////////////////////////////////////////
/// @class FraigHandle FraigHandle.h "ym/FraigHandle.h"
/// @brief Fraig の枝を表すクラス
///
/// 具体的にはノード番号と極性を表す1ビットの情報をまとめたもの
/// ノード番号0番は特殊なノードで定数0を表す
//////////////////////////////////////////////////////////////////////
class FraigHandle
{
  friend class MgrImpl;

public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタと生成/内容の設定
  //////////////////////////////////////////////////////////////////////

  /// @brief 空のコンストラクタ
  FraigHandle() = default;

  /// @brief 定数０のハンドルを返す．
  static
  FraigHandle
  zero()
  {
    return FraigHandle{0};
  }

  /// @brief 定数1のハンドルを返す．
  static
  FraigHandle
  one()
  {
    return FraigHandle{1};
  }

  /// @brief デストラクタ
  ~FraigHandle() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 内部の情報を取得するメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 否定の枝を返す．
  FraigHandle
  operator~() const
  {
    return FraigHandle{mPackedData ^ 1UL};
  }

  /// @brief 極性をかけ合わせる．
  FraigHandle
  operator*(
    bool inv ///< [in] 極性
  ) const
  {
    return FraigHandle{mPackedData ^ static_cast<SizeType>(inv)};
  }

  /// @brief ノード番号を得る．
  SizeType
  node_id() const
  {
    return mPackedData >> 1;
  }

  /// @brief 極性を得る．
  /// @return 反転しているとき true を返す．
  bool
  inv() const
  {
    return static_cast<bool>(mPackedData & 1UL);
  }

  /// @brief 定数0を指しているとき true を返す．
  bool
  is_zero() const
  {
    return mPackedData == 0UL;
  }

  /// @brief 定数1を指しているとき true を返す．
  bool
  is_one() const
  {
    return mPackedData == 1UL;
  }

  /// @brief 定数を指しているとき true を返す．
  ///
  /// = is_zero() || is_one()
  bool
  is_const() const
  {
    return (mPackedData & ~1UL) == 0UL;
  }

  /// @brief ハッシュ値を返す．
  SizeType
  hash() const
  {
    return static_cast<SizeType>((mPackedData * mPackedData) >> 20);
  }

  /// @brief 等価比較関数
  bool
  operator==(
    FraigHandle src2 ///< [in] オペランド
  ) const
  {
    return mPackedData == src2.mPackedData;
  }

  /// @brief 非等価比較演算
  bool
  operator!=(
    FraigHandle src2  ///< [in] オペランド2
  )
  {
    return !operator==(src2);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を直接指定したコンストラクタ
  explicit
  FraigHandle(
    SizeType data ///< [in] 内容
  ) : mPackedData{data}
  {
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード番号＋極性
  SizeType mPackedData;

};

END_NAMESPACE_YM_FRAIG

#endif // FRAIGHANDLE_H
