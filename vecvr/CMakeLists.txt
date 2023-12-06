cmake_minimum_required(VERSION 3.10)

project(hServer)

set(CMAKE_CXX_STANDARD 11)

# Add source files here
set(SOURCES
    ./main/main.cpp
    ./main/webserver.cpp
	./main/mediaserver.cpp
    ./../3rdparty/civetweb/src/CivetServer.cpp
    ./../3rdparty/civetweb/src/civetweb.c
    ./main/link/linkhandler.cpp
    ./main/link/linkstream.cpp
)

# Add header files here
set(HEADERS
   ./main/link/linkstream.hpp
)

# Add resource files here
set(RESOURCES
   ./main/vscmainwindows.qrc
)

# Add the include directories
include_directories(
    ./GeneratedFiles
    ./
    ./GeneratedFiles/Debug
    ../main
    ../playback
    # Add other include directories...
)

# Add the target executable
add_executable(hServer ${SOURCES} ${HEADERS} ${RESOURCES})

# Add additional compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_WEBSOCKET -DLINUX_OS -DQT_DLL -DQT_MULTIMEDIA_LIB -DQT_MULTIMEDIAWIDGETS_LIB -DQT_NETWORK_LIB -DQT_QML_LIB -DQT_QUICK_LIB -DQT_SQL_LIB -DQT_CONCURRENT_LIB -DQT_HELP_LIB -DQT_PRINTSUPPORT_LIB -DQT_SCRIPT_LIB -DQT_SVG_LIB -DQT_WEBKITWIDGETS_LIB -DQT_WIDGETS_LIB -DQT_XML_LIB -DQT_XMLPATTERNS_LIB -DQT_NO_BLUEZ -DQT_NO_UDEV -DWITH_OPENSSL -DWITH_DOM -DWITH_PURE_VIRTUAL")

# Add the libraries
target_link_libraries(hServer
    # Add other libraries...
    -lrt
    -L"./../../3rdparty/leveldb/"
    -L"./Debug"
    -L"./../../3rdparty/onvifc/prj_linux/lib"
    -L"./../../3rdparty/leveldb/"
    ../../output/$$(VE_INSTALL_DIR)/lib/x86_64-linux-gnu
    -lZLMediaKit
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
)
