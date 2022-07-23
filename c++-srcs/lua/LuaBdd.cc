
/// @file LuaBdd.cc
/// @brief LuaBdd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/LuaBdd.h"
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
  LuaBdd lua{L};

  auto obj = lua.to_bddmgr(1);
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
  LuaBdd& lua
)
{
  return lua.new_bdd();
}

// Bdd 用のデストラクタ
int
bdd_gc(
  lua_State* L
)
{
  LuaBdd lua{L};

  auto obj = lua.to_bdd(1);
  if ( obj ) {
    // デストラクタを明示的に起動する．
    obj->~Bdd();
  }

  // メモリは Lua が開放する．
  return 0;
}

// 恒偽関数を作る．
int
bddmgr_zero(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 1 ) {
    return lua.error_end("Error: BddMgr:zero() expects no arguments.");
  }

  auto obj = lua.to_bddmgr(1);
  ASSERT_COND( obj != nullptr );

  auto bddp = lua.new_bdd();
  *bddp = obj->zero();

  return 1;
}

// 恒真関数を作る．
int
bddmgr_one(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 1 ) {
    return lua.error_end("Error: BddMgr:one() expects no arguments.");
  }

  auto obj = lua.to_bddmgr(1);
  ASSERT_COND( obj != nullptr );

  auto bddp = lua.new_bdd();
  *bddp = obj->one();

  return 1;
}

// リテラル関数を作る．
int
bddmgr_literal(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 2 && n != 3 ) {
    return lua.error_end("Error: BddMgr:literal() expects one or two arguments.");
  }
  bool inv{false};
  if ( n == 3 ) {
    inv = lua.to_boolean(3);
  }

  bool ok;
  lua_Integer val;
  tie(ok, val) = lua.to_integer(2);
  if ( !ok ) {
    return lua.error_end("Error: BddMgr:literal(): 1st argument should be an integer.");
  }

  auto obj = lua.to_bddmgr(1);
  ASSERT_COND( obj != nullptr );

  auto bddp = lua.new_bdd();
  SizeType index = val;
  *bddp = obj->literal(VarId{index}, inv);

  return 1;
}

// 肯定リテラル関数を作る．
int
bddmgr_posi_literal(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 2 ) {
    return lua.error_end("Error: BddMgr:posi_literal() expects one argument.");
  }

  bool ok;
  lua_Integer val;
  tie(ok, val) = lua.to_integer(2);
  if ( !ok ) {
    return lua.error_end("Error: BddMgr:posi_literal(): 1st argument should be an integer.");
  }

  auto obj = lua.to_bddmgr(1);
  ASSERT_COND( obj != nullptr );

  auto bddp = lua.new_bdd();
  SizeType index = val;
  *bddp = obj->posi_literal(VarId{index});

  return 1;
}

// 否定リテラル関数を作る．
int
bddmgr_nega_literal(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 2 ) {
    return lua.error_end("Error: BddMgr:nega_literal() expects one arguments");
  }

  bool ok;
  lua_Integer val;
  tie(ok, val) = lua.to_integer(2);
  if ( !ok ) {
    return lua.error_end("Error: BddMgr:nega_literal(): 1st argument should be an integer.");
  }

  auto obj = lua.to_bddmgr(1);
  ASSERT_COND( obj != nullptr );

  auto bddp = lua.new_bdd();
  SizeType index = val;
  *bddp = obj->nega_literal(VarId{index});

  return 1;
}

// 真理値表形式の文字列からBDDを作る．
int
bddmgr_from_truth(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 2 ) {
    return lua.error_end("Error: BddMgr:from_truth() expects one argument.");
  }

  string str = lua.to_string(2);

  auto obj = lua.to_bddmgr(1);
  ASSERT_COND( obj != nullptr );

  auto bddp = lua.new_bdd();
  *bddp = obj->from_truth(str);

  return 1;
}

// ノード数を返す．
int
bddmgr_node_num(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 1 ) {
    return lua.error_end("Error: BddMgr:node_num() expects no arguments.");
  }

  auto obj = lua.to_bddmgr(L, 1);
  ASSERT_COND( obj != nullptr );

  SizeType v = obj->node_num();

  lua.push_integer(v);
  return 1;
}

