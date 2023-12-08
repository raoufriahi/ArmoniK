cmake_minimum_required(VERSION 3.10)

project(UbPlayer)

set(CMAKE_CXX_STANDARD 11)
execute_process(COMMAND lsb_release -si OUTPUT_VARIABLE CA_OS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND uname -m COMMAND sed "s/x86_//;s/i[3-6]86/32/" OUTPUT_VARIABLE CA_ARCH OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND lsb_release -sr OUTPUT_VARIABLE CA_VER OUTPUT_STRIP_TRAILING_WHITESPACE)
set(CA_INSTALL_DIR "${CA_OS}-${CA_VER}-${CA_ARCH}bit")
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/../output/${CA_INSTALL_DIR})

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
   src/link/linkstream.hpp
)

# Add the include directories
include_directories(
    ../include
    ./include
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
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -DUSE_WEBSOCKET -DLINUX_OS -DQT_DLL -DQT_MULTIMEDIA_LIB -DQT_MULTIMEDIAWIDGETS_LIB -DQT_NETWORK_LIB -DQT_QML_LIB -DQT_QUICK_LIB -DQT_SQL_LIB -DQT_CONCURRENT_LIB -DQT_HELP_LIB -DQT_PRINTSUPPORT_LIB -DQT_SCRIPT_LIB -DQT_SVG_LIB -DQT_WEBKITWIDGETS_LIB -DQT_WIDGETS_LIB -DQT_XML_LIB -DQT_XMLPATTERNS_LIB -DQT_NO_BLUEZ -DQT_NO_UDEV -DWITH_OPENSSL -DWITH_DOM -DWITH_PURE_VIRTUAL")
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