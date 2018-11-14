# ===================================================================
# ym-logic/gtest のための CMakeLists.txt
# ===================================================================


# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  ${PROJECT_SOURCE_DIR}/ym-logic/c++-src/alg
  ${PROJECT_SOURCE_DIR}/ym-logic/c++-src/npn
  ${GTEST_INCLUDE_DIR}
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

ym_add_gtest ( logic_VarId_test
  common/VarIdTest.cc
  $<TARGET_OBJECTS:ym_base_ad>
  $<TARGET_OBJECTS:ym_logic_ad>
  )

ym_add_gtest ( logic_Literal_test
  common/LiteralTest.cc
  $<TARGET_OBJECTS:ym_base_ad>
  $<TARGET_OBJECTS:ym_logic_ad>
  )

ym_add_gtest ( logic_Expr_test
  expr/ExprTest.cc
  $<TARGET_OBJECTS:ym_base_ad>
  $<TARGET_OBJECTS:ym_logic_ad>
  )

ym_add_gtest ( logic_TvFunc_test
  tvfunc/TvFuncTest.cc
  $<TARGET_OBJECTS:ym_base_ad>
  $<TARGET_OBJECTS:ym_logic_ad>
  )

ym_add_gtest ( logic_TvFuncMove_test
  tvfunc/TvFuncMove.cc
  $<TARGET_OBJECTS:ym_base_ad>
  $<TARGET_OBJECTS:ym_logic_ad>
  )

ym_add_gtest ( logic_InputInfo_test
  npn/InputInfoTest.cc
  $<TARGET_OBJECTS:ym_base_ad>
  $<TARGET_OBJECTS:ym_logic_ad>
  )

ym_add_gtest ( logic_IgPartition_test
  npn/IgPartitionTest.cc
  $<TARGET_OBJECTS:ym_base_ad>
  $<TARGET_OBJECTS:ym_logic_ad>
  )

ym_add_gtest ( logic_NpnMap_test
  npn/NpnMapTest.cc
  $<TARGET_OBJECTS:ym_base_ad>
  $<TARGET_OBJECTS:ym_logic_ad>
  )

ym_add_gtest ( logic_w01_test
  npn/w01test.cc
  $<TARGET_OBJECTS:ym_base_ad>
  $<TARGET_OBJECTS:ym_logic_ad>
  )

ym_add_gtest ( logic_NpnMgr_test
  npn/NpnMgrTest.cc
  $<TARGET_OBJECTS:ym_base_ad>
  $<TARGET_OBJECTS:ym_logic_ad>
  )

ym_add_gtest ( logic_AlgMgr_test
  alg/AlgMgrTest.cc
  $<TARGET_OBJECTS:ym_base_ad>
  $<TARGET_OBJECTS:ym_logic_ad>
  )

ym_add_gtest ( logic_AlgCube_test
  alg/AlgCubeTest.cc
  $<TARGET_OBJECTS:ym_base_ad>
  $<TARGET_OBJECTS:ym_logic_ad>
  )

ym_add_gtest ( logic_AlgCover_test
  alg/AlgCoverTest.cc
  $<TARGET_OBJECTS:ym_base_ad>
  $<TARGET_OBJECTS:ym_logic_ad>
  )


# ===================================================================
#  インストールターゲットの設定
# ===================================================================
