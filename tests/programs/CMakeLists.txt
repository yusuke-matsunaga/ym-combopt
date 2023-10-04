
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

add_executable( coloring
  coloring.cc
  $<TARGET_OBJECTS:ym_combopt_obj>
  $<TARGET_OBJECTS:ym_base_obj>
  $<TARGET_OBJECTS:ym_json_obj>
  )

target_link_libraries ( coloring
  ${YM_LIB_DEPENDS}
  )

add_executable( coloring_d
  coloring.cc
  $<TARGET_OBJECTS:ym_combopt_obj_d>
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_json_obj_d>
  )

target_compile_options ( coloring_d
  PRIVATE "-g"
  )

target_link_libraries ( coloring_d
  ${YM_LIB_DEPENDS}
  )
