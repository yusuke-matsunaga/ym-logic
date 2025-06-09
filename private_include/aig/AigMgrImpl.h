#ifndef AIGMGRIMPL_H
#define AIGMGRIMPL_H

/// @file AigMgrImpl.h
/// @brief AigMgrImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/aig.h"
#include "ym/Expr.h"
#include "ym/SopCover.h"
#include "aig/AigNode.h"
#include "AigTable.h"
#include "ym/JsonValue.h"


BEGIN_NAMESPACE_YM_AIG

class ValArray;
class EdgeDict;

//////////////////////////////////////////////////////////////////////
/// @class EventMgr EventMgr.h "EventMgr.h"
/// @brief AIG の変更のイベントを扱う基底クラス
//////////////////////////////////////////////////////////////////////
class EventMgr
{
public:

  /// @brief コンストラクタ
  EventMgr(
    AigMgrImpl* mgr ///< [in] 親のマネージャ
  );

  /// @brief デストラクタ
  virtual
  ~EventMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ハンドルの内容が変化したときに呼ばれる関数
  virtual
  void
  change_proc(
    AigHandle* handle ///< [in] 対象のハンドル
  );

  /// @brief ノードのファンインが変化したときに呼ばれる関数
  virtual
  void
  change_proc(
    AigNode* node ///< [in] 対象のノード
  );

  /// @brief ノードが削除されるときに呼ばれる関数
  virtual
  void
  delete_proc(
    AigNode* node ///< [in] 対象のノード
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のマネージャ
  AigMgrImpl* mMgr{nullptr};

};


//////////////////////////////////////////////////////////////////////
/// @class AigMgrImpl AigMgrImpl.h "AigMgrImpl.h"
/// @brief AigMgr の実装クラス
//////////////////////////////////////////////////////////////////////
class AigMgrImpl
{
  friend class AigHandle;

public:

  /// @brief コンストラクタ
  AigMgrImpl();

  /// @brief デストラクタ
  ~AigMgrImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード数を返す．
  SizeType
  node_num() const
  {
    return mNodeArray.size();
  }

  /// @brief 入力ノード数を返す．
  SizeType
  input_num() const
  {
    return mInputArray.size();
  }

  /// @brief 入力の枝を返す．
  ///
  /// この枝は常に反転なし
  AigEdge
  input(
    SizeType input_id ///< [in] 入力番号 ( 0 <= input_id < input_num() )
  ) const
  {
    if ( input_id >= input_num() ) {
      throw std::out_of_range{"'input_id' is out of range"};
    }
    return AigEdge{mInputArray[input_id], false};
  }

  /// @brief ANDノード数を返す．
  SizeType
  and_num() const;

  /// @brief ANDノードの入力側からのトポロジカル順のリストを得る．
  std::vector<AigNode*>
  and_list() const;

  /// @brief 論理シミュレーションを行う．
  /// @return output_list に対応する出力値のリストを返す．
  AigBitVect
  eval(
    const vector<AigBitVect>& input_vals, ///< [in] 入力値のリスト
    AigEdge output                        ///< [in] 出力
  ) const;

  /// @brief 論理シミュレーションを行う．
  vector<AigBitVect>
  eval(
    const vector<AigBitVect>& input_vals, ///< [in] 入力値のリスト
    const vector<AigEdge>& output_list    ///< [in] 出力のリスト
  ) const;

  /// @brief ノード数を返す．
  SizeType
  size(
    const vector<AigEdge>& edge_list ///< [in] 根の枝のリスト
  ) const;

  /// @brief dot 形式で出力する．
  void
  gen_dot(
    ostream& s,                       ///< [in] 出力ストリーム
    const vector<AigEdge>& edge_list, ///< [in] 根の枝のリスト
    const JsonValue& option           ///< [in] オプションを表す JSON オブジェクト
  ) const;

  /// @brief 外部入力ノードを作る．
  AigEdge
  make_input()
  {
    auto node = _new_input();
    return AigEdge{node, false};
  }

