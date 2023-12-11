cmake_minimum_required(VERSION 3.15)
project(onvif)
# Add the include directories
include_directories(
    ${CMAKE_INSTALL_PREFIX}/include
    ./include
    ./include/video
    ./src/
    ../3rdparty/live555/BasicUsageEnvironment/include  
    ../3rdparty/live555/groupsock/include
    ../3rdparty/live555/liveMedia/include 
    ../3rdparty/live555/UsageEnvironment/include
    ../3rdparty/live555/live555/include/
    ../3rdparty/civetweb/include/
)


# Add source files here
set( SOURCES
    ../3rdparty/civetweb/src/CivetServer.cpp
    ./src/onvifagentlib.cpp
    ./src/tinyprocess/process_unix.cpp
    ./src/tinyprocess/process.cpp
    ./include/video/h264_bit_reader.cpp
    ./include/video/h264_parser.cpp
    ./include/video/h5s_h264_sink.cpp
    ./include/video/h5slib.cpp
    ./include/onvifclidevice.pb.cc
    ./include/onvifclimedia.pb.cc
    ./include/onvifcli.pb.cc
    ./include/onvifcliptz.pb.cc
    ../3rdparty/civetweb/src/civetweb.c
    )

ADD_LIBRARY(${PROJECT_NAME} SHARED 
            ${SOURCES})

install(TARGETS ${PROJECT_NAME} 
        LIBRARY DESTINATION lib/
        ARCHIVE DESTINATION lib/)