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

set (sop_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/SopCover.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/SopCube.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/SopMgr.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/SopMgr_cube.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/SopMgr_sort.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/ConcateOp.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/DiffOp.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/MulOp.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/DivOp.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CompOp.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CommonCube.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CheckIntersect.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CheckContainment.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/KernelGen.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/LitSet.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/LitSetCheck.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/LitSetUnion.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/LitSetIntersect.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/BestKernel.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/OneLevel0Kernel.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/WeakDivision.cc
  PARENT_SCOPE
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================


# ===================================================================
#  インストールターゲットの設定
# ===================================================================
