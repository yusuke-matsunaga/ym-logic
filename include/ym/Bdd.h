#ifndef BDD_H
#define BDD_H

/// @file Bdd.h
/// @brief Bdd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"


BEGIN_NAMESPACE_YM

class BddEdge;
class BddMgrImpl;
class BddVar;

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

  /// @brief 代入演算子
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
  make_invalid()
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
  ) const;

  /// @brief コファクターを計算する．
  /// @return 結果を返す．
  Bdd
  cofactor(
    BddVar var, ///< [in] 変数
    bool inv    ///< [in] 反転フラグ
                ///<  - false: 反転なし (正極性)
                ///<  - true:  反転あり (負極性)
  ) const;

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
    BddVar var, ///< [in] 変数
    bool inv    ///< [in] 反転フラグ
                ///<  - false: 反転なし (正極性)
                ///<  - true:  反転あり (負極性)
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

  /// @brief 与えられた変数がサポートの時 true を返す．
  bool
  check_sup(
    BddVar var ///< [in] 変数
  ) const;

  /// @brief 与えられた変数に対して対称の時 true を返す．
  bool
  check_sym(
    BddVar var1,     ///< [in] 変数1
    BddVar var2,     ///< [in] 変数2
    bool inv = false ///< [in] 反転フラグ
  ) const;

  /// @brief 根の変数とコファクターを求める．
  ///
  /// 自身が葉のノードの場合，不正な変数を返す．
  /// f0, f1 には自分自身が入る．
  BddVar
  root_decomp(
    Bdd& f0, ///< [out] 負のコファクター
    Bdd& f1  ///< [out] 正のコファクター
  ) const;

  /// @brief 根の変数を得る．
  ///
  /// 自身が葉のノードの場合，不正な変数を返す．
  BddVar
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

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name その他の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード数を返す．
  SizeType
  size() const;

  /// @brief ハッシュ値を返す．
  SizeType
  hash() const;

  /// @brief 内容を出力する．
  void
  display(
    ostream& s ///< [in] 出力ストリーム
  ) const;

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

  /// @brief 根の枝を変更する．
  void
  change_root(
    BddEdge new_root ///< [in] 変更する枝
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

END_NAMESPACE_YM

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