  /// @brief ANDノードを作る．
  AigEdge
  and_op(
    const AigEdge& fanin0, ///< [in] ファンイン0のハンドル
    const AigEdge& fanin1  ///< [in] ファンイン1のハンドル
  )
  {
    // step1: 境界条件
    AigEdge ans_edge;
    if ( _special_case(fanin0, fanin1, ans_edge) ) {
      return ans_edge;
    }

    // step2: 構造テーブルを探す．
    AigNode key{0, fanin0, fanin1};
    auto p = mAndTable.find(&key);
    if ( p != mAndTable.end() ) {
      auto node = *p;
      return AigEdge{node, false};
    }

    // step3: 新しいノードを作る．
    auto node = _new_and(fanin0, fanin1);
    return AigEdge{node, false};
  }

  /// @brief ORノードを作る．
  AigEdge
  or_op(
    const AigEdge& fanin0, ///< [in] ファンイン0のハンドル
    const AigEdge& fanin1  ///< [in] ファンイン1のハンドル
  )
  {
    // DeMorgan の法則
    return ~and_op(~fanin0, ~fanin1);
  }

  /// @brief xORノードを作る．
  AigEdge
  xor_op(
    const AigEdge& fanin0, ///< [in] ファンイン0のハンドル
    const AigEdge& fanin1  ///< [in] ファンイン1のハンドル
  )
  {
    // ab' + a'b を作る．
    auto h1 = and_op(fanin0, ~fanin1);
    auto h2 = and_op(~fanin0, fanin1);
    return or_op(h1, h2);
  }

  /// @brief 複数の入力の AND を表すハンドルを返す．
  AigEdge
  and_op(
    const vector<AigEdge>& fanin_list ///< [in] ファンインのハンドルのリスト
  )
  {
    auto n = fanin_list.size();
    return and_sub(fanin_list, 0, n);
  }

  /// @brief 複数の入力の OR を表すハンドルを返す．
  AigEdge
  or_op(
    const vector<AigEdge>& fanin_list ///< [in] ファンインのハンドルのリスト
  )
  {
    auto n = fanin_list.size();
    return or_sub(fanin_list, 0, n);
  }

  /// @brief 複数の入力の XOR を表すハンドルを返す．
  AigEdge
  xor_op(
    const vector<AigEdge>& fanin_list ///< [in] ファンインのハンドルのリスト
  )
  {
    auto n = fanin_list.size();
    return xor_sub(fanin_list, 0, n);
  }

  /// @brief Expr から変換する．
  AigEdge
  from_expr(
    const Expr& expr ///< [in] 論理式
  )
  {
    auto nmax = expr.input_size();
    while ( input_num() <= nmax ) {
      make_input();
    }
    std::vector<AigEdge> input_list(nmax);
    for ( SizeType i = 0; i < nmax; ++ i ) {
      input_list[i] = AigEdge{mInputArray[i], false};
    }
    return from_expr(expr, input_list);
  }

  /// @brief Expr から変換する．
  AigEdge
  from_expr(
    const Expr& expr,                      ///< [in] 論理式
    const std::vector<AigEdge>& input_list ///< [in] 入力の枝のリスト
  );

  /// @brief 複数の Expr から変換する．
  vector<AigEdge>
  from_expr_list(
    const vector<Expr>& expr_list ///< [in] 論理式のリスト
  )
  {
    SizeType nmax = 0;
    for ( auto& expr: expr_list ) {
      nmax = std::max(nmax, expr.input_size());
    }
    while ( input_num() <= nmax ) {
      make_input();
    }
    std::vector<AigEdge> input_list(nmax);
    for ( SizeType i = 0; i < nmax; ++ i ) {
      input_list[i] = AigEdge{mInputArray[i], false};
    }
    return from_expr_list(expr_list, input_list);
  }

  /// @brief 複数の Expr から変換する．
  vector<AigEdge>
  from_expr_list(
    const vector<Expr>& expr_list,         ///< [in] 論理式のリスト
    const std::vector<AigEdge>& input_list ///< [in] 入力の枝のリスト
  )
  {
    vector<AigEdge> ans_list;
    ans_list.reserve(expr_list.size());
    for ( auto& expr: expr_list ) {
      auto h = from_expr(expr, input_list);
      ans_list.push_back(h);
    }
    return ans_list;
  }

  /// @brief SopCover から変換する．
  ///
  /// カバーの i 番目の変数は i 番目の入力を用いる．
  AigEdge
  from_cover(
    const SopCover& cover ///< [in] カバー
  )
  {
    auto nmax = cover.variable_num();
    while ( input_num() <= nmax ) {
      make_input();
    }
    std::vector<AigEdge> input_list(nmax);
    for ( SizeType i = 0; i < nmax; ++ i ) {
      input_list[i] = AigEdge{mInputArray[i], false};
    }
    return from_cover(cover, input_list);
  }

