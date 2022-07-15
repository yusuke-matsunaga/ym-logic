
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
#  truth_test
# ===================================================================

set ( truth_test_SOURCES
  truth_test.cc
  )

add_executable ( truth_test
  ${truth_test_SOURCES}
  $<TARGET_OBJECTS:ym_logic_obj>
  $<TARGET_OBJECTS:ym_base_obj>
  )

target_link_libraries ( truth_test
  ${YM_LIB_DEPENDS}
  )


# ===================================================================
#  truth_test_d
# ===================================================================

add_executable ( truth_test_d
  ${truth_test_SOURCES}
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_base_obj_d>
  )

target_compile_options ( truth_test_d
  PRIVATE "-g"
  )

target_link_libraries ( truth_test_d
  ${YM_LIB_DEPENDS}
  )


# ===================================================================
#  dot_test
# ===================================================================

set ( dot_test_SOURCES
  dot_test.cc
  )

add_executable ( dot_test
  ${dot_test_SOURCES}
  $<TARGET_OBJECTS:ym_logic_obj>
  $<TARGET_OBJECTS:ym_base_obj>
  )

target_link_libraries ( dot_test
  ${YM_LIB_DEPENDS}
  )


# ===================================================================
#  dot_test_d
# ===================================================================

add_executable ( dot_test_d
  ${dot_test_SOURCES}
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_base_obj_d>
  )

target_compile_options ( dot_test_d
  PRIVATE "-g"
  )

target_link_libraries ( dot_test_d
  ${YM_LIB_DEPENDS}
  )
