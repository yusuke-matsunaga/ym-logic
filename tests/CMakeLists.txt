
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  ${PROJECT_SOURCE_DIR}/private_include
  )


# ===================================================================
#  マクロの定義
# ===================================================================


# ===================================================================
# サブディレクトリの設定
# ===================================================================


# ===================================================================
#  npn_test
# ===================================================================

set ( npn_test_SOURCES
  npn_test.cc
  )

add_executable ( npn_test
  ${npn_test_SOURCES}
  $<TARGET_OBJECTS:ym_logic_obj>
  $<TARGET_OBJECTS:ym_base_obj>
  )

target_link_libraries ( npn_test
  ${YM_LIB_DEPENDS}
  )


# ===================================================================
#  npn_test_d
# ===================================================================

add_executable ( npn_test_d
  ${npn_test_SOURCES}
  $<TARGET_OBJECTS:ym_logic_obj_ad>
  $<TARGET_OBJECTS:ym_base_obj_ad>
  )

target_compile_options ( npn_test_d
  PRIVATE "-g"
  )

target_link_libraries ( npn_test_d
  ${YM_LIB_DEPENDS}
  )


# ===================================================================
#  npn_test2
# ===================================================================

set ( npn_test2_SOURCES
  npn_test2.cc
  )

add_executable ( npn_test2
  ${npn_test2_SOURCES}
  $<TARGET_OBJECTS:ym_logic_obj>
  $<TARGET_OBJECTS:ym_base_obj>
  )

target_link_libraries ( npn_test2
  ${YM_LIB_DEPENDS}
  )


# ===================================================================
#  npn_test2_d
# ===================================================================

add_executable ( npn_test2_d
  ${npn_test2_SOURCES}
  $<TARGET_OBJECTS:ym_logic_obj_ad>
  $<TARGET_OBJECTS:ym_base_obj_ad>
  )

target_compile_options ( npn_test2_d
  PRIVATE "-g"
  )

target_link_libraries ( npn_test2_d
  ${YM_LIB_DEPENDS}
  )
