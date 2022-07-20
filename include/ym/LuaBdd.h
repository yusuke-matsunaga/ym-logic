#ifndef LUABDD_H
#define LUABDD_H

/// @file LuaBdd.h
/// @brief LuaBdd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bdd_nsdef.h"
#include <lua.hpp>


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class LuaBdd LuaBdd.h "LuaBdd.h"
/// @brief lua の Bdd 拡張
//////////////////////////////////////////////////////////////////////
class LuaBdd
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief Bdd 関係の初期化を行う．
  static
  void
  init(
    lua_State* L,                  ///< [in] lua インタープリタ
    vector<struct luaL_Reg>& mylib ///< [out] モジュールに登録する関数のリスト
  );

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
  );

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
  );

};

END_NAMESPACE_YM

#endif // LUABDD_H
