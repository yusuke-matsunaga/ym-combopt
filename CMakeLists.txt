# ===================================================================
# CMAKE のおまじない
# ===================================================================


# ===================================================================
# プロジェクト名，バージョンの設定
# ===================================================================


# ===================================================================
# オプション
# ===================================================================


# ===================================================================
# パッケージの検査
# ===================================================================


# ===================================================================
# ヘッダファイルの生成
# ===================================================================


# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  private_include
  )


# ===================================================================
#  マクロの定義
# ===================================================================


# ===================================================================
# サブディレクトリの設定
# ===================================================================

add_subdirectory ( gtest/udgraph )
add_subdirectory ( python )


# ===================================================================
#  ソースの設定
# ===================================================================

set ( mincov_SOURCES
  src/mincov/Greedy.cc
  src/mincov/Exact.cc
  src/mincov/LbCS.cc
  src/mincov/LbMAX.cc
  src/mincov/LbMIS1.cc
  src/mincov/LbMIS2.cc
  src/mincov/LbMIS3.cc
  src/mincov/MaxClique.cc
  src/mincov/McColList.cc
  src/mincov/McRowList.cc
  src/mincov/McMatrix.cc
  src/mincov/MinCov.cc
  src/mincov/MisNodeHeap.cc
  src/mincov/SelCS.cc
  src/mincov/SelNaive.cc
  src/mincov/SelSimple.cc
  )

set ( udgraph_SOURCES
  src/udgraph/UdGraph.cc
  src/udgraph/UdGraph_dimacs.cc
  )

set ( coloring_SOURCES
  src/coloring/coloring.cc
  src/coloring/ColGraph.cc
  src/coloring/Dsatur.cc
  src/coloring/IsCov.cc
  src/coloring/Isx.cc
  src/coloring/Isx2.cc
  src/coloring/TabuCol.cc
  )

set ( max_clique_SOURCES
  src/max_clique/max_clique.cc
  src/max_clique/MclqSolver.cc
  src/max_clique/MclqSolver_greedy.cc
  src/max_clique/MclqSolver_exact.cc
  )

set ( ym_combopt_SOURCES
  ${mincov_SOURCES}
  ${udgraph_SOURCES}
  ${coloring_SOURCES}
  ${max_clique_SOURCES}
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================

ym_add_object_library ( ym_combopt
  OBJECT
  ${ym_combopt_SOURCES}
  )
