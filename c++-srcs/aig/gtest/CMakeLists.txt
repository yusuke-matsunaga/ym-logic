
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  ${GTEST_INCLUDE_DIR}
  ../
  )


# ===================================================================
# サブディレクトリの設定
# ===================================================================


# ===================================================================
#  ソースファイルの設定
# ===================================================================


# ===================================================================
#  テスト用のターゲットの設定
# ===================================================================

ym_add_gtest ( logic_aig_AigNode_test
  AigNode_test.cc
  $<TARGET_OBJECTS:ym_logic_obj_d>
  ${YM_SUBMODULE_OBJ_D_LIST}
  DEFINITIONS "-DTESTDATA_DIR=\"${TESTDATA_DIR}\""
  )

ym_add_gtest ( logic_aig_AigEdge_test
  AigEdge_test.cc
  $<TARGET_OBJECTS:ym_logic_obj_d>
  ${YM_SUBMODULE_OBJ_D_LIST}
  DEFINITIONS "-DTESTDATA_DIR=\"${TESTDATA_DIR}\""
  )

ym_add_gtest ( logic_aig_AigHandle_test
  AigHandle_test.cc
  $<TARGET_OBJECTS:ym_logic_obj_d>
  ${YM_SUBMODULE_OBJ_D_LIST}
  DEFINITIONS "-DTESTDATA_DIR=\"${TESTDATA_DIR}\""
  )

ym_add_gtest ( logic_aig_AigMgr_test
  AigMgr_test.cc
  $<TARGET_OBJECTS:ym_logic_obj_d>
  ${YM_SUBMODULE_OBJ_D_LIST}
  DEFINITIONS "-DTESTDATA_DIR=\"${TESTDATA_DIR}\""
  )

ym_add_gtest ( logic_aig_AigMgrImpl_test
  AigMgrImpl_test.cc
  $<TARGET_OBJECTS:ym_logic_obj_d>
  ${YM_SUBMODULE_OBJ_D_LIST}
  DEFINITIONS "-DTESTDATA_DIR=\"${TESTDATA_DIR}\""
  )

ym_add_gtest ( logic_aig_Cov2Aig_test
  cov2aig_test.cc
  $<TARGET_OBJECTS:ym_logic_obj_d>
  ${YM_SUBMODULE_OBJ_D_LIST}
  DEFINITIONS "-DTESTDATA_DIR=\"${TESTDATA_DIR}\""
  )

ym_add_gtest ( logic_aig_copy_test
  copy_test.cc
  $<TARGET_OBJECTS:ym_logic_obj_d>
  ${YM_SUBMODULE_OBJ_D_LIST}
  DEFINITIONS "-DTESTDATA_DIR=\"${TESTDATA_DIR}\""
  )
