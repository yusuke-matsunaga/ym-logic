
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


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set ( rwt_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/Cut.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/CutMgr.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/PatMgr.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/Npn4.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/RwtMgr.cc
  PARENT_SCOPE
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================
