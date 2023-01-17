#ifndef EXPRMGR_H
#define EXPRMGR_H

/// @file ExprMgr.h
/// @brief ExprMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2020, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Expr.h"
#include "ExprNode.h"
#include "ExprNodePtr.h"


BEGIN_NAMESPACE_YM_LOGIC

class RepStringParser;

//////////////////////////////////////////////////////////////////////
/// @class ExprMgr ExprMgr.h "ExprMgr.h"
/// @brief ExprNode の管理を行うクラス
//////////////////////////////////////////////////////////////////////
class ExprMgr
{
  friend class ExprNode;

private:

  /// @brief コンストラクタ
  ExprMgr();

  /// @brief デストラクタ
  ~ExprMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 静的関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 唯一のインスタンスを返す．
  static
  ExprMgr&
  the_obj();

  /// @brief 確保したメモリを開放する．
  /// @note メモリリークチェックのための関数
  static
  void
  clear_memory();


public:
  //////////////////////////////////////////////////////////////////////
  /// @name ExprNode を作る基本演算
  /// @{

  /// @brief 恒偽関数を作る．
  ExprNodePtr
  make_zero();

  /// @brief 恒真関数を作る．
  ExprNodePtr
  make_one();

  /// @brief 肯定のリテラルを作る．
  ExprNodePtr
  make_posi_literal(
    SizeType varid ///< [in] 変数番号
  );

  /// @brief 否定のリテラルを作る．
  ExprNodePtr
  make_nega_literal(
    SizeType varid ///< [in] 変数番号
  );

  /// @brief AND ノードの生成
  ///
  /// - 子供も AND ノードの場合にはマージする．
  /// - 子供が定数ノードの場合には値に応じた簡単化を行う．
  /// - 同一の子供ノードがあった場合には重複を取り除く
  ExprNodePtr
  make_and(
    SizeType begin ///< [in] ノードスタック中の開始位置
  );

  /// @brief OR ノードの生成
  ///
  /// - 子供も OR ノードの場合にはマージする．
  /// - 子供が定数ノードの場合には値に応じた簡単化を行う．
  /// - 同一の子供ノードがあった場合には重複を取り除く
  ExprNodePtr
  make_or(
    SizeType begin ///< [in] ノードスタック中の開始位置
  );

  /// @brief XOR ノードの生成
  ///
  /// - 子供も XOR ノードの場合にはマージする．
  /// - 子供が定数ノードの場合には値に応じた簡単化を行う．
  /// - 同一の子供ノードがあった場合には個数の偶奇に応じた処理を行う．
  ExprNodePtr
  make_xor(
    SizeType begin ///< [in] ノードスタック中の開始位置
  );

  /// @brief ノードスタックにノードを入れる．
  void
  nodestack_push(
    const ExprNode* node ///< [in] 対象のノード
  );

  /// @brief ノードスタックの先頭位置を返す．
  SizeType
  nodestack_top();

  /// @brief ノードスタックの先頭位置を戻す．
  ///
  /// この関数を明示的に使う必要があるののはノードを
  /// プッシュしておきながら make_XXX を呼ばないとき
  void
  nodestack_pop(
    SizeType oldtop
  );

  /// @brief rep_string 形式の文字列を読み込む．
  ExprNodePtr
  from_rep_string(
    RepStringParser& parser
  );

  /// @}
  //////////////////////////////////////////////////////////////////////


public:

  //////////////////////////////////////////////////////////////////////
  /// @name 式を元に新たな式を作り出す関数
  /// @{

  /// @brief 否定の形(双対形)を返す．
  ExprNodePtr
  complement(
    const ExprNode* node ///< [in] ノード
  );

