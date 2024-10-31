#ifndef BDDVAR_H
#define BDDVAR_H

/// @file BddVar.h
/// @brief BddVar のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/BddObj.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
/// @class BddVar BddVar.h "BddVar.h"
/// @brief BDD の変数を表すクラス
///
/// - 実際には整数で表されるが順序は意味を持たない．加算などの算術
///   演算も定義されない．
/// - 同一性のチェックのみが意味をもつ演算となる．
/// - 特定の BddMgr に属する．
//////////////////////////////////////////////////////////////////////
class BddVar :
  public BddObj
{
  friend class BddMgrImpl;
  friend class BddLit;
private:

  /// @brief コンストラクタ
  BddVar(
    BddMgrImpl* mgr, ///< [in] BDDマネージャ
    SizeType varid  ///< [in] 変数番号
  ) : BddObj{mgr},
      mVarId{varid}
  {
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 通常のコンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief 空のコンストラクタ
  ///
  /// 不正な値となる．
  BddVar() = default;

  /// @brief コピーコンストラクタ
  BddVar(
    const BddVar& src ///< [in] コピー元のオブジェクト
  ) : BddObj{src},
      mVarId{src.mVarId}
  {
  }

  /// @brief デストラクタ
  ~BddVar() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 肯定のリテラルを返す．
  BddLit
  to_posilit() const;

  /// @brief 否定のリテラルを返す．
  BddLit
  to_negalit() const;

  /// @brief 等価比較演算子
  bool
  operator==(
    const BddVar& right
  ) const
  {
    if ( mVarId == right.mVarId && _mgr() == right._mgr() ) {
      return true;
    }
    return false;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const BddVar& right
  ) const
  {
    return !operator==(right);
  }

  /// @brief ハッシュ関数
  SizeType
  hash() const
  {
    return mVarId;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数番号
  SizeType mVarId;

};

END_NAMESPACE_YM_DD

#endif // BDDVAR_H
