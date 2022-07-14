#ifndef BDDNODETABLE_H
#define BDDNODETABLE_H

/// @file BddNodeTable.h
/// @brief BddNodeTable のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "BddNode.h"
#include "BddEdge.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class BddNodeTable BddNodeTable.h "BddNodeTable.h"
/// @brief BddNode のハッシュ表
//////////////////////////////////////////////////////////////////////
class BddNodeTable
{
public:

  /// @brief コンストラクタ
  BddNodeTable();

  /// @brief デストラクタ
  ~BddNodeTable();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 該当するノードを探す．
  ///
  /// なければ nullptr を返す．
  BddNode*
  find(
    SizeType index, ///< [in] インデックス
    BddEdge edge0,  ///< [in] 0枝
    BddEdge edge1   ///< [in] 1枝
  ) const;

  /// @brief ノードを登録する．
  void
  put(
    BddNode* node ///< [in] 結果のノード
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 表を拡張する．
  void
  extend(
    SizeType req_size ///< [in] 要求サイズ
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 表のサイズ
  SizeType mSize{0};

  // ハッシュ用のモジュロサイズ
  SizeType mHashSize{0};

  // 表の本体
  BddNode** mTable{nullptr};

  // 格納されているノード数
  SizeType mCount{0};

  // テーブルを拡張する目安
  SizeType mNextLimit;

};

END_NAMESPACE_YM

#endif // BDDNODETABLE_H
