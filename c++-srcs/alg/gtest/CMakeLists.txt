# ===================================================================
# alg/gtest のための CMakeLists.txt
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

ym_add_gtest ( logic_AlgCube_test
  AlgCubeTest.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

ym_add_gtest ( logic_AlgCover_test
  AlgCoverTest.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

ym_add_gtest ( logic_Kernel_test
  KernelTest.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

target_include_directories( logic_Kernel_test
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/..
  )

ym_add_gtest ( logic_GenFactor_test
  GenFactorTest.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

target_include_directories( logic_GenFactor_test
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/..
  )


# ===================================================================
#  インストールターゲットの設定
# ===================================================================
