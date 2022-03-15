// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <set>
#include <vector>
#include <cstdint>
#include <gtest/gtest.h>
#include "parser/NodeWayRelationParser.h"
#include "util/osm/OsmStats.h"
#include "util/osm/GetOsmStats.h"
#include "correctosmelevation/osm/RoutesFromRelations.h"

using util::osm::OsmStats;
using util::osm::GetOsmStats;
using parser::NodeWayRelationParser;
using correctosmelevation::osm::RoutesFromRelations;
using RoutePaths = std::vector<std::vector<uint64_t>>;

// ____________________________________________________________________________
TEST(ROUTESFROMRELATIONSTEST, allRoutes) {
  GetOsmStats handler;
  NodeWayRelationParser statsParser("./testMap.osm", &handler);
  statsParser.parse();
  OsmStats osmStats = handler.getOsmStats();

  RoutesFromRelations routesFromRelations("./testMap.osm", osmStats,
                                          "ele", 0, 100);

  std::set<uint64_t> correctedWayIds;
  std::vector<RoutePaths> routes;
  std::vector<RoutePaths> rivers;
  std::vector<RoutePaths> tunnelsAndBridges;

  routesFromRelations.getRoutesAndIds(correctedWayIds, routes, rivers,
                                      tunnelsAndBridges);

  // 2 tunnels and 1 bridge
  ASSERT_EQ((size_t)3, tunnelsAndBridges.size());
  ASSERT_EQ((size_t)3, tunnelsAndBridges[0].size());

  // The way before the tunnel.
  ASSERT_EQ((size_t)6, tunnelsAndBridges[0][0].size());
  ASSERT_EQ((uint64_t)10, tunnelsAndBridges[0][0][0]);
  ASSERT_EQ((uint64_t)4, tunnelsAndBridges[0][0][1]);
  ASSERT_EQ((uint64_t)3, tunnelsAndBridges[0][0][2]);
  ASSERT_EQ((uint64_t)2, tunnelsAndBridges[0][0][3]);
  ASSERT_EQ((uint64_t)12, tunnelsAndBridges[0][0][4]);
  ASSERT_EQ((uint64_t)1, tunnelsAndBridges[0][0][5]);

  // The tunnel itself.
  ASSERT_EQ((size_t)3, tunnelsAndBridges[0][1].size());
  ASSERT_EQ((uint64_t)10, tunnelsAndBridges[0][1][0]);
  ASSERT_EQ((uint64_t)9, tunnelsAndBridges[0][1][1]);
  ASSERT_EQ((uint64_t)11, tunnelsAndBridges[0][1][2]);

  // The way after the tunnel.
  ASSERT_EQ((size_t)5, tunnelsAndBridges[0][2].size());
  ASSERT_EQ((uint64_t)11, tunnelsAndBridges[0][2][0]);
  ASSERT_EQ((uint64_t)5, tunnelsAndBridges[0][2][1]);
  ASSERT_EQ((uint64_t)6, tunnelsAndBridges[0][2][2]);
  ASSERT_EQ((uint64_t)7, tunnelsAndBridges[0][2][3]);
  ASSERT_EQ((uint64_t)8, tunnelsAndBridges[0][2][4]);

  // The way before the tunnel.
  ASSERT_EQ((size_t)4, tunnelsAndBridges[1][0].size());
  ASSERT_EQ((uint64_t)132, tunnelsAndBridges[1][0][0]);
  ASSERT_EQ((uint64_t)131, tunnelsAndBridges[1][0][1]);
  ASSERT_EQ((uint64_t)130, tunnelsAndBridges[1][0][2]);
  ASSERT_EQ((uint64_t)129, tunnelsAndBridges[1][0][3]);

  // The tunnel itself.
  ASSERT_EQ((size_t)11, tunnelsAndBridges[1][1].size());
  ASSERT_EQ((uint64_t)132, tunnelsAndBridges[1][1][0]);
  ASSERT_EQ((uint64_t)133, tunnelsAndBridges[1][1][1]);
  ASSERT_EQ((uint64_t)134, tunnelsAndBridges[1][1][2]);
  ASSERT_EQ((uint64_t)135, tunnelsAndBridges[1][1][3]);
  ASSERT_EQ((uint64_t)136, tunnelsAndBridges[1][1][4]);
  ASSERT_EQ((uint64_t)137, tunnelsAndBridges[1][1][5]);
  ASSERT_EQ((uint64_t)138, tunnelsAndBridges[1][1][6]);
  ASSERT_EQ((uint64_t)139, tunnelsAndBridges[1][1][7]);
  ASSERT_EQ((uint64_t)140, tunnelsAndBridges[1][1][8]);
  ASSERT_EQ((uint64_t)141, tunnelsAndBridges[1][1][9]);
  ASSERT_EQ((uint64_t)142, tunnelsAndBridges[1][1][10]);

  // The way after the tunnel.
  ASSERT_EQ((size_t)4, tunnelsAndBridges[1][2].size());
  ASSERT_EQ((uint64_t)142, tunnelsAndBridges[1][2][0]);
  ASSERT_EQ((uint64_t)143, tunnelsAndBridges[1][2][1]);
  ASSERT_EQ((uint64_t)144, tunnelsAndBridges[1][2][2]);
  ASSERT_EQ((uint64_t)145, tunnelsAndBridges[1][2][3]);

  // The way before the bridge.
  ASSERT_EQ((size_t)4, tunnelsAndBridges[2][0].size());
  ASSERT_EQ((uint64_t)145, tunnelsAndBridges[2][0][0]);
  ASSERT_EQ((uint64_t)144, tunnelsAndBridges[2][0][1]);
  ASSERT_EQ((uint64_t)143, tunnelsAndBridges[2][0][2]);
  ASSERT_EQ((uint64_t)142, tunnelsAndBridges[2][0][3]);

  // The bridge itself.
  ASSERT_EQ((size_t)9, tunnelsAndBridges[2][1].size());
  ASSERT_EQ((uint64_t)145, tunnelsAndBridges[2][1][0]);
  ASSERT_EQ((uint64_t)146, tunnelsAndBridges[2][1][1]);
  ASSERT_EQ((uint64_t)147, tunnelsAndBridges[2][1][2]);
  ASSERT_EQ((uint64_t)148, tunnelsAndBridges[2][1][3]);
  ASSERT_EQ((uint64_t)149, tunnelsAndBridges[2][1][4]);
  ASSERT_EQ((uint64_t)150, tunnelsAndBridges[2][1][5]);
  ASSERT_EQ((uint64_t)151, tunnelsAndBridges[2][1][6]);
  ASSERT_EQ((uint64_t)152, tunnelsAndBridges[2][1][7]);
  ASSERT_EQ((uint64_t)153, tunnelsAndBridges[2][1][8]);

  // The way after the bridge.
  ASSERT_EQ((size_t)5, tunnelsAndBridges[2][2].size());
  ASSERT_EQ((uint64_t)153, tunnelsAndBridges[2][2][0]);
  ASSERT_EQ((uint64_t)154, tunnelsAndBridges[2][2][1]);
  ASSERT_EQ((uint64_t)155, tunnelsAndBridges[2][2][2]);
  ASSERT_EQ((uint64_t)156, tunnelsAndBridges[2][2][3]);
  ASSERT_EQ((uint64_t)157, tunnelsAndBridges[2][2][4]);


  // Check routes.
  ASSERT_EQ((size_t)7, routes.size());

  ASSERT_EQ((size_t)1, routes[0].size());
  ASSERT_EQ((size_t)18, routes[0][0].size());
  ASSERT_EQ((uint64_t)16, routes[0][0][0]);
  ASSERT_EQ((uint64_t)17, routes[0][0][1]);
  ASSERT_EQ((uint64_t)18, routes[0][0][2]);
  ASSERT_EQ((uint64_t)1, routes[0][0][3]);
  ASSERT_EQ((uint64_t)12, routes[0][0][4]);
  ASSERT_EQ((uint64_t)2, routes[0][0][5]);
  ASSERT_EQ((uint64_t)3, routes[0][0][6]);
  ASSERT_EQ((uint64_t)4, routes[0][0][7]);
  ASSERT_EQ((uint64_t)10, routes[0][0][8]);
  ASSERT_EQ((uint64_t)9, routes[0][0][9]);
  ASSERT_EQ((uint64_t)11, routes[0][0][10]);
  ASSERT_EQ((uint64_t)5, routes[0][0][11]);
  ASSERT_EQ((uint64_t)6, routes[0][0][12]);
  ASSERT_EQ((uint64_t)7, routes[0][0][13]);
  ASSERT_EQ((uint64_t)8, routes[0][0][14]);
  ASSERT_EQ((uint64_t)15, routes[0][0][15]);
  ASSERT_EQ((uint64_t)14, routes[0][0][16]);
  ASSERT_EQ((uint64_t)13, routes[0][0][17]);

  ASSERT_EQ((size_t)1, routes[1].size());
  ASSERT_EQ((size_t)17, routes[1][0].size());
  ASSERT_EQ((uint64_t)56, routes[1][0][0]);
  ASSERT_EQ((uint64_t)57, routes[1][0][1]);
  ASSERT_EQ((uint64_t)60, routes[1][0][2]);
  ASSERT_EQ((uint64_t)61, routes[1][0][3]);
  ASSERT_EQ((uint64_t)62, routes[1][0][4]);
  ASSERT_EQ((uint64_t)63, routes[1][0][5]);
  ASSERT_EQ((uint64_t)64, routes[1][0][6]);
  ASSERT_EQ((uint64_t)65, routes[1][0][7]);
  ASSERT_EQ((uint64_t)66, routes[1][0][8]);
  ASSERT_EQ((uint64_t)68, routes[1][0][9]);
  ASSERT_EQ((uint64_t)69, routes[1][0][10]);
  ASSERT_EQ((uint64_t)73, routes[1][0][11]);
  ASSERT_EQ((uint64_t)74, routes[1][0][12]);
  ASSERT_EQ((uint64_t)75, routes[1][0][13]);
  ASSERT_EQ((uint64_t)76, routes[1][0][14]);
  ASSERT_EQ((uint64_t)77, routes[1][0][15]);
  ASSERT_EQ((uint64_t)87, routes[1][0][16]);

  ASSERT_EQ((size_t)1, routes[2].size());
  ASSERT_EQ((size_t)14, routes[2][0].size());
  ASSERT_EQ((uint64_t)83, routes[2][0][0]);
  ASSERT_EQ((uint64_t)88, routes[2][0][1]);
  ASSERT_EQ((uint64_t)89, routes[2][0][2]);
  ASSERT_EQ((uint64_t)90, routes[2][0][3]);
  ASSERT_EQ((uint64_t)91, routes[2][0][4]);
  ASSERT_EQ((uint64_t)92, routes[2][0][5]);
  ASSERT_EQ((uint64_t)96, routes[2][0][6]);
  ASSERT_EQ((uint64_t)97, routes[2][0][7]);
  ASSERT_EQ((uint64_t)100, routes[2][0][8]);
  ASSERT_EQ((uint64_t)101, routes[2][0][9]);
  ASSERT_EQ((uint64_t)104, routes[2][0][10]);
  ASSERT_EQ((uint64_t)105, routes[2][0][11]);
  ASSERT_EQ((uint64_t)108, routes[2][0][12]);
  ASSERT_EQ((uint64_t)109, routes[2][0][13]);

  ASSERT_EQ((size_t)1, routes[3].size());
  ASSERT_EQ((size_t)19, routes[3][0].size());
  ASSERT_EQ((uint64_t)87, routes[3][0][0]);
  ASSERT_EQ((uint64_t)77, routes[3][0][1]);
  ASSERT_EQ((uint64_t)76, routes[3][0][2]);
  ASSERT_EQ((uint64_t)75, routes[3][0][3]);
  ASSERT_EQ((uint64_t)74, routes[3][0][4]);
  ASSERT_EQ((uint64_t)73, routes[3][0][5]);
  ASSERT_EQ((uint64_t)69, routes[3][0][6]);
  ASSERT_EQ((uint64_t)70, routes[3][0][7]);
  ASSERT_EQ((uint64_t)71, routes[3][0][8]);
  ASSERT_EQ((uint64_t)72, routes[3][0][9]);
  ASSERT_EQ((uint64_t)65, routes[3][0][10]);
  ASSERT_EQ((uint64_t)64, routes[3][0][11]);
  ASSERT_EQ((uint64_t)63, routes[3][0][12]);
  ASSERT_EQ((uint64_t)62, routes[3][0][13]);
  ASSERT_EQ((uint64_t)61, routes[3][0][14]);
  ASSERT_EQ((uint64_t)60, routes[3][0][15]);
  ASSERT_EQ((uint64_t)57, routes[3][0][16]);
  ASSERT_EQ((uint64_t)58, routes[3][0][17]);
  ASSERT_EQ((uint64_t)59, routes[3][0][18]);

  ASSERT_EQ((size_t)1, routes[4].size());
  ASSERT_EQ((size_t)14, routes[4][0].size());
  ASSERT_EQ((uint64_t)110, routes[4][0][0]);
  ASSERT_EQ((uint64_t)111, routes[4][0][1]);
  ASSERT_EQ((uint64_t)106, routes[4][0][2]);
  ASSERT_EQ((uint64_t)107, routes[4][0][3]);
  ASSERT_EQ((uint64_t)102, routes[4][0][4]);
  ASSERT_EQ((uint64_t)103, routes[4][0][5]);
  ASSERT_EQ((uint64_t)98, routes[4][0][6]);
  ASSERT_EQ((uint64_t)99, routes[4][0][7]);
  ASSERT_EQ((uint64_t)93, routes[4][0][8]);
  ASSERT_EQ((uint64_t)94, routes[4][0][9]);
  ASSERT_EQ((uint64_t)95, routes[4][0][10]);
  ASSERT_EQ((uint64_t)89, routes[4][0][11]);
  ASSERT_EQ((uint64_t)88, routes[4][0][12]);
  ASSERT_EQ((uint64_t)83, routes[4][0][13]);

  ASSERT_EQ((size_t)1, routes[5].size());
  ASSERT_EQ((size_t)29, routes[5][0].size());
  ASSERT_EQ((uint64_t)129, routes[5][0][0]);
  ASSERT_EQ((uint64_t)130, routes[5][0][1]);
  ASSERT_EQ((uint64_t)131, routes[5][0][2]);
  ASSERT_EQ((uint64_t)132, routes[5][0][3]);
  ASSERT_EQ((uint64_t)133, routes[5][0][4]);
  ASSERT_EQ((uint64_t)134, routes[5][0][5]);
  ASSERT_EQ((uint64_t)135, routes[5][0][6]);
  ASSERT_EQ((uint64_t)136, routes[5][0][7]);
  ASSERT_EQ((uint64_t)137, routes[5][0][8]);
  ASSERT_EQ((uint64_t)138, routes[5][0][9]);
  ASSERT_EQ((uint64_t)139, routes[5][0][10]);
  ASSERT_EQ((uint64_t)140, routes[5][0][11]);
  ASSERT_EQ((uint64_t)141, routes[5][0][12]);
  ASSERT_EQ((uint64_t)142, routes[5][0][13]);
  ASSERT_EQ((uint64_t)143, routes[5][0][14]);
  ASSERT_EQ((uint64_t)144, routes[5][0][15]);
  ASSERT_EQ((uint64_t)145, routes[5][0][16]);
  ASSERT_EQ((uint64_t)146, routes[5][0][17]);
  ASSERT_EQ((uint64_t)147, routes[5][0][18]);
  ASSERT_EQ((uint64_t)148, routes[5][0][19]);
  ASSERT_EQ((uint64_t)149, routes[5][0][20]);
  ASSERT_EQ((uint64_t)150, routes[5][0][21]);
  ASSERT_EQ((uint64_t)151, routes[5][0][22]);
  ASSERT_EQ((uint64_t)152, routes[5][0][23]);
  ASSERT_EQ((uint64_t)153, routes[5][0][24]);
  ASSERT_EQ((uint64_t)154, routes[5][0][25]);
  ASSERT_EQ((uint64_t)155, routes[5][0][26]);
  ASSERT_EQ((uint64_t)156, routes[5][0][27]);
  ASSERT_EQ((uint64_t)157, routes[5][0][28]);

  ASSERT_EQ((size_t)1, routes[6].size());
  ASSERT_EQ((size_t)12, routes[6][0].size());
  ASSERT_EQ((uint64_t)170, routes[6][0][0]);
  ASSERT_EQ((uint64_t)171, routes[6][0][1]);
  ASSERT_EQ((uint64_t)172, routes[6][0][2]);
  ASSERT_EQ((uint64_t)173, routes[6][0][3]);
  ASSERT_EQ((uint64_t)174, routes[6][0][4]);
  ASSERT_EQ((uint64_t)175, routes[6][0][5]);
  ASSERT_EQ((uint64_t)176, routes[6][0][6]);
  ASSERT_EQ((uint64_t)177, routes[6][0][7]);
  ASSERT_EQ((uint64_t)178, routes[6][0][8]);
  ASSERT_EQ((uint64_t)179, routes[6][0][9]);
  ASSERT_EQ((uint64_t)180, routes[6][0][10]);
  ASSERT_EQ((uint64_t)181, routes[6][0][11]);


  // Check the rivers.
  ASSERT_EQ((size_t)2, rivers.size());

  // First river.
  ASSERT_EQ((size_t)4, rivers[0].size());

  ASSERT_EQ((size_t)13, rivers[0][0].size());
  ASSERT_EQ((uint64_t)19, rivers[0][0][0]);
  ASSERT_EQ((uint64_t)35, rivers[0][0][1]);
  ASSERT_EQ((uint64_t)20, rivers[0][0][2]);
  ASSERT_EQ((uint64_t)24, rivers[0][0][3]);
  ASSERT_EQ((uint64_t)25, rivers[0][0][4]);
  ASSERT_EQ((uint64_t)26, rivers[0][0][5]);
  ASSERT_EQ((uint64_t)22, rivers[0][0][6]);
  ASSERT_EQ((uint64_t)23, rivers[0][0][7]);
  ASSERT_EQ((uint64_t)28, rivers[0][0][8]);
  ASSERT_EQ((uint64_t)29, rivers[0][0][9]);
  ASSERT_EQ((uint64_t)36, rivers[0][0][10]);
  ASSERT_EQ((uint64_t)33, rivers[0][0][11]);
  ASSERT_EQ((uint64_t)34, rivers[0][0][12]);

  ASSERT_EQ((size_t)6, rivers[0][1].size());
  ASSERT_EQ((uint64_t)19, rivers[0][1][0]);
  ASSERT_EQ((uint64_t)35, rivers[0][1][1]);
  ASSERT_EQ((uint64_t)20, rivers[0][1][2]);
  ASSERT_EQ((uint64_t)21, rivers[0][1][3]);
  ASSERT_EQ((uint64_t)22, rivers[0][1][4]);
  ASSERT_EQ((uint64_t)23, rivers[0][1][5]);

  ASSERT_EQ((size_t)5, rivers[0][2].size());
  ASSERT_EQ((uint64_t)22, rivers[0][2][0]);
  ASSERT_EQ((uint64_t)23, rivers[0][2][1]);
  ASSERT_EQ((uint64_t)32, rivers[0][2][2]);
  ASSERT_EQ((uint64_t)33, rivers[0][2][3]);
  ASSERT_EQ((uint64_t)34, rivers[0][2][4]);

  ASSERT_EQ((size_t)4, rivers[0][3].size());
  ASSERT_EQ((uint64_t)23, rivers[0][3][0]);
  ASSERT_EQ((uint64_t)28, rivers[0][3][1]);
  ASSERT_EQ((uint64_t)29, rivers[0][3][2]);
  ASSERT_EQ((uint64_t)30, rivers[0][3][3]);

  // Second river.
  ASSERT_EQ((size_t)1, rivers[1].size());
  ASSERT_EQ((size_t)10, rivers[1][0].size());
  ASSERT_EQ((uint64_t)37, rivers[1][0][0]);
  ASSERT_EQ((uint64_t)38, rivers[1][0][1]);
  ASSERT_EQ((uint64_t)39, rivers[1][0][2]);
  ASSERT_EQ((uint64_t)40, rivers[1][0][3]);
  ASSERT_EQ((uint64_t)41, rivers[1][0][4]);
  ASSERT_EQ((uint64_t)42, rivers[1][0][5]);
  ASSERT_EQ((uint64_t)43, rivers[1][0][6]);
  ASSERT_EQ((uint64_t)44, rivers[1][0][7]);
  ASSERT_EQ((uint64_t)45, rivers[1][0][8]);
  ASSERT_EQ((uint64_t)46, rivers[1][0][9]);


  // Check the ids of the corrected ways.
  ASSERT_EQ((size_t)34, correctedWayIds.size());
  ASSERT_EQ(true, correctedWayIds.contains(1));
  ASSERT_EQ(true, correctedWayIds.contains(2));
  ASSERT_EQ(true, correctedWayIds.contains(3));
  ASSERT_EQ(true, correctedWayIds.contains(4));
  ASSERT_EQ(true, correctedWayIds.contains(5));
  ASSERT_EQ(true, correctedWayIds.contains(20));
  ASSERT_EQ(true, correctedWayIds.contains(21));
  ASSERT_EQ(true, correctedWayIds.contains(22));
  ASSERT_EQ(true, correctedWayIds.contains(23));
  ASSERT_EQ(true, correctedWayIds.contains(24));
  ASSERT_EQ(true, correctedWayIds.contains(25));
  ASSERT_EQ(true, correctedWayIds.contains(28));
  ASSERT_EQ(true, correctedWayIds.contains(29));
  ASSERT_EQ(true, correctedWayIds.contains(30));
  ASSERT_EQ(true, correctedWayIds.contains(31));
  ASSERT_EQ(true, correctedWayIds.contains(32));
  ASSERT_EQ(true, correctedWayIds.contains(33));
  ASSERT_EQ(true, correctedWayIds.contains(34));
  ASSERT_EQ(true, correctedWayIds.contains(35));
  ASSERT_EQ(true, correctedWayIds.contains(36));
  ASSERT_EQ(true, correctedWayIds.contains(37));
  ASSERT_EQ(true, correctedWayIds.contains(38));
  ASSERT_EQ(true, correctedWayIds.contains(39));
  ASSERT_EQ(true, correctedWayIds.contains(43));
  ASSERT_EQ(true, correctedWayIds.contains(44));
  ASSERT_EQ(true, correctedWayIds.contains(45));
  ASSERT_EQ(true, correctedWayIds.contains(46));
  ASSERT_EQ(true, correctedWayIds.contains(47));
  ASSERT_EQ(true, correctedWayIds.contains(48));
  ASSERT_EQ(true, correctedWayIds.contains(49));
  ASSERT_EQ(true, correctedWayIds.contains(50));
  ASSERT_EQ(true, correctedWayIds.contains(54));
  ASSERT_EQ(true, correctedWayIds.contains(55));
  ASSERT_EQ(true, correctedWayIds.contains(56));
}

