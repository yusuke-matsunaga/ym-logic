#ifndef DOTGEN_H
#define DOTGEN_H

/// @file DotGen.h
/// @brief DotGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/DotWriter.h"
#include "ym/JsonValue.h"


BEGIN_NAMESPACE_YM_AIG

class AigEdge;
class AigNode;

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
    const JsonValue& option ///< [in] オプションを表すJSONオブジェクト
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
    ostream& s,                        ///< [in] 出力ストリーム
    const vector<AigEdge>& root_list,  ///< [in] 根の枝のリスト
    const vector<AigNode*>& node_list  ///< [in] ノードのリスト
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性値をセットする．
  void
  set_attr(
    const JsonValue& attr_json ///< [in] 属性値の辞書を表す JSON オブジェクト
  );

  /// @brief 変数ラベルをセットする．
  void
  set_label(
    const string& name,          ///< [in] 名前(エラーメッセージ用)
    const JsonValue& label_json, ///< [in] 変数ラベル配列を表す JSON オブジェクト
    std::unordered_map<SizeType, string>& label_dict ///< [in] 結果を格納する辞書
  );

  /// @brief ルート名を返す．
  string
  root_name(
    SizeType i ///< [in] 番号
  );

  /// @brief ノード名を返す．
  string
  node_name(
    const AigNode* node ///< [in] ノード
  );

  /// @brief 変数ラベルの属性を追加する．
  void
  add_label_attr(
    std::unordered_map<string, string>& attr_list, ///< [in] 属性の辞書
    SizeType input_id                              ///< [in] 入力番号
  );

  /// @brief 枝の内容を出力する．
  void
  write_edge(
    DotWriter& writer,       ///< [in] dot 出力着
    const string& from_node, ///< [in] 始点のノード名
    AigEdge edge             ///< [in] 枝
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数ラベルの辞書
  unordered_map<SizeType, string> mLabelDict;

  // TeX用変数ラベルの辞書
  unordered_map<SizeType, string> mTexLblDict;

  // グラフの属性リスト
  unordered_map<string, string> mGraphAttrList;

  // 根の属性リスト
  unordered_map<string, string> mRootAttrList;

  // ノードの属性リスト
  unordered_map<string, string> mNodeAttrList;

  // 終端の属性リスト
  unordered_map<string, string> mTerminalAttrList;

  // 定数ノードの属性リスト
  unordered_map<string, string> mConstAttrList;

  // 枝の属性リスト
  unordered_map<string, string> mEdgeAttrList;

};

END_NAMESPACE_YM_AIG

#endif // DOTGEN_H