  /// @brief リテラルを論理式に置き換える．
  ///
  /// sub の中に varid 番目のリテラルが含まれている場合でも
  /// 正常に処理を行う．
  /// sub の中のリテラルは展開しない
  ExprNodePtr
  compose(
    const ExprNode* node,  ///< [in] ノード
    SizeType varid,        ///< [in] 置き換え対象の変数番号
    const ExprNodePtr& sub ///< [in] 置き換え先の論理式
  );

  /// @brief 一度に複数の置き換えを行う．
  /// @param[in] compmap
  ExprNodePtr
  compose(
    const ExprNode* node,                        ///< [in] ノード
    const unordered_map<SizeType, Expr>& compmap ///< [in] 置き換え対象の変数番号と置き換え先の論理式
                                                 /// を入れた連想配列
  );

  /// @brief 変数番号をマッピングし直す
  ExprNodePtr
  remap_var(
    const ExprNode* node,                           ///< [in] ノード
    const unordered_map<SizeType, SizeType>& varmap ///< [in] 置き換え元と置き換え先の変数番号を入れた連想配列
  );

  /// @brief 簡単化を行う．
  ExprNodePtr
  simplify(
    const ExprNode* node ///< [in] ノード
  );

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  // 統計情報を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 使用されているメモリ量を返す．
  SizeType
  used_size();

  /// @brief 使用されているノード数を返す．
  SizeType
  node_num();

  /// @brief used_size() の最大値を返す．
  SizeType
  max_used_size();

  /// @brief nodenum() の最大値を返す．
  SizeType
  max_node_num();

  /// @brief 実際に確保したメモリ量を返す．
  SizeType
  allocated_size();

  /// @brief 実際に確保した回数を返す．
  SizeType
  allocated_count();

  /// @brief 内部状態を出力する．
  void
  print_stats(
    ostream& s ///< [in] 出力ストリーム
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  // make_and()/make_or() 用のサブルーティン
  // node が mTmpNodeList の中に含まれていないか調べる．
  // 同相で同じものがふくまれていたらなにもせず false を返す．
  // 逆相で同じものがふくまれていたらなにもせず true を返す．
  // 同じものがふくまれていなかったら node を mTmpNodeList に追加して
  // false を返す．
  bool
  check_node(
    const ExprNode* node ///< [in] ノード
  );

  // make_xor() 用のサブルーティン
  // node が mTmpNodeList の中に含まれていないか調べる(XOR用)．
  // 同相で同じものが含まれていたら重複したものを mTmpNodeList から削除して
  // false を返す．
  // 逆相で同じものが含まれていたら重複したものを mTmpNodeList から削除して
  // true を返す．
  // 同じものが含まれていなかったら node を mTmpNodeList に追加して
  // false を返す．
  bool
  check_node2(
    const ExprNode* node ///< [in] ノード
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  // id 番めまでのリテラルノードを作る．
  void
  make_literals(
    SizeType id ///< [in] ID
  );

  // ExprNode を確保して内容を設定する．
  ExprNode*
  alloc_node(
    ExprNode::Type type ///< [in] タイプ
  );

  // ExprNode を削除する．
  void
  free_node(
    ExprNode* node ///< [in] ノード
  );

  // ExprNode の入力数から必要なサイズを計算する．
  static
  SizeType
  calc_size(
    SizeType nc ///< [in] 子供のオペランド数
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 唯一の定数0ノード
  ExprNodePtr mConst0;

  // 唯一の定数1ノード
  ExprNodePtr mConst1;

  // リテラルを表すノードの配列
  vector<ExprNodePtr> mLiteralArray;

  // 作業領域として使われるノードの配列
  ExprNodeList mTmpNodeList;

  // 再帰関数のなかで作業領域として使われるノードの配列
  ExprNodeList mNodeStack;

  // 使用中のノード数
  SizeType mNodeNum;

  // 使用した最大のノード数
  SizeType mMaxNodeNum;

  // 絶対に開放されないノード数
  SizeType mStuckNodeNum;

};

END_NAMESPACE_YM_LOGIC

#endif // EXPRMGR_H
