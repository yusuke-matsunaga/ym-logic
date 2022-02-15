
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  ${PROJECT_SOURCE_DIR}/ym-base/cython-src
  ${PROJECT_SOURCE_DIR}/ym-logic/cython-src
  )


# ===================================================================
# サブディレクトリの設定
# ===================================================================


# ===================================================================
#  ソースファイルの設定
# ===================================================================


# ===================================================================
# ターゲットの設定
# ===================================================================


# ===================================================================
#  cython 用のソースファイルの設定
# ===================================================================

set ( CYTHON_SOURCES
  pym_logic.pyx
  )

set_source_files_properties (
  ${CYTHON_SOURCES}
  PROPERTIES CYTHON_IS_CXX TRUE
  )


# ===================================================================
#  module ターゲットの設定
# ===================================================================

cython_add_module ( pym_logic
  ${CYTHON_SOURCES}
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_logic_obj_d>
  )

target_compile_options ( pym_logic
  PRIVATE "-g"
  )

target_link_libraries ( pym_logic
  ${YM_LIB_DEPENDS}
  )


# ===================================================================
#  テストの設定
# ===================================================================

set ( RUN_PYM_LOGIC
  env PYTHONPATH=${PROJECT_BINARY_DIR}/ym-logic/py-test
  ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/pym_logic_test.py)

add_test ( logic_py_VarId_test
  ${RUN_PYM_LOGIC} VarIdTest
  )

add_test ( logic_py_Literal_test
  ${RUN_PYM_LOGIC} LiteralTest
  )

add_test ( logic_py_Expr_test
  ${RUN_PYM_LOGIC} ExprTest
  )


# ===================================================================
#  インストールターゲットの設定
# ===================================================================
