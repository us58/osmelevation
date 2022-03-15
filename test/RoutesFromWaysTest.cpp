// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <set>
#include <vector>
#include <cstdint>
#include <gtest/gtest.h>
#include "parser/NodeWayRelationParser.h"
#include "util/osm/OsmStats.h"
#include "util/osm/GetOsmStats.h"
#include "correctosmelevation/osm/RoutesFromWays.h"

using util::osm::OsmStats;
using util::osm::GetOsmStats;
using parser::NodeWayRelationParser;
using correctosmelevation::osm::RoutesFromWays;

// ____________________________________________________________________________
TEST(ROUTESFROMWAYSTEST, allRoutes) {
  GetOsmStats handler;
  NodeWayRelationParser statsParser("./testMap.osm", &handler);
  statsParser.parse();
  OsmStats osmStats = handler.getOsmStats();

  // Manually add the ways already contained in routes.
  std::set<uint64_t> correctedWayIds;
  correctedWayIds.emplace(1);
  correctedWayIds.emplace(2);
  correctedWayIds.emplace(3);
  correctedWayIds.emplace(4);
  correctedWayIds.emplace(5);
  correctedWayIds.emplace(20);
  correctedWayIds.emplace(21);
  correctedWayIds.emplace(22);
  correctedWayIds.emplace(23);
  correctedWayIds.emplace(24);
  correctedWayIds.emplace(25);
  correctedWayIds.emplace(28);
  correctedWayIds.emplace(29);
  correctedWayIds.emplace(30);
  correctedWayIds.emplace(31);
  correctedWayIds.emplace(32);
  correctedWayIds.emplace(33);
  correctedWayIds.emplace(34);
  correctedWayIds.emplace(35);
  correctedWayIds.emplace(36);
  correctedWayIds.emplace(37);
  correctedWayIds.emplace(38);
  correctedWayIds.emplace(39);
  correctedWayIds.emplace(43);
  correctedWayIds.emplace(44);
  correctedWayIds.emplace(45);
  correctedWayIds.emplace(46);
  correctedWayIds.emplace(47);
  correctedWayIds.emplace(48);
  correctedWayIds.emplace(49);
  correctedWayIds.emplace(50);
  correctedWayIds.emplace(54);
  correctedWayIds.emplace(55);
  correctedWayIds.emplace(56);

  RoutesFromWays routesFromWays("./testMap.osm", osmStats, "ele", 0, 100);
  const auto routes = routesFromWays.getRoutesAndExcludeIds(correctedWayIds);

  ASSERT_EQ((size_t)9, routes.size());

  ASSERT_EQ((size_t)1, routes[0].size());
  ASSERT_EQ((uint64_t)3, routes[0][0].size());
  ASSERT_EQ((uint64_t)47, routes[0][0][0]);
  ASSERT_EQ((uint64_t)48, routes[0][0][1]);
  ASSERT_EQ((uint64_t)49, routes[0][0][2]);

  ASSERT_EQ((size_t)1, routes[1].size());
  ASSERT_EQ((uint64_t)5, routes[1][0].size());
  ASSERT_EQ((uint64_t)49, routes[1][0][0]);
  ASSERT_EQ((uint64_t)50, routes[1][0][1]);
  ASSERT_EQ((uint64_t)51, routes[1][0][2]);
  ASSERT_EQ((uint64_t)52, routes[1][0][3]);
  ASSERT_EQ((uint64_t)53, routes[1][0][4]);

  ASSERT_EQ((size_t)1, routes[2].size());
  ASSERT_EQ((uint64_t)3, routes[2][0].size());
  ASSERT_EQ((uint64_t)53, routes[2][0][0]);
  ASSERT_EQ((uint64_t)54, routes[2][0][1]);
  ASSERT_EQ((uint64_t)55, routes[2][0][2]);

  ASSERT_EQ((size_t)1, routes[3].size());
  ASSERT_EQ((uint64_t)3, routes[3][0].size());
  ASSERT_EQ((uint64_t)74, routes[3][0][0]);
  ASSERT_EQ((uint64_t)78, routes[3][0][1]);
  ASSERT_EQ((uint64_t)79, routes[3][0][2]);

  ASSERT_EQ((size_t)1, routes[4].size());
  ASSERT_EQ((uint64_t)9, routes[4][0].size());
  ASSERT_EQ((uint64_t)80, routes[4][0][0]);
  ASSERT_EQ((uint64_t)81, routes[4][0][1]);
  ASSERT_EQ((uint64_t)82, routes[4][0][2]);
  ASSERT_EQ((uint64_t)83, routes[4][0][3]);
  ASSERT_EQ((uint64_t)84, routes[4][0][4]);
  ASSERT_EQ((uint64_t)85, routes[4][0][5]);
  ASSERT_EQ((uint64_t)86, routes[4][0][6]);
  ASSERT_EQ((uint64_t)87, routes[4][0][7]);
  ASSERT_EQ((uint64_t)80, routes[4][0][8]);

  ASSERT_EQ((size_t)1, routes[5].size());
  ASSERT_EQ((uint64_t)9, routes[5][0].size());
  ASSERT_EQ((uint64_t)112, routes[5][0][0]);
  ASSERT_EQ((uint64_t)113, routes[5][0][1]);
  ASSERT_EQ((uint64_t)114, routes[5][0][2]);
  ASSERT_EQ((uint64_t)115, routes[5][0][3]);
  ASSERT_EQ((uint64_t)116, routes[5][0][4]);
  ASSERT_EQ((uint64_t)117, routes[5][0][5]);
  ASSERT_EQ((uint64_t)118, routes[5][0][6]);
  ASSERT_EQ((uint64_t)119, routes[5][0][7]);
  ASSERT_EQ((uint64_t)120, routes[5][0][8]);

  ASSERT_EQ((size_t)1, routes[6].size());
  ASSERT_EQ((uint64_t)4, routes[6][0].size());
  ASSERT_EQ((uint64_t)158, routes[6][0][0]);
  ASSERT_EQ((uint64_t)159, routes[6][0][1]);
  ASSERT_EQ((uint64_t)160, routes[6][0][2]);
  ASSERT_EQ((uint64_t)161, routes[6][0][3]);

  ASSERT_EQ((size_t)1, routes[7].size());
  ASSERT_EQ((uint64_t)5, routes[7][0].size());
  ASSERT_EQ((uint64_t)161, routes[7][0][0]);
  ASSERT_EQ((uint64_t)162, routes[7][0][1]);
  ASSERT_EQ((uint64_t)163, routes[7][0][2]);
  ASSERT_EQ((uint64_t)164, routes[7][0][3]);
  ASSERT_EQ((uint64_t)165, routes[7][0][4]);

  ASSERT_EQ((size_t)1, routes[8].size());
  ASSERT_EQ((uint64_t)5, routes[8][0].size());
  ASSERT_EQ((uint64_t)165, routes[8][0][0]);
  ASSERT_EQ((uint64_t)166, routes[8][0][1]);
  ASSERT_EQ((uint64_t)167, routes[8][0][2]);
  ASSERT_EQ((uint64_t)168, routes[8][0][3]);
  ASSERT_EQ((uint64_t)169, routes[8][0][4]);
}

