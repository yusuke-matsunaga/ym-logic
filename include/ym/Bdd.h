#ifndef BDD_H
#define BDD_H

/// @file Bdd.h
/// @brief Bdd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/Literal.h"
#include "ym/BinEnc.h"


BEGIN_NAMESPACE_YM_DD

class BddVarSet;
class DdEdge;
class BddMgrImpl;

//////////////////////////////////////////////////////////////////////
/// @class Bdd Bdd.h "ym/Bdd.h"
/// @brief BDD を表すクラス
//////////////////////////////////////////////////////////////////////
class Bdd
{
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
  operator^(
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
    SizeType var, ///< [in] 変数
    bool inv      ///< [in] 反転フラグ
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
  ) const
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
  operator^=(
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
    SizeType var, ///< [in] 変数
    bool inv      ///< [in] 反転フラグ
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
                    /// そうでない場合は std::invalid_argument 例外が
                    /// 送出される．
  );

  /// @brief cofactor_int の別名
  /// @return 自分自身への参照を返す．
  Bdd&
  operator/=(
    const Bdd& cube ///< [in] コファクターのキューブ
                    /// cube.is_cube() = true でなければならない．
                    /// そうでない場合は std::invalid_argument 例外が
                    /// 送出される．
  )
  {
    return cofactor_int(cube);
  }

  /// @brief If-Then-Else 演算
  friend
  Bdd
  ite(
    const Bdd& cond,   ///< [in] 条件
    const Bdd& then_f, ///< [in] 条件が成り立ったとき選ばれる関数
    const Bdd& else_f  ///< [in] 条件が成り立たなかった時選ばれる関数
  );

  /// @brief ドントケアを利用した簡単化を行う．
  friend
  Bdd
  simplify(
    const Bdd& on,  ///< [in] オンセット
    const Bdd& dc   ///< [in] ドントケアセット
  );

  /// @brief (単一)compose演算
  Bdd
  compose(
    SizeType index,  ///< [in] 対象のインデックス
    const Bdd& cfunc ///< [in] 置き換える関数
  ) const
  {
    return multi_compose({{index, cfunc}});
  }

  /// @brief (単一)compose演算を行って代入する．
  Bdd&
  compose_int(
    SizeType index,  ///< [in] 対象のインデックス
    const Bdd& cfunc ///< [in] 置き換える関数
  )
  {
    return multi_compose_int({{index, cfunc}});
  }

  /// @brief 複合compose演算
  Bdd
  multi_compose(
    const unordered_map<SizeType, Bdd>& compose_map ///< [in] 変換マップ
  ) const;

  /// @brief 複合compose演算を行って代入する．
  Bdd&
  multi_compose_int(
    const unordered_map<SizeType, Bdd>& compose_map ///< [in] 変換マップ
  );

  /// @brief 変数順を入れ替える演算
  ///
  /// 極性も入れ替え可能
  Bdd
  remap_vars(
    const unordered_map<SizeType, Literal>& varmap ///< [in] 変数の対応表
  ) const;

  /// @brief 変数シフト演算
  ///
  /// var をひとつ下にずらす．
  Bdd
  shift_var(
    SizeType var ///< [in] 移動元の変数
  ) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


protected:
  //////////////////////////////////////////////////////////////////////
  /// @name サポート集合を表すBDDに関する演算
  //////////////////////////////////////////////////////////////////////

  /// @brief サポート集合のユニオンを計算して代入する．
  Bdd&
  support_cup_int(
    const Bdd& right ///< [in] 第2オペランド
  );

  /// @brief サポート集合のインターセクションを計算して代入する．
  Bdd&
  support_cap_int(
    const Bdd& right ///< [in] 第2オペランド
  );

  /// @brief サポート集合の差を計算して代入する．
  Bdd&
  support_diff_int(
    const Bdd& right ///< [in] 第2オペランド
  );

  /// @brief サポート集合が共通部分を持つか調べる．
  bool
  support_check_intersect(
    const Bdd& right ///< [in] 第2オペランド
  ) const;

  /// @brief 変数のリストに変換する．
  ///
  /// 変換できない時は例外を送出する．
  vector<SizeType>
  to_varlist() const;


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

  /// @brief 積項の時 true を返す．
  bool
  is_cube() const;

