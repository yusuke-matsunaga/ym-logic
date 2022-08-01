#ifndef NODECOLLECTOR_H
#define NODECOLLECTOR_H

/// @file NodeCollector.h
/// @brief NodeCollector のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bdd_nsdef.h"
#include "ym/Bdd.h"


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
  NodeCollector(
    const vector<BddEdge>& root_list ///< [in] 根の枝のリスト
  );

  /// @brief デストラクタ
  ~NodeCollector() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 根の枝を表す整数のリストを返す．
  const vector<BddEdge>&
  root_list() const
  {
    return mRootList;
  }

  /// @brief ノードリストを返す．
  const vector<BddNode*>&
  node_list() const
  {
    return mNodeList;
  }

  /// @brief ノード番号を得る．
  SizeType
  node_id(
    BddNode* node ///< [in] ノード
  ) const
  {
    return mNodeMap.at(node);
  }

  /// @brief BddEdge の情報を整数値に変換する．
  SizeType
  edge2int(
    BddEdge edge ///< [in] 枝
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードを集める．
  void
  get_node(
    BddEdge edge ///< [in] 枝
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 根の枝のリスト
  vector<BddEdge> mRootList;

  // ノードリスト
  vector<BddNode*> mNodeList;

  // ノード番号の対応表
  unordered_map<BddNode*, SizeType> mNodeMap;

};

END_NAMESPACE_YM_BDD

#endif // NODECOLLECTOR_H
