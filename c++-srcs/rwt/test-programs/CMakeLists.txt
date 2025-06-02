# ===================================================================
# ym-logic/gtest のための CMakeLists.txt
# ===================================================================


# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories()


# ===================================================================
# サブディレクトリの設定
# ===================================================================


# ===================================================================
#  ソースファイルの設定
# ===================================================================


# ===================================================================
#  ターゲットの設定
# ===================================================================

add_executable( rwt_test
  rwt_test.cc
  $<TARGET_OBJECTS:ym_base_obj>
  $<TARGET_OBJECTS:ym_bn_obj>
  $<TARGET_OBJECTS:ym_cell_obj>
  $<TARGET_OBJECTS:ym_logic_obj>
  )

target_link_libraries ( rwt_test
  ${YM_LIB_DEPENDS}
  )

target_compile_options ( rwt_test
  PRIVATE "-O3"
  )

target_compile_definitions ( rwt_test
  PRIVATE "-DNDEBUG"
  )

add_executable( rwt_test_d
  rwt_test.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_bn_obj_d>
  $<TARGET_OBJECTS:ym_cell_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

target_link_libraries ( rwt_test_d
  ${YM_LIB_DEPENDS}
  )

target_compile_options ( rwt_test_d
  PRIVATE "-g"
  )


# ===================================================================
#  インストールターゲットの設定
# ===================================================================
