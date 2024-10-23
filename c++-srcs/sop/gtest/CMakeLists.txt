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

ym_add_gtest ( logic_SopMgr_test
  SopMgrTest.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

target_include_directories( logic_SopMgr_test
  PRIVATE
  ${PROJECT_SOURCE_DIR}/ym-logic/c++-srcs/sop
  )

ym_add_gtest ( logic_SopCube_test
  SopCubeTest.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

ym_add_gtest ( logic_SopCover_test
  SopCoverTest.cc
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
  ${PROJECT_SOURCE_DIR}/ym-logic/c++-srcs/sop
  )

ym_add_gtest ( logic_GenFactor_test
  GenFactorTest.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

target_include_directories( logic_GenFactor_test
  PRIVATE
  ${PROJECT_SOURCE_DIR}/ym-logic/c++-srcs/sop
  )


# ===================================================================
#  インストールターゲットの設定
# ===================================================================
