
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  )


# ===================================================================
# サブディレクトリの設定
# ===================================================================


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set ( fraig_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/FraigMgr.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/FraigNode.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/FraigSat.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/MgrImpl.cc
  PARENT_SCOPE
  )


# ===================================================================
# ターゲットの設定
# ===================================================================
