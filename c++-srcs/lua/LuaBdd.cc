
/// @file LuaBdd.cc
/// @brief LuaBdd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/LuaBdd.h"
#include "ym/Luapp.h"
#include "ym/Bdd.h"
#include "ym/BddMgr.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// BddMgr 用のシグネチャ
const char* BDDMGR_SIGNATURE{"Luapp.BddMgr"};

// Bdd 用のシグネチャ
const char* BDD_SIGNATURE{"Luapp.Bdd"};

// BddMgr を生成する関数
int
bddmgr_new(
  lua_State* L
)
{
  Luapp lua{L};

  // メモリ領域は Lua で確保する．
  void* p = lua.new_userdata(sizeof(BddMgr));
  auto obj = new (p) BddMgr{};

  // BddMgr 用の metatable を取ってくる．
  lua.L_getmetatable(BDDMGR_SIGNATURE);

  // それを今作った userdata にセットする．
  lua.set_metatable(-2);

  return 1;
}

// BddMgr 用のデストラクタ
int
bddmgr_gc(
  lua_State* L
)
{
  auto obj = LuaBdd::to_bddmgr(L, 1);
  if ( obj ) {
    // デストラクタを明示的に起動する．
    obj->~BddMgr();
  }

  // メモリは Lua が開放する．
  return 0;
}

// Bdd を生成する関数
Bdd*
bdd_new(
  Luapp& lua
)
{
  // メモリ領域は Lua で確保する．
  void* p = lua.new_userdata(sizeof(Bdd));
  auto obj = new (p) Bdd{};

  // Bdd 用の metatable を取ってくる．
  lua.L_getmetatable(BDD_SIGNATURE);

  // それを今作った userdata にセットする．
  lua.set_metatable(-2);

  return obj;
}

// Bdd 用のデストラクタ
int
bdd_gc(
  lua_State* L
)
{
  auto obj = LuaBdd::to_bdd(L, 1);
  if ( obj ) {
    // デストラクタを明示的に起動する．
    obj->~Bdd();
  }

  // メモリは Lua が開放する．
  return 0;
}

// 恒儀関数を作る．
int
bddmgr_make_zero(
  lua_State* L
)
{
  Luapp lua{L};

  SizeType n = lua.get_top();
  if ( n != 1 ) {
    return lua.error_end("Error: BddMgr:make_zero() expects no arguments.");
  }

  auto obj = LuaBdd::to_bddmgr(L, 1);
  ASSERT_COND( obj != nullptr );

  auto bddp = bdd_new(lua);
  auto src = obj->make_zero();
  src.display(cout);

  *bddp = src;


  return 1;
}

// 内容を出力する．
int
bdd_display(
  lua_State* L
)
{
  Luapp lua{L};

  SizeType n = lua.get_top();
  if ( n != 1 && n != 2 ) {
    return lua.error_end("Error: Bdd:display() expects at most one argument.");
  }

  auto obj = LuaBdd::to_bdd(L, 1);
  ASSERT_COND( obj != nullptr );

  if ( n == 1 ) {
    obj->display(cout);
  }
  else { // n == 2
    auto filename = lua.to_string(2);
    ofstream s{filename};
    if ( !s ) {
      ostringstream buf;
      buf << "Error: Bdd:display(): Could not create "
	  << filename << ".";
      return lua.error_end(buf.str());
    }
    obj->display(s);
  }

  return 0;
}

END_NONAMESPACE

// @brief Bdd 関係の初期化を行う．
void
LuaBdd::init(
  lua_State* L,
  vector<struct luaL_Reg>& mylib
)
{
  Luapp lua{L};

  // BddMgr のメンバ関数(メソッド)テーブル
  static const struct luaL_Reg bddmgr_mt[] = {
    {"make_zero",     bddmgr_make_zero},
    {nullptr,         nullptr}
  };

  lua.reg_metatable(BDDMGR_SIGNATURE, bddmgr_gc, bddmgr_mt);

  // Bdd のメンバ関数(メソッド)テーブル
  static const struct luaL_Reg bdd_mt[] = {
    {"display",       bdd_display},
    {nullptr,         nullptr}
  };

  lua.reg_metatable(BDD_SIGNATURE, bdd_gc, bdd_mt);

  // 生成関数を登録する．
  mylib.push_back({"new_bddmgr",    bddmgr_new});
}

// @brief 対象を BddMgr として取り出す．
BddMgr*
LuaBdd::to_bddmgr(
  lua_State* L,
  int idx
)
{
  Luapp lua{L};
  auto p = lua.L_checkudata(idx, BDDMGR_SIGNATURE);
  return reinterpret_cast<BddMgr*>(p);
}

// @brief 対象を Bdd として取り出す．
Bdd*
LuaBdd::to_bdd(
  lua_State* L,
  int idx
)
{
  Luapp lua{L};
  auto p = lua.L_checkudata(idx, BDD_SIGNATURE);
  return reinterpret_cast<Bdd*>(p);
}

END_NAMESPACE_YM
