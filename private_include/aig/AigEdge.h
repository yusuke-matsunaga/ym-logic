#ifndef AIGEDGE_H
#define AIGEDGE_H

/// @file AigEdge.h
/// @brief AigEdge のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/aig.h"


BEGIN_NAMESPACE_YM_AIG

class AigNode;

//////////////////////////////////////////////////////////////////////
/// @class AigEdge AigEdge.h "AigEdge.h"
/// @brief AIG の枝を表すクラス
///
/// 意味的には AigNode へのポインタと反転属性を持つが，
/// 内部では１ワードにパックしている．
/// このクラスでは一度設定された値は変化しない．
///
/// このクラスの使用目的は関数呼び出しのパラメータとして枝の情報を渡す際に
/// 1ワードの変数として扱えるようにすること．
/// いわゆる 'borrowed reference' なので AigNode の参照回数は変化しない．
//////////////////////////////////////////////////////////////////////
class AigEdge
{
  friend class AigHandle;

public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief 空のコンストラクタ
  AigEdge() = default;

  /// @brief 内容を指定したコンストラクタ
  AigEdge(
    AigNode* node, ///< [in] ノード
    bool inv       ///< [in] 反転属性
  ) : AigEdge{reinterpret_cast<PtrIntType>(node) | inv}
  {
  }

  /// @brief デストラクタ
  ~AigEdge() = default;

  /// @brief 定数0のハンドルを返す．
  static
  AigEdge
  zero()
  {
    return AigEdge{nullptr, false};
  }

  /// @brief 定数1のハンドルを返す．
  static
  AigEdge
  one()
  {
    return AigEdge{nullptr, true};
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 内部の情報を取得するメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードを返す．
  AigNode*
  node() const
  {
    return reinterpret_cast<AigNode*>(mPackedData & ~1UL);
  }

  /// @brief 反転属性を得る．
  bool
  inv() const
  {
    return static_cast<bool>(mPackedData & 1UL);
  }

  /// @brief 定数0の時 true を返す．
  bool
  is_zero() const
  {
    return mPackedData == 0UL;
  }

  /// @brief 定数1の時 true を返す．
  bool
  is_one() const
  {
    return mPackedData == 1UL;
  }

  /// @brief 定数の時 true を返す．
  bool
  is_const() const
  {
    return (mPackedData & ~1UL) == 0UL;
  }

  /// @brief 外部入力ノードへのハンドルの時 true を返す．
  bool
  is_input() const;

  /// @brief 外部入力ノードへのハンドルの時，入力番号を返す．
  ///
  /// is_input() == false の時は例外を送出する．
  SizeType
  input_id() const;

  /// @brief ANDノードへのハンドルの時，true を返す．
  bool
  is_and() const;

  /// @brief ANDノードへのハンドルの時，ファンインのハンドルを返す．
  ///
  /// is_and() == false 時は例外を送出する．
  AigEdge
  fanin(
    SizeType pos ///< [in] 位置 ( 0 or 1 )
  ) const;

  /// @brief ANDノードへのハンドルの時，ファンイン0のハンドルを返す．
  ///
  /// is_and() == false 時は例外を送出する．
  AigEdge
  fanin0() const;

  /// @brief ANDノードへのハンドルの時，ファンイン1のハンドルを返す．
  ///
  /// is_and() == false 時は例外を送出する．
  AigEdge
  fanin1() const;

  /// @brief ANDグループのファンインのリストを返す．
  ///
  /// ANDグループとは反転していない枝で繋がったANDノードの集合
  vector<AigEdge>
  ex_fanin_list() const;

  /// @brief ユニークなインデックス値を返す．
  SizeType
  index() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 演算
  //////////////////////////////////////////////////////////////////////

  /// @brief 否定したハンドルを返す．
  AigEdge
  operator~() const
  {
    return AigEdge{mPackedData ^ 1UL};
  }

  /// @brief 反転属性との掛け算
  AigEdge
  operator*(
    bool inv ///< [in] 反転属性
  ) const
  {
    return AigEdge{mPackedData ^ static_cast<PtrIntType>(inv)};
  }

  /// @brief 同じノードを指す反転なしの枝を返す．
  AigEdge
  positive_edge() const
  {
    return AigEdge{mPackedData & ~1UL};
  }

  /// @brief 同じノードを指す反転ありの枝を返す．
  AigEdge
  negative_edge() const
  {
    return AigEdge{mPackedData | 1UL};
  }

  /// @brief 等価比較関数
  bool
  operator==(
    const AigEdge& right ///< [in] オペランド
  ) const
  {
    return mPackedData == right.mPackedData;
  }

  /// @brief 非等価比較関数
  bool
  operator!=(
    const AigEdge& right ///< [in] オペランド
  ) const
  {
    return !operator==(right);
  }

  /// @brief 小なり比較演算
  ///
  /// index() の大小で比較する．
  bool
  operator<(
    const AigEdge& right ///< [in] オペランド
  ) const
  {
    return index() < right.index();
  }

  /// @brief 大なり比較演算
  bool
  operator>(
    const AigEdge& right ///< [in] オペランド
  ) const
  {
    return right.operator<(*this);
  }

  /// @brief 小なりイコール比較演算
  bool
  operator<=(
    const AigEdge& right ///< [in] オペランド
  ) const
  {
    return !right.operator<(*this);
  }

  /// @brief 大なりイコール比較演算
  bool
  operator>=(
    const AigEdge& right ///< [in] オペランド
  ) const
  {
    return !operator<(right);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 生のコンストラクタ
  explicit
  AigEdge(
    PtrIntType data ///< [in] 生のデータ
  ) : mPackedData{data}
  {
  }

  /// @brief ex_fanin_list() の下請け関数
  void
  ex_fanin_sub(
    vector<AigEdge>& fanin_list ///< [out] 結果を格納するリスト
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードへのポインタと反転属性をパックしたもの
  PtrIntType mPackedData{0UL};

};

END_NAMESPACE_YM_AIG

BEGIN_NAMESPACE_STD

// AigEdge をキーにしたハッシュ関数クラスの定義
template <>
struct hash<YM_NAMESPACE::nsAig::AigEdge>
{
  SizeType
  operator()(
    const YM_NAMESPACE::nsAig::AigEdge& edge
  ) const
  {
    return edge.index();
  }
};

END_NAMESPACE_STD

#endif // AIGEDGE_H
