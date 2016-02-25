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

#add_subdirectory(gtest)


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
  src/npn/NpnBaseConf.cc
  src/npn/NpnConf.cc
  src/npn/NpnMap.cc
  src/npn/NpnMapM.cc
  src/npn/NpnMgr.cc
  src/npn/NpnMgrImpl.cc
  src/npn/NpnRawSig.cc
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

#ym_add_library(ym_logic
#  SHARED
#  DESTINATION ${YM_INSTALL_LIBDIR}
#  )

#target_link_libraries(ym_logic
#  ym_common
#  )

#target_link_libraries(ym_logic_p
#  ym_common_p
#  )

#target_link_libraries(ym_logic_d
#  ym_common_d
#  )

add_library ( ym_logic
  OBJECT
  ${ym_logic_SOURCES}
  )

target_compile_options ( ym_logic
  PRIVATE "-O3" "-fPIC"
  )

target_compile_definitions ( ym_logic
  PRIVATE "-DNDEBUG"
  )


# ===================================================================
#  インストールターゲットの設定
# ===================================================================
