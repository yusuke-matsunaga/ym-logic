
# ===================================================================
# パッケージの検査
# ===================================================================


# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  ${Python3_INCLUDE_DIRS}
  )


# ===================================================================
# サブディレクトリの設定
# ===================================================================


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set ( py_ymlogic_SOURCES
  py_ymlogic.cc
  )


# ===================================================================
#  executable ターゲットの設定
# ===================================================================

add_executable( py_ymlogic
  ${py_ymlogic_SOURCES}
  $<TARGET_OBJECTS:py_ymlogic_obj>
  $<TARGET_OBJECTS:py_ymbase_obj>
  $<TARGET_OBJECTS:ym_logic_obj>
  $<TARGET_OBJECTS:ym_base_obj>
  )

target_compile_options( py_ymlogic
  PRIVATE "-O3"
  )

target_compile_definitions( py_ymlogic
  PRIVATE "-DNODEBUG"
  )

target_link_libraries ( py_ymlogic
  ${YM_LIB_DEPENDS}
  ${Python3_LIBRARIES}
  )

add_executable( py_ymlogic_d
  ${py_ymlogic_SOURCES}
  $<TARGET_OBJECTS:py_ymlogic_obj_d>
  $<TARGET_OBJECTS:py_ymbase_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  $<TARGET_OBJECTS:ym_base_obj_d>
  )

target_compile_options( py_ymlogic_d
  PRIVATE "-g"
  )

target_link_libraries ( py_ymlogic_d
  ${YM_LIB_DEPENDS}
  ${Python3_LIBRARIES}
  )


# ===================================================================
# テストターゲット
# ===================================================================

add_test( logic_py_bddmgr_test
  py_ymlogic -m pytest ${CMAKE_CURRENT_SOURCE_DIR}/bddmgr_test.py
  )

add_test( logic_py_expr_test
  py_ymlogic -m pytest ${CMAKE_CURRENT_SOURCE_DIR}/expr_test.py
  )

add_test( logic_py_literal_test
  py_ymlogic -m pytest ${CMAKE_CURRENT_SOURCE_DIR}/literal_test.py
  )

add_test( logic_py_primtype_test
  py_ymlogic -m pytest ${CMAKE_CURRENT_SOURCE_DIR}/primtype_test.py
  )

add_test( logic_py_sopcube_test
  py_ymlogic -m pytest ${CMAKE_CURRENT_SOURCE_DIR}/sopcube_test.py
  )

add_test( logic_py_sopcover_test
  py_ymlogic -m pytest ${CMAKE_CURRENT_SOURCE_DIR}/sopcover_test.py
  )
