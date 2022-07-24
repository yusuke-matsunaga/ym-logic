#ifndef BDD_H
#define BDD_H

/// @file Bdd.h
/// @brief Bdd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bdd_nsdef.h"
#include "ym/VarId.h"
#include "ym/Literal.h"


BEGIN_NAMESPACE_YM_BDD

class BddEdge;
class BddMgrImpl;

//////////////////////////////////////////////////////////////////////
/// @class Bdd Bdd.h "ym/Bdd.h"
/// @brief BDD を表すクラス
//////////////////////////////////////////////////////////////////////
class Bdd
{
  friend class BddMgr;
  friend class BddMgrImpl;

public:

  /// @brief 空のコンストラクタ
  ///
  /// 不正な値となる．
  Bdd();

  /// @brief コピーコンストラクタ
  Bdd(
    const Bdd& src ///< [in] コピー元のBDD
  );

  /// @brief コピー代入演算子
  Bdd&
  operator=(
    const Bdd& src ///< [in] コピー元のBDD
  );

  /// @brief デストラクタ
  ~Bdd();


public:
  //////////////////////////////////////////////////////////////////////
  /// @name オブジェクト生成用のクラスメソッド
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 不正な値を作る．
  static
  Bdd
  invalid()
  {
    return Bdd{};
  }

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 論理演算
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 否定した関数を返す．
  /// @return 結果を返す．
  Bdd
  invert() const;

  /// @brief invert の別名
  /// @return 結果を返す．
  Bdd
  operator~() const
  {
    return invert();
  }

  /// @brief 極性をかけ合わせる．
  Bdd
  operator*(
    bool inv ///< [in] 反転フラグ
  ) const;

  /// @brief 論理積を返す．
  /// @return 結果を返す．
  Bdd
  and_op(
    const Bdd& right ///< [in] オペランド
  ) const;

  /// @brief and_op の別名
  /// @return 結果を返す．
  Bdd
  operator&(
    const Bdd& right ///< [in] オペランド
  ) const
  {
    return and_op(right);
  }

  /// @brief 論理和を返す．
  /// @return 結果を返す．
  Bdd
  or_op(
    const Bdd& right ///< [in] オペランド
  ) const;

  /// @brief or_op の別名
  /// @return 結果を返す．
  Bdd
  operator|(
    const Bdd& right ///< [in] オペランド
  ) const
  {
    return or_op(right);
  }

  /// @brief 排他的論理和を返す．
  /// @return 結果を返す．
  Bdd
  xor_op(
    const Bdd& right ///< [in] オペランド
  ) const;

  /// @brief xor_op の別名
  /// @return 結果を返す．
  Bdd
  operator^(
    const Bdd& right ///< [in] オペランド
  ) const
  {
    return xor_op(right);
  }

  /// @brief コファクターを計算する．
  /// @return 結果を返す．
  Bdd
  cofactor(
    VarId var, ///< [in] 変数
    bool inv   ///< [in] 反転フラグ
               ///<  - false: 反転なし (正極性)
               ///<  - true:  反転あり (負極性)
  ) const;

  /// @brief コファクターを計算する．
  /// @return 結果を返す．
  Bdd
  cofactor(
    Literal lit ///< [in] リテラル
  ) const;

  /// @brief cofactor の別名
  Bdd
  operator/(
    Literal lit ///< [in] リテラル
  ) const
  {
    return cofactor(lit);
  }

  /// @brief コファクターを計算する．
  Bdd
  cofactor(
    const Bdd& cube ///< [in] コファクターのキューブ
                    /// cube.is_cube() = true でなければならない．
  ) const;

  /// @brief cofactor の別名
  Bdd
  operator/(
    const Bdd& cube ///< [in] コファクターのキューブ
                    /// cube.is_cube() = true でなければならない．
  )
  {
    return cofactor(cube);
  }

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 論理演算を伴った代入
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 自分自身を否定する．
  /// @return 自分自身への参照を返す．
  Bdd&
  invert_int();

