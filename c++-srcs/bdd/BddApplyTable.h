#ifndef BDDAPPLYTABLE_H
#define BDDAPPLYTABLE_H

/// @file BddApplyTable.h
/// @brief BddApplyTable のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "BddEdge.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class BddApplyTable BddApplyTable.h "BddApplyTable.h"
/// @brief BDD の apply 演算の結果を保持するハッシュ表
//////////////////////////////////////////////////////////////////////
class BddApplyTable
{
public:

  /// @brief コンストラクタ
  BddApplyTable();

  /// @brief デストラクタ
  ~BddApplyTable();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief テーブルサイズを変更する．
  void
  resize(
    SizeType req_size ///< [in] サイズ
  );

  /// @brief 演算結果が登録されているか調べる．
  bool
  find(
    SizeType op,    ///< [in] 演算の種類
    BddEdge e1,     ///< [in] オペランド1
    BddEdge e2,     ///< [in] オペランド2
    BddEdge& result ///< [out] 結果を格納する変数
  );

  /// @brief 演算結果を登録する．
  void
  put(
    SizeType op,    ///< [in] 演算の種類
    BddEdge e1,     ///< [in] オペランド1
    BddEdge e2,     ///< [in] オペランド2
    BddEdge& result ///< [in] 結果
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ハッシュ表のノード
  struct Cell
  {
    SizeType mOp;    // 演算の種類
    BddEdge mLeft;   // オペランド1
    BddEdge mRight;  // オペランド2
    BddEdge mResult; // 結果
  };

  // テーブルサイズ
  SizeType mSize{0};

  // ハッシュ用のサイズ
  SizeType mHashSize{0};

  // テーブル本体
  Cell* mTable{nullptr};

};

END_NAMESPACE_YM

#endif // BDDAPPLYTABLE_H
