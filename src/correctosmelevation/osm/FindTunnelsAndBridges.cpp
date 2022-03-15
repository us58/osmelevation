// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <cstdint>
#include <vector>
#include <utility>
#include <osmium/relations/members_database.hpp>
#include <osmium/osm/way.hpp>
#include "correctosmelevation/osm/FindTunnelsAndBridges.h"

using correctosmelevation::osm::FindTunnelsAndBridges;
using RoutePaths = std::vector<std::vector<uint64_t>>;

// ____________________________________________________________________________
FindTunnelsAndBridges::FindTunnelsAndBridges(
    const MembersDatabase<osmium::Way>& wayDatabase,
    const std::vector<std::pair<uint64_t, uint64_t>>& pathWays) :
    _wayDatabase(wayDatabase), _pathWays(pathWays) {}

// ____________________________________________________________________________
std::vector<RoutePaths> FindTunnelsAndBridges::getTunnelsAndBridges() {
  // Start at the second way because there must be at least one way
  // in front of a tunnel or bridge.
  size_t current = 1;
  while (current < _pathWays.size()) {
    const auto& tags = _wayDatabase.get(_pathWays[current].second)->tags();
    const bool tunnelOrBridge = tags.has_key("tunnel") ||
                                tags.has_key("bridge");
    if (tunnelOrBridge) {
      current = newTunnelOrBridge(current);
    }
    ++current;
  }
  return std::move(_tunnelsAndBridges);
}

// ____________________________________________________________________________
size_t FindTunnelsAndBridges::newTunnelOrBridge(const size_t start) {
  _tunnelsAndBridges.emplace_back(RoutePaths());
  _tunnelsAndBridges.back().resize(3);

  bool beforeEmbankmentOrIncline = true;
  bool afterEmbankmentOrIncline = true;

  // Add the way in front of the tunnel.
  addWay(true, start - 1);
  beforeEmbankmentOrIncline = embankmentOrIncline(start - 1);

  // The position of the start of the tunnel/bridge
  // from the start of _pathWays.
  size_t currentWayPos = start;

  // The position of the current way from the start of this tunnel/bridge.
  size_t posInTunnelOrBridge = 0;
  while (currentWayPos < _pathWays.size()) {
    const auto& tags =
        _wayDatabase.get(_pathWays[currentWayPos].second)->tags();

    const bool tunnelOrBridge = tags.has_key("tunnel") ||
                                tags.has_key("bridge");
    if (!tunnelOrBridge) { break; }
    addTunnelOrBridgeWay(posInTunnelOrBridge, currentWayPos);
    ++posInTunnelOrBridge;
    ++currentWayPos;
  }
  // Tunnel or bridge not complete (the way after is missing).
  if (currentWayPos == _pathWays.size()) {
    _tunnelsAndBridges.pop_back();
  } else {
    // Add the way directly after the tunnel.
    addWay(false, currentWayPos);
    afterEmbankmentOrIncline = embankmentOrIncline(currentWayPos);
    // Not valid if before or after is incline or embankment.
    if (beforeEmbankmentOrIncline || afterEmbankmentOrIncline) {
      _tunnelsAndBridges.pop_back();
    }
  }
  return currentWayPos;
}

// ____________________________________________________________________________
bool FindTunnelsAndBridges::embankmentOrIncline(const size_t way) const {
  const auto& tags = _wayDatabase.get(_pathWays[way].second)->tags();
  const bool embankment = tags.has_key("embankment");
  const bool incline = tags.has_key("incline");
  return embankment || incline;
}

// ____________________________________________________________________________
void FindTunnelsAndBridges::addTunnelOrBridgeWay(
    const size_t posInTunnelOrBridge, const size_t wayPos) {
  const auto& nodes = _wayDatabase.get(_pathWays[wayPos].second)->nodes();

  // Add the start-node if first way.
  if (posInTunnelOrBridge == 0) {
    if (_pathWays[wayPos].first == nodes.front()) {
      _tunnelsAndBridges.back()[1].emplace_back(nodes.back().ref());
    } else {
      _tunnelsAndBridges.back()[1].emplace_back(nodes.front().ref());
    }
  }
  // Check if the way is needed forward or backward.
  if (_pathWays[wayPos].first == nodes.front()) {
    for (auto rit = nodes.crbegin() + 1; rit != nodes.crend(); ++rit) {
      _tunnelsAndBridges.back()[1].emplace_back(rit->ref());
    }
  } else {
    for (auto rit = nodes.cbegin() + 1; rit != nodes.cend(); ++rit) {
      _tunnelsAndBridges.back()[1].emplace_back(rit->ref());
    }
  }
}

// ____________________________________________________________________________
void FindTunnelsAndBridges::addWay(const bool before, const size_t wayPos) {
  const auto& nodes = _wayDatabase.get(_pathWays[wayPos].second)->nodes();

  // The way in front of and after should point towards the tunnel/bridge.
  if (_pathWays[wayPos].first == nodes.front()) {
    if (before) {
      for (auto rit = nodes.cbegin(); rit != nodes.cend(); ++rit) {
        _tunnelsAndBridges.back()[0].emplace_back(rit->ref());
      }
    } else {
      for (auto rit = nodes.crbegin(); rit != nodes.crend(); ++rit) {
        _tunnelsAndBridges.back()[2].emplace_back(rit->ref());
      }
    }
  } else {
    if (before) {
      for (auto rit = nodes.crbegin(); rit != nodes.crend(); ++rit) {
        _tunnelsAndBridges.back()[0].emplace_back(rit->ref());
      }
    } else {
      for (auto rit = nodes.cbegin(); rit != nodes.cend(); ++rit) {
        _tunnelsAndBridges.back()[2].emplace_back(rit->ref());
      }
    }
  }
}
