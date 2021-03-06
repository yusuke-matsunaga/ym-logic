# ===================================================================
# ym-logic のための CMakeLists.txt
# ===================================================================


# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  ${PROJECT_BINARY_DIR}/include
  ${PROJECT_SOURCE_DIR}/ym-common/include
  ${PROJECT_SOURCE_DIR}/ym-base/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/private_include
  )


# ===================================================================
# サブディレクトリの設定
# ===================================================================

add_subdirectory(c++-test)
add_subdirectory(py-test)
add_subdirectory(tests)


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set (sop_SOURCES
  c++-src/sop/SopCover.cc
  c++-src/sop/SopCube.cc
  c++-src/sop/SopMgr.cc
  c++-src/sop/KernelGen.cc
  c++-src/sop/LitSet.cc
  c++-src/sop/BestKernel.cc
  c++-src/sop/OneLevel0Kernel.cc
  c++-src/sop/WeakDivision.cc
  )

set (expr_SOURCES
  c++-src/expr/Expr.cc
  c++-src/expr/ExprMgr.cc
  c++-src/expr/ExprNode.cc
  c++-src/expr/ExprParser.cc
  c++-src/expr/ExprWriter.cc
  )

set (npn_SOURCES
  c++-src/npn/IgPartition.cc
  c++-src/npn/NpnMap.cc
  c++-src/npn/NpnMapM.cc
  c++-src/npn/NpnMgr.cc
  c++-src/npn/PolConf.cc
  c++-src/npn/walsh_01_normalize.cc
  c++-src/npn/walsh_2_refine.cc
  c++-src/npn/walsh_w0_refine.cc
  c++-src/npn/walsh_w1_refine.cc
  )

set (tvfunc_SOURCES
  c++-src/tvfunc/TvFunc.cc
  c++-src/tvfunc/TvFunc_walsh.cc
  c++-src/tvfunc/TvFuncM.cc
  )

set (ym_logic_SOURCES
  ${sop_SOURCES}
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
