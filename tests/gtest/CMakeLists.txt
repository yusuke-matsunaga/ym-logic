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

ym_add_gtest ( logic_VarId_test
  common/VarIdTest.cc
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

ym_add_gtest ( logic_Literal_test
  common/LiteralTest.cc
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

ym_add_gtest ( logic_Expr_test
  expr/ExprTest.cc
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

ym_add_gtest ( logic_TvFunc_test
  tvfunc/TvFuncTest.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

target_include_directories ( logic_TvFunc_test
  PRIVATE
  ${PROJECT_SOURCE_DIR}/ym-base/include
  )

ym_add_gtest ( logic_TvFuncMove_test
  tvfunc/TvFuncMove.cc
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

ym_add_gtest ( logic_InputInfo_test
  npn/InputInfoTest.cc
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

target_include_directories( logic_InputInfo_test
  PRIVATE
  ${PROJECT_SOURCE_DIR}/ym-logic/c++-srcs/npn
  )

ym_add_gtest ( logic_IgPartition_test
  npn/IgPartitionTest.cc
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

target_include_directories( logic_IgPartition_test
  PRIVATE
  ${PROJECT_SOURCE_DIR}/ym-logic/c++-srcs/npn
  )

ym_add_gtest ( logic_NpnMap_test
  npn/NpnMapTest.cc
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

ym_add_gtest ( logic_w01_test
  npn/w01test.cc
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

target_include_directories( logic_w01_test
  PRIVATE
  ${PROJECT_SOURCE_DIR}/ym-logic/c++-srcs/npn
  )

ym_add_gtest ( logic_NpnMgr_test
  npn/NpnMgrTest.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

target_include_directories( logic_NpnMgr_test
  PRIVATE
  ${PROJECT_SOURCE_DIR}/ym-base/include
  ${PROJECT_SOURCE_DIR}/ym-logic/c++-srcs/npn
  )

ym_add_gtest ( logic_SopMgr_test
  sop/SopMgrTest.cc
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

target_include_directories( logic_SopMgr_test
  PRIVATE
  ${PROJECT_SOURCE_DIR}/ym-logic/c++-srcs/sop
  )

ym_add_gtest ( logic_SopCube_test
  sop/SopCubeTest.cc
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

ym_add_gtest ( logic_SopCover_test
  sop/SopCoverTest.cc
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

ym_add_gtest ( logic_Kernel_test
  sop/KernelTest.cc
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

target_include_directories( logic_Kernel_test
  PRIVATE
  ${PROJECT_SOURCE_DIR}/ym-logic/c++-srcs/sop
  )

ym_add_gtest ( logic_GenFactor_test
  sop/GenFactorTest.cc
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

target_include_directories( logic_GenFactor_test
  PRIVATE
  ${PROJECT_SOURCE_DIR}/ym-logic/c++-srcs/sop
  )

ym_add_gtest( logic_BddMgr_test
  bdd/BddMgr_test.cc
  bdd/BddTest.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

ym_add_gtest( logic_Bdd_test
  bdd/Bdd_test.cc
  bdd/BddTest.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

ym_add_gtest( logic_BddEdge_test
  bdd/BddEdge_test.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

target_include_directories ( logic_BddEdge_test
  PRIVATE
  ${PROJECT_SOURCE_DIR}/ym-logic/c++-srcs/bdd
  )

# ===================================================================
#  インストールターゲットの設定
# ===================================================================