// ____________________________________________________________________________
TEST(ROUTESFROMRELATIONSTEST, routesRangeSize1Test) {
  GetOsmStats handler;
  NodeWayRelationParser statsParser("./testMap.osm", &handler);
  statsParser.parse();
  OsmStats osmStats = handler.getOsmStats();

  RoutesFromRelations routesFromRelations1("./testMap.osm", osmStats,
                                           "ele", 0, 1);

  std::set<uint64_t> correctedWayIds;
  std::vector<RoutePaths> routes;
  std::vector<RoutePaths> rivers;
  std::vector<RoutePaths> tunnelsAndBridges;

  routesFromRelations1.getRoutesAndIds(correctedWayIds, routes, rivers,
                                       tunnelsAndBridges);

  // The first route relation contains the route with tunnel.
  ASSERT_EQ((size_t)1, tunnelsAndBridges.size());
  ASSERT_EQ((size_t)3, tunnelsAndBridges[0].size());

  ASSERT_EQ((size_t)1, routes.size());
  ASSERT_EQ((size_t)1, routes[0].size());
  ASSERT_EQ((size_t)18, routes[0][0].size());

  ASSERT_EQ((size_t)0, rivers.size());
  ASSERT_EQ((size_t)5, correctedWayIds.size());

  RoutesFromRelations routesFromRelations2("./testMap.osm", osmStats,
                                           "ele", 1, 2);
  routes.clear();
  rivers.clear();
  tunnelsAndBridges.clear();
  routesFromRelations2.getRoutesAndIds(correctedWayIds, routes, rivers, tunnelsAndBridges);

  // The second route relation contains the complex river.
  ASSERT_EQ((size_t)0, tunnelsAndBridges.size());

  ASSERT_EQ((size_t)0, routes.size());

  ASSERT_EQ((size_t)1, rivers.size());
  ASSERT_EQ((size_t)4, rivers[0].size());
  ASSERT_EQ((size_t)13, rivers[0][0].size());
  ASSERT_EQ((size_t)6, rivers[0][1].size());
  ASSERT_EQ((size_t)5, rivers[0][2].size());
  ASSERT_EQ((size_t)4, rivers[0][3].size());

  ASSERT_EQ((size_t)5, correctedWayIds.size());

  RoutesFromRelations routesFromRelations3("./testMap.osm", osmStats,
                                           "ele", 2, 3);
  routes.clear();
  rivers.clear();
  tunnelsAndBridges.clear();
  routesFromRelations3.getRoutesAndIds(correctedWayIds, routes, rivers, tunnelsAndBridges);

  // The third relation contains the simple river.
  ASSERT_EQ((size_t)0, tunnelsAndBridges.size());

  ASSERT_EQ((size_t)0, routes.size());

  ASSERT_EQ((size_t)1, rivers.size());
  ASSERT_EQ((size_t)1, rivers[0].size());
  ASSERT_EQ((size_t)10, rivers[0][0].size());

  ASSERT_EQ((size_t)5, correctedWayIds.size());

  RoutesFromRelations routesFromRelations4("./testMap.osm", osmStats,
                                           "ele", 3, 4);
  routes.clear();
  rivers.clear();
  tunnelsAndBridges.clear();
  routesFromRelations4.getRoutesAndIds(correctedWayIds, routes, rivers, tunnelsAndBridges);

  // The 4th route relation contains the complex route.
  ASSERT_EQ((size_t)0, tunnelsAndBridges.size());

  ASSERT_EQ((size_t)4, routes.size());
  ASSERT_EQ((size_t)1, routes[0].size());
  ASSERT_EQ((size_t)17, routes[0][0].size());
  ASSERT_EQ((size_t)1, routes[1].size());
  ASSERT_EQ((size_t)14, routes[1][0].size());
  ASSERT_EQ((size_t)1, routes[2].size());
  ASSERT_EQ((size_t)19, routes[2][0].size());
  ASSERT_EQ((size_t)1, routes[3].size());
  ASSERT_EQ((size_t)14, routes[3][0].size());

  ASSERT_EQ((size_t)0, rivers.size());
  ASSERT_EQ((size_t)23, correctedWayIds.size());

  RoutesFromRelations routesFromRelations5("./testMap.osm", osmStats,
                                           "ele", 4, 5);
  routes.clear();
  rivers.clear();
  tunnelsAndBridges.clear();
  routesFromRelations5.getRoutesAndIds(correctedWayIds, routes, rivers, tunnelsAndBridges);

  // The 5th route contains a tunnel and a bridge.
  ASSERT_EQ((size_t)2, tunnelsAndBridges.size());
  ASSERT_EQ((size_t)3, tunnelsAndBridges[0].size());
  ASSERT_EQ((size_t)4, tunnelsAndBridges[0][0].size());
  ASSERT_EQ((size_t)11, tunnelsAndBridges[0][1].size());
  ASSERT_EQ((size_t)4, tunnelsAndBridges[0][2].size());

  ASSERT_EQ((size_t)3, tunnelsAndBridges[1].size());
  ASSERT_EQ((size_t)4, tunnelsAndBridges[1][0].size());
  ASSERT_EQ((size_t)9, tunnelsAndBridges[1][1].size());
  ASSERT_EQ((size_t)5, tunnelsAndBridges[1][2].size());

  ASSERT_EQ((size_t)1, routes.size());
  ASSERT_EQ((size_t)1, routes[0].size());
  ASSERT_EQ((size_t)29, routes[0][0].size());

  ASSERT_EQ((size_t)0, rivers.size());
  ASSERT_EQ((size_t)31, correctedWayIds.size());

  RoutesFromRelations routesFromRelations6("./testMap.osm", osmStats,
                                           "ele", 5, 6);
  routes.clear();
  rivers.clear();
  tunnelsAndBridges.clear();
  routesFromRelations6.getRoutesAndIds(correctedWayIds, routes, rivers, tunnelsAndBridges);

  // No more route relations left.
  ASSERT_EQ((size_t)0, tunnelsAndBridges.size());

  ASSERT_EQ((size_t)1, routes.size());
  ASSERT_EQ((size_t)1, routes[0].size());
  ASSERT_EQ((size_t)12, routes[0][0].size());

  ASSERT_EQ((size_t)0, rivers.size());
  ASSERT_EQ((size_t)34, correctedWayIds.size());

  RoutesFromRelations routesFromRelations7("./testMap.osm", osmStats,
                                           "ele", 6, 7);
  routes.clear();
  rivers.clear();
  tunnelsAndBridges.clear();
  routesFromRelations7.getRoutesAndIds(correctedWayIds, routes, rivers, tunnelsAndBridges);

  // No more route relations left.
  ASSERT_EQ((size_t)0, tunnelsAndBridges.size());

  ASSERT_EQ((size_t)0, routes.size());

  ASSERT_EQ((size_t)0, rivers.size());
  ASSERT_EQ((size_t)34, correctedWayIds.size());
}

