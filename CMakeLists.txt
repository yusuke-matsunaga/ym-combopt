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
  include
  private_include
  )


# ===================================================================
#  マクロの定義
# ===================================================================


# ===================================================================
# サブディレクトリの設定
# ===================================================================

add_subdirectory ( tests )


# ===================================================================
#  ソースの設定
# ===================================================================

set ( mincov_SOURCES
  c++-srcs/mincov/Solver.cc
  c++-srcs/mincov/Greedy.cc
  c++-srcs/mincov/Exact.cc
  c++-srcs/mincov/lb/LbCalc.cc
  c++-srcs/mincov/lb/LbCS.cc
  c++-srcs/mincov/lb/LbMIS1.cc
  c++-srcs/mincov/lb/LbMIS2.cc
  c++-srcs/mincov/lb/LbMIS3.cc
  c++-srcs/mincov/lb/LbMAX.cc
  c++-srcs/mincov/lb/MisNodeHeap.cc
  c++-srcs/mincov/MaxClique.cc
  c++-srcs/mincov/MinCov.cc
  c++-srcs/mincov/sel/Selector.cc
  c++-srcs/mincov/sel/SelCS.cc
  c++-srcs/mincov/sel/SelNaive.cc
  c++-srcs/mincov/sel/SelSimple.cc
  )

set ( matrix_SOURCES
  c++-srcs/matrix/McHeadList.cc
  c++-srcs/matrix/McColList.cc
  c++-srcs/matrix/McRowList.cc
#  c++-srcs/matrix/McBlock.cc
  c++-srcs/matrix/McMatrix.cc
  c++-srcs/matrix/McMatrixImpl.cc
  )

set ( udgraph_SOURCES
  c++-srcs/udgraph/UdGraph.cc
  c++-srcs/udgraph/UdGraph_dimacs.cc
  )

set ( coloring_SOURCES
  c++-srcs/coloring/UdGraph_coloring.cc
  c++-srcs/coloring/ColGraph.cc
  c++-srcs/coloring/Dsatur.cc
  c++-srcs/coloring/IsCov.cc
  c++-srcs/coloring/Isx.cc
  c++-srcs/coloring/Isx2.cc
  c++-srcs/coloring/TabuCol.cc
  )

set ( max_clique_SOURCES
  c++-srcs/max_clique/MclqSolver.cc
  c++-srcs/max_clique/MclqSolver_greedy.cc
  c++-srcs/max_clique/MclqSolver_exact.cc
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
