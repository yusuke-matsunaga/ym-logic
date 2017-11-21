
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
  $<TARGET_OBJECTS:ym_logic>
  $<TARGET_OBJECTS:ym_base>
  )

target_compile_options ( npn_test
  PRIVATE "-g"
  )

target_link_libraries ( npn_test
  ${YM_LIB_DEPENDS}
  )


# ===================================================================
#  npn_test_d
# ===================================================================

add_executable ( npn_test_d
  ${npn_test_SOURCES}
  $<TARGET_OBJECTS:ym_logic_d>
  $<TARGET_OBJECTS:ym_base_d>
  )

target_compile_options ( npn_test_d
  PRIVATE "-g"
  )

target_link_libraries ( npn_test_d
  ${YM_LIB_DEPENDS}
  )