  /// @brief 極性をかけ合わせて代入する．
  /// @return 自分自身への参照を返す．
  Bdd&
  operator*=(
    bool inv ///< [in] 反転フラグ
  );

  /// @brief 論理積を計算して代入する．
  /// @return 自分自身への参照を返す．
  Bdd&
  and_int(
    const Bdd& right ///< [in] オペランド
  );

  /// @brief and_int の別名
  /// @return 自分自身への参照を返す．
  Bdd&
  operator&=(
    const Bdd& right ///< [in] オペランド
  )
  {
    return and_int(right);
  }

  /// @brief 論理和を計算して代入する．
  /// @return 自分自身への参照を返す．
  Bdd&
  or_int(
    const Bdd& right ///< [in] オペランド
  );

  /// @brief or_int の別名
  /// @return 自分自身への参照を返す．
  Bdd&
  operator|=(
    const Bdd& right ///< [in] オペランド
  )
  {
    return or_int(right);
  }

  /// @brief 排他的論理和を計算して代入する．
  /// @return 自分自身への参照を返す．
  Bdd&
  xor_int(
    const Bdd& right ///< [in] オペランド
  );

  /// @brief xor_int の別名
  /// @return 自分自身への参照を返す．
  Bdd&
  operator^=(
    const Bdd& right ///< [in] オペランド
  )
  {
    return xor_int(right);
  }

  /// @brief コファクターを計算して代入する．
  /// @return 自分自身への参照を返す．
  Bdd&
  cofactor_int(
    VarId var, ///< [in] 変数
    bool inv   ///< [in] 反転フラグ
               ///<  - false: 反転なし (正極性)
               ///<  - true:  反転あり (負極性)
  );

  /// @brief コファクターを計算して代入する．
  /// @return 自分自身への参照を返す．
  Bdd&
  cofactor_int(
    Literal lit ///< [in] リテラル
  );

  /// @brief cofactor_int の別名
  /// @return 自分自身への参照を返す．
  Bdd&
  operator/=(
    Literal lit ///< [in] リテラル
  )
  {
    return cofactor_int(lit);
  }

  /// @brief コファクターを計算して代入する．
  /// @return 自分自身への参照を返す．
  Bdd&
  cofactor_int(
    const Bdd& cube ///< [in] コファクターのキューブ
                    /// cube.is_cube() = true でなければならない．
  );

  /// @brief cofactor_int の別名
  /// @return 自分自身への参照を返す．
  Bdd&
  operator/=(
    const Bdd& cube ///< [in] コファクターのキューブ
                    /// cube.is_cube() = true でなければならない．
  )
  {
    return cofactor_int(cube);
  }

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

  /// @brief 積項の時 true を返す．
  bool
  is_cube() const;

  /// @brief 正リテラルの積項の時 true を返す．
  bool
  is_posicube() const;

  /// @brief 与えられた変数がサポートの時 true を返す．
  bool
  check_sup(
    VarId var ///< [in] 変数
  ) const;

  /// @brief 与えられた変数に対して対称の時 true を返す．
  bool
  check_sym(
    VarId var1,      ///< [in] 変数1
    VarId var2,      ///< [in] 変数2
    bool inv = false ///< [in] 反転フラグ
  ) const;

  /// @brief サポート変数のリストを得る．
  /// @return サポート変数の論理積を表すBDDを返す．
  Bdd
  get_support() const;

  /// @brief 1となるパスを求める．
  /// @return 論理積を表すBDDを返す．
  ///
  /// 結果のBDDは is_cube() = true となっている．
  Bdd
  get_onepath() const;

  /// @brief 0となるパスを求める．
  /// @return 論理積を表すBDDを返す．
  ///
  /// 結果のBDDは is_cube() = true となっている．
  Bdd
  get_zeropath() const;

  /// @brief 根の変数とコファクターを求める．
  ///
  /// 自身が葉のノードの場合，不正な変数を返す．
  /// f0, f1 には自分自身が入る．
  VarId
  root_decomp(
    Bdd& f0, ///< [out] 負のコファクター
    Bdd& f1  ///< [out] 正のコファクター
  ) const;