// GC を起動するしきい値を返す．
int
bddmgr_gc_limit(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 1 ) {
    return lua.error_end("Error: BddMgr:gc_limit() expects no arguments.");
  }

  auto obj = lua.to_bddmgr(1);
  ASSERT_COND( obj != nullptr );

  SizeType v = obj->gc_limit();

  lua.push_integer(v);
  return 1;
}

// GC を起動するしきい値を設定する．
int
bddmgr_set_gc_limit(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 2 ) {
    return lua.error_end("Error: BddMgr:gc_limit() expects one argument.");
  }

  auto obj = lua.to_bddmgr(1);
  ASSERT_COND( obj != nullptr );

  bool ok;
  lua_Integer v;
  tie(ok, v) = lua.to_integer(2);
  if ( !ok ) {
    return lua.error_end("Error: BddMgr::set_gc_limit(): 1st argument should be an integer.");
  }

  obj->set_gc_limit(v);

  lua.push_boolean(true);
  return 1;
}

// GC の起動を許可する．
int
bddmgr_enable_gc(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 1 ) {
    return lua.error_end("Error: BddMgr:enable_gc() expects no arguments.");
  }

  auto obj = lua.to_bddmgr(1);
  ASSERT_COND( obj != nullptr );

  obj->enable_gc();

  lua.push_boolean(true);
  return 1;
}

// GC の起動を禁止する．
int
bddmgr_disable_gc(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 1 ) {
    return lua.error_end("Error: BddMgr:disable_gc() expects no arguments.");
  }

  auto obj = lua.to_bddmgr(1);
  ASSERT_COND( obj != nullptr );

  obj->disable_gc();

  lua.push_boolean(true);
  return 1;
}

// NOT 演算を行う．
int
bdd_not(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 1 ) {
    return lua.error_end("Error: Bdd:not() expects one arguments.");
  }

  auto obj = lua.to_bdd(1);
  ASSERT_COND( obj != nullptr );

  auto bddp = lua.new_bdd();
  *bddp = obj->invert();

  return 1;
}

// AND 演算を行う．
int
bdd_and(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 2 ) {
    return lua.error_end("Error: Bdd:not() expects two arguments.");
  }

  auto obj1 = lua.to_bdd(L, 1);
  ASSERT_COND( obj1 != nullptr );

  auto obj2 = lua.to_bdd(2);
  ASSERT_COND( obj2 != nullptr );

  auto bddp = lua.new_bdd();
  *bddp = obj1->and_op(*obj2);

  return 1;
}

// OR 演算を行う．
int
bdd_or(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 2 ) {
    return lua.error_end("Error: Bdd:or() expects two arguments.");
  }

  auto obj1 = lua.to_bdd(1);
  ASSERT_COND( obj1 != nullptr );

  auto obj2 = lua.to_bdd(2);
  ASSERT_COND( obj2 != nullptr );

  auto bddp = lua.new_bdd();
  *bddp = obj1->or_op(*obj2);

  return 1;
}

// XOR 演算を行う．
int
bdd_xor(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 2 ) {
    return lua.error_end("Error: Bdd:xor() expects two arguments.");
  }

  auto obj1 = lua.to_bdd(1);
  ASSERT_COND( obj1 != nullptr );

  auto obj2 = lua.to_bdd(2);
  ASSERT_COND( obj2 != nullptr );

  auto bddp = lua.new_bdd();
  *bddp = obj1->xor_op(*obj2);

  return 1;
}

// cofactor 演算を行う．
int
bdd_cofactor(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 2 ) {
    return lua.error_end("Error: Bdd:cofactor() expects two arguments.");
  }

  auto obj1 = lua.to_bdd(1);
  ASSERT_COND( obj1 != nullptr );

  auto obj2 = lua.to_bdd(2);
  ASSERT_COND( obj2 != nullptr );

  auto bddp = lua.new_bdd();
  *bddp = obj1->cofactor(*obj2);

  return 1;
}

// 等価比較演算を行う．
int
bdd_eq(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 2 ) {
    return lua.error_end("Error: Bdd:not() expects two arguments.");
  }

  auto obj1 = lua.to_bdd(1);
  ASSERT_COND( obj1 != nullptr );

  auto obj2 = lua.to_bdd(2);
  ASSERT_COND( obj2 != nullptr );

  auto b = *obj1 == *obj2;
  lua.push_boolean(b);

  return 1;
}

