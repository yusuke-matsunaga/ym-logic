# ===================================================================
# ym-logic/gtest/npn のための CMakeLists.txt
# ===================================================================


# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  ../../src/npn
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

ym_add_gtest(InputInfoTest
  InputInfoTest.cc
  $<TARGET_OBJECTS:ym_base_ad>
  $<TARGET_OBJECTS:ym_logic_ad>
  )

ym_add_gtest(IgPartitionTest
  IgPartitionTest.cc
  $<TARGET_OBJECTS:ym_base_ad>
  $<TARGET_OBJECTS:ym_logic_ad>
  )

ym_add_gtest(NpnMapTest
  NpnMapTest.cc
  $<TARGET_OBJECTS:ym_base_ad>
  $<TARGET_OBJECTS:ym_logic_ad>
  )

ym_add_gtest(w01test
  w01test.cc
  $<TARGET_OBJECTS:ym_base_ad>
  $<TARGET_OBJECTS:ym_logic_ad>
  )

ym_add_gtest(NpnMgrTest
  NpnMgrTest.cc
  $<TARGET_OBJECTS:ym_base_ad>
  $<TARGET_OBJECTS:ym_logic_ad>
  )


# ===================================================================
#  インストールターゲットの設定
# ===================================================================
