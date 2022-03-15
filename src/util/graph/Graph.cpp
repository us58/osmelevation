// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <iostream>
#include <vector>
#include <utility>
#include <set>
#include <unordered_map>
#include <cstdint>
#include "util/graph/Edge.h"
#include "util/graph/Graph.h"

using util::graph::Edge;
using util::graph::Graph;

// ____________________________________________________________________________
Graph::Graph(const uint64_t estimatedNodeCount) {
  _graph.reserve(estimatedNodeCount);
}

// ____________________________________________________________________________
std::unordered_map<uint64_t, std::vector<Edge>> Graph::getGraph() const {
  return _graph;
}

// ____________________________________________________________________________
uint64_t Graph::getStartpoint() {
  if (_startpoints.empty()) {
    return 0;
  }
  const uint64_t startPoint = *_startpoints.begin();
  _startpoints.erase(_startpoints.begin());
  return startPoint;
}

// ____________________________________________________________________________
std::vector<std::pair<uint64_t, uint64_t>> Graph::getUnfinishedPath() {
  if (_unfinishedPaths.empty()) {
    return std::vector<std::pair<uint64_t, uint64_t>>();
  }
  const auto unfinishedPath = *_unfinishedPaths.begin();
  _unfinishedPaths.pop_front();
  return unfinishedPath;
}

// ____________________________________________________________________________
void Graph::addEdge(const uint64_t v1, const uint64_t v2,
                    const uint64_t id, const bool oppositeExists) {
  auto edgeIt = _graph.try_emplace(v1, std::vector<Edge>());

  edgeIt.first->second.emplace_back(id, v2, oppositeExists);

  // Keep track of the in-degrees of the nodes.

  _inDegree.try_emplace(v1, 0);
  auto endpoint = _inDegree.emplace(v2, 0).first;
  endpoint->second++;
}

// ____________________________________________________________________________
void Graph::findStartpoints() {
  _startpoints.clear();
  for (auto edgeIt = _inDegree.begin(); edgeIt != _inDegree.end(); ) {
    if (edgeIt->second == 0) {
      _startpoints.emplace(edgeIt->first);
    } else if (edgeIt->second == 1) {
      // If the node has in-degree and out-degree of 1,
      // and the outgoing/ingoing edge are opposites, it's also a startpoint.
      if (_graph.contains(edgeIt->first) &&
          _graph.find(edgeIt->first)->second.size() == 1 &&
          _graph.find(edgeIt->first)->second.front().reversedEdgeExists) {
        _startpoints.emplace(edgeIt->first);
      }
    }
    edgeIt++;
  }
  _inDegree.clear();
}

// ____________________________________________________________________________
bool Graph::removeStartNode(const uint64_t node) {
  const bool existed = _startpoints.erase(node);
  return existed;
}

// ____________________________________________________________________________
std::vector<std::pair<uint64_t, uint64_t>> Graph::traverseGraph(
    const uint64_t startNode,
    const std::vector<std::pair<uint64_t, uint64_t>>& unfinishedPath,
    const bool fromStartPoint, const bool river) {
  std::vector<std::pair<uint64_t, uint64_t>> path;
  std::set<uint64_t> visitedNodes;
  visitedNodes.emplace(startNode);

  // The nodes used in the unfinished path can not be used again.
  for (const auto& edge : unfinishedPath) {
    visitedNodes.emplace(edge.first);
  }

  // Denote if the constructed path has not been seen before
  // (at least one node has not been seen before).
  bool uniquePath = false;

  // Count the number of consecutive already visited edges.
  uint64_t consecutiveKnown = 0;

  // Traverse nodes.
  for (auto nodeIt = _graph.find(startNode); nodeIt != _graph.end(); ) {
    // Refer to an edge that was used in a prior
    // traversal (i.e. is present in _usedEdges),
    // or was not used before respectively.
    Edge* nextEdgeUsed = nullptr;
    Edge* nextEdgeUnused = nullptr;

    // Prefer edges marked with oppositeExists=true, i.e.
    // the edge is present in both directions.
    bool foundPreferredEdgeUsed = false;
    bool foundPreferredEdgeUnused = false;

    // Counter for the number of unused outgoing edges of the node.
    uint8_t availableBranches = 0;

    // Look at all outgoing edges of the node.
    for (auto edgeIt = nodeIt->second.begin();
         edgeIt != nodeIt->second.end(); ) {
      // Cycle if the endpoint was already visited.
      if (!visitedNodes.contains(edgeIt->pointsTo)) {
        const bool preferredEdge = edgeIt->reversedEdgeExists;
        const bool unused = !_usedEdges.contains(edgeIt->edgeId);

        // Keep track of the number of outgoing unused edges.
        if (unused) { ++availableBranches; }

        if (!foundPreferredEdgeUnused && unused) {
          nextEdgeUnused = &*edgeIt;
          if (preferredEdge) { foundPreferredEdgeUnused = true; }
        }
        if (!foundPreferredEdgeUsed && !unused) {
          nextEdgeUsed = &*edgeIt;
          if (preferredEdge) { foundPreferredEdgeUsed = true; }
        }
      }
      ++edgeIt;
    }
    // Prefer an unused edge over an used.
    auto nextEdge = (nextEdgeUnused) ? nextEdgeUnused : nextEdgeUsed;

    // If traversing from a startpoint of the graph, always take an edge
    // that is marked with oppositeExists=true. Prefer an used edge
    // over an unused in this case.
    if (fromStartPoint && foundPreferredEdgeUsed) {
        nextEdge = nextEdgeUsed;
    }

    // If no edge with an unvisited endpoint was found, terminate traversal.
    if (!nextEdge) {
      nodeIt = _graph.end();
    } else {
      if (_usedEdges.emplace(nextEdge->edgeId).second) {
        uniquePath = true;
      } else {
        ++consecutiveKnown;
      }
      // If there were at least two available branches,
      // add an unfinished path consisting of the current path
      // until this node.
      if (availableBranches >= 2) {
        // Reset the counter, unvisited edge found.
        addUnfinishedPath(path, river);
      }
      // Continue with traversal.
      path.emplace_back(nextEdge->pointsTo, nextEdge->edgeId);
      nodeIt = _graph.find(nextEdge->pointsTo);
      visitedNodes.emplace(nextEdge->pointsTo);
    }

    // Stop if river and the last edge has already been seen.
    if (river && consecutiveKnown > 0) {
      nodeIt = _graph.end();
    }
  }
  return (uniquePath) ? path : std::vector<std::pair<uint64_t, uint64_t>>();
}

// ____________________________________________________________________________
void Graph::addUnfinishedPath(
    const std::vector<std::pair<uint64_t, uint64_t>>& path, const bool river) {
  std::vector<std::pair<uint64_t, uint64_t>> unfinishedPath;

  if (river && path.size() > 0) {
    unfinishedPath.emplace_back(path.back());
  } else {
    unfinishedPath.insert(unfinishedPath.end(), path.begin(), path.end());
  }
  _unfinishedPaths.emplace_back(unfinishedPath);
}