// 変数によるコファクターを求める．
int
bdd_vcofactor(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 2 && n != 3 ) {
    return lua.error_end("Error: Bdd:is_valid() expects one or two arguments.");
  }

  auto obj = lua.to_bdd(1);
  ASSERT_COND( obj != nullptr );

  bool ok;
  lua_Integer v;
  tie(ok, v) = lua.to_integer(2);
  if ( !ok ) {
    return lua.error_end("Error: Bdd:vcofactor(): 1st argument should be an integer.");
  }

  bool inv{false};
  if ( n == 3 ) {
    inv = lua.to_boolean(3);
  }

  auto bddp = lua.new_bdd();
  *bddp = obj->cofactor(VarId(v), inv);

  return 1;
}

// 適正な値を持っている時 true を返す．
int
bdd_is_valid(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 1 ) {
    return lua.error_end("Error: Bdd:is_valid() expects no arguments.");
  }

  auto obj = lua.to_bdd(1);
  ASSERT_COND( obj != nullptr );

  lua.push_boolean(obj->is_valid());
  return 1;
}

// 不正値を持っている時 true を返す．
int
bdd_is_invalid(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 1 ) {
    return lua.error_end("Error: Bdd:is_invalid() expects no arguments.");
  }

  auto obj = lua.to_bdd(1);
  ASSERT_COND( obj != nullptr );

  lua.push_boolean(obj->is_invalid());
  return 1;
}

// 恒偽関数の時 true を返す．
int
bdd_is_zero(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 1 ) {
    return lua.error_end("Error: Bdd:is_zero() expects no arguments.");
  }

  auto obj = lua.to_bdd(1);
  ASSERT_COND( obj != nullptr );

  lua.push_boolean(obj->is_zero());
  return 1;
}

// 恒真関数の時 true を返す．
int
bdd_is_one(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 1 ) {
    return lua.error_end("Error: Bdd:is_one() expects no arguments.");
  }

  auto obj = lua.to_bdd(1);
  ASSERT_COND( obj != nullptr );

  lua.push_boolean(obj->is_one());
  return 1;
}

// 積項の時 true を返す．
int
bdd_is_cube(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 1 ) {
    return lua.error_end("Error: Bdd:is_cube() expects no arguments.");
  }

  auto obj = lua.to_bdd(1);
  ASSERT_COND( obj != nullptr );

  lua.push_boolean(obj->is_cube());
  return 1;
}

// 正リテラルの積項の時 true を返す．
int
bdd_is_posicube(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 1 ) {
    return lua.error_end("Error: Bdd:is_posicube() expects no arguments.");
  }

  auto obj = lua.to_bdd(1);
  ASSERT_COND( obj != nullptr );

  lua.push_boolean(obj->is_posicube());
  return 1;
}

// 与えられた変数がサポートの時 true を返す．
int
bdd_check_sup(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 2 ) {
    return lua.error_end("Error: Bdd:check_sup() expects one argument.");
  }

  auto obj = lua.to_bdd(1);
  ASSERT_COND( obj != nullptr );

  bool ok;
  lua_Integer v;
  tie(ok, v) = lua.to_integer(2);
  if ( !ok ) {
    return lua.error_end("Error: Bdd:check_sup(): 1st argument should be an integer.");
  }

  bool b = obj->check_sup(VarId(v));
  lua.push_boolean(b);
  return 1;
}

// 与えられた変数が対称の時 true を返す．
int
bdd_check_sym(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 3 && n != 4) {
    return lua.error_end("Error: Bdd:check_sup() expects two or three arguments.");
  }

  auto obj = lua.to_bdd(1);
  ASSERT_COND( obj != nullptr );

  bool ok;
  lua_Integer v1;
  tie(ok, v1) = lua.to_integer(2);
  if ( !ok ) {
    return lua.error_end("Error: Bdd:check_sup(): 1st argument should be an integer.");
  }

  lua_Integer v2;
  tie(ok, v2) = lua.to_integer(3);
  if ( !ok ) {
    return lua.error_end("Error: Bdd:check_sup(): 2nd argument should be an integer.");
  }

  bool inv{false};
  if ( n == 4 ) {
    inv = lua.to_boolean(4);
  }

  bool b = obj->check_sym(VarId(v1), VarId(v2), inv);
  lua.push_boolean(b);
  return 1;
}

