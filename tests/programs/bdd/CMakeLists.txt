
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
#  truth_test2
# ===================================================================

set ( truth_test2_SOURCES
  truth_test2.cc
  )

add_executable ( truth_test2
  ${truth_test2_SOURCES}
  $<TARGET_OBJECTS:ym_logic_obj>
  $<TARGET_OBJECTS:ym_base_obj>
  )

target_link_libraries ( truth_test2
  ${YM_LIB_DEPENDS}
  )


# ===================================================================
#  truth_test_d
# ===================================================================

add_executable ( truth_test2_d
  ${truth_test2_SOURCES}
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_base_obj_d>
  )

target_compile_options ( truth_test2_d
  PRIVATE "-g"
  )

target_link_libraries ( truth_test2_d
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


# ===================================================================
#  blif2bdd
# ===================================================================

set ( blif2bdd_SOURCES
  blif2bdd.cc
  Bnet2Bdd.cc
  )

add_executable ( blif2bdd
  ${blif2bdd_SOURCES}
  $<TARGET_OBJECTS:ym_bnet_obj>
  $<TARGET_OBJECTS:ym_cell_obj>
  $<TARGET_OBJECTS:ym_logic_obj>
  $<TARGET_OBJECTS:ym_base_obj>
  )

target_include_directories ( blif2bdd
  PRIVATE
  ${PROJECT_SOURCE_DIR}/ym-bnet/include
  ${PROJECT_SOURCE_DIR}/ym-cell/include
  )

target_link_libraries ( blif2bdd
  ${YM_LIB_DEPENDS}
  )


# ===================================================================
#  blif2bdd_d
# ===================================================================

add_executable ( blif2bdd_d
  ${blif2bdd_SOURCES}
  $<TARGET_OBJECTS:ym_bnet_obj_d>
  $<TARGET_OBJECTS:ym_cell_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_base_obj_d>
  )

target_include_directories ( blif2bdd_d
  PRIVATE
  ${PROJECT_SOURCE_DIR}/ym-bnet/include
  ${PROJECT_SOURCE_DIR}/ym-cell/include
  )

target_compile_options ( blif2bdd_d
  PRIVATE "-g"
  )

target_link_libraries ( blif2bdd_d
  ${YM_LIB_DEPENDS}
  )
