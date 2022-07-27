#ifndef DOTGEN_H
#define DOTGEN_H

/// @file DotGen.h
/// @brief DotGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bdd_nsdef.h"
#include "NodeCollector.h"
#include "DotWriter.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @class DotGen DotGen.h "DotGen.h"
/// @brief dot 形式の出力を行うクラス
//////////////////////////////////////////////////////////////////////
class DotGen :
  public NodeCollector
{
public:

  /// @brief コンストラクタ
  DotGen(
    ostream& s,                                    ///< [in] 出力ストリーム
    const unordered_map<string, string>& attr_dict ///< [in] 属性値の辞書
  );

  /// @brief デストラクタ
  ~DotGen() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief dot 形式で出力する．
  void
  write(
    const vector<BddEdge>& root_list ///< [in] 根のリスト
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ルート名を返す．
  string
  root_name(
    SizeType i ///< [in] 番号
  );

  /// @brief ノード名を返す．
  string
  node_name(
    BddNode* node ///< [in] ノード
  );

  /// @brief 枝の内容を出力する．
  void
  write_edge(
    const string& from_node, ///< [in] 始点のノード名
    BddEdge edge,            ///< [in] 枝
    bool zero                ///< [in] 0枝の時 true にするフラグ
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // Dot出力器
  DotWriter mWriter;

  // グラフの属性リスト
  unordered_map<string, string> mGraphAttrList;

  // 根の属性リスト
  unordered_map<string, string> mRootAttrList;

  // ノードの属性リスト
  unordered_map<string, string> mNodeAttrList;

  // 終端0の属性リスト
  unordered_map<string, string> mTerminal0AttrList;

  // 終端1の属性リスト
  unordered_map<string, string> mTerminal1AttrList;

  // 0枝の属性リスト
  unordered_map<string, string> mEdge0AttrList;

  // 1枝の属性リスト
  unordered_map<string, string> mEdge1AttrList;

};

END_NAMESPACE_YM_BDD

#endif // DOTGEN_H