// サポートを求める．
int
bdd_get_support(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 1 ) {
    return lua.error_end("Error: Bdd:get_support() expects no arguments.");
  }

  auto obj = lua.to_bdd(1);
  ASSERT_COND( obj != nullptr );

  auto bddp = lua.new_bdd();
  *bddp = obj->get_support();

  return 1;
}

// 1となるパスを求める．
int
bdd_get_onepath(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 1 ) {
    return lua.error_end("Error: Bdd:get_onepath() expects no arguments.");
  }

  auto obj = lua.to_bdd(1);
  ASSERT_COND( obj != nullptr );

  auto bddp = lua.new_bdd();
  *bddp = obj->get_onepath();

  return 1;
}

// 0となるパスを求める．
int
bdd_get_zeropath(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 1 ) {
    return lua.error_end("Error: Bdd:get_zeropath() expects no arguments.");
  }

  auto obj = lua.to_bdd(1);
  ASSERT_COND( obj != nullptr );

  auto bddp = lua.new_bdd();
  *bddp = obj->get_zeropath();

  return 1;
}

// 根の変数で分解する．
int
bdd_root_decomp(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 1 ) {
    return lua.error_end("Error: Bdd:root_decomp() expects no arguments.");
  }

  auto obj = lua.to_bdd(1);
  ASSERT_COND( obj != nullptr );

  Bdd f0;
  Bdd f1;
  auto var = obj->root_decomp(f0, f1);

  auto bddp0 = lua.new_bdd();
  *bddp0 = f0;

  auto bddp1 = lua.new_bdd();
  *bddp1 = f1;

  lua.push_integer(var.val());

  return 3;
}

// 根の変数を求める．
int
bdd_root_var(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 1 ) {
    return lua.error_end("Error: Bdd:root_var() expects no arguments.");
  }

  auto obj = lua.to_bdd(1);
  ASSERT_COND( obj != nullptr );

  auto var = obj->root_var();

  lua.push_integer(var.val());
  return 1;
}

// 負のコファクターを求める．
int
bdd_root_cofactor0(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 1 ) {
    return lua.error_end("Error: Bdd:root_cofactor0() expects no arguments.");
  }

  auto obj = lua.to_bdd(1);
  ASSERT_COND( obj != nullptr );

  auto bddp = lua.new_bdd();
  *bddp = obj->root_cofactor0();

  return 1;
}

// 正のコファクターを求める．
int
bdd_root_cofactor1(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 1 ) {
    return lua.error_end("Error: Bdd:root_cofactor1() expects no arguments.");
  }

  auto obj = lua.to_bdd(1);
  ASSERT_COND( obj != nullptr );

  auto bddp = lua.new_bdd();
  *bddp = obj->root_cofactor1();

  return 1;
}

// 根の枝が反転している時に true を返す．
int
bdd_root_inv(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 1 ) {
    return lua.error_end("Error: Bdd:root_inv() expects no arguments.");
  }

  auto obj = lua.to_bdd(1);
  ASSERT_COND( obj != nullptr );

  bool b = obj->root_inv();
  lua.push_boolean(b);

  return 1;
}

// 評価を行う．
int
bdd_eval(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 2 ) {
    return lua.error_end("Error: Bdd:eval() expects one argument.");
  }

  if ( !lua.is_table(2) ) {
    return lua.error_end("Error: Bdd:eval(): 1st argument should be a table.");
  }

  auto obj = lua.to_bdd(1);
  ASSERT_COND( obj != nullptr );

  // 要素数
  SizeType ni = lua.L_len(2);

  // 入力値のリストを作る．
  vector<bool> inputs(ni);
  for ( SizeType i = 0; i < ni; ++ i ) {
    auto t = lua.get_table(2, i + 1);
    if ( t != LUA_TBOOLEAN ) {
      return lua.error_end("Error: Bdd:eval(): 1st argument should an array of Boolean.");
    }
    auto b = lua.to_boolean(-1);
    lua.pop(1);
    inputs[i] = b;
  }

  bool b = obj->eval(inputs);
  lua.push_boolean(b);

  return 1;
}

// ノード数を返す．
int
bdd_size(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 1 ) {
    return lua.error_end("Error: Bdd:size() expects no arguments.");
  }

  auto obj = lua.to_bdd(1);
  ASSERT_COND( obj != nullptr );

  lua.push_integer(obj->size());

  return 1;
}

