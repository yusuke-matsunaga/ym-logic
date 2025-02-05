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

ym_add_gtest ( logic_TvFunc_test
  TvFuncTest.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

ym_add_gtest ( logic_TvFunc_move_test
  TvFuncTest_move.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

ym_add_gtest ( logic_TvFunc_analyze_test
  TvFuncTest_analyze.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

ym_add_gtest ( logic_TvFunc_bcf_test
  TvFuncTest_bcf.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

ym_add_gtest ( logic_Tv2Bdd_test
  Tv2BddTest.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )


# ===================================================================
#  インストールターゲットの設定
# ===================================================================
