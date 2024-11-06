#ifndef BDDVAR_H
#define BDDVAR_H

/// @file BddVar.h
/// @brief BddVar のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/Bdd.h"


BEGIN_NAMESPACE_YM_DD

class DdEdge;

//////////////////////////////////////////////////////////////////////
/// @class BddVar BddVar.h "BddVar.h"
/// @brief BDD の変数を表すクラス
///
/// - 実際にはリテラル関数を表す BDD
//////////////////////////////////////////////////////////////////////
class BddVar :
  public Bdd
{
  friend class BddMgrImpl;
  friend class BddLit;
private:

  /// @brief 内容を指定したコンストラクタ
  ///
  /// root は正のリテラル関数だけが正しい値
  BddVar(
    BddMgrImpl* mgr,
    DdEdge root
  );

  /// @brief Bdd からのコピーコンストラクタ
  ///
  /// src は正のリテラル関数だけが正しい値
  BddVar(
    const Bdd& src   ///< [in] コピー元のオブジェクト
  ) : Bdd{src}
  {
    _check_valid();
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 通常のコンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief 空のコンストラクタ
  ///
  /// 不正な値となる．
  BddVar() = default;

  // コピーコンストラクタとコピー代入演算子はデフォルト実装でOK

  /// @brief デストラクタ
  ~BddVar() = default;

  /// @brief 不正な値を作るクラスメソッド
  static
  BddVar
  invalid()
  {
    return BddVar{};
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数番号を返す．
  SizeType
  id() const;

  /// @brief インデックスを返す．
  SizeType
  index() const;

  /// @brief 肯定のリテラルを返す．
  BddLit
  posilit() const;

  /// @brief 否定のリテラルを返す．
  BddLit
  negalit() const;

  /// @brief 等価比較演算子
  bool
  operator==(
    const BddVar& right
  ) const
  {
    return Bdd::operator==(right);
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const BddVar& right
  ) const
  {
    return !operator==(right);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 正しいBDDかチェックする．
  void
  _check_valid() const;

};

/// @brief ストリーム出力演算子
inline
ostream&
operator<<(
  ostream& s,
  const BddVar& var
)
{
  s << "v" << var.id();
  return s;
}

END_NAMESPACE_YM_DD

BEGIN_NAMESPACE_STD

// BddVar をキーにしたハッシュ関数クラスの定義
template <>
struct hash<YM_NAMESPACE::BddVar>
{
  SizeType
  operator()(
    const YM_NAMESPACE::BddVar& var
  ) const
  {
    return var.hash();
  }
};

END_NAMESPACE_STD

#endif // BDDVAR_H
