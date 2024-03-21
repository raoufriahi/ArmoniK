cmake_minimum_required(VERSION 3.15)

project(veuilib)
# Add source files
set(SOURCES
    ../include/config/linksystem.pb.cc
    ../include/config/linkproto.pb.cc
    ../include/config/vidconf.pb.cc
    ../include/config/restsystem.pb.cc
    # Add other source files here
)

# Add include directories
include_directories(
    .
    ${CMAKE_INSTALL_PREFIX}/include
    ./GeneratedFiles
    ./../onvifcpplib/include
    ../velib/include/utility 
    ../velib/include/vdb 
    ../3rdparty/protobuf/src
)

# Add link directories
link_directories(
    ../3rdparty/leveldb
    ../3rdparty/openssl-linux
    ../3rdparty/onvifc/prj_linux/lib
    ../3rdparty/leveldb
    ../veuilib/rtspserver/lib/linux
    ../onvifcpplib/lib/linux
    ../../output/$$(VE_INSTALL_DIR)/lib
)

# Add libraries
ADD_LIBRARY(${PROJECT_NAME} SHARED 
            ${SOURCES})

install(TARGETS ${PROJECT_NAME} 
        LIBRARY DESTINATION lib/
        ARCHIVE DESTINATION lib/)