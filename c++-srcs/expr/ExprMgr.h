#ifndef EXPRMGR_H
#define EXPRMGR_H

/// @file ExprMgr.h
/// @brief ExprMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Expr.h"
#include "ExprNode.h"


BEGIN_NAMESPACE_YM_LOGIC

class RepStringParser;

//////////////////////////////////////////////////////////////////////
/// @class ExprMgr ExprMgr.h "ExprMgr.h"
/// @brief ExprNode の管理を行うクラス
///
/// 演算子ノードを作る時の一時的なスタックを提供する．
//////////////////////////////////////////////////////////////////////
class ExprMgr
{
public:

  /// @brief コンストラクタ
  ExprMgr() = default;

  /// @brief デストラクタ
  ~ExprMgr() = default;


public:
  //////////////////////////////////////////////////////////////////////
  /// @name ExprNode を作る基本演算
  /// @{

  /// @brief AND ノードの生成
  ///
  /// - 子供も AND ノードの場合にはマージする．
  /// - 子供が定数ノードの場合には値に応じた簡単化を行う．
  /// - 同一の子供ノードがあった場合には重複を取り除く
  Expr::NodePtr
  and_op(
    SizeType begin ///< [in] ノードスタック中の開始位置
  );

  /// @brief OR ノードの生成
  ///
  /// - 子供も OR ノードの場合にはマージする．
  /// - 子供が定数ノードの場合には値に応じた簡単化を行う．
  /// - 同一の子供ノードがあった場合には重複を取り除く
  Expr::NodePtr
  or_op(
    SizeType begin ///< [in] ノードスタック中の開始位置
  );

  /// @brief XOR ノードの生成
  ///
  /// - 子供も XOR ノードの場合にはマージする．
  /// - 子供が定数ノードの場合には値に応じた簡単化を行う．
  /// - 同一の子供ノードがあった場合には個数の偶奇に応じた処理を行う．
  Expr::NodePtr
  xor_op(
    SizeType begin ///< [in] ノードスタック中の開始位置
  );

  /// @brief ノードスタックにノードを入れる．
  void
  nodestack_push(
    const Expr::NodePtr& node ///< [in] 対象のノード
  );

  /// @brief ノードスタックの先頭位置を返す．
  SizeType
  nodestack_top();

  /// @brief ノードスタックの先頭位置を戻す．
  ///
  /// この関数を明示的に使う必要があるののはノードを
  /// プッシュしておきながら XXX_op を呼ばないとき
  void
  nodestack_pop(
    SizeType oldtop
  );

  /// @brief rep_string 形式の文字列を読み込む．
  Expr::NodePtr
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
  Expr::NodePtr
  complement(
    const Expr::NodePtr& node ///< [in] ノード
  );

  /// @brief リテラルを論理式に置き換える．
  ///
  /// sub の中に varid 番目のリテラルが含まれている場合でも
  /// 正常に処理を行う．
  /// sub の中のリテラルは展開しない
  Expr::NodePtr
  compose(
    const Expr::NodePtr& node,  ///< [in] ノード
    SizeType varid,             ///< [in] 置き換え対象の変数番号
    const Expr::NodePtr& sub    ///< [in] 置き換え先の論理式
  );

  /// @brief 一度に複数の置き換えを行う．
  Expr::NodePtr
  compose(
    const Expr::NodePtr& node,      ///< [in] ノード
    const Expr::ComposeMap& compmap ///< [in] 置き換え対象の変数番号と置き換え先の論理式
                                    ///<      を入れた連想配列
  );

  /// @brief 変数番号をマッピングし直す
  Expr::NodePtr
  remap_var(
    const Expr::NodePtr& node, ///< [in] ノード
    const Expr::VarMap& varmap ///< [in] 置き換え元と置き換え先の変数番号を
                               ///<      入れた連想配列
  );

  /// @brief 簡単化を行う．
  Expr::NodePtr
  simplify(
    const Expr::NodePtr& node ///< [in] ノード
  );

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  // and_op()/or_op() 用のサブルーティン
  // node が mTmpNodeList の中に含まれていないか調べる．
  // 同相で同じものがふくまれていたらなにもせず false を返す．
  // 逆相で同じものがふくまれていたらなにもせず true を返す．
  // 同じものがふくまれていなかったら node を mTmpNodeList に追加して
  // false を返す．
  bool
  check_node(
    const Expr::NodePtr& node ///< [in] ノード
  );

  // xor_op() 用のサブルーティン
  // node が mTmpNodeList の中に含まれていないか調べる(XOR用)．
  // 同相で同じものが含まれていたら重複したものを mTmpNodeList から削除して
  // false を返す．
  // 逆相で同じものが含まれていたら重複したものを mTmpNodeList から削除して
  // true を返す．
  // 同じものが含まれていなかったら node を mTmpNodeList に追加して
  // false を返す．
  bool
  check_node2(
    const Expr::NodePtr& node ///< [in] ノード
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 作業領域として使われるノードの配列
  vector<Expr::NodePtr> mTmpNodeList;

  // 再帰関数のなかで作業領域として使われるノードの配列
  vector<Expr::NodePtr> mNodeStack;

};

END_NAMESPACE_YM_LOGIC

#endif // EXPRMGR_H
