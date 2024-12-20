#ifndef DDNODETABLE_H
#define DDNODETABLE_H

/// @file DdNodeTable.h
/// @brief DdNodeTable のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "DdNode.h"
#include "DdEdge.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
/// @class DdNodeTable DdNodeTable.h "DdNodeTable.h"
/// @brief DdNode の管理を行うクラス
///
/// 同じ edge0, edge1 を持つノードを共有する処理を行う．
/// このクラスでは同一のレベルのノードのみを対象にする．
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
  /// @return 新規にノードを作った時の true を返す．
  bool
  new_node(
    SizeType level, ///< [in] レベル
    DdEdge edge0,   ///< [in] 0枝
    DdEdge edge1,   ///< [in] 1枝
    DdNode*& node   ///< [out] 結果を格納する変数
  );

  /// @brief ノードを登録する．
  void
  reg_node(
    DdNode* node
  );

  /// @brief ノード数を返す．
  SizeType
  node_num()
  {
    return mNodeNum;
  }

  /// @brief 保持しているノードのレベルを変更する．
  void
  chg_level(
    SizeType new_level
  );

  /// @brief 保持しているノードに対して処理を行う．
  void
  scan(
    std::function<bool(DdNode*)> func
  );

  /// @brief ガーベージコレクションを行う．
  /// @return 削除したノード数を返す．
  SizeType
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

};

END_NAMESPACE_YM_DD

#endif // BDDNODETABLE_H
