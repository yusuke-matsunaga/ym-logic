#ifndef ZDD_H
#define ZDD_H

/// @file Zdd.h
/// @brief Zdd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/ZddMgrHolder.h"
#include "ym/BinEnc.h"
#include "ym/JsonValue.h"


BEGIN_NAMESPACE_YM_DD

class DdEdge;

//////////////////////////////////////////////////////////////////////
/// @class Zdd Zdd.h "ym/Zdd.h"
/// @brief ZDD を表すクラス
//////////////////////////////////////////////////////////////////////
class Zdd :
  public ZddMgrHolder
{
  friend class ZddMgrHolder;

public:

  /// @brief 空のコンストラクタ
  ///
  /// 不正な値となる．
  Zdd() = default;

  /// @brief コピーコンストラクタ
  Zdd(
    const Zdd& src ///< [in] コピー元のZDD
  );

  /// @brief コピー代入演算子
  Zdd&
  operator=(
    const Zdd& src ///< [in] コピー元のZDD
  );

  /// @brief デストラクタ
  ~Zdd();


public:
  //////////////////////////////////////////////////////////////////////
  /// @name オブジェクト生成用のクラスメソッド
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 不正な値を作る．
  static
  Zdd
  invalid()
  {
    return Zdd{};
  }

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 集合演算
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 補集合演算
  /// @return 結果を返す．
  Zdd
  invert() const;

  /// @brief invert の別名
  /// @return 結果を返す．
  Zdd
  operator~() const
  {
    return invert();
  }

  /// @brief 共通集合演算
  /// @return 結果を返す．
  Zdd
  cap(
    const Zdd& right ///< [in] オペランド
  ) const;

  /// @brief cap_op の別名
  /// @return 結果を返す．
  Zdd
  operator&(
    const Zdd& right ///< [in] オペランド
  ) const
  {
    return cap(right);
  }

  /// @brief ユニオン演算
  /// @return 結果を返す．
  Zdd
  cup(
    const Zdd& right ///< [in] オペランド
  ) const;

  /// @brief cup の別名
  /// @return 結果を返す．
  Zdd
  operator|(
    const Zdd& right ///< [in] オペランド
  ) const
  {
    return cup(right);
  }

  /// @brief 集合差演算
  /// @return 結果を返す．
  Zdd
  diff(
    const Zdd& right ///< [in] オペランド
  ) const;

  /// @brief diff の別名
  /// @return 結果を返す．
  Zdd
  operator-(
    const Zdd& right ///< [in] オペランド
  ) const
  {
    return diff(right);
  }

  /// @brief 直積演算
  /// @return 結果を返す．
  Zdd
  product(
    const Zdd& right ///< [in] オペランド
  ) const;

  /// @brief product の別名
  /// @return 結果を返す．
  Zdd
  operator*(
    const Zdd& right ///< [in] オペランド
  ) const
  {
    return product(right);
  }

  /// @brief 要素を含む集合を求める．
  /// @return 結果を返す．
  Zdd
  onset(
    const ZddItem& item ///< [in] 要素
  ) const;

  /// @brief 要素を含まない集合を求める．
  /// @return 結果を返す．
  Zdd
  offset(
    const ZddItem& item ///< [in] 要素
  ) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 集合演算を伴った代入
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 自分自身を否定する．
  /// @return 自分自身への参照を返す．
  Zdd&
  invert_int();

  /// @brief 共通集合を計算して代入する．
  /// @return 自分自身への参照を返す．
  Zdd&
  cap_int(
    const Zdd& right ///< [in] オペランド
  );

  /// @brief cap_int の別名
  /// @return 自分自身への参照を返す．
  Zdd&
  operator&=(
    const Zdd& right ///< [in] オペランド
  )
  {
    return cap_int(right);
  }

  /// @brief ユニオンを計算して代入する．
  /// @return 自分自身への参照を返す．
  Zdd&
  cup_int(
    const Zdd& right ///< [in] オペランド
  );

  /// @brief cup_int の別名
  /// @return 自分自身への参照を返す．
  Zdd&
  operator|=(
    const Zdd& right ///< [in] オペランド
  )
  {
    return cup_int(right);
  }

  /// @brief 集合差を計算して代入する．
  /// @return 自分自身への参照を返す．
  Zdd&
  diff_int(
    const Zdd& right ///< [in] オペランド
  );

  /// @brief diff_int の別名
  /// @return 自分自身への参照を返す．
  Zdd&
  operator-=(
    const Zdd& right ///< [in] オペランド
  )
  {
    return diff_int(right);
  }

  /// @brief 直積を計算して代入する．
  /// @return 自分自身への参照を返す．
  Zdd&
  product_int(
    const Zdd& right ///< [in] オペランド
  );

  /// @brief product_int の別名
  /// @return 自分自身への参照を返す．
  Zdd&
  operator*=(
    const Zdd& right ///< [in] オペランド
  )
  {
    return product_int(right);
  }

  /// @brief onset を計算して代入する．
  /// @return 自分自身への参照を返す．
  Zdd&
  onset_int(
    const ZddItem& item ///< [in] 要素
  );

  /// @brief offset を計算して代入する．
  /// @return 自分自身への参照を返す．
  Zdd&
  offset_int(
    const ZddItem& item ///< [in] 要素
  );

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 内容を取得する関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数0の時 true を返す．
  bool
  is_zero() const;

  /// @brief 定数1の時 true を返す．
  bool
  is_one() const;

  /// @brief 定数の時 true を返す．
  bool
  is_const() const;

  /// @brief シングルトンの時 true を返す．
  bool
  is_singleton() const;

  /// @brief サポートのリストを返す．
  std::vector<ZddItem>
  get_support_list() const;

  /// @brief 根の要素とコファクターを求める．
  ///
  /// 自身が葉のノードの場合，ZddItem::invalid() を返す．
  /// f0, f1 には自分自身が入る．
  ZddItem
  root_decomp(
    Zdd& f0, ///< [out] 負のコファクター
    Zdd& f1  ///< [out] 正のコファクター
  ) const;

  /// @brief 根の要素を得る．
  ///
  /// 自身が葉のノードの場合，ZddItem::invalid() を返す．
  ZddItem
  root_item() const;

  /// @brief 負のコファクターを返す．
  ///
  /// 自身が葉のノードの場合，自分自身を返す．
  Zdd
  root_cofactor0() const;

  /// @brief 正のコファクターを返す．
  ///
  ///
  /// 自身が葉のノードの場合，自分自身を返す．
  Zdd
  root_cofactor1() const;

  /// @brief 根が否定されている時 true を返す．
  bool
  root_inv() const;

  /// @brief 等価比較演算
  ///
  /// 構造が同じで異なるZDDとの等価比較は
  /// is_identical() で行う．
  bool
  operator==(
    const Zdd& right ///< [in] オペランド
  ) const;

  /// @brief 非等価比較演算
  bool
  operator!=(
    const Zdd& right ///< [in] オペランド2
  ) const
  {
    return !operator==(right);
  }

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name その他の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のマネージャを返す．
  ZddMgr
  mgr() const;

  /// @brief ノード数を返す．
  SizeType
  size() const;

  /// @brief 集合の要素数を数える．
  SizeType
  count() const;

  /// @brief 同じ構造を持つか調べる．
  ///
  /// 同じマネージャに属するZDDなら同じノードだが
  /// マネージャが異なる場合には構造を調べる必要がある．
  /// その場合，変数番号ではなくレベルを参照する．
  bool
  is_identical(
    const Zdd& right ///< [in] 比較対象のZDD
  ) const;

  /// @brief ハッシュ値を返す．
  SizeType
  hash() const;

  /// @brief 内容を出力する．
  void
  display(
    std::ostream& s ///< [in] 出力ストリーム
  ) const;

  /// @brief dot 形式で出力する．
  ///
  /// - option は以下のようなキーを持った JSON オブジェクト
  ///   * attr: dot の各種属性値を持った辞書
  ///     属性値は <グループ名> ':' <属性名> で表す．
  ///     グループ名は以下の通り
  ///     - graph:     グラフ全体
  ///     - root:      根のノード
  ///     - node:      通常のノード
  ///     - terminal:  終端ノード
  ///     - terminal0: 定数0の終端ノード
  ///     - terminal1: 定数1の終端ノード
  ///     グループ名と ':' がない場合には全てのグループに対して同一の属性値
  ///     を適用する．
  ///     具体的な属性名と属性値については graphviz の使用を参照すること．
  ///   * var_label: 変数ラベルを表す配列．配列のキーは変数番号
  ///   * var_texlbl: TeX用の変数ラベルを表す配列．配列のキーは変数番号
  ///   * var_label と var_texlbl は排他的となる．var_texlbl がある時，
  ///     var_label は無視される．
  void
  gen_dot(
    std::ostream& s,        ///< [in] 出力ストリーム
    const JsonValue& option ///< [in] オプションを表す JSON オブジェクト
    = JsonValue{}
  ) const;

  /// @brief 構造を表す整数配列を作る．
  std::vector<SizeType>
  rep_data() const;

  /// @brief 独自形式でバイナリダンプする．
  ///
  /// 復元には ZddMgr::restore() を用いる．
  void
  dump(
    BinEnc& s ///< [in] 出力ストリーム
  ) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 複数のZDDを扱うクラスメソッド
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 複数のZDDのノード数を数える．
  static
  SizeType
  zdd_size(
    const std::vector<Zdd>& zdd_list ///< [in] ZDDのリスト
  );

  /// @brief 複数のZDDの内容を出力する．
  static
  void
  display(
    std::ostream& s,                 ///< [in] 出力ストリーム
    const std::vector<Zdd>& zdd_list ///< [in] ZDDのリスト
  );

  /// @brief 複数のZDDを dot 形式で出力する．
  ///
  /// - option は以下のようなキーを持った JSON オブジェクト
  ///   * attr: dot の各種属性値を持った辞書
  ///     属性値は <グループ名> ':' <属性名> で表す．
  ///     グループ名は以下の通り
  ///     - graph:     グラフ全体
  ///     - root:      根のノード
  ///     - node:      通常のノード
  ///     - terminal:  終端ノード
  ///     - terminal0: 定数0の終端ノード
  ///     - terminal1: 定数1の終端ノード
  ///     グループ名と ':' がない場合には全てのグループに対して同一の属性値
  ///     を適用する．
  ///     具体的な属性名と属性値については graphviz の使用を参照すること．
  ///   * var_label: 変数ラベルを表す配列．配列のキーは変数番号
  ///   * var_texlbl: TeX用の変数ラベルを表す配列．配列のキーは変数番号
  ///   * var_label と var_texlbl は排他的となる．var_texlbl がある時，
  ///     var_label は無視される．
  static
  void
  gen_dot(
    std::ostream& s,                  ///< [in] 出力ストリーム
    const std::vector<Zdd>& zdd_list, ///< [in] ZDDのリスト
    const JsonValue& option           ///< [in] オプションを表す JSON オブジェクト
    = JsonValue{}
  );

  /// @brief 構造を表す整数配列を作る．
  static
  std::vector<SizeType>
  rep_data(
    const std::vector<Zdd>& zdd_list ///< [in] ZDDのリスト
  );

  /// @brief 複数のZDDを独自形式でバイナリダンプする．
  ///
  /// 復元には ZddMgr::restore() を用いる．
  static
  void
  dump(
    BinEnc& s,                       ///< [in] 出力ストリーム
    const std::vector<Zdd>& zdd_list ///< [in] ZDDのリスト
  );

  /// @}
  //////////////////////////////////////////////////////////////////////


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスで用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を指定したコンストラクタ
  Zdd(
    const ZddMgrHolder& mgr,
    DdEdge root
  );

  /// @brief 根の枝を返す．
  DdEdge
  root() const;

  /// @brief 適正な状態か調べる．
  ///
  /// is_valid() でない時は std::invalid_argument 例外を送出する．
  void
  _check_valid() const
  {
    if ( !is_valid() ) {
      throw std::invalid_argument("invalid ZDD");
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 根の枝を変更する．
  void
  _change_root(
    DdEdge new_root ///< [in] 変更する枝
  );

  /// @brief onset() の下請け関数
  DdEdge
  _onset(
    const ZddItem& item ///< [in] 要素
  ) const;

  /// @brief offset() の下請け関数
  DdEdge
  _offset(
    const ZddItem& item ///< [in] 要素
  ) const;

  /// @brief 共通集合演算
  /// @return 結果を返す．
  DdEdge
  _cap(
    const Zdd& right ///< [in] オペランド
  ) const;

  /// @brief ユニオン演算
  /// @return 結果を返す．
  DdEdge
  _cup(
    const Zdd& right ///< [in] オペランド
  ) const;

  /// @brief 集合差演算
  /// @return 結果を返す．
  DdEdge
  _diff(
    const Zdd& right ///< [in] オペランド
  ) const;

  /// @brief 直積演算
  /// @return 結果を返す．
  DdEdge
  _product(
    const Zdd& right ///< [in] オペランド
  ) const;

  /// @brief 補集合演算
  /// @return 結果を返す．
  DdEdge
  _invert() const;

  /// @brief レベルに対応した要素を返す．
  ZddItem
  _level_to_item(
    SizeType level
  ) const;

  /// @brief ZDDのリストからマネージャを取り出す．
  ///
  /// 異なるマネージャを持つZDDが混在している場合，
  /// 例外を送出する．
  /// 空リストの場合は nullptr を返す．
  static
  ZddMgrImpl*
  _mgr(
    const std::vector<Zdd>& zdd_list ///< [in] ZDDのリスト
  );

  /// @brief ZDDのリストから枝のリストに変換する．
  static
  std::vector<DdEdge>
  _conv_to_edgelist(
    const std::vector<Zdd>& zdd_list ///< [in] BDDのリスト
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 根の枝
  PtrIntType mRoot;

};

END_NAMESPACE_YM_DD

BEGIN_NAMESPACE_STD

// Zdd をキーにしたハッシュ関数クラスの定義
template <>
struct hash<YM_NAMESPACE::Zdd>
{
  SizeType
  operator()(
    const YM_NAMESPACE::Zdd& bdd
  ) const
  {
    return bdd.hash();
  }
};

END_NAMESPACE_STD

#endif // ZDD_H
