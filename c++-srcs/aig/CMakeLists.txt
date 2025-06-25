
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  )


# ===================================================================
# サブディレクトリの設定
# ===================================================================

add_subdirectory( gtest )
add_subdirectory( test-programs )


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set ( aig_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/AigHandle.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AigEdge.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AigMgr.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AigMgrImpl.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/AigMgrHolder.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/DotGen.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/replace.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/rwt/CalcMerit.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/rwt/Cut.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/rwt/CutMgr.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/rwt/Npn4.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/rwt/PatMgr.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/rwt/Pat2Aig.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/rwt/rewrite.cc
  PARENT_SCOPE
  )


# ===================================================================
# ターゲットの設定
# ===================================================================


# ===================================================================
#  テストの設定
# ===================================================================


# ===================================================================
#  インストールターゲットの設定
# ===================================================================
