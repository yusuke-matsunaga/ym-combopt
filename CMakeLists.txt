# ===================================================================
# ym-combopt のための CMakeLists.txt
# ===================================================================


# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  ${PROJECT_BINARY_DIR}/include
  ${PROJECT_SOURCE_DIR}/ym-common/include
  ${PROJECT_SOURCE_DIR}/ym-base/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/private_include
  )


# ===================================================================
#  マクロの定義
# ===================================================================


# ===================================================================
# サブディレクトリの設定
# ===================================================================

add_subdirectory ( c++-test/udgraph )
add_subdirectory ( py-test )


# ===================================================================
#  ソースの設定
# ===================================================================

set ( mincov_SOURCES
  c++-src/mincov/Greedy.cc
  c++-src/mincov/Exact.cc
  c++-src/mincov/lb/LbCS.cc
  c++-src/mincov/lb/LbMIS1.cc
  c++-src/mincov/lb/LbMIS2.cc
  c++-src/mincov/lb/LbMIS3.cc
  c++-src/mincov/lb/MisNodeHeap.cc
  c++-src/mincov/MaxClique.cc
  c++-src/mincov/MinCov.cc
  c++-src/mincov/sel/SelCS.cc
  c++-src/mincov/sel/SelNaive.cc
  c++-src/mincov/sel/SelSimple.cc
  )

set ( matrix_SOURCES
  c++-src/matrix/McHeadList.cc
  c++-src/matrix/McColList.cc
  c++-src/matrix/McRowList.cc
#  c++-src/matrix/McBlock.cc
  c++-src/matrix/McMatrixImpl.cc
  )

set ( udgraph_SOURCES
  c++-src/udgraph/UdGraph.cc
  c++-src/udgraph/UdGraph_dimacs.cc
  )

set ( coloring_SOURCES
  c++-src/coloring/coloring.cc
  c++-src/coloring/ColGraph.cc
  c++-src/coloring/Dsatur.cc
  c++-src/coloring/IsCov.cc
  c++-src/coloring/Isx.cc
  c++-src/coloring/Isx2.cc
  c++-src/coloring/TabuCol.cc
  )

set ( max_clique_SOURCES
  c++-src/max_clique/max_clique.cc
  c++-src/max_clique/MclqSolver.cc
  c++-src/max_clique/MclqSolver_greedy.cc
  c++-src/max_clique/MclqSolver_exact.cc
  )

set ( ym_combopt_SOURCES
  ${mincov_SOURCES}
  ${matrix_SOURCES}
  ${udgraph_SOURCES}
  ${coloring_SOURCES}
  ${max_clique_SOURCES}
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================

ym_add_object_library ( ym_combopt
  ${ym_combopt_SOURCES}
  )
