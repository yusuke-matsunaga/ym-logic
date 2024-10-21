# ===================================================================
# ym-logic のための CMakeLists.txt
# ===================================================================


# ===================================================================
# パッケージの検査
# ===================================================================


# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  ${PROJECT_BINARY_DIR}/include
  ${PROJECT_SOURCE_DIR}/ym-common/include
  ${PROJECT_SOURCE_DIR}/ym-base/include
  include
  private_include
  )


# ===================================================================
# サブディレクトリの設定
# ===================================================================

add_subdirectory(py_capi)
add_subdirectory(tests)


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set (sop_SOURCES
  c++-srcs/sop/SopCover.cc
  c++-srcs/sop/SopCube.cc
  c++-srcs/sop/SopMgr.cc
  c++-srcs/sop/SopMgr_cube.cc
  c++-srcs/sop/SopMgr_sort.cc
  c++-srcs/sop/ConcateOp.cc
  c++-srcs/sop/DiffOp.cc
  c++-srcs/sop/MulOp.cc
  c++-srcs/sop/DivOp.cc
  c++-srcs/sop/CompOp.cc
  c++-srcs/sop/CommonCube.cc
  c++-srcs/sop/CheckIntersect.cc
  c++-srcs/sop/CheckContainment.cc
  c++-srcs/sop/KernelGen.cc
  c++-srcs/sop/LitSet.cc
  c++-srcs/sop/LitSetCheck.cc
  c++-srcs/sop/LitSetUnion.cc
  c++-srcs/sop/LitSetIntersect.cc
  c++-srcs/sop/BestKernel.cc
  c++-srcs/sop/OneLevel0Kernel.cc
  c++-srcs/sop/WeakDivision.cc
  )

set (expr_SOURCES
  c++-srcs/expr/Expr.cc
  c++-srcs/expr/Expr_analyze.cc
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
  c++-srcs/tvfunc/TvFunc_analyze.cc
  c++-srcs/tvfunc/TvFunc_bcf.cc
  c++-srcs/tvfunc/TvFunc_walsh.cc
  c++-srcs/tvfunc/TvFuncM.cc
  c++-srcs/tvfunc/Tv2Bdd.cc
  )

set (dd_SOURCES
  c++-srcs/dd/DdNodeMgr.cc
  c++-srcs/dd/CopyOp.cc
  c++-srcs/dd/IdentOp.cc
  c++-srcs/dd/DotGen.cc
  c++-srcs/dd/DotWriter.cc
  c++-srcs/dd/NodeCollector.cc
  c++-srcs/dd/display.cc

  c++-srcs/dd/Bdd.cc
  c++-srcs/dd/Bdd_truth.cc
  c++-srcs/dd/BddMgr.cc
  c++-srcs/dd/BddMgrImpl.cc
  c++-srcs/dd/BddCheckSupOp.cc
  c++-srcs/dd/BddCheckSymOp.cc
  c++-srcs/dd/BddCofactorOp.cc
  c++-srcs/dd/BddIteOp.cc
  c++-srcs/dd/BddSimp.cc
  c++-srcs/dd/BddMultiCompOp.cc
  c++-srcs/dd/BddOneOp.cc
  c++-srcs/dd/BddSupOp.cc
  c++-srcs/dd/BddTruthOp.cc
  c++-srcs/dd/BddVarSet.cc

  c++-srcs/dd/Zdd.cc
  c++-srcs/dd/ZddMgr.cc
  c++-srcs/dd/ZddMgrImpl.cc
  c++-srcs/dd/ZddInvOp.cc
  c++-srcs/dd/ZddCapOp.cc
  c++-srcs/dd/ZddCupOp.cc
  c++-srcs/dd/ZddDiffOp.cc
  c++-srcs/dd/ZddCofactorOp.cc
  c++-srcs/dd/ZddCountOp.cc
  c++-srcs/dd/ZddProductOp.cc
  )

set (ym_logic_SOURCES
  ${sop_SOURCES}
  ${expr_SOURCES}
  ${npn_SOURCES}
  ${tvfunc_SOURCES}
  ${dd_SOURCES}
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