  /// @brief SopCover から変換する．
  ///
  /// カバーの i 番目の変数は input_list[i] を用いる．
  AigEdge
  from_cover(
    const SopCover& cover,                 ///< [in] カバー
    const std::vector<AigEdge>& input_list ///< [in] 変数に対応する枝のリスト
  );

  /// @brief SopCube から変換する．
  ///
  /// キューブの i 番目の変数は i 番目の入力を用いる．
  AigEdge
  from_cube(
    const SopCube& cube ///< [in] キューブ
  )
  {
    auto nmax = cube.variable_num();
    while ( input_num() <= nmax ) {
      make_input();
    }
    std::vector<AigEdge> input_list(nmax);
    for ( SizeType i = 0; i < nmax; ++ i ) {
      input_list[i] = AigEdge{mInputArray[i], false};
    }
    return from_cube(cube, input_list);
  }

  /// @brief SopCube から変換する．
  ///
  /// キューブの i 番目の変数は input_list[i] を用いる．
  AigEdge
  from_cube(
    const SopCube& cube,                   ///< [in] カバー
    const std::vector<AigEdge>& input_list ///< [in] 変数に対応する枝のリスト
  );

  /// @brief コファクター演算
  ///
  /// - cube に含まれるノードの値を固定した AIG を作る．
  AigEdge
  cofactor(
    const vector<AigEdge>& cedge_list, ///< [in] コファクターキューブ
    AigEdge edge                       ///< [in] 対象の枝
  );

  /// @brief コファクター演算
  ///
  /// - cube に含まれるノードの値を固定した AIG を作る．
  vector<AigEdge>
  cofactor(
    const vector<AigEdge>& cedge_list, ///< [in] コファクターキューブ
    const vector<AigEdge>& edge_list   ///< [in] 対象の枝のリスト
  );

  /// @brief AND ノードを探す．
  ///
  /// なければ nullptr を返す．
  AigNode*
  find_and(
    const AigEdge& fanin0, ///< [in] ファンイン0のハンドル
    const AigEdge& fanin1  ///< [in] ファンイン1のハンドル
  ) const
  {
    AigNode key{0, fanin0, fanin1};
    auto p = mAndTable.find(&key);
    if ( p != mAndTable.end() ) {
      return *p;
    }
    return nullptr;
  }

  /// @brief local rewriting を行う．
  void
  rewrite();

  /// @brief 参照回数が0のノードを取り除く
  ///
  /// ノードのID番号が変わる可能性がある．
  void
  sweep();

  /// @brief AigHandle を登録する．
  void
  add_handle(
    AigHandle* handle ///< [in] 対象のハンドル
  )
  {
    auto edge = handle->_edge();
    if ( mHandleHash.count(handle) > 0 ) {
      throw std::logic_error{"already exists"};
    }
    mHandleHash.emplace(handle);
  }

  /// @brief AigHandle の登録を削除する．
  void
  delete_handle(
    AigHandle* handle ///< [in] 対象のハンドル
  )
  {
    auto edge = handle->_edge();
    mHandleHash.erase(handle);
  }

  /// @brief EventMgr を追加する．
  void
  add_event_mgr(
    EventMgr* evmgr ///< [in] イベントマネージャ
  )
  {
    mEvHash.emplace(evmgr);
  }

  /// @brief EventMgr を削除する．
  void
  delete_event_mgr(
    EventMgr* evmgr ///< [in] イベントマネージャ
  )
  {
    mEvHash.erase(evmgr);
  }