  /// @brief 正リテラルの積項の時 true を返す．
  bool
  is_posicube() const;

  /// @brief サポート変数のサイズを返す．
  SizeType
  support_size() const;

  /// @brief 与えられた変数がサポートの時 true を返す．
  bool
  check_sup(
    SizeType var ///< [in] 変数
  ) const;

  /// @brief 与えられた変数に対して対称の時 true を返す．
  bool
  check_sym(
    SizeType var1,   ///< [in] 変数1
    SizeType var2,   ///< [in] 変数2
    bool inv = false ///< [in] 反転フラグ
  ) const;

  /// @brief サポート変数を表すBDD(BddVarSet)を返す．
  BddVarSet
  get_support() const;

  /// @brief サポート変数のリスト(vector)を得る．
  vector<SizeType>
  get_support_list() const;

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
  /// 自身が葉のノードの場合，BAD_VARID を返す．
  /// f0, f1 には自分自身が入る．
  SizeType
  root_decomp(
    Bdd& f0, ///< [out] 負のコファクター
    Bdd& f1  ///< [out] 正のコファクター
  ) const;

  /// @brief 根の変数を得る．
  ///
  /// 自身が葉のノードの場合，BAD_VARID を返す．
  SizeType
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
  ///
  /// 構造が同じで異なるBDDとの等価比較は
  /// is_identical() で行う．
  bool
  operator==(
    const Bdd& right ///< [in] オペランド
  ) const;

  /// @brief 非等価比較演算
  bool
  operator!=(
    const Bdd& right ///< [in] オペランド2
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
  BddMgr
  mgr() const;

  /// @brief ノード数を返す．
  SizeType
  size() const;

  /// @brief リテラルのリストの変換する．
  ///
  /// 変換できない時は例外を送出する．
  vector<Literal>
  to_litlist() const;

  /// @brief 内容を真理値表の文字列に変換する．
  ///
  /// 実際の変数が input_num を超えた時は例外を送出する．
  string
  to_truth(
    SizeType input_num ///< [in] 入力変数の数
  ) const;

  /// @brief 同じ構造を持つか調べる．
  ///
  /// 同じマネージャに属するBDDなら同じノードだが
  /// マネージャが異なる場合には構造を調べる必要がある．
  bool
  is_identical(
    const Bdd& right ///< [in] 比較対象のBDD
  ) const;

  /// @brief 内容を出力する．
  void
  display(
    ostream& s ///< [in] 出力ストリーム
  ) const;

  /// @brief dot 形式で出力する．
  void
  gen_dot(
    ostream& s,                                    ///< [in] 出力ストリーム
    const unordered_map<string, string>& attr_dict ///< [in] 属性値の辞書
    = {}
  ) const;

  /// @brief 構造を表す整数配列を作る．
  vector<SizeType>
  rep_data() const;

  /// @brief 独自形式でバイナリダンプする．
  ///
  /// 復元には BddMgr::restore() を用いる．
  void
  dump(
    BinEnc& s ///< [in] 出力ストリーム
  ) const;

  /// @brief ハッシュ値を返す．
  SizeType
  hash() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を指定したコンストラクタ
  Bdd(
    BddMgrImpl* mgr,
    DdEdge root
  );

  /// @brief 根の枝を変更する．
  void
  change_root(
    DdEdge new_root ///< [in] 変更する枝
  );

  /// @brief 適正な状態か調べる．
  ///
  /// is_valid() でない時は std::invalid_argument 例外を送出する．
  void
  _check_valid() const
  {
    if ( !is_valid() ) {
      throw std::invalid_argument("invalid BDD");
    }
  }

  /// @brief リテラルリストを表すBDDか調べる．
  ///
  /// is_cube() でない時は std::invalid_argument 例外を送出する．
  void
  _check_cube() const
  {
    if ( !is_cube() ) {
      throw std::invalid_argument("not a cube");
    }
  }

  /// @brief 変数リストを表すBDDか調べる．
  ///
  /// is_posicube() でない時は std::invalid_argument 例外を送出する．
  void
  _check_posicube() const
  {
    if ( !is_posicube() ) {
      throw std::invalid_argument("not a positive cube");
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マネージャ
  BddMgrImpl* mMgr{nullptr};

  // 根の枝
  PtrIntType mRoot;

};

END_NAMESPACE_YM_DD

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
