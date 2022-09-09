#ifndef LUABDD_H
#define LUABDD_H

/// @file LuaBdd.h
/// @brief LuaBdd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bdd_nsdef.h"
#include "ym/Bdd.h"
#include "ym/Luapp.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class LuaBdd LuaBdd.h "LuaBdd.h"
/// @brief lua の Bdd 拡張
//////////////////////////////////////////////////////////////////////
class LuaBdd :
  public Luapp
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief 空のコンストラクタ
  ///
  /// デフォルトの状態で初期化される．<br>
  /// このオブジェクトとともに Lua インタプリタも破壊される．
  LuaBdd() = default;

  /// @brief 詳細なパラメータを指定したコンストラクタ
  ///
  /// このオブジェクトとともに Lua インタプリタも破壊される．
  LuaBdd(
    lua_Alloc f, ///< [in] メモリアロケータ
    void* ud     ///< [in] ユーザーデータ
  ) : Luapp{f, ud}
  {
  }

  /// @brief すでに生成済みのインタプリタを用いるコンストラクタ
  ///
  /// このオブジェクトが破壊されても Lua インタプリタは破壊されない．
  LuaBdd(
    lua_State* L ///< [in] lua インタプリタ
  ) : Luapp{L}
  {
  }

  /// @brief デストラクタ
  ///
  /// 自前で Lua インタプリタを生成した場合には破壊される．
  ~LuaBdd() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief Bdd 関係の初期化を行う．
  void
  init(
    const char* parent, ///< [in] 親のモジュール名
    const char* name    ///< [in] 自身の名前
  );

  /// @brief 対象が BddMgr の時 true を返す．
  bool
  is_bddmgr(
    int idx       ///< [in] スタック上のインデックス
  )
  {
    auto obj = to_bddmgr(idx);
    return obj != nullptr;
  }

  /// @brief 対象を BddMgr として取り出す．
  ///
  /// BddMgr でない時は nullptr を返す．
  BddMgr*
  to_bddmgr(
    int idx       ///< [in] スタック上のインデックス
  );

  /// @brief 対象が Bdd の時 true を返す．
  bool
  is_bdd(
    int idx       ///< [in] スタック上のインデックス
  )
  {
    auto obj = to_bdd(idx);
    return obj != nullptr;
  }

  /// @brief 対象を Bdd として取り出す．
  ///
  /// Bdd でない時は nullptr を返す．
  Bdd*
  to_bdd(
    int idx       ///< [in] スタック上のインデックス
  );

  /// @brief スタックから Bdd のリストを取り出す．
  vector<Bdd>
  to_bdd_list(
    int idx	  ///< [in] スタック上のインデックス
  );

  /// @brief lua の管理下にある Bdd オブジェクトを作る．
  Bdd*
  new_bdd();

  /// @brief 対象が BddVarSet の時 true を返す．
  bool
  is_bddvarset(
    int idx       ///< [in] スタック上のインデックス
  )
  {
    auto obj = to_bddvarset(idx);
    return obj != nullptr;
  }

  /// @brief 対象を BddVarSet として取り出す．
  ///
  /// BddVarSet でない時は nullptr を返す．
  BddVarSet*
  to_bddvarset(
    int idx       ///< [in] スタック上のインデックス
  );

  /// @brief lua の管理下にある BddVarSet オブジェクトを作る．
  BddVarSet*
  new_bddvarset();


public:
  //////////////////////////////////////////////////////////////////////
  // クラスメソッド
  // 同名のメンバ関数があるが，こちらはこのクラスを継承しないで
  // インターフェイスのみを継承したいときに用いる．
  //////////////////////////////////////////////////////////////////////

  /// @brief Bdd 関係の初期化を行う．
  static
  void
  init(
    lua_State* L,       ///< [in] lua インタープリタ
    const char* parent, ///< [in] 親のモジュール名
    const char* name    ///< [in] 自身の名前
  )
  {
    LuaBdd lua{L};
    lua.init(parent, name);
  }

  /// @brief 対象が BddMgr の時 true を返す．
  static
  bool
  is_bddmgr(
    lua_State* L, ///< [in] lua インタープリタ
    int idx       ///< [in] スタック上のインデックス
  )
  {
    auto obj = to_bddmgr(L, idx);
    return obj != nullptr;
  }

  /// @brief 対象を BddMgr として取り出す．
  ///
  /// BddMgr でない時は nullptr を返す．
  static
  BddMgr*
  to_bddmgr(
    lua_State* L, ///< [in] lua インタープリタ
    int idx       ///< [in] スタック上のインデックス
  )
  {
    LuaBdd lua{L};
    return lua.to_bddmgr(idx);
  }

  /// @brief 対象が Bdd の時 true を返す．
  static
  bool
  is_bdd(
    lua_State* L, ///< [in] lua インタープリタ
    int idx       ///< [in] スタック上のインデックス
  )
  {
    auto obj = to_bdd(L, idx);
    return obj != nullptr;
  }

  /// @brief 対象を Bdd として取り出す．
  ///
  /// Bdd でない時は nullptr を返す．
  static
  Bdd*
  to_bdd(
    lua_State* L, ///< [in] lua インタープリタ
    int idx       ///< [in] スタック上のインデックス
  )
  {
    LuaBdd lua{L};
    return lua.to_bdd(idx);
  }

  /// @brief スタックから Bdd のリストを取り出す．
  static
  vector<Bdd>
  to_bdd_list(
    lua_State* L, ///< [in] lua インタープリタ
    int idx	  ///< [in] スタック上のインデックス
  )
  {
    LuaBdd lua{L};
    return lua.to_bdd_list(idx);
  }

};

END_NAMESPACE_YM

#endif // LUABDD_H
