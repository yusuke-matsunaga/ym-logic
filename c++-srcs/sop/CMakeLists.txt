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
  ${CMAKE_CURRENT_SOURCE_DIR}/SopBase.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/SopBase_cube.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/SopCube.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/SopCube_check.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/SopCover.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/SopCover_concate.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/SopCover_diff.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/SopCover_compare.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/SopCover_sort.cc
  #${CMAKE_CURRENT_SOURCE_DIR}/MulOp.cc
  #${CMAKE_CURRENT_SOURCE_DIR}/DivOp.cc
  #${CMAKE_CURRENT_SOURCE_DIR}/CommonCube.cc
  #${CMAKE_CURRENT_SOURCE_DIR}/CheckIntersect.cc
  #${CMAKE_CURRENT_SOURCE_DIR}/CheckContainment.cc
  #${CMAKE_CURRENT_SOURCE_DIR}/LitSet.cc
  #${CMAKE_CURRENT_SOURCE_DIR}/LitSetCheck.cc
  #${CMAKE_CURRENT_SOURCE_DIR}/LitSetUnion.cc
  #${CMAKE_CURRENT_SOURCE_DIR}/LitSetIntersect.cc
  PARENT_SCOPE
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================


# ===================================================================
#  インストールターゲットの設定
# ===================================================================