// ____________________________________________________________________________
TEST(ROUTESFROMWAYSTEST, routesRangeTest) {
  GetOsmStats handler;
  NodeWayRelationParser statsParser("./testMap.osm", &handler);
  statsParser.parse();
  OsmStats osmStats = handler.getOsmStats();

  // Manually add the ways already contained in routes.
  std::set<uint64_t> correctedWayIds;
  correctedWayIds.emplace(1);
  correctedWayIds.emplace(2);
  correctedWayIds.emplace(3);
  correctedWayIds.emplace(4);
  correctedWayIds.emplace(5);
  correctedWayIds.emplace(20);
  correctedWayIds.emplace(21);
  correctedWayIds.emplace(22);
  correctedWayIds.emplace(23);
  correctedWayIds.emplace(24);
  correctedWayIds.emplace(25);
  correctedWayIds.emplace(28);
  correctedWayIds.emplace(29);
  correctedWayIds.emplace(30);
  correctedWayIds.emplace(31);
  correctedWayIds.emplace(32);
  correctedWayIds.emplace(33);
  correctedWayIds.emplace(34);
  correctedWayIds.emplace(35);
  correctedWayIds.emplace(36);
  correctedWayIds.emplace(37);
  correctedWayIds.emplace(38);
  correctedWayIds.emplace(39);
  correctedWayIds.emplace(43);
  correctedWayIds.emplace(44);
  correctedWayIds.emplace(45);
  correctedWayIds.emplace(46);
  correctedWayIds.emplace(47);
  correctedWayIds.emplace(48);
  correctedWayIds.emplace(49);
  correctedWayIds.emplace(50);
  correctedWayIds.emplace(54);
  correctedWayIds.emplace(55);
  correctedWayIds.emplace(56);

  // 7 ways should be found in total.
  RoutesFromWays routesFromWays1("./testMap.osm", osmStats, "ele", 0, 10);
  const auto routes1 = routesFromWays1.getRoutesAndExcludeIds(correctedWayIds);
  ASSERT_EQ((size_t)9, routes1.size());

  // Test with Range of size 1
  RoutesFromWays routesFromWays2("./testMap.osm", osmStats, "ele", 0, 1);
  const auto routes2 = routesFromWays2.getRoutesAndExcludeIds(correctedWayIds);
  ASSERT_EQ((size_t)1, routes2.size());

  RoutesFromWays routesFromWays3("./testMap.osm", osmStats, "ele", 1, 2);
  const auto routes3 = routesFromWays3.getRoutesAndExcludeIds(correctedWayIds);
  ASSERT_EQ((size_t)1, routes3.size());

  RoutesFromWays routesFromWays4("./testMap.osm", osmStats, "ele", 2, 3);
  const auto routes4 = routesFromWays4.getRoutesAndExcludeIds(correctedWayIds);
  ASSERT_EQ((size_t)1, routes4.size());

  RoutesFromWays routesFromWays5("./testMap.osm", osmStats, "ele", 3, 4);
  const auto routes5 = routesFromWays5.getRoutesAndExcludeIds(correctedWayIds);
  ASSERT_EQ((size_t)1, routes5.size());

  RoutesFromWays routesFromWays6("./testMap.osm", osmStats, "ele", 4, 5);
  const auto routes6 = routesFromWays6.getRoutesAndExcludeIds(correctedWayIds);
  ASSERT_EQ((size_t)1, routes6.size());

  RoutesFromWays routesFromWays7("./testMap.osm", osmStats, "ele", 5, 6);
  const auto routes7 = routesFromWays7.getRoutesAndExcludeIds(correctedWayIds);
  ASSERT_EQ((size_t)1, routes7.size());

  RoutesFromWays routesFromWays8("./testMap.osm", osmStats, "ele", 6, 7);
  const auto routes8 = routesFromWays8.getRoutesAndExcludeIds(correctedWayIds);
  ASSERT_EQ((size_t)1, routes8.size());

  RoutesFromWays routesFromWays9("./testMap.osm", osmStats, "ele", 7, 8);
  const auto routes9 = routesFromWays9.getRoutesAndExcludeIds(correctedWayIds);
  ASSERT_EQ((size_t)1, routes9.size());

  RoutesFromWays routesFromWays10("./testMap.osm", osmStats, "ele", 8, 9);
  const auto routes10 = routesFromWays10.getRoutesAndExcludeIds(correctedWayIds);
  ASSERT_EQ((size_t)1, routes10.size());

  RoutesFromWays routesFromWays11("./testMap.osm", osmStats, "ele", 9, 10);
  const auto routes11 = routesFromWays11.getRoutesAndExcludeIds(correctedWayIds);
  ASSERT_EQ((size_t)0, routes11.size());


  
  // Test with Range of size 3
  RoutesFromWays routesFromWays12("./testMap.osm", osmStats, "ele", 0, 3);
  const auto routes12 = routesFromWays12.getRoutesAndExcludeIds(correctedWayIds);
  ASSERT_EQ((size_t)3, routes12.size());

  RoutesFromWays routesFromWays13("./testMap.osm", osmStats, "ele", 3, 6);
  const auto routes13 = routesFromWays13.getRoutesAndExcludeIds(correctedWayIds);
  ASSERT_EQ((size_t)3, routes13.size());

  RoutesFromWays routesFromWays14("./testMap.osm", osmStats, "ele", 6, 9);
  const auto routes14 = routesFromWays14.getRoutesAndExcludeIds(correctedWayIds);
  ASSERT_EQ((size_t)3, routes14.size());

  RoutesFromWays routesFromWays15("./testMap.osm", osmStats, "ele", 9, 12);
  const auto routes15 = routesFromWays15.getRoutesAndExcludeIds(correctedWayIds);
  ASSERT_EQ((size_t)0, routes15.size());


  // Test with Range of size 4
  RoutesFromWays routesFromWays16("./testMap.osm", osmStats, "ele", 0, 4);
  const auto routes16 = routesFromWays16.getRoutesAndExcludeIds(correctedWayIds);
  ASSERT_EQ((size_t)4, routes16.size());

  RoutesFromWays routesFromWays17("./testMap.osm", osmStats, "ele", 4, 8);
  const auto routes17 = routesFromWays17.getRoutesAndExcludeIds(correctedWayIds);
  ASSERT_EQ((size_t)4, routes17.size());

  RoutesFromWays routesFromWays18("./testMap.osm", osmStats, "ele", 8, 12);
  const auto routes18 = routesFromWays18.getRoutesAndExcludeIds(correctedWayIds);
  ASSERT_EQ((size_t)1, routes18.size());

  RoutesFromWays routesFromWays19("./testMap.osm", osmStats, "ele", 12, 16);
  const auto routes19 = routesFromWays19.getRoutesAndExcludeIds(correctedWayIds);
  ASSERT_EQ((size_t)0, routes19.size());
}
