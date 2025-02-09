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

set (alg_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/AlgBase.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AlgCover_compare.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AlgCover_concate.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AlgCover_sort.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AlgCover_diff.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AlgCover_commoncube.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AlgCover_algdiv.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AlgCover_product.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/KernelGen.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/LitSet.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/BestKernel.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/OneLevel0Kernel.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/WeakDivision.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/GenFactor.cc
  PARENT_SCOPE
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================


# ===================================================================
#  インストールターゲットの設定
# ===================================================================
