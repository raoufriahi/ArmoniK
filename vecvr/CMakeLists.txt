cmake_minimum_required(VERSION 3.10)
project(UbPlayer)
# Add source files here
set(SOURCES
    src/main.cpp
    src/webserver.cpp
	src/mediaserver.cpp
    src/link/linkhandler.cpp
    src/link/linkstream.cpp
    ../3rdparty/civetweb/src/CivetServer.cpp
    ../3rdparty/civetweb/src/civetweb.c
)
# Add header files here
set(HEADERS
   ./include/link/linkstream.hpp
)
# Add the include directories
include_directories(
    ./include
    ../include
    ../veuilib/include
    ../xcmnlib/include
    ../xcmnlib/include/utility
    ../3rdparty/leveldb/include
    ../3rdparty/spdlog/include
    ../3rdparty/protobuf/src
    ../3rdparty/civetweb/include
    ../velib/include/vdb
    ../velib/include/vplay
    ../onvif/include
    ${CMAKE_INSTALL_PREFIX}/include/poco/Data/include
    ${CMAKE_INSTALL_PREFIX}/include/poco/Foundation/include
    ${CMAKE_INSTALL_PREFIX}/include/poco/Util/include
    ${CMAKE_INSTALL_PREFIX}/include/poco/Data/SQLite/include
    ${CMAKE_INSTALL_PREFIX}/include/QtWidgets
    ${CMAKE_INSTALL_PREFIX}/include/QtNetwork
    ${CMAKE_INSTALL_PREFIX}/include
    ${CMAKE_INSTALL_PREFIX}/include/QtCore
    ${CMAKE_INSTALL_PREFIX}/include/ZLMediaKit
    ${CMAKE_INSTALL_PREFIX}/include/ZLMediaKit/H264
    ${CMAKE_INSTALL_PREFIX}/include/ZLToolKit
)
link_directories(${PROJECT_NAME} "${CMAKE_INSTALL_PREFIX}/lib")
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME}
    -lZLMediaKit
    -lQt5Core
    -lZLToolKit
    -lcmnlib
    -lavcodec
    -lavformat
    -lavutil
    -lavfilter
    -lswscale
    -lleveldb
    -lvelib
    -lveuilib
    -lPocoFoundation
    -lPocoDataSQLite
    -lPocoData
    -lPocoUtil
    -lprotobuf
    -lssl
    -lcrypto
    -ldl
    -lpthread
)

install(TARGETS ${PROJECT_NAME} 
       RUNTIME DESTINATION bin/
       ARCHIVE DESTINATION bin/)