  /// @brief 内容を出力する．
  void
  print(
    std::ostream& s ///< [in] 出力ストリーム
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief and_op() の下請け関数
  AigEdge
  and_sub(
    const vector<AigEdge>& fanin_list, ///< [in] ファンインのリスト
    SizeType begin,                    ///< [in] 開始位置
    SizeType end                       ///< [in] 終了位置
  );

  /// @brief or_op() の下請け関数
  AigEdge
  or_sub(
    const vector<AigEdge>& fanin_list, ///< [in] ファンインのリスト
    SizeType begin,                    ///< [in] 開始位置
    SizeType end                       ///< [in] 終了位置
  );

  /// @brief xor_op() の下請け関数
  AigEdge
  xor_sub(
    const vector<AigEdge>& fanin_list, ///< [in] ファンインのリスト
    SizeType begin,                    ///< [in] 開始位置
    SizeType end                       ///< [in] 終了位置
  );

  /// @brief eval() の下請け関数
  void
  eval_sub(
    const vector<AigBitVect>& input_vals, ///< [in] 入力値のリスト
    ValArray& val_array ///< [in] シミュレーション結果を格納するオブジェクト
  ) const;

  /// @brief cofactor() の下請け関数
  AigEdge
  cofactor_sub(
    AigEdge edge,      ///< [in] 対象の枝
    EdgeDict& res_dict ///< [in] 結果を格納する辞書
  );

  /// @brief 境界条件を調べる．
  /// @return 条件が成り立った時に true を返す．
  bool
  _special_case(
    AigEdge fanin0,   ///< [in] ファンイン0
    AigEdge fanin1,   ///< [in] ファンイン1
    AigEdge& new_edge ///< [out] 結果の枝
  )
  {
    if ( fanin0.is_zero() ) {
      new_edge = AigEdge::zero();
      return true;
    }
    if ( fanin1.is_zero() ) {
      new_edge = AigEdge::zero();
      return true;
    }
    if ( fanin0.is_one() ) {
      new_edge = fanin1;
      return true;
    }
    if ( fanin1.is_one() ) {
      new_edge = fanin0;
      return true;
    }
    if ( fanin0 == fanin1 ) {
      new_edge = fanin0;
      return true;
    }
    if ( fanin0 == ~fanin1 ) {
      new_edge = AigEdge::zero();
      return true;
    }
    return false;
  }

  /// @brief ノードのファンインを変更する．
  void
  _change_fanin(
    AigNode* node,  ///< [in] 対象のノード
    AigEdge fanin0, ///< [in] ファンイン0
    AigEdge fanin1  ///< [in] ファンイン1
  );

  /// @brief change イベントを伝える．
  void
  _propagate_change(
    AigHandle* handle ///< [in] 変更のあったハンドル
  )
  {
    for ( auto evmgr: mEvHash ) {
      evmgr->change_proc(handle);
    }
  }

  /// @brief change イベントを伝える．
  void
  _propagate_change(
    AigNode* node ///< [in] 変更のあったノード
  )
  {
    for ( auto evmgr: mEvHash ) {
      evmgr->change_proc(node);
    }
  }

  /// @brief delete イベントを伝える．
  void
  _propagate_delete(
    AigNode* node ///< [in] 削除されたノード
  )
  {
    for ( auto evmgr: mEvHash ) {
      evmgr->delete_proc(node);
    }
  }

  /// @brief 入力ノードを作る．
  AigNode*
  _new_input()
  {
    auto input_id = mInputArray.size();
    auto id = mNodeArray.size();
    auto node = new AigNode(id, input_id);
    mNodeArray.push_back(std::unique_ptr<AigNode>{node});
    mInputArray.push_back(node);
    return node;
  }

  /// @brief ANDノードを作る．
  AigNode*
  _new_and(
    AigEdge fanin0,
    AigEdge fanin1
  );

  /// @brief ノードの参照回数を増やす．
  ///
  /// 場合によってはファンインのノードに再帰する．
  void
  _inc_node_ref(
    AigNode* node
  );

  /// @brief ノードの参照回数を減らす．
  ///
  /// 場合によってはファンインのノードに再帰する．
  /// 参照回数が 0 になったノードは sweep() で削除される．
  void
  _dec_node_ref(
    AigNode* node
  );

  /// @brief 内部情報が正しいかチェックする．
  void
  _sanity_check() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 参照回数
  SizeType mRefCount{0};

  // ID番号をキーにして AigNode を収めた配列
  // AigNode の所有権を持つ．
  std::vector<std::unique_ptr<AigNode>> mNodeArray;

  // 入力番号をキーにして入力ノードを収めた配列
  std::vector<AigNode*> mInputArray;

  // ANDノードの構造ハッシュ
  AigTable mAndTable;

  // EventMgr のハッシュ
  std::unordered_set<EventMgr*> mEvHash;

  // AigHandle のハッシュ
  std::unordered_set<AigHandle*> mHandleHash;

};

END_NAMESPACE_YM_AIG

#endif // AIGMGRIMPL_H
