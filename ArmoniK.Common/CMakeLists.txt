cmake_minimum_required(VERSION 3.15)
project(avconfig)

file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADER "include/*.h")

include_directories(
    ./header
    ./header/config
    ./header/utility
    ./header/vdb
    ../include   
    ../xcmnlib/include  
    ../3rdparty/leveldb/include
    ../3rdparty/spdlog/include/
    ../3rdparty/protobuf/src/
    ../3rdparty/poco/Foundation/include/
    ../velib/include
    )
add_definitions(-DLINUX_OS=1)
ADD_LIBRARY( ${PROJECT_NAME} SHARED
              ${SOURCES} ${HEADER})


install(TARGETS ${PROJECT_NAME} 
            LIBRARY DESTINATION lib/
            ARCHIVE DESTINATION lib/)
