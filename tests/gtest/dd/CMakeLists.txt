# ===================================================================
# ym-logic/gtest のための CMakeLists.txt
# ===================================================================


# ===================================================================
# インクルードパスの設定
# ===================================================================


# ===================================================================
# サブディレクトリの設定
# ===================================================================


# ===================================================================
#  ソースファイルの設定
# ===================================================================


# ===================================================================
#  ターゲットの設定
# ===================================================================

ym_add_gtest( logic_dd_DdEdge_test
  DdEdge_test.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

target_include_directories ( logic_dd_DdEdge_test
  PRIVATE
  ${PROJECT_SOURCE_DIR}/ym-logic/c++-srcs/dd
  )

ym_add_gtest( logic_dd_BddMgr_test
  BddMgr_test.cc
  BddTest.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

ym_add_gtest( logic_dd_Bdd_test
  Bdd_test.cc
  BddTest.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

ym_add_gtest( logic_dd_ZddTest
  ZddTest.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )


# ===================================================================
#  インストールターゲットの設定
# ===================================================================
