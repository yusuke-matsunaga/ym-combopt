# ===================================================================
# ym-combopt のための CMakeLists.txt
# ===================================================================


# ===================================================================
# インクルードパスの設定
# ===================================================================


# ===================================================================
#  マクロの定義
# ===================================================================


# ===================================================================
# サブディレクトリの設定
# ===================================================================


# ===================================================================
#  ソースの設定
# ===================================================================

set ( mincov_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/Solver.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/Greedy.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/Exact.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/lb/LbCalc.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/lb/LbCS.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/lb/LbMIS1.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/lb/LbMIS2.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/lb/LbMIS3.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/lb/LbMAX.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/lb/MisNodeHeap.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/MaxClique.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/MinCov.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/sel/Selector.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/sel/SelCS.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/sel/SelNaive.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/sel/SelSimple.cc
  PARENT_SCOPE
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================
