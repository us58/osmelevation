// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <gtest/gtest.h>
#include "util/graph/Graph.h"

using util::graph::Graph;

const std::vector<std::pair<uint64_t, uint64_t>> unfinishedPathEmpty;


// ____________________________________________________________________________
TEST(GraphTest, addEdge) {
  Graph g;
  g.addEdge(1, 2, 100, true);
  g.addEdge(2, 3, 101, false);
  g.addEdge(2, 4, 102, true);
  g.addEdge(4, 5, 103, true);
  g.addEdge(5, 6, 104, true);

  auto graph = g.getGraph();

  ASSERT_EQ((size_t)4, graph.size());

  ASSERT_EQ((size_t)1, graph[1].size());
  ASSERT_EQ((uint64_t)2, graph[1][0].pointsTo);
  ASSERT_EQ((uint64_t)100, graph[1][0].edgeId);
  ASSERT_EQ(true, graph[1][0].reversedEdgeExists);

  ASSERT_EQ((size_t)2, graph[2].size());
  ASSERT_EQ((uint64_t)3, graph[2][0].pointsTo);
  ASSERT_EQ((uint64_t)101, graph[2][0].edgeId);
  ASSERT_EQ(false, graph[2][0].reversedEdgeExists);
  ASSERT_EQ((uint64_t)4, graph[2][1].pointsTo);
  ASSERT_EQ((uint64_t)102, graph[2][1].edgeId);
  ASSERT_EQ(true, graph[2][1].reversedEdgeExists);

  ASSERT_EQ((size_t)1, graph[4].size());
  ASSERT_EQ((uint64_t)5, graph[4][0].pointsTo);
  ASSERT_EQ((uint64_t)103, graph[4][0].edgeId);
  ASSERT_EQ(true, graph[4][0].reversedEdgeExists);

  ASSERT_EQ((size_t)1, graph[5].size());
  ASSERT_EQ((uint64_t)6, graph[5][0].pointsTo);
  ASSERT_EQ((uint64_t)104, graph[5][0].edgeId);
  ASSERT_EQ(true, graph[5][0].reversedEdgeExists);
}

// ____________________________________________________________________________
TEST(GraphTest, findStartpoints) {
  Graph g;
  g.addEdge(1, 2, 100, false);
  g.addEdge(2, 3, 101, false);
  g.addEdge(2, 4, 102, false);
  g.addEdge(4, 5, 103, false);
  g.addEdge(5, 6, 104, false);

  auto graph = g.getGraph();
  g.findStartpoints();

  auto startPoint = g.getStartpoint();
  ASSERT_EQ((uint64_t)1, startPoint);
  startPoint = g.getStartpoint();
  ASSERT_EQ((uint64_t)0, startPoint);
}

// ____________________________________________________________________________
TEST(GraphTest, buildPath) {
  Graph g;

  // forward
  g.addEdge(1, 2, 100, true);
  g.addEdge(2, 3, 101, false);
  g.addEdge(2, 4, 102, true);
  g.addEdge(4, 5, 103, true);
  g.addEdge(5, 6, 104, true);

  // backward
  g.addEdge(6, 5, 104, true);
  g.addEdge(5, 4, 103, true);
  g.addEdge(4, 2, 102, true);
  g.addEdge(2, 1, 100, true);

  auto graph = g.getGraph();
  g.findStartpoints();

  auto startPoint = g.getStartpoint();
  ASSERT_EQ((uint64_t)1, startPoint);
  startPoint = g.getStartpoint();
  ASSERT_EQ((uint64_t)6, startPoint);
  startPoint = g.getStartpoint();
  ASSERT_EQ((uint64_t)0, startPoint);

  const auto resultF = g.traverseGraph(1, unfinishedPathEmpty, true, false);
  ASSERT_EQ((size_t)4, resultF.size());
  ASSERT_EQ((uint64_t)2, resultF[0].first);
  ASSERT_EQ((uint64_t)100, resultF[0].second);

  ASSERT_EQ((uint64_t)4, resultF[1].first);
  ASSERT_EQ((uint64_t)102, resultF[1].second);

  ASSERT_EQ((uint64_t)5, resultF[2].first);
  ASSERT_EQ((uint64_t)103, resultF[2].second);

  ASSERT_EQ((uint64_t)6, resultF[3].first);
  ASSERT_EQ((uint64_t)104, resultF[3].second);

  const auto resultB = g.traverseGraph(6, unfinishedPathEmpty, true, false);
  ASSERT_EQ((size_t)0, resultB.size());

  // Check the unfinished path and build the remaining path.
  auto unfinishedPath = g.getUnfinishedPath();
  ASSERT_EQ((size_t)1, unfinishedPath.size());
  ASSERT_EQ((uint64_t)2, unfinishedPath[0].first);
  ASSERT_EQ((uint64_t)100, unfinishedPath[0].second);

  const auto remainingPath = g.traverseGraph(unfinishedPath.back().first, unfinishedPath, false, false);
  ASSERT_EQ((size_t)1, remainingPath.size());
  ASSERT_EQ((uint64_t)3, remainingPath[0].first);
  ASSERT_EQ((uint64_t)101, remainingPath[0].second);

  unfinishedPath = g.getUnfinishedPath();
  ASSERT_EQ((size_t)0, unfinishedPath.size());
}

