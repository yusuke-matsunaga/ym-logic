# ===================================================================
# ym-logic/gtest のための CMakeLists.txt
# ===================================================================


# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  ../
  )

# ===================================================================
# サブディレクトリの設定
# ===================================================================


# ===================================================================
#  ソースファイルの設定
# ===================================================================


# ===================================================================
#  ターゲットの設定
# ===================================================================

add_executable( bdd_test
  bdd_test.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

target_link_libraries ( bdd_test
  ${YM_LIB_DEPENDS}
  )

add_executable( loop_test
  loop_test.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

target_link_libraries ( loop_test
  ${YM_LIB_DEPENDS}
  )


# ===================================================================
#  インストールターゲットの設定
# ===================================================================