// 複数の BDD のノード数を返す．
int
size_of_bdds(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 1 ) {
    return lua.error_end("Error: size_of_bdds() expects one argument.");
  }

  auto bdd_list = LuaBdd::to_bdd_list(L, 1);
  SizeType v = Bdd::size(bdd_list);
  lua.push_integer(v);
  return 1;
}

// 変数のリストに変換する．
int
bdd_to_varlist(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 1 ) {
    return lua.error_end("Error: Bdd:to_varlist() expects no arguments.");
  }

  auto obj = lua.to_bdd(1);
  ASSERT_COND( obj != nullptr );

  auto varlist = obj->to_varlist();

  // 要素数
  SizeType nv = varlist.size();

  // 結果のテーブルを作る．
  lua.create_table(nv, 0);
  for ( SizeType i = 0; i < nv; ++ i ) {
    SizeType v = varlist[i].val();
    lua.push_integer(v);
    lua.set_table(-2, i + 1);
  }

  return 1;
}

// 変数のリストに変換する．
int
bdd_to_litlist(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 1 ) {
    return lua.error_end("Error: Bdd:to_varlist() expects no arguments.");
  }

  auto obj = lua.to_bdd(1);
  ASSERT_COND( obj != nullptr );

  auto litlist = obj->to_litlist();

  // 要素数
  SizeType nl = litlist.size();

  // 結果のテーブルを作る．
  lua.create_table(nl, 0);
  for ( SizeType i = 0; i < nl; ++ i ) {
    auto lit= litlist[i];
    // リテラルを要素数2のテーブルで表す．
    lua.create_table(2, 0);
    lua.push_integer(lit.varid().val());
    lua.set_table(-2, 1);
    lua.push_boolean(lit.is_negative());
    lua.set_table(-2, 2);
    lua.set_table(-2, i + 1);
  }

  return 1;
}

// 真理値表の文字列に変換する．
int
bdd_to_truth(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 2 ) {
    return lua.error_end("Error: Bdd:to_truth() expects one argument.");
  }

  auto obj = lua.to_bdd(1);
  ASSERT_COND( obj != nullptr );

  bool ok;
  lua_Integer v;
  tie(ok, v) = lua.to_integer(2);
  if ( !ok ) {
    return lua.error_end("Error: Bdd:to_truth(): 1st argument should be an integer.");
  }

  auto str = obj->to_truth(v);
  lua.push_string(str.c_str());

  return 1;
}

// 内容を出力する．
int
bdd_display(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 1 && n != 2 ) {
    return lua.error_end("Error: Bdd:display() expects at most one argument.");
  }

  auto obj = lua.to_bdd(1);
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

  lua.push_boolean(true);
  return 1;
}

// dot 形式で出力する．
int
bdd_gen_dot(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 2 ) {
    return lua.error_end("Error: Bdd:gen_dot() expects one argument.");
  }

  auto obj = lua.to_bdd(1);
  ASSERT_COND( obj != nullptr );

  auto filename = lua.to_string(2);
  ofstream s{filename};
  if ( !s ) {
    ostringstream buf;
    buf << "Error: Bdd:gen_dot(): Could not create "
	<< filename << ".";
    return lua.error_end(buf.str());
  }
  obj->gen_dot(s);

  lua.push_boolean(true);
  return 1;
}

// 複数の BDD を dot 形式で出力する．
int
gen_dot_for_bdds(
  lua_State* L
)
{
  LuaBdd lua{L};

  SizeType n = lua.get_top();
  if ( n != 2 ) {
    return lua.error_end("Error: Bdd:gen_dot() expects one argument.");
  }

  auto bdd_list = LuaBdd::to_bdd_list(L, 1);

  auto filename = lua.to_string(2);
  ofstream s{filename};
  if ( !s ) {
    ostringstream buf;
    buf << "Error: Bdd:gen_dot(): Could not create "
	<< filename << ".";
    return lua.error_end(buf.str());
  }
  Bdd::gen_dot(s, bdd_list);

  lua.push_boolean(true);
  return 1;
}

END_NONAMESPACE

