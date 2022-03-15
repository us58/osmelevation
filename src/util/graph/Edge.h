// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#ifndef SRC_UTIL_GRAPH_EDGE_H_
#define SRC_UTIL_GRAPH_EDGE_H_

#include <cstdint>

namespace util {
namespace graph {

/*
 * Store a directed edge of a graph as the endpoint of the edge,
 * the unique id of the edge and store if there is another edge
 * with the same endpoints but points in the opposite direction. 
 */
struct Edge {
  Edge(const uint64_t edgeId, const uint64_t pointsTo,
       bool reversedEdgeExists) :
       edgeId(edgeId), pointsTo(pointsTo),
       reversedEdgeExists(reversedEdgeExists) {}

  // The unique id of edge.
  uint64_t edgeId;

  // The id of the node the edge points to.
  uint64_t pointsTo;

  // True if there exists an edge that has the same endpoints,
  // but the direction is the oppsite.
  bool reversedEdgeExists;
};

}  // namespace graph
}  // namespace util

#endif  // SRC_UTIL_GRAPH_EDGE_H_
