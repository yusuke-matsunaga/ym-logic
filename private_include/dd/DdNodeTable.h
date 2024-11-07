#ifndef DDNODETABLE_H
#define DDNODETABLE_H

/// @file DdNodeTable.h
/// @brief DdNodeTable のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "dd/DdNode.h"
#include "dd/DdEdge.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
/// @class DdNodeTable DdNodeTable.h "DdNodeTable.h"
/// @brief DdNode の管理を行うクラス
///
/// 同じ index, edge0, edge1 を持つノードを共有する処理を行う．
//////////////////////////////////////////////////////////////////////
class DdNodeTable
{
public:

  /// @brief コンストラクタ
  DdNodeTable(
    SizeType varid ///< [in] 変数番号
  );

  /// @brief デストラクタ
  ~DdNodeTable();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数番号を返す．
  SizeType
  varid() const
  {
    return mVarId;
  }

  /// @brief ノードを作る．
  const DdNode*
  new_node(
    SizeType index, ///< [in] インデックス
    DdEdge edge0,   ///< [in] 0枝
    DdEdge edge1    ///< [in] 1枝
  );

  /// @brief ノード数を返す．
  SizeType
  node_num()
  {
    return mNodeNum;
  }

  /// @brief ガーベージコレクションを行う．
  void
  garbage_collection();


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

  // 変数番号
  SizeType mVarId;

  // 表のサイズ
  SizeType mSize{0};

  // ハッシュ用のモジュロサイズ
  // 素数になるように調整されている．
  SizeType mHashSize{0};

  // 表の本体
  DdNode** mTable{nullptr};

  // 格納されているノード数
  SizeType mNodeNum{0};

  // テーブルを拡張する目安
  SizeType mNextLimit;

  // ガーベージノード数
  SizeType mGarbageNum{0};

};

END_NAMESPACE_YM_DD

#endif // BDDNODETABLE_H
