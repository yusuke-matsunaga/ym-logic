
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  ${CMAKE_CURRENT_SOURCE_DIR}
  )


# ===================================================================
# サブディレクトリの設定
# ===================================================================

add_subdirectory ( gtest )
add_subdirectory ( test-programs )


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set ( rwt_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/CalcMerit.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/Cut.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CutMgr.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/Npn4.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PatMgr.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/Pat2Aig.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/rewrite.cc
  PARENT_SCOPE
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================
