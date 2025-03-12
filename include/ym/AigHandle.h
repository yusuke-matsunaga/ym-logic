#ifndef AIGHANDLE_H
#define AIGHANDLE_H

/// @file AigHandle.h
/// @brief AigHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/aig.h"
#include "ym/AigMgrHolder.h"
#include "ym/JsonValue.h"


BEGIN_NAMESPACE_YM_AIG

class AigEdge;

//////////////////////////////////////////////////////////////////////
/// @class AigHandle AigHandle.h "AigHandle.h"
/// @brief AIG の枝を表すクラス
///
/// 意味的には AigNode へのポインタと反転属性を持った AigEdge
/// と同様だが，AigNode の管理のために AigMgrImpl のポインタも持つ．
/// AigMgrImpl は参照回数を管理しており，最後の参照がなくなった時に
/// 開放される．
//////////////////////////////////////////////////////////////////////
class AigHandle :
  public AigMgrHolder
{
  friend class AigMgrHolder;

public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief 空のコンストラクタ
  ///
  /// 不正な値となる．
  AigHandle();

  /// @brief コピーコンストラクタ
  AigHandle(
    const AigHandle& src
  );

  /// @brief デストラクタ
  ~AigHandle();

  /// @brief 定数0のハンドルを返す．
  static
  AigHandle
  zero();

  /// @brief 定数1のハンドルを返す．
  static
  AigHandle
  one();


public:
  //////////////////////////////////////////////////////////////////////
  // 内部の情報を取得するメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief マネージャを返す．
  AigMgr
  mgr() const;

  /// @brief マネージャのホルダを返す．
  AigMgrHolder
  mgr_holder() const;

  /// @brief 反転属性を得る．
  bool
  inv() const;

  /// @brief 定数0の時 true を返す．
  bool
  is_zero() const;

  /// @brief 定数1の時 true を返す．
  bool
  is_one() const;

  /// @brief 定数の時 true を返す．
  bool
  is_const() const;

  /// @brief 外部入力ノードへのハンドルの時 true を返す．
  bool
  is_input() const;

  /// @brief 外部入力ノードへのハンドルの時，入力番号を返す．
  ///
  /// is_input() == false の時は例外を送出する．
  SizeType
  input_id() const;

  /// @brief ANDノードへのハンドルの時，true を返す．
  bool
  is_and() const;

  /// @brief ANDノードへのハンドルの時，ファンインのハンドルを返す．
  ///
  /// is_and() == false 時は例外を送出する．
  AigHandle
  fanin(
    SizeType pos ///< [in] 位置 ( 0 or 1 )
  ) const;

  /// @brief ANDノードへのハンドルの時，ファンイン0のハンドルを返す．
  ///
  /// is_and() == false 時は例外を送出する．
  AigHandle
  fanin0() const;

  /// @brief ANDノードへのハンドルの時，ファンイン1のハンドルを返す．
  ///
  /// is_and() == false 時は例外を送出する．
  AigHandle
  fanin1() const;

  /// @brief ANDグループのファンインのリストを返す．
  ///
  /// ANDグループとは反転していない枝で繋がったANDノードの集合
  vector<AigHandle>
  ex_fanin_list() const;

  /// @brief 論理シミュレーションを行う．
  ///
  /// input_vals のサイズが入力数よりも小さい場合は例外が送出される．
  AigBitVect
  eval(
    const vector<AigBitVect>& input_vals ///< [in] 入力値のリスト
  ) const;

  /// @brief コファクター演算
  ///
  /// - cube に含まれるノードの値を固定した AIG を作る．
  AigHandle
  cofactor(
    const vector<AigHandle>& cube ///< [in] コファクターキューブ
  ) const;

  /// @brief ユニークなインデックス値を返す．
  ///
  /// ただしマネージャの情報は見ていない．
  SizeType
  index() const;

  /// @brief ハッシュ値を返す．
  SizeType
  hash() const;

  /// @brief dot 形式で出力する．
  ///
  /// - option は以下のようなキーを持った JSON オブジェクト
  ///   * attr: dot の各種属性値を持った辞書
  ///     属性値は <グループ名> ':' <属性名> で表す．
  ///     グループ名は以下の通り
  ///     - graph:    グラフ全体
  ///     - root:     根のノード
  ///     - node:     通常のノード
  ///     - terminal: 入力ノード
  ///     - edge:     枝
  ///     グループ名と ':' がない場合には全てのグループに対して同一の属性値
  ///     を適用する．
  ///     具体的な属性名と属性値については graphviz の仕様を参照すること．
  ///   * var_label: 変数ラベルを表す配列．配列のキーは変数番号
  ///   * var_texlbl: TeX用の変数ラベルを表す配列．配列のキーは変数番号
  ///   * var_label と var_texlbl は排他的となる．var_texlbl がある時，
  ///     var_label は無視される．
  void
  gen_dot(
    ostream& s,             ///< [in] 出力ストリーム
    const JsonValue& option ///< [in] オプションを表す JSON オブジェクト
    = JsonValue{}
  ) const;

  /// @brief dot 形式で出力する．
  ///
  /// - option は以下のようなキーを持った JSON オブジェクト
  ///   * attr: dot の各種属性値を持った辞書
  ///     属性値は <グループ名> ':' <属性名> で表す．
  ///     グループ名は以下の通り
  ///     - graph:    グラフ全体
  ///     - root:     根のノード
  ///     - node:     通常のノード
  ///     - terminal: 入力ノード
  ///     - edge:     枝
  ///     グループ名と ':' がない場合には全てのグループに対して同一の属性値
  ///     を適用する．
  ///     具体的な属性名と属性値については graphviz の仕様を参照すること．
  ///   * var_label: 変数ラベルを表す配列．配列のキーは変数番号
  ///   * var_texlbl: TeX用の変数ラベルを表す配列．配列のキーは変数番号
  ///   * var_label と var_texlbl は排他的となる．var_texlbl がある時，
  ///     var_label は無視される．
  static
  void
  gen_dot(
    ostream& s,                         ///< [in] 出力ストリーム
    const vector<AigHandle>& root_list, ///< [in] 出力のリスト
    const JsonValue& option             ///< [in] オプションを表す JSON オブジェクト
    = JsonValue{}
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 演算
  //////////////////////////////////////////////////////////////////////

  /// @brief 否定したハンドルを返す．
  AigHandle
  operator~() const;

  /// @brief 反転属性との掛け算
  AigHandle
  operator*(
    bool inv ///< [in] 反転属性
  ) const;

  /// @brief 同じノードを指す反転なしのハンドルを返す．
  AigHandle
  positive_handle() const;

  /// @brief 同じノードを指す反転ありのハンドルを返す．
  AigHandle
  negative_handle() const;

  /// @brief 論理積
  AigHandle
  operator&(
    const AigHandle& right ///< [in] オペランド
  ) const;

  /// @brief 論理積付き代入
  AigHandle&
  operator&=(
    const AigHandle& right ///< [in] オペランド
  );

  /// @brief 論理和
  AigHandle
  operator|(
    const AigHandle& right ///< [in] オペランド
  ) const;

  /// @brief 論理和付き代入
  AigHandle&
  operator|=(
    const AigHandle& right ///< [in] オペランド
  );

  /// @brief 排他的論理和
  AigHandle
  operator^(
    const AigHandle& right ///< [in] オペランド
  ) const;

  /// @brief 排他的論理和付き代入
  AigHandle
  operator^=(
    const AigHandle& right ///< [in] オペランド
  );

  /// @brief 等価比較関数
  bool
  operator==(
    const AigHandle& right ///< [in] オペランド
  ) const;

  /// @brief 非等価比較関数
  bool
  operator!=(
    const AigHandle& right ///< [in] オペランド
  ) const
  {
    return !operator==(right);
  }

  /// @brief 小なり比較演算
  ///
  /// index() の大小で比較する．
  bool
  operator<(
    const AigHandle& right ///< [in] オペランド
  ) const;

  /// @brief 大なり比較演算
  bool
  operator>(
    const AigHandle& right ///< [in] オペランド
  ) const
  {
    return right.operator<(*this);
  }

  /// @brief 小なりイコール比較演算
  bool
  operator<=(
    const AigHandle& right ///< [in] オペランド
  ) const
  {
    return !right.operator<(*this);
  }

  /// @brief 大なりイコール比較演算
  bool
  operator>=(
    const AigHandle& right ///< [in] オペランド
  ) const
  {
    return !operator<(right);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を指定したコンストラクタ
  AigHandle(
    const AigMgrHolder& mgr, ///< [in] マネージャ
    AigEdge edge          ///< [in] 枝
  );

  /// @brief マネージャが同じかチェックする．
  void
  _check_mgr(
    const AigHandle& other
  ) const
  {
    if ( !is_const() &&
	 !other.is_const() &&
	 !check_mgr(other) ) {
      throw std::invalid_argument{"AigMgr mismatch"};
    }
  }

  /// @brief 枝を返す．
  AigEdge
  _edge() const;

  /// @brief 枝をセットする．
  void
  _set_edge(
    AigEdge edge
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 枝
  PtrIntType mEdge{0};

};

END_NAMESPACE_YM_AIG

BEGIN_NAMESPACE_STD

// AigHandle をキーにしたハッシュ関数クラスの定義
template <>
struct hash<YM_NAMESPACE::AigHandle>
{
  SizeType
  operator()(
    const YM_NAMESPACE::AigHandle& h
  ) const
  {
    return h.hash();
  }
};

END_NAMESPACE_STD

#endif // AIGHANDLE_H