// ____________________________________________________________________________
TEST(GraphTest, buildPathTwoEnds) {
  Graph g;
  g.addEdge(1, 2, 100, false);
  g.addEdge(2, 3, 101, false);
  g.addEdge(3, 4, 102, false);
  g.addEdge(3, 5, 103, false);

  g.findStartpoints();
  auto startPoint = g.getStartpoint();
  ASSERT_EQ((uint64_t)1, startPoint);
  startPoint = g.getStartpoint();
  ASSERT_EQ((uint64_t)0, startPoint);

  auto result = g.traverseGraph(1, unfinishedPathEmpty, true, false);
  ASSERT_EQ((size_t)3, result.size());
  ASSERT_EQ((uint64_t)2, result[0].first);
  ASSERT_EQ((uint64_t)100, result[0].second);

  ASSERT_EQ((uint64_t)3, result[1].first);
  ASSERT_EQ((uint64_t)101, result[1].second);

  ASSERT_EQ((uint64_t)5, result[2].first);
  ASSERT_EQ((uint64_t)103, result[2].second);

  // Check the unfinished path and build the remaining path.
  auto unfinishedPath = g.getUnfinishedPath();
  ASSERT_EQ((size_t)2, unfinishedPath.size());
  ASSERT_EQ((uint64_t)2, unfinishedPath[0].first);
  ASSERT_EQ((uint64_t)100, unfinishedPath[0].second);

  ASSERT_EQ((uint64_t)3, unfinishedPath[1].first);
  ASSERT_EQ((uint64_t)101, unfinishedPath[1].second);

  const auto remainingPath = g.traverseGraph(unfinishedPath.back().first, unfinishedPath, false, false);
  ASSERT_EQ((size_t)1, remainingPath.size());
  ASSERT_EQ((uint64_t)4, remainingPath[0].first);
  ASSERT_EQ((uint64_t)102, remainingPath[0].second);

  unfinishedPath = g.getUnfinishedPath();
  ASSERT_EQ((size_t)0, unfinishedPath.size());
}

// ____________________________________________________________________________
TEST(GraphTest, buildPathSameEndpointsForwardFirsts) {
  Graph g;

  // forward
  g.addEdge(1, 2, 100, true);
  g.addEdge(2, 3, 101, false);
  g.addEdge(3, 5, 102, false);
  g.addEdge(5, 6, 103, true);

  // backward
  g.addEdge(6, 5, 103, true);
  g.addEdge(5, 4, 104, false);
  g.addEdge(4, 2, 105, false);
  g.addEdge(2, 1, 100, true);

  g.findStartpoints();
  auto startPoint = g.getStartpoint();
  ASSERT_EQ((uint64_t)1, startPoint);
  startPoint = g.getStartpoint();
  ASSERT_EQ((uint64_t)6, startPoint);
  startPoint = g.getStartpoint();
  ASSERT_EQ((uint64_t)0, startPoint);

  // forward
  auto resultF = g.traverseGraph(1, unfinishedPathEmpty, true, false);
  ASSERT_EQ((size_t)4, resultF.size());
  ASSERT_EQ((uint64_t)2, resultF[0].first);
  ASSERT_EQ((uint64_t)100, resultF[0].second);

  ASSERT_EQ((uint64_t)3, resultF[1].first);
  ASSERT_EQ((uint64_t)101, resultF[1].second);

  ASSERT_EQ((uint64_t)5, resultF[2].first);
  ASSERT_EQ((uint64_t)102, resultF[2].second);

  ASSERT_EQ((uint64_t)6, resultF[3].first);
  ASSERT_EQ((uint64_t)103, resultF[3].second);

  // backward
  auto resultB = g.traverseGraph(6, unfinishedPathEmpty, true, false);
  ASSERT_EQ((size_t)4, resultB.size());
  ASSERT_EQ((uint64_t)5, resultB[0].first);
  ASSERT_EQ((uint64_t)103, resultB[0].second);

  ASSERT_EQ((uint64_t)4, resultB[1].first);
  ASSERT_EQ((uint64_t)104, resultB[1].second);

  ASSERT_EQ((uint64_t)2, resultB[2].first);
  ASSERT_EQ((uint64_t)105, resultB[2].second);

  ASSERT_EQ((uint64_t)1, resultB[3].first);
  ASSERT_EQ((uint64_t)100, resultB[3].second);

  // All unfinished paths should already been built.
  auto unfinishedPath = g.getUnfinishedPath();
  ASSERT_EQ((size_t)3, unfinishedPath.size());
  auto remainingPath = g.traverseGraph(unfinishedPath.back().first, unfinishedPath, false, false);
  ASSERT_EQ((size_t)0, remainingPath.size());

  unfinishedPath = g.getUnfinishedPath();
  ASSERT_EQ((size_t)0, unfinishedPath.size());
}