// ____________________________________________________________________________
TEST(ROUTESFROMRELATIONSTEST, routesRangeSize2Test) {
  GetOsmStats handler;
  NodeWayRelationParser statsParser("./testMap.osm", &handler);
  statsParser.parse();
  OsmStats osmStats = handler.getOsmStats();

  RoutesFromRelations routesFromRelations1("./testMap.osm", osmStats,
                                           "ele", 0, 2);

  std::set<uint64_t> correctedWayIds;
  std::vector<RoutePaths> routes;
  std::vector<RoutePaths> rivers;
  std::vector<RoutePaths> tunnelsAndBridges;

  routesFromRelations1.getRoutesAndIds(correctedWayIds, routes, rivers,
                                       tunnelsAndBridges);

  // The first 2 route relations contain the route with tunnel and
  // the complex river.
  ASSERT_EQ((size_t)1, tunnelsAndBridges.size());
  ASSERT_EQ((size_t)3, tunnelsAndBridges[0].size());

  ASSERT_EQ((size_t)1, routes.size());
  ASSERT_EQ((size_t)1, routes[0].size());
  ASSERT_EQ((size_t)18, routes[0][0].size());

  ASSERT_EQ((size_t)1, rivers.size());
  ASSERT_EQ((size_t)4, rivers[0].size());
  ASSERT_EQ((size_t)13, rivers[0][0].size());
  ASSERT_EQ((size_t)6, rivers[0][1].size());
  ASSERT_EQ((size_t)5, rivers[0][2].size());
  ASSERT_EQ((size_t)4, rivers[0][3].size());

  ASSERT_EQ((size_t)5, correctedWayIds.size());

  RoutesFromRelations routesFromRelations2("./testMap.osm", osmStats,
                                           "ele", 2, 4);
  routes.clear();
  rivers.clear();
  tunnelsAndBridges.clear();
  routesFromRelations2.getRoutesAndIds(correctedWayIds, routes, rivers, tunnelsAndBridges);

  // The third and 4th route relation contains the simple river
  // and  complex route.
  ASSERT_EQ((size_t)0, tunnelsAndBridges.size());

  ASSERT_EQ((size_t)4, routes.size());
  ASSERT_EQ((size_t)1, routes[0].size());
  ASSERT_EQ((size_t)17, routes[0][0].size());
  ASSERT_EQ((size_t)1, routes[1].size());
  ASSERT_EQ((size_t)14, routes[1][0].size());
  ASSERT_EQ((size_t)1, routes[2].size());
  ASSERT_EQ((size_t)19, routes[2][0].size());
  ASSERT_EQ((size_t)1, routes[3].size());
  ASSERT_EQ((size_t)14, routes[3][0].size());

  ASSERT_EQ((size_t)1, rivers.size());
  ASSERT_EQ((size_t)1, rivers[0].size());
  ASSERT_EQ((size_t)10, rivers[0][0].size());

  ASSERT_EQ((size_t)23, correctedWayIds.size());

  RoutesFromRelations routesFromRelations3("./testMap.osm", osmStats,
                                           "ele", 4, 6);
  routes.clear();
  rivers.clear();
  tunnelsAndBridges.clear();
  routesFromRelations3.getRoutesAndIds(correctedWayIds, routes, rivers, tunnelsAndBridges);

  ASSERT_EQ((size_t)2, tunnelsAndBridges.size());
  ASSERT_EQ((size_t)3, tunnelsAndBridges[0].size());
  ASSERT_EQ((size_t)4, tunnelsAndBridges[0][0].size());
  ASSERT_EQ((size_t)11, tunnelsAndBridges[0][1].size());
  ASSERT_EQ((size_t)4, tunnelsAndBridges[0][2].size());

  ASSERT_EQ((size_t)3, tunnelsAndBridges[1].size());
  ASSERT_EQ((size_t)4, tunnelsAndBridges[1][0].size());
  ASSERT_EQ((size_t)9, tunnelsAndBridges[1][1].size());
  ASSERT_EQ((size_t)5, tunnelsAndBridges[1][2].size());

  ASSERT_EQ((size_t)2, routes.size());
  ASSERT_EQ((size_t)1, routes[0].size());
  ASSERT_EQ((size_t)29, routes[0][0].size());
  ASSERT_EQ((size_t)1, routes[1].size());
  ASSERT_EQ((size_t)12, routes[1][0].size());

  ASSERT_EQ((size_t)0, rivers.size());
  ASSERT_EQ((size_t)34, correctedWayIds.size());

  RoutesFromRelations routesFromRelations4("./testMap.osm", osmStats,
                                           "ele", 6, 8);
  routes.clear();
  rivers.clear();
  tunnelsAndBridges.clear();
  routesFromRelations4.getRoutesAndIds(correctedWayIds, routes, rivers, tunnelsAndBridges);

  // No more route relations left.
  ASSERT_EQ((size_t)0, tunnelsAndBridges.size());

  ASSERT_EQ((size_t)0, routes.size());

  ASSERT_EQ((size_t)0, rivers.size());
  ASSERT_EQ((size_t)34, correctedWayIds.size());
}

