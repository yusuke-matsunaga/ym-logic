# ===================================================================
# ym-logic のための CMakeLists.txt
# ===================================================================


# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  ${CMAKE_CURRENT_SOURCE_DIR}/private_include
  )


# ===================================================================
# サブディレクトリの設定
# ===================================================================

add_subdirectory(gtest)
add_subdirectory(tests)


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set (expr_SOURCES
  src/expr/Expr.cc
  src/expr/ExprMgr.cc
  src/expr/ExprNode.cc
  src/expr/ExprParser.cc
  src/expr/ExprWriter.cc
  src/expr/Literal.cc
  )

set (npn_SOURCES
  src/npn/IgPartition.cc
  src/npn/NpnMap.cc
  src/npn/NpnMapM.cc
  src/npn/NpnMgr.cc
  src/npn/PolConf.cc
  src/npn/walsh_01_normalize.cc
  src/npn/walsh_2_refine.cc
  src/npn/walsh_w0_refine.cc
  src/npn/walsh_w1_refine.cc
  )

set (tvfunc_SOURCES
  src/tvfunc/TvFunc.cc
  src/tvfunc/TvFuncM.cc
  )

set (ym_logic_SOURCES
  ${expr_SOURCES}
  ${npn_SOURCES}
  ${tvfunc_SOURCES}
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================

ym_add_object_library ( ym_logic
  ${ym_logic_SOURCES}
  )


# ===================================================================
#  インストールターゲットの設定
# ===================================================================
