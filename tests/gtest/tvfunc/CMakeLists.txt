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

ym_add_gtest ( logic_TvFunc_test
  TvFuncTest.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

#target_include_directories ( logic_TvFunc_test
#  PRIVATE
#  ${PROJECT_SOURCE_DIR}/ym-base/include
#  )

ym_add_gtest ( logic_TvFuncMove_test
  TvFuncMove.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

ym_add_gtest ( logic_TvFuncAnalyze_test
  TvFuncAnalyzeTest.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

ym_add_gtest ( logic_Tv2Bdd_test
  Tv2BddTest.cc
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

target_include_directories ( logic_Tv2Bdd_test
  PRIVATE
  ${PROJECT_SOURCE_DIR}/ym-logic/c++-srcs/tvfunc
  )


# ===================================================================
#  インストールターゲットの設定
# ===================================================================
