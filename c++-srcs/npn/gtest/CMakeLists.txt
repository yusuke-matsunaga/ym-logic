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

ym_add_gtest ( logic_InputInfo_test
  InputInfoTest.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

ym_add_gtest ( logic_IgPartition_test
  IgPartitionTest.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

target_include_directories( logic_IgPartition_test
  PRIVATE
  ${PROJECT_SOURCE_DIR}/ym-logic/c++-srcs/npn
  )

ym_add_gtest ( logic_NpnMap_test
  NpnMapTest.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

ym_add_gtest ( logic_w01_test
  w01test.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

target_include_directories( logic_w01_test
  PRIVATE
  ${PROJECT_SOURCE_DIR}/ym-logic/c++-srcs/npn
  )

ym_add_gtest ( logic_NpnMgr_test
  NpnMgrTest.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

target_include_directories( logic_NpnMgr_test
  PRIVATE
  ${PROJECT_SOURCE_DIR}/ym-base/include
  ${PROJECT_SOURCE_DIR}/ym-logic/c++-srcs/npn
  )


# ===================================================================
#  インストールターゲットの設定
# ===================================================================
