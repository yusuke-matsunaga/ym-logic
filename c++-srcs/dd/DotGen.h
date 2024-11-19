#ifndef DOTGEN_H
#define DOTGEN_H

/// @file DotGen.h
/// @brief DotGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/JsonValue.h"


BEGIN_NAMESPACE_YM_DD

class DdEdge;
class DdInfoMgr;
class DotWriter;

//////////////////////////////////////////////////////////////////////
/// @class DotGen DotGen.h "DotGen.h"
/// @brief dot 形式の出力を行うクラス
//////////////////////////////////////////////////////////////////////
class DotGen
{
public:

  /// @brief コンストラクタ
  explicit
  DotGen(
    const std::unordered_map<string, string>& attr_dict ///< [in] 属性値の辞書
  );

  /// @brief コンストラクタ
  explicit
  DotGen(
    const JsonValue& attr = JsonValue{} ///< [in] 属性値を表すJSONオブジェクト
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
    ostream& s,               ///< [in] 出力ストリーム
    const DdInfoMgr& info_mgr ///< [in] グラフ構造の情報を持つオブジェクト
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
    SizeType id ///< [in] 番号
  );

  /// @brief 枝の内容を出力する．
  void
  write_edge(
    DotWriter& writer,       ///< [in] dot 出力着
    const string& from_node, ///< [in] 始点のノード名
    SizeType edge,           ///< [in] 枝
    bool zero                ///< [in] 0枝の時 true にするフラグ
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

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

END_NAMESPACE_YM_DD

#endif // DOTGEN_H
