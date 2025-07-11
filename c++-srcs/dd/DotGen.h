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
#include "ym/DotWriter.h"


BEGIN_NAMESPACE_YM_DD

class DdEdge;
class DdInfoMgr;

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
    std::ostream& s,          ///< [in] 出力ストリーム
    const DdInfoMgr& info_mgr ///< [in] グラフ構造の情報を持つオブジェクト
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
    const std::string& name,                              ///< [in] 名前(エラーメッセージ用)
    const JsonValue& label_json,                          ///< [in] 変数ラベル配列を表す JSON オブジェクト
    std::unordered_map<SizeType, std::string>& label_dict ///< [in] 結果を格納する辞書
  );

  /// @brief ルート名を返す．
  std::string
  root_name(
    SizeType i ///< [in] 番号
  );

  /// @brief ノード名を返す．
  std::string
  node_name(
    SizeType id ///< [in] 番号
  );

  /// @brief 枝の内容を出力する．
  void
  write_edge(
    DotWriter& writer,            ///< [in] dot 出力着
    const std::string& from_node, ///< [in] 始点のノード名
    SizeType edge,                ///< [in] 枝
    bool zero                     ///< [in] 0枝の時 true にするフラグ
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数ラベルの辞書
  std::unordered_map<SizeType, std::string> mLabelDict;

  // TeX用変数ラベルの辞書
  std::unordered_map<SizeType, std::string> mTexLblDict;

  // グラフの属性リスト
  std::unordered_map<std::string, std::string> mGraphAttrList;

  // 根の属性リスト
  std::unordered_map<std::string, std::string> mRootAttrList;

  // ノードの属性リスト
  std::unordered_map<std::string, std::string> mNodeAttrList;

  // 終端0の属性リスト
  std::unordered_map<std::string, std::string> mTerminal0AttrList;

  // 終端1の属性リスト
  std::unordered_map<std::string, std::string> mTerminal1AttrList;

  // 0枝の属性リスト
  std::unordered_map<std::string, std::string> mEdge0AttrList;

  // 1枝の属性リスト
  std::unordered_map<std::string, std::string> mEdge1AttrList;

};

END_NAMESPACE_YM_DD

#endif // DOTGEN_H
