
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  )


# ===================================================================
#  マクロの定義
# ===================================================================


# ===================================================================
# サブディレクトリの設定
# ===================================================================


# ===================================================================
#  mwc_test
# ===================================================================

set ( mwc_test_SOURCES
  mwc_test.cc
  )

add_executable ( mwc_test
  ${mwc_test_SOURCES}
  $<TARGET_OBJECTS:ym_logic_obj>
  $<TARGET_OBJECTS:ym_base_obj>
  $<TARGET_OBJECTS:ym_json_obj_d>
  )

target_link_libraries ( mwc_test
  ${YM_LIB_DEPENDS}
  )


# ===================================================================
#  mwc_test_d
# ===================================================================

add_executable ( mwc_test_d
  ${mwc_test_SOURCES}
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_json_obj_d>
  )

target_compile_options ( mwc_test_d
  PRIVATE "-g"
  )

target_link_libraries ( mwc_test_d
  ${YM_LIB_DEPENDS}
  )