// ____________________________________________________________________________
TEST(GraphTest, buildPathSameEndpointsBackwardFirsts) {
  Graph g;

  // forward
  g.addEdge(1, 2, 100, true);
  g.addEdge(2, 3, 101, false);
  g.addEdge(3, 5, 102, false);
  g.addEdge(5, 6, 103, true);

  // backward
  g.addEdge(6, 5, 103, true);
  g.addEdge(5, 4, 104, false);
  g.addEdge(4, 2, 105, false);
  g.addEdge(2, 1, 100, true);

  // backward
  auto resultB = g.traverseGraph(6, unfinishedPathEmpty, true, false);
  ASSERT_EQ((size_t)4, resultB.size());
  ASSERT_EQ((uint64_t)5, resultB[0].first);
  ASSERT_EQ((uint64_t)103, resultB[0].second);

  ASSERT_EQ((uint64_t)4, resultB[1].first);
  ASSERT_EQ((uint64_t)104, resultB[1].second);

  ASSERT_EQ((uint64_t)2, resultB[2].first);
  ASSERT_EQ((uint64_t)105, resultB[2].second);

  ASSERT_EQ((uint64_t)1, resultB[3].first);
  ASSERT_EQ((uint64_t)100, resultB[3].second);
  
  // forward
  auto resultF = g.traverseGraph(1, unfinishedPathEmpty, true, false);
  ASSERT_EQ((size_t)4, resultF.size());
  ASSERT_EQ((uint64_t)2, resultF[0].first);
  ASSERT_EQ((uint64_t)100, resultF[0].second);

  ASSERT_EQ((uint64_t)3, resultF[1].first);
  ASSERT_EQ((uint64_t)101, resultF[1].second);

  ASSERT_EQ((uint64_t)5, resultF[2].first);
  ASSERT_EQ((uint64_t)102, resultF[2].second);

  ASSERT_EQ((uint64_t)6, resultF[3].first);
  ASSERT_EQ((uint64_t)103, resultF[3].second);

  // All unfinished paths should already been built.
  auto unfinishedPath = g.getUnfinishedPath();
  ASSERT_EQ((size_t)3, unfinishedPath.size());
  auto remainingPath = g.traverseGraph(unfinishedPath.back().first, unfinishedPath, false, false);
  ASSERT_EQ((size_t)0, remainingPath.size());

  unfinishedPath = g.getUnfinishedPath();
  ASSERT_EQ((size_t)0, unfinishedPath.size());
}

