#ifndef ZDD_H
#define ZDD_H

/// @file Zdd.h
/// @brief Zdd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/Literal.h"
#include "ym/ZddInfo.h"
#include "ym/BinEnc.h"


BEGIN_NAMESPACE_YM_ZDD

class ZddVarSet;
class ZddEdge;
class ZddMgrImpl;

//////////////////////////////////////////////////////////////////////
/// @class Zdd Zdd.h "ym/Zdd.h"
/// @brief ZDD を表すクラス
//////////////////////////////////////////////////////////////////////
class Zdd
{
  friend class ZddMgr;

public:

  /// @brief 空のコンストラクタ
  ///
  /// 不正な値となる．
  Zdd();

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
  cap_op(
    const Zdd& right ///< [in] オペランド
  ) const;

  /// @brief cap_op の別名
  /// @return 結果を返す．
  Zdd
  operator&(
    const Zdd& right ///< [in] オペランド
  ) const
  {
    return cap_op(right);
  }

  /// @brief ユニオン演算
  /// @return 結果を返す．
  Zdd
  cup_op(
    const Zdd& right ///< [in] オペランド
  ) const;

  /// @brief cup_op の別名
  /// @return 結果を返す．
  Zdd
  operator|(
    const Zdd& right ///< [in] オペランド
  ) const
  {
    return cup_op(right);
  }

  /// @brief 集合差演算
  /// @return 結果を返す．
  Zdd
  diff_op(
    const Zdd& right ///< [in] オペランド
  ) const;

  /// @brief diff_op の別名
  /// @return 結果を返す．
  Zdd
  operator-(
    const Zdd& right ///< [in] オペランド
  ) const
  {
    return diff_op(right);
  }

  /// @brief 直積演算
  /// @return 結果を返す．
  Zdd
  product_op(
    const Zdd& right ///< [in] オペランド
  ) const;

  /// @brief product_op の別名
  /// @return 結果を返す．
  Zdd
  operator*(
    const Zdd& right ///< [in] オペランド
  ) const
  {
    return product_op(right);
  }

  /// @brief 変数を含む集合を求める．
  /// @return 結果を返す．
  Zdd
  onset(
    SizeType var ///< [in] 変数
  ) const;

  /// @brief 変数を含まない集合を求める．
  /// @return 結果を返す．
  Zdd
  offset(
    SizeType var ///< [in] 変数
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
    SizeType var ///< [in] 変数
  );

  /// @brief offset を計算して代入する．
  /// @return 自分自身への参照を返す．
  Zdd&
  offset_int(
    SizeType var ///< [in] 変数
  );

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 内容を取得する関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 適正な値を持っている時に true を返す．
  bool
  is_valid() const
  {
    return mMgr != nullptr;
  }

  /// @brief 不正値の時に true を返す．
  bool
  is_invalid() const
  {
    return !is_valid();
  }

  /// @brief 定数0の時 true を返す．
  bool
  is_zero() const;

  /// @brief 定数1の時 true を返す．
  bool
  is_one() const;

  /// @brief 定数の時 true を返す．
  bool
  is_const() const;

  /// @brief 根の変数とコファクターを求める．
  ///
  /// 自身が葉のノードの場合，BAD_VARID を返す．
  /// f0, f1 には自分自身が入る．
  SizeType
  root_decomp(
    Zdd& f0, ///< [out] 負のコファクター
    Zdd& f1  ///< [out] 正のコファクター
  ) const;

  /// @brief 根の変数を得る．
  ///
  /// 自身が葉のノードの場合，BAD_VARID を返す．
  SizeType
  root_var() const;

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

#if 0
  /// @brief 評価を行う．
  bool
  eval(
    const vector<bool>& inputs ///< [in] 入力値ベクタ
  ) const;
#endif

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

  /// @brief ノード数を返す．
  SizeType
  size() const;

  /// @brief 複数のZDDのノード数を数える．
  static
  SizeType
  size(
    const vector<Zdd>& bdd_list ///< [in] ZDDのリスト
  );

  /// @brief 集合の要素数を数える．
  SizeType
  count() const;

  /// @brief 同じ構造を持つか調べる．
  ///
  /// 同じマネージャに属するZDDなら同じノードだが
  /// マネージャが異なる場合には構造を調べる必要がある．
  bool
  is_identical(
    const Zdd& right ///< [in] 比較対象のZDD
  ) const;

  /// @brief ハッシュ値を返す．
  SizeType
  hash() const;

  /// @brief ノードの情報を取り出す．
  vector<ZddInfo>
  node_info(
    SizeType& root_edge ///< [out] 根の情報を格納する変数
  ) const;

  /// @brief 複数のZDDのノードの情報を取り出す．
  static
  vector<ZddInfo>
  node_info(
    const vector<Zdd>& bdd_list,     ///< [in] ZDDのリスト
    vector<SizeType>& root_edge_list ///< [out] 根の情報を格納するリスト
  );

  /// @brief 内容を出力する．
  void
  display(
    ostream& s ///< [in] 出力ストリーム
  ) const;

  /// @brief 複数のZDDの内容を出力する．
  static
  void
  display(
    ostream& s,                 ///< [in] 出力ストリーム
    const vector<Zdd>& bdd_list ///< [in] ZDDのリスト
  );

  /// @brief dot 形式で出力する．
  void
  gen_dot(
    ostream& s,                                    ///< [in] 出力ストリーム
    const unordered_map<string, string>& attr_dict ///< [in] 属性値の辞書
    = {}
  ) const;

  /// @brief 複数のZDDを dot 形式で出力する．
  static
  void
  gen_dot(
    ostream& s,                                    ///< [in] 出力ストリーム
    const vector<Zdd>& bdd_list,                   ///< [in] ZDDのリスト
    const unordered_map<string, string>& attr_dict ///< [in] 属性値の辞書
    = {}
  );

  /// @brief 独自形式でバイナリダンプする．
  ///
  /// 復元には ZddMgr::restore() を用いる．
  void
  dump(
    BinEnc& s ///< [in] 出力ストリーム
  ) const;

  /// @brief 複数のZDDを独自形式でバイナリダンプする．
  ///
  /// 復元には ZddMgr::restore() を用いる．
  static
  void
  dump(
    BinEnc& s,                  ///< [in] 出力ストリーム
    const vector<Zdd>& bdd_list ///< [in] ZDDのリスト
  );

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を指定したコンストラクタ
  Zdd(
    ZddMgrImpl* mgr,
    ZddEdge root
  );

  /// @brief マネージャが異なる場合コピーする．
  ZddEdge
  copy_edge(
    const Zdd& src
  ) const;

  /// @brief product_op の下請け関数
  static
  ZddEdge
  product_step(
    ZddEdge left,
    ZddEdge right
  );

  /// @brief 根の枝を変更する．
  void
  change_root(
    ZddEdge new_root ///< [in] 変更する枝
  );

  /// @brief ZDDの根の枝のリストを作る．
  static
  ZddMgrImpl*
  root_list(
    const vector<Zdd>& bdd_list, ///< [in] ZDDのリスト
    vector<ZddEdge>& edge_list   ///< [out] 根の枝を格納するのリスト
  );

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
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マネージャ
  ZddMgrImpl* mMgr{nullptr};

  // 根の枝
  PtrIntType mRoot;

};

END_NAMESPACE_YM_ZDD

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
