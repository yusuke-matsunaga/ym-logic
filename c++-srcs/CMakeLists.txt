# ===================================================================
# ym-logic のための CMakeLists.txt
# ===================================================================


# ===================================================================
# パッケージの検査
# ===================================================================


# ===================================================================
# インクルードパスの設定
# ===================================================================

# ===================================================================
# サブディレクトリの設定
# ===================================================================

add_subdirectory(gtest)
add_subdirectory(sop/gtest)
add_subdirectory(expr/gtest)
add_subdirectory(tvfunc/gtest)
add_subdirectory(npn/gtest)
add_subdirectory(dd/gtest)


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set (sop_SOURCES
  sop/SopCover.cc
  sop/SopCube.cc
  sop/SopMgr.cc
  sop/SopMgr_cube.cc
  sop/SopMgr_sort.cc
  sop/ConcateOp.cc
  sop/DiffOp.cc
  sop/MulOp.cc
  sop/DivOp.cc
  sop/CompOp.cc
  sop/CommonCube.cc
  sop/CheckIntersect.cc
  sop/CheckContainment.cc
  sop/KernelGen.cc
  sop/LitSet.cc
  sop/LitSetCheck.cc
  sop/LitSetUnion.cc
  sop/LitSetIntersect.cc
  sop/BestKernel.cc
  sop/OneLevel0Kernel.cc
  sop/WeakDivision.cc
  )

set (expr_SOURCES
  expr/Expr.cc
  expr/Expr_analyze.cc
  expr/ExprMgr.cc
  expr/ExprNode.cc
  expr/ExprParser.cc
  expr/ExprWriter.cc
  )

set (npn_SOURCES
  npn/IgPartition.cc
  npn/NpnMap.cc
  npn/NpnMapM.cc
  npn/NpnMgr.cc
  npn/PolConf.cc
  npn/walsh_01_normalize.cc
  npn/walsh_2_refine.cc
  npn/walsh_w0_refine.cc
  npn/walsh_w1_refine.cc
  )

set (tvfunc_SOURCES
  tvfunc/TvFunc.cc
  tvfunc/TvFunc_analyze.cc
  tvfunc/TvFunc_bcf.cc
  tvfunc/TvFunc_walsh.cc
  tvfunc/TvFuncM.cc
  tvfunc/Tv2Bdd.cc
  )

set (dd_SOURCES
  dd/DdNodeMgr.cc
  dd/DdNodeTable.cc
  dd/CopyOp.cc
  dd/DotGen.cc
  dd/DotWriter.cc
  dd/NodeCollector.cc
  dd/display.cc

  dd/Bdd.cc
  dd/Bdd_truth.cc
  dd/BddMgr.cc
  dd/BddMgrImpl.cc
  dd/BddCheckSupOp.cc
  dd/BddCheckSymOp.cc
  dd/BddCofactorOp.cc
  dd/BddCopyOp.cc
  dd/BddIteOp.cc
  dd/BddSimp.cc
  dd/BddMultiCompOp.cc
  dd/BddOneOp.cc
  dd/BddSupOp.cc
  dd/BddTruthOp.cc
  dd/BddVarSet.cc

  dd/Zdd.cc
  dd/ZddMgr.cc
  dd/ZddMgrImpl.cc
  dd/ZddInvOp.cc
  dd/ZddCapOp.cc
  dd/ZddCupOp.cc
  dd/ZddDiffOp.cc
  dd/ZddCofactorOp.cc
  dd/ZddCopyOp.cc
  dd/ZddCountOp.cc
  dd/ZddProductOp.cc
  dd/ZddSupOp.cc
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
