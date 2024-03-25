cmake_minimum_required(VERSION 3.15)
project(velib)

# Add the include directories
include_directories(
    ${CMAKE_INSTALL_PREFIX}/include
    ./include/utility
    ./include/vdb
    ./include/vplay
    ./include/
    ./src/utility
    ./src/vdb
    ./src/vplay
    ./src/vrender
    ./3rdparty/SDL2/include
    ./3rdparty/libyuv/include
    ../3rdparty/spdlog/include
    ../3rdparty/poco/Data/include
    ../3rdparty/poco/Foundation/include
    ../3rdparty/poco/Data/SQLite/include
    ../xcmnlib/include
    ../xcmnlib/include/utility
    )
# Add source files here
set( SOURCES
    ./src/utility/license.cpp
    ./src/utility/base64.cpp
    ./src/vplay/vplay.cpp
    ./src/vplay/vsmotalgo.cpp
    ./src/vrender/novideo.cpp
    ./src/vrender/vehwcmd.cpp
    ./src/vdb/indexdb.cpp
    ./src/vdb/veventdb.cpp
    ./src/vdb/mediafile.cpp
    ./src/vdb/pbsession.cpp
    ./src/vdb/pbwrapper.cpp
    ./src/vdb/recordsession.cpp
    ./src/vdb/recordwrapper.cpp
    ./src/vdb/vdb.cpp
    ./src/vdb/vdbfile.cpp
    ./src/vdb/hdfsrecsession.cpp
    ./src/vdb/hdfswrapper.cpp
    ./src/vdb/vhdfsdb.cpp
    ./src/vrender/sdl_render.cpp
    )

ADD_LIBRARY(${PROJECT_NAME} SHARED 
            ${SOURCES})
install(TARGETS ${PROJECT_NAME} 
        LIBRARY DESTINATION lib/
        ARCHIVE DESTINATION lib/)