// @brief Bdd 関係の初期化を行う．
void
LuaBdd::init(
  vector<struct luaL_Reg>& mylib
)
{
  // BddMgr のメンバ関数(メソッド)テーブル
  static const struct luaL_Reg bddmgr_mt[] = {
    {"zero",         bddmgr_zero},
    {"one",          bddmgr_one},
    {"literal",      bddmgr_literal},
    {"posi_literal", bddmgr_posi_literal},
    {"nega_literal", bddmgr_nega_literal},
    {"from_truth",   bddmgr_from_truth},
    {"node_num",     bddmgr_node_num},
    {"gc_limit",     bddmgr_gc_limit},
    {"set_gc_limit", bddmgr_set_gc_limit},
    {"enable_gc",    bddmgr_enable_gc},
    {"disable_gc",   bddmgr_disable_gc},
    {nullptr,        nullptr}
  };

  reg_metatable(BDDMGR_SIGNATURE, bddmgr_gc, bddmgr_mt);

  // Bdd のメンバ関数(メソッド)テーブル
  static const struct luaL_Reg bdd_mt[] = {
    {"__bnot",         bdd_not},
    {"__band",         bdd_and},
    {"__bor",          bdd_or},
    {"__bxor",         bdd_xor},
    {"__div",          bdd_cofactor},
    {"__eq",           bdd_eq},
    {"vcofactor",      bdd_vcofactor},
    {"is_valid",       bdd_is_valid},
    {"is_invalid",     bdd_is_invalid},
    {"is_zero",        bdd_is_zero},
    {"is_one",         bdd_is_one},
    {"is_cube",        bdd_is_cube},
    {"is_posicube",    bdd_is_posicube},
    {"check_sup",      bdd_check_sup},
    {"check_sym",      bdd_check_sym},
    {"get_support",    bdd_get_support},
    {"get_onepath",    bdd_get_onepath},
    {"get_zeropath",   bdd_get_zeropath},
    {"root_decomp",    bdd_root_decomp},
    {"root_var",       bdd_root_var},
    {"root_cofactor0", bdd_root_cofactor0},
    {"root_cofactor1", bdd_root_cofactor1},
    {"root_inv",       bdd_root_inv},
    {"eval",           bdd_eval},
    {"size",           bdd_size},
    {"to_varlist",     bdd_to_varlist},
    {"to_litlist",     bdd_to_litlist},
    {"to_truth",       bdd_to_truth},
    {"display",        bdd_display},
    {"gen_dot",        bdd_gen_dot},
    {nullptr,          nullptr}
  };

  reg_metatable(BDD_SIGNATURE, bdd_gc, bdd_mt);

  // グローバル関数を登録する．
  mylib.push_back({"new_bddmgr",   bddmgr_new});
  mylib.push_back({"size_of_bdds", size_of_bdds});
  mylib.push_back({"gen_dot_for_bdds", gen_dot_for_bdds});
}

// @brief 対象を BddMgr として取り出す．
BddMgr*
LuaBdd::to_bddmgr(
  int idx
)
{
  auto p = L_checkudata(idx, BDDMGR_SIGNATURE);
  return reinterpret_cast<BddMgr*>(p);
}

// @brief 対象を Bdd として取り出す．
Bdd*
LuaBdd::to_bdd(
  int idx
)
{
  auto p = L_checkudata(idx, BDD_SIGNATURE);
  return reinterpret_cast<Bdd*>(p);
}

// @brief スタックから Bdd のリストを取り出す．
vector<Bdd>
LuaBdd::to_bdd_list(
  int idx
)
{
  if ( !is_table(idx) ) {
    return {};
  }

  SizeType nl = L_len(idx);
  vector<Bdd> bdd_list(nl);
  for ( SizeType i = 0; i < nl; ++ i ) {
    get_table(idx, i + 1);
    auto obj = to_bdd(-1);
    if ( obj == nullptr ) {
      return {};
    }
    bdd_list[i] = *obj;
    pop(1);
  }
  return bdd_list;
}

// @brief lua の管理下にある Bdd オブジェクトを作る．
Bdd*
LuaBdd::new_bdd()
{
  // メモリ領域は Lua で確保する．
  void* p = new_userdata(sizeof(Bdd));
  auto obj = new (p) Bdd{};

  // Bdd 用の metatable を取ってくる．
  L_getmetatable(BDD_SIGNATURE);

  // それを今作った userdata にセットする．
  set_metatable(-2);

  return obj;
}

END_NAMESPACE_YM
