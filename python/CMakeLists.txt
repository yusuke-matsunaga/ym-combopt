

# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  ${CMAKE_CURRENT_SOURCE_DIR}/../../ym-base/cython-src
  ${CMAKE_CURRENT_SOURCE_DIR}/../cython-src
  )


# ===================================================================
#  マクロの定義
# ===================================================================


# ===================================================================
# サブディレクトリの設定
# ===================================================================

add_subdirectory ( debug )
add_subdirectory ( cython-src )


# ===================================================================
#  cython 用のソースファイルの設定
# ===================================================================

set ( CYTHON_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/../cython-src/combopt.pyx
  )

set_source_files_properties (
  ${CYTHON_SOURCES}
  PROPERTIES CYTHON_IS_CXX TRUE
  )


# ===================================================================
#  module ターゲットの設定
# ===================================================================

cython_add_module ( combopt
  ${CYTHON_SOURCES}
  $<TARGET_OBJECTS:ym_base>
  $<TARGET_OBJECTS:ym_combopt>
  )

target_compile_options ( combopt
  PRIVATE "-O3"
  )

target_compile_definitions ( combopt
  PRIVATE "-DNDEBUG"
  )

target_link_libraries ( combopt
  ${YM_LIB_DEPENDS}
  )

cython_add_module ( combopt_d
  ${CYTHON_SOURCES}
  $<TARGET_OBJECTS:ym_base_d>
  $<TARGET_OBJECTS:ym_combopt_d>
  )

target_compile_options ( combopt_d
  PRIVATE "-g"
  )

target_link_libraries ( combopt_d
  ${YM_LIB_DEPENDS}
  )

add_custom_command (
  TARGET combopt_d
  POST_BUILD
  COMMAND ${CMAKE_COMMAND}
  ARGS -E copy $<TARGET_FILE:combopt_d> ${CMAKE_CURRENT_BINARY_DIR}/debug/combopt.so
  )
