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


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set (dd_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/DdNodeMgr.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/DdNodeTable.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/DotGen.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/DdInfoMgr.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/IdentOp.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/NodeCollector.cc

  ${CMAKE_CURRENT_SOURCE_DIR}/Bdd.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/Bdd_truth.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/BddMgr.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/BddMgrImpl.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/BddMgrPtr.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/BddCheckSupOp.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/BddCheckSymOp.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/BddCofactorOp.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/BddCopyOp.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/BddIteOp.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/BddSimp.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/BddMultiCompOp.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/BddOneOp.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/BddSupOp.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/BddTruthOp.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/BddVarSet.cc

  ${CMAKE_CURRENT_SOURCE_DIR}/Zdd.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ZddMgr.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ZddMgrImpl.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ZddMgrPtr.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ZddInvOp.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ZddCapOp.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ZddCupOp.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ZddDiffOp.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ZddCofactorOp.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ZddCopyOp.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ZddCountOp.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ZddProductOp.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ZddSupOp.cc
  PARENT_SCOPE
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================


# ===================================================================
#  インストールターゲットの設定
# ===================================================================
