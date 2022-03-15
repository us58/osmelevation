add_library(correctelevationcorrect
        CorrectRoute.h CorrectRoute.cpp
        CorrectRiver.h CorrectRiver.cpp
        SmoothRoute.h SmoothRoute.cpp
        CorrectTunnelOrBridge.h CorrectTunnelOrBridge.cpp)

target_link_libraries(correctelevationcorrect)