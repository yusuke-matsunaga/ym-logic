# ===================================================================
# ym-logic のための CMakeLists.txt
# ===================================================================


# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  ${PROJECT_BINARY_DIR}/include
  ${PROJECT_SOURCE_DIR}/ym-common/include
  include
  private_include
  )


# ===================================================================
# サブディレクトリの設定
# ===================================================================

add_subdirectory(tests/gtest)
add_subdirectory(tests/python)
add_subdirectory(tests/programs)


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set (sop_SOURCES
  c++-srcs/sop/SopCover.cc
  c++-srcs/sop/SopCube.cc
  c++-srcs/sop/SopMgr.cc
  c++-srcs/sop/KernelGen.cc
  c++-srcs/sop/LitSet.cc
  c++-srcs/sop/BestKernel.cc
  c++-srcs/sop/OneLevel0Kernel.cc
  c++-srcs/sop/WeakDivision.cc
  )

set (expr_SOURCES
  c++-srcs/expr/Expr.cc
  c++-srcs/expr/ExprMgr.cc
  c++-srcs/expr/ExprNode.cc
  c++-srcs/expr/ExprParser.cc
  c++-srcs/expr/ExprWriter.cc
  )

set (npn_SOURCES
  c++-srcs/npn/IgPartition.cc
  c++-srcs/npn/NpnMap.cc
  c++-srcs/npn/NpnMapM.cc
  c++-srcs/npn/NpnMgr.cc
  c++-srcs/npn/PolConf.cc
  c++-srcs/npn/walsh_01_normalize.cc
  c++-srcs/npn/walsh_2_refine.cc
  c++-srcs/npn/walsh_w0_refine.cc
  c++-srcs/npn/walsh_w1_refine.cc
  )

set (tvfunc_SOURCES
  c++-srcs/tvfunc/TvFunc.cc
  c++-srcs/tvfunc/TvFunc_walsh.cc
  c++-srcs/tvfunc/TvFuncM.cc
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
