

# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  #${PROJECT_SOURCE_DIR}/ym-base/cython-src
  #${PROJECT_SOURCE_DIR}/ym-combopt/cython-src
  )


# ===================================================================
#  マクロの定義
# ===================================================================


# ===================================================================
# サブディレクトリの設定
# ===================================================================


# ===================================================================
#  cython 用のソースファイルの設定
# ===================================================================

set ( CYTHON_SOURCES
  pym_combopt.pyx
  )

set_source_files_properties (
  ${CYTHON_SOURCES}
  PROPERTIES CYTHON_IS_CXX TRUE
  )


# ===================================================================
#  module ターゲットの設定
# ===================================================================

cython_add_module ( pym_combopt
  ${CYTHON_SOURCES}
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_combopt_obj_d>
  )

target_compile_options ( pym_combopt
  PRIVATE "-g"
  )


target_link_libraries ( pym_combopt
  ${YM_LIB_DEPENDS}
  )