// ____________________________________________________________________________
TEST(GraphTest, buildPathMultipleStartpoints) {
  Graph g;
  // forward
  g.addEdge(2, 3, 100, false);
  g.addEdge(3, 4, 101, true);
  g.addEdge(4, 5, 102, true);
  g.addEdge(5, 7, 103, false);

  // backward
  g.addEdge(6, 5, 104, false);
  g.addEdge(5, 4, 102, true);
  g.addEdge(4, 3, 101, true);
  g.addEdge(3, 1, 105, false);

  g.findStartpoints();
  auto startPoint = g.getStartpoint();
  ASSERT_EQ((uint64_t)2, startPoint);
  startPoint = g.getStartpoint();
  ASSERT_EQ((uint64_t)6, startPoint);
  startPoint = g.getStartpoint();
  ASSERT_EQ((uint64_t)0, startPoint);

  // forward
  auto resultF = g.traverseGraph(2, unfinishedPathEmpty, true, false);
  ASSERT_EQ((size_t)4, resultF.size());
  ASSERT_EQ((uint64_t)3, resultF[0].first);
  ASSERT_EQ((uint64_t)100, resultF[0].second);

  ASSERT_EQ((uint64_t)4, resultF[1].first);
  ASSERT_EQ((uint64_t)101, resultF[1].second);

  ASSERT_EQ((uint64_t)5, resultF[2].first);
  ASSERT_EQ((uint64_t)102, resultF[2].second);

  ASSERT_EQ((uint64_t)7, resultF[3].first);
  ASSERT_EQ((uint64_t)103, resultF[3].second);

  // backward
  auto resultB = g.traverseGraph(6, unfinishedPathEmpty, true, false);
  ASSERT_EQ((size_t)4, resultB.size());
  ASSERT_EQ((uint64_t)5, resultB[0].first);
  ASSERT_EQ((uint64_t)104, resultB[0].second);

  ASSERT_EQ((uint64_t)4, resultB[1].first);
  ASSERT_EQ((uint64_t)102, resultB[1].second);

  ASSERT_EQ((uint64_t)3, resultB[2].first);
  ASSERT_EQ((uint64_t)101, resultB[2].second);

  ASSERT_EQ((uint64_t)1, resultB[3].first);
  ASSERT_EQ((uint64_t)105, resultB[3].second);

  // All unfinished paths should already been built.
  auto unfinishedPath = g.getUnfinishedPath();
  ASSERT_EQ((size_t)1, unfinishedPath.size());
  auto remainingPath = g.traverseGraph(unfinishedPath.back().first, unfinishedPath, false, false);
  ASSERT_EQ((size_t)0, remainingPath.size());

  unfinishedPath = g.getUnfinishedPath();
  ASSERT_EQ((size_t)0, unfinishedPath.size());
}

// ____________________________________________________________________________
TEST(GraphTest, pathRiver) {
  Graph g;

  g.addEdge(1, 2, 100, false);
  g.addEdge(2, 3, 101, false);
  g.addEdge(3, 4, 102, false);
  g.addEdge(4, 6, 103, false);
  g.addEdge(6, 7, 104, false);
  g.addEdge(6, 8, 105, false);
  g.addEdge(2, 5, 106, false);
  g.addEdge(5, 4, 107, false);

  g.findStartpoints();
  auto startPoint = g.getStartpoint();
  ASSERT_EQ((uint64_t)1, startPoint);
  startPoint = g.getStartpoint();
  ASSERT_EQ((uint64_t)0, startPoint);

  auto result = g.traverseGraph(1, unfinishedPathEmpty, true, true);
  ASSERT_EQ((size_t)5, result.size());
  ASSERT_EQ((uint64_t)2, result[0].first);
  ASSERT_EQ((uint64_t)100, result[0].second);

  ASSERT_EQ((uint64_t)5, result[1].first);
  ASSERT_EQ((uint64_t)106, result[1].second);

  ASSERT_EQ((uint64_t)4, result[2].first);
  ASSERT_EQ((uint64_t)107, result[2].second);

  ASSERT_EQ((uint64_t)6, result[3].first);
  ASSERT_EQ((uint64_t)103, result[3].second);

  ASSERT_EQ((uint64_t)8, result[4].first);
  ASSERT_EQ((uint64_t)105, result[4].second);


  auto unfinishedPath = g.getUnfinishedPath();
  ASSERT_EQ((size_t)1, unfinishedPath.size());
  auto remainingPath = g.traverseGraph(unfinishedPath.back().first, unfinishedPath, false, true);
  ASSERT_EQ((size_t)3, remainingPath.size());
  ASSERT_EQ((uint64_t)3, remainingPath[0].first);
  ASSERT_EQ((uint64_t)101, remainingPath[0].second);

  ASSERT_EQ((uint64_t)4, remainingPath[1].first);
  ASSERT_EQ((uint64_t)102, remainingPath[1].second);

  ASSERT_EQ((uint64_t)6, remainingPath[2].first);
  ASSERT_EQ((uint64_t)103, remainingPath[2].second);


  unfinishedPath = g.getUnfinishedPath();
  ASSERT_EQ((size_t)1, unfinishedPath.size());
  remainingPath = g.traverseGraph(unfinishedPath.back().first, unfinishedPath, false, true);
  ASSERT_EQ((size_t)1, remainingPath.size());
  ASSERT_EQ((uint64_t)7, remainingPath[0].first);
  ASSERT_EQ((uint64_t)104, remainingPath[0].second);


  unfinishedPath = g.getUnfinishedPath();
  ASSERT_EQ((size_t)0, unfinishedPath.size());
}
