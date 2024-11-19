#ifndef NODECOLLECTOR_H
#define NODECOLLECTOR_H

/// @file NodeCollector.h
/// @brief NodeCollector のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"


BEGIN_NAMESPACE_YM_DD

class DdNode;
class DdEdge;

//////////////////////////////////////////////////////////////////////
/// @class NodeCollector NodeCollector.h "NodeCollector.h"
/// @brief DdNode を集めるクラス
//////////////////////////////////////////////////////////////////////
class NodeCollector
{
public:

  /// @brief コンストラクタ
  NodeCollector(
    const vector<DdEdge>& root_list ///< [in] 根の枝のリスト
  );

  /// @brief デストラクタ
  ~NodeCollector() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードのリストを返す．
  const vector<const DdNode*>&
  node_list() const
  {
    return mNodeList;
  }

  /// @brief DdEdge の情報を整数値に変換する．
  SizeType
  edge2int(
    DdEdge edge ///< [in] 枝
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードを集める．
  void
  get_node(
    DdEdge edge ///< [in] 枝
  );

  /// @brief ノード番号を得る．
  SizeType
  node_id(
    const DdNode* node ///< [in] ノード
  ) const
  {
    return mNodeMap.at(node);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード番号の対応表
  unordered_map<const DdNode*, SizeType> mNodeMap;

  // ノードリスト
  vector<const DdNode*> mNodeList;

};

END_NAMESPACE_YM_DD

#endif // NODECOLLECTOR_H
