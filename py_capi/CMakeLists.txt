
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
  PyPrimType.cc
  PyLiteral.cc
  PyExpr.cc
  PyNpnMap.cc
  PyTvFunc.cc
  PyBddMgr.cc
  PyBdd.cc
  PyBddVarSet.cc
  PySopCover.cc
  PySopCube.cc
  PyAigHandle.cc
  PyAigMgr.cc
  ymlogic_module.cc
  )


# ===================================================================
#  library ターゲットの設定
# ===================================================================

ym_add_object_library( py_ymlogic
  ${py_ymlogic_SOURCES}
  )

add_library ( py_ymlogiclib
  SHARED
  $<TARGET_OBJECTS:py_ymlogic_obj>
  $<TARGET_OBJECTS:py_ymbase_obj>
  $<TARGET_OBJECTS:ym_logic_obj>
  $<TARGET_OBJECTS:ym_base_obj>
  )

set_target_properties( py_ymlogiclib
  PROPERTIES
  PREFIX ""
  OUTPUT_NAME "ymlogic"
  )

target_link_libraries( py_ymlogiclib
  ${YM_LIB_DEPENDS}
  ${Python3_LIBRARIES}
  )

# ===================================================================
# テストターゲット
# ===================================================================