  /// @brief 根の変数を得る．
  ///
  /// 自身が葉のノードの場合，不正な変数を返す．
  VarId
  root_var() const;

  /// @brief 負のコファクターを返す．
  ///
  /// 自身が葉のノードの場合，自分自身を返す．
  Bdd
  root_cofactor0() const;

  /// @brief 正のコファクターを返す．
  ///
  ///
  /// 自身が葉のノードの場合，自分自身を返す．
  Bdd
  root_cofactor1() const;

  /// @brief 根が否定されている時 true を返す．
  bool
  root_inv() const;

  /// @brief 評価を行う．
  bool
  eval(
    const vector<bool>& inputs ///< [in] 入力値ベクタ
  ) const;

  /// @brief 等価比較演算
  bool
  operator==(
    const Bdd& right ///< [in] オペランド
  ) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name その他の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード数を返す．
  SizeType
  size() const;

  /// @brief 複数のBDDのノード数を数える．
  static
  SizeType
  size(
    const vector<Bdd>& bdd_list ///< [in] BDDのリスト
  );

  /// @brief 変数のリストに変換する．
  ///
  /// 変換できない時は例外を送出する．
  vector<VarId>
  to_varlist() const;

  /// @brief リテラルのリストの変換する．
  ///
  /// 変換できない時は例外を送出する．
  vector<Literal>
  to_litlist() const;

  /// @brief 内容を真理値表の文字列に変換する．
  ///
  /// 実際の変数が input_num を超えた時の動作は不定
  string
  to_truth(
    SizeType input_num ///< [in] 入力変数の数
  ) const;

  /// @brief ハッシュ値を返す．
  SizeType
  hash() const;

  /// @brief 内容を出力する．
  void
  display(
    ostream& s ///< [in] 出力ストリーム
  ) const;

  /// @brief dot 形式で出力する．
  void
  gen_dot(
    ostream& s ///< [in] 出力ストリーム
  ) const;

  /// @brief 複数のBDDを dot 形式で出力する．
  static
  void
  gen_dot(
    ostream& s,                 ///< [in] 出力ストリーム
    const vector<Bdd>& bdd_list ///< [in] BDDのリスト
  );

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を指定したコンストラクタ
  Bdd(
    BddMgrImpl* mgr,
    BddEdge root
  );

  /// @brief マネージャが異なる場合コピーする．
  BddEdge
  copy_edge(
    const Bdd& src
  ) const;

  /// @brief 根の枝を変更する．
  void
  change_root(
    BddEdge new_root ///< [in] 変更する枝
  );

  /// @brief BDDの根の枝のリストを作る．
  static
  BddMgrImpl*
  root_list(
    const vector<Bdd>& bdd_list, ///< [in] BDDのリスト
    vector<BddEdge>& edge_list   ///< [out] 根の枝を格納するのリスト
  );

  /// @brief If-Then-Else 演算
  friend
  Bdd
  ite(
    const Bdd& cond,   ///< [in] 条件
    const Bdd& then_f, ///< [in] 条件が成り立ったとき選ばれる関数
    const Bdd& else_f  ///< [in] 条件が成り立たなかった時選ばれる関数
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マネージャ
  BddMgrImpl* mMgr{nullptr};

  // 根の枝
  ympuint mRoot;

};

/// @brief 非等価比較演算
inline
bool
operator!=(
  const Bdd& left, ///< [in] オペランド1
  const Bdd& right ///< [in] オペランド2
)
{
  return !left.operator==(right);
}

END_NAMESPACE_YM_BDD

BEGIN_NAMESPACE_STD

// Bdd をキーにしたハッシュ関数クラスの定義
template <>
struct hash<YM_NAMESPACE::Bdd>
{
  SizeType
  operator()(
    const YM_NAMESPACE::Bdd& bdd
  ) const
  {
    return bdd.hash();
  }
};

END_NAMESPACE_STD

#endif // BDD_H