// ____________________________________________________________________________
TEST(ROUTESFROMRELATIONSTEST, routesRangeSize3Test) {
  GetOsmStats handler;
  NodeWayRelationParser statsParser("./testMap.osm", &handler);
  statsParser.parse();
  OsmStats osmStats = handler.getOsmStats();

  RoutesFromRelations routesFromRelations1("./testMap.osm", osmStats,
                                           "ele", 0, 3);

  std::set<uint64_t> correctedWayIds;
  std::vector<RoutePaths> routes;
  std::vector<RoutePaths> rivers;
  std::vector<RoutePaths> tunnelsAndBridges;

  routesFromRelations1.getRoutesAndIds(correctedWayIds, routes, rivers,
                                       tunnelsAndBridges);

  ASSERT_EQ((size_t)1, tunnelsAndBridges.size());
  ASSERT_EQ((size_t)3, tunnelsAndBridges[0].size());

  ASSERT_EQ((size_t)1, routes.size());
  ASSERT_EQ((size_t)1, routes[0].size());
  ASSERT_EQ((size_t)18, routes[0][0].size());

  ASSERT_EQ((size_t)2, rivers.size());
  ASSERT_EQ((size_t)4, rivers[0].size());
  ASSERT_EQ((size_t)13, rivers[0][0].size());
  ASSERT_EQ((size_t)6, rivers[0][1].size());
  ASSERT_EQ((size_t)5, rivers[0][2].size());
  ASSERT_EQ((size_t)4, rivers[0][3].size());
  ASSERT_EQ((size_t)1, rivers[1].size());
  ASSERT_EQ((size_t)10, rivers[1][0].size());

  ASSERT_EQ((size_t)5, correctedWayIds.size());

  RoutesFromRelations routesFromRelations2("./testMap.osm", osmStats,
                                           "ele", 3, 6);
  routes.clear();
  rivers.clear();
  tunnelsAndBridges.clear();
  routesFromRelations2.getRoutesAndIds(correctedWayIds, routes, rivers, tunnelsAndBridges);

  ASSERT_EQ((size_t)2, tunnelsAndBridges.size());
  ASSERT_EQ((size_t)3, tunnelsAndBridges[0].size());
  ASSERT_EQ((size_t)4, tunnelsAndBridges[0][0].size());
  ASSERT_EQ((size_t)11, tunnelsAndBridges[0][1].size());
  ASSERT_EQ((size_t)4, tunnelsAndBridges[0][2].size());

  ASSERT_EQ((size_t)3, tunnelsAndBridges[1].size());
  ASSERT_EQ((size_t)4, tunnelsAndBridges[1][0].size());
  ASSERT_EQ((size_t)9, tunnelsAndBridges[1][1].size());
  ASSERT_EQ((size_t)5, tunnelsAndBridges[1][2].size());

  ASSERT_EQ((size_t)6, routes.size());
  ASSERT_EQ((size_t)1, routes[0].size());
  ASSERT_EQ((size_t)17, routes[0][0].size());
  ASSERT_EQ((size_t)1, routes[1].size());
  ASSERT_EQ((size_t)14, routes[1][0].size());
  ASSERT_EQ((size_t)1, routes[2].size());
  ASSERT_EQ((size_t)19, routes[2][0].size());
  ASSERT_EQ((size_t)1, routes[3].size());
  ASSERT_EQ((size_t)14, routes[3][0].size());
  ASSERT_EQ((size_t)1, routes[4].size());
  ASSERT_EQ((size_t)29, routes[4][0].size());
  ASSERT_EQ((size_t)1, routes[5].size());
  ASSERT_EQ((size_t)12, routes[5][0].size());

  ASSERT_EQ((size_t)0, rivers.size());

  ASSERT_EQ((size_t)34, correctedWayIds.size());

  RoutesFromRelations routesFromRelations3("./testMap.osm", osmStats,
                                           "ele", 6, 9);
  routes.clear();
  rivers.clear();
  tunnelsAndBridges.clear();
  routesFromRelations3.getRoutesAndIds(correctedWayIds, routes, rivers, tunnelsAndBridges);

  // No more route relations left.
  ASSERT_EQ((size_t)0, tunnelsAndBridges.size());

  ASSERT_EQ((size_t)0, routes.size());

  ASSERT_EQ((size_t)0, rivers.size());
  ASSERT_EQ((size_t)34, correctedWayIds.size());
}
