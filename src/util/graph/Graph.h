// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#ifndef SRC_UTIL_GRAPH_GRAPH_H_
#define SRC_UTIL_GRAPH_GRAPH_H_

#include <vector>
#include <set>
#include <list>
#include <utility>
#include <unordered_map>
#include <cstdint>
#include "util/graph/Edge.h"

namespace util {
namespace graph {

using util::graph::Edge;

/*
 * Graph that holds directed edges in an adjacency list where
 * nodes point to outgoing egdes. A node is represented by an unique id.
 */ 
class Graph {
 public:
  explicit Graph(const uint64_t estimatedNodeCount);

  // Default ctor.
  Graph() {}

  // Get the graph given as an adjacency list where nodes
  // point to their outgoing edges.
  std::unordered_map<uint64_t, std::vector<Edge>> getGraph() const;

  // Get a startpoint if existing, i.e. a node with in-degree 0;
  // otherwise return 0.
  uint64_t getStartpoint();

  // Get an unfinished path that was added in a traversal of the graph.
  // If none exists, return an empty path.
  std::vector<std::pair<uint64_t, uint64_t>> getUnfinishedPath();

  // Add an edge, defined by start-node and end-node.
  // Also keep track of the in-degrees of the edges' start-nodes.
  void addEdge(const uint64_t v1, const uint64_t v2,
               const uint64_t id, const bool oppositeExists);

  // Find all nodes which have in-degree 0.
  void findStartpoints();

  // Check if a node is a start node, i.e. the node has in-degree 0.
  bool removeStartNode(const uint64_t node);

  // Traverse the built graph starting from the provided start-node,
  // until a node with out-degree 0 is found.
  // Once a node has been visited, it cannot be visited again.
  // A special rule applies when there are multiple choices for a next edge:
  // Prefer an edge that is marked as oppositeExists=true.
  std::vector<std::pair<uint64_t, uint64_t>> traverseGraph(
      const uint64_t startNode,
      const std::vector<std::pair<uint64_t, uint64_t>>& unfinishedPath,
      const bool fromStartPoint, const bool river);

  // Add a complete path or only the last node of the path to
  // the _unfinishedPaths list. An unused edge is reachable
  // from the last node of an unfinished path.
  void addUnfinishedPath(
      const std::vector<std::pair<uint64_t, uint64_t>>& path,
      const bool river);

 private:
  // The graph saved as an adjacency list where nodes point to adjacent edges.
  std::unordered_map<uint64_t, std::vector<Edge>> _graph;

  // The in-degree of each node.
  std::unordered_map<uint64_t, uint8_t> _inDegree;

  // The set of startpoints that have been found by findStartpoints().
  std::set<uint64_t> _startpoints;

  // All edges (given by id) that have been used so far.
  std::set<uint64_t> _usedEdges;

  // Paths where an unused edge is reachable from the last node of the path.
  std::list<std::vector<std::pair<uint64_t, uint64_t>>> _unfinishedPaths;
};

}  // namespace graph
}  // namespace util

#endif  // SRC_UTIL_GRAPH_GRAPH_H_
