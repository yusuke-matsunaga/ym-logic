#ifndef NODECOLLECTOR_H
#define NODECOLLECTOR_H

/// @file NodeCollector.h
/// @brief NodeCollector のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bdd_nsdef.h"


BEGIN_NAMESPACE_YM_BDD

class BddNode;
class BddEdge;

//////////////////////////////////////////////////////////////////////
/// @class NodeCollector NodeCollector.h "NodeCollector.h"
/// @brief BddNode を集めるクラス
//////////////////////////////////////////////////////////////////////
class NodeCollector
{
public:

  /// @brief コンストラクタ
  NodeCollector() = default;

  /// @brief デストラクタ
  ~NodeCollector() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードを集める．
  void
  get_node(
    BddEdge edge ///< [in] 枝
  );

  /// @brief ノード番号を得る．
  SizeType
  node_id(
    BddNode* node ///< [in] ノード
  ) const
  {
    return mNodeMap.at(node);
  }

  /// @brief ノードリストを返す．
  const vector<BddNode*>&
  node_list() const
  {
    return mNodeList;
  }

  /// @brief インデックスの最大値+1を得る．
  SizeType
  max_index() const
  {
    return mIndexedNodeList.size();
  }

  /// @brief インデックスごとのノードリストを返す．
  const vector<BddNode*>&
  indexed_node_list(
    SizeType index ///< [in] インデックス ( 0 <= index < max_index() )
  ) const
  {
    ASSERT_COND( 0 <= index && index < max_index() );
    return mIndexedNodeList[index];
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードリスト
  vector<BddNode*> mNodeList;

  // インデックスごとのノードリスト
  vector<vector<BddNode*>> mIndexedNodeList;

  // ノード番号の対応表
  unordered_map<BddNode*, SizeType> mNodeMap;

};

END_NAMESPACE_YM_BDD

#endif // NODECOLLECTOR